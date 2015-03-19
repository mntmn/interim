#include <sys/time.h>
#include <stdio.h>
#include <lightning.h>
#include <stdint.h>
#include <stdlib.h>

typedef jit_word_t (*funcptr)();
static jit_state_t *_jit;
static jit_state_t *_jit_saved;
static jit_word_t stack_ptr, stack_base;

#include "compiler.c"

#include "libBareMetal.h"

static uint32_t* bm_vram = 0;
static uint32_t bm_screen_w = 0;
static uint32_t bm_screen_h = 0;
static uint32_t bm_screen_bpp = 0;

static uint8_t* bm_framebuffer;

void init_bm_video() {
  bm_vram = (uint32_t*)b_system_config(20,0);
  //printf("video ram: %p\n",bm_vram);
  
	bm_screen_w = b_system_config(21, 0);
	bm_screen_h = b_system_config(22, 0);
	bm_screen_bpp = b_system_config(23, 0);
  
  printf("resolution: %d x %d x %d vram at: %p\n",bm_screen_w,bm_screen_h,bm_screen_bpp,bm_vram);

  // 0xfd000000

  bm_framebuffer = malloc(bm_screen_w*bm_screen_h*bm_screen_bpp/8);
  //bm_framebuffer = bm_vram;
}

// ethernet: dest mac[6] | source mac[6] | type[2]

#define ETH_TYPE_ARP 0x0806
#define ETH_TYPE_IP 0x0800
#define PROTO_IP 0x800
#define ARP_REQUEST 1
#define ARP_REPLY 2
#define PROTO_IP_UDP 0x11
#define PROTO_IP_TCP 0x6

#define TCP_NS  256
#define TCP_CWR 128
#define TCP_ECE 64
#define TCP_URG 32
#define TCP_ACK 16
#define TCP_PSH 8
#define TCP_RST 4
#define TCP_SYN 2
#define TCP_FIN 1


// size: 14
typedef struct eth_header {
  uint8_t dest_mac[6];
  uint8_t src_mac[6];
  uint16_t type;
  uint8_t payload;
} eth_header;

// size: 28
typedef struct arp_packet {
  uint16_t hwtype;
  uint16_t proto;
  uint8_t hwsize;
  uint8_t proto_size;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
} arp_packet;

// size: 20
typedef struct ipv4_packet {
  uint8_t version;
  uint8_t services;
  uint16_t size;
  uint16_t id;
  uint8_t flags;
  uint8_t frag_offset; // 0x4000?
  uint8_t ttl;
  uint8_t proto;
  uint16_t checksum;
  uint8_t src_ip[4];
  uint8_t dest_ip[4];
  uint8_t data;
} ipv4_packet;

typedef struct udp_packet {
  uint16_t src_port;
  uint16_t dest_port;
  uint16_t size; // size-8 = body size in bytes
  uint16_t checksum;
  uint8_t data;
} udp_packet;

typedef struct tcp_packet {
  uint16_t src_port;
  uint16_t dest_port;
  uint32_t seqnum;
  uint32_t acknum;
  uint8_t data_offset;
  uint8_t flags;
  uint16_t window;
  uint16_t checksum;
  uint16_t urg_pointer;
  uint8_t data;
} tcp_packet;

uint16_t swap16(uint16_t in) {
  uint16_t out = in<<8 | ((in&0xff00)>>8);
  return out;
}

uint32_t swap32(uint32_t in) {
  uint32_t out = in<<24 | ((in&0xff00)<<8) | ((in&0xff0000)>>8) | ((in&0xff000000)>>24);
  return out;
}

char broadcast_mac[] = {0xff,0xff,0xff,0xff,0xff,0xff};
char my_mac[] = {0x52,0x54,0xde,0xad,0xbe,0xef};
char their_mac[] = {0xff,0xff,0xff,0xff,0xff,0xff};

char my_ip[] = {10,0,0,4};
char their_ip[] = {10,0,0,1};
//char their_ip[] = {91,250,115,15};
//char their_ip[] = {91,217,189,42};

char* tx_packet;
char* rx_packet;

static Cell* udp_cell;
static uint32_t my_seqnum = 23;
static uint32_t their_seqnum = 0;
static uint32_t my_tcp_port = 5000;

static Cell* my_tcp_connected_callback;
static Cell* my_tcp_data_callback;

void send_tcp_packet(int srcport, int port, uint8_t flags, uint32_t seqnum, uint32_t acknum, uint8_t* data, uint16_t size);

void eth_task() {
  int len = 0;
  
  len = b_ethernet_rx(rx_packet);
  if (len) {
    //printf("eth rx: %d\n",len);
    //b_system_misc(debug_dump_mem, rx_packet, len);
    //printf("\n");

    eth_header* e = (eth_header*)rx_packet;

    //printf("ETH type: %x\n",swap16(e->type));

    if (swap16(e->type) == ETH_TYPE_ARP) {
      arp_packet* arp = (arp_packet*)(((uint8_t*)e)+14);

      if (swap16(arp->opcode) == ARP_REQUEST) {
        uint8_t* sip = arp->sender_ip;
        uint8_t* tip = arp->target_ip;
        printf("arp opcode: %x %d.%d.%d.%d looking for: %d.%d.%d.%d\n",swap16(arp->opcode),
               sip[0],sip[1],sip[2],sip[3],
               tip[0],tip[1],tip[2],tip[3]);

        if (*(uint32_t*)tip == *(uint32_t*)my_ip) {
          // build response
          eth_header* te = (eth_header*)tx_packet;
          memcpy(te->dest_mac, arp->sender_mac, 6);
          memcpy(te->src_mac,  my_mac, 6);
          te->type = swap16(ETH_TYPE_ARP);
      
          arp_packet* ta = (arp_packet*)(((uint8_t*)te)+14);
          ta->hwtype = swap16(1);
          ta->proto = swap16(PROTO_IP);
          ta->hwsize = 6;
          ta->proto_size = 4;
          ta->opcode = swap16(ARP_REPLY);

          memcpy(ta->sender_mac, my_mac, 6);
          memcpy(ta->sender_ip,  arp->target_ip, 4);
          memcpy(ta->target_mac, arp->sender_mac, 6);
          memcpy(ta->target_ip,  arp->sender_ip, 4);

          printf("sending arp reply.\n");
          b_ethernet_tx(tx_packet, 14+28);

          // their mac address is probably the router :3
          memcpy(their_mac, e->src_mac, 6);
        }
      } else {
        //printf("unhandled arp opcode: %x\n",swap16(arp->opcode));
      }
    }
    else if (swap16(e->type) == ETH_TYPE_IP) {
      ipv4_packet* i4 = (ipv4_packet*)(((uint8_t*)e)+14);
      //printf("ip packet! proto: %x\n",i4->proto);

      if (i4->proto == PROTO_IP_UDP) {
        udp_packet* u = (udp_packet*)(&i4->data);
        uint8_t* sip = i4->src_ip;

        // TODO: we need an arp table
        memcpy(their_mac, e->src_mac, 6);
        
        //printf("got udp packet from %d.%d.%d.%d. size: %d\n",sip[0],sip[1],sip[2],sip[3], swap16(u->size)-8);
        //printf("> %s\n",&u->data);

        memcpy(udp_cell->addr, &u->data, 1500);
        udp_cell->size = u->size-8;
      }
      else if (i4->proto == PROTO_IP_TCP) {
        tcp_packet* rxt = (tcp_packet*)(&i4->data);

        int payload_size = swap16(i4->size)-20-20;

        if (rxt->flags == (TCP_SYN|TCP_ACK)) {
          // reply to SYN ACK

          my_seqnum++;
          their_seqnum = swap32(rxt->seqnum)+1;
          //printf("REPLY TO SYN ACK: %x\n",rxt->flags);
          send_tcp_packet(swap16(rxt->dest_port),swap16(rxt->src_port),TCP_ACK,my_seqnum,their_seqnum,NULL,0);
          
          if (my_tcp_connected_callback) {
            funcptr fn = (funcptr)my_tcp_connected_callback->next;
            return fn();
          }
        }
        else if ((rxt->flags&TCP_FIN) == TCP_FIN) {
          send_tcp_packet(swap16(rxt->dest_port),swap16(rxt->src_port),TCP_RST,my_seqnum,their_seqnum,NULL,0);
          
          my_seqnum++;
        }
        else if (payload_size>0) {
          // receive and reply to data
          uint32_t old_seqnum = their_seqnum;
          their_seqnum = swap32(rxt->seqnum)+payload_size;
          //printf("REPLY TO PSH acknum: %d\n",their_seqnum);
          send_tcp_packet(swap16(rxt->dest_port),swap16(rxt->src_port),TCP_ACK,my_seqnum,their_seqnum,NULL,0);

          memcpy(udp_cell->addr, &rxt->data, payload_size);
          udp_cell->size = payload_size;
          *((uint8_t*)udp_cell->addr+payload_size) = 0;

          if (old_seqnum != their_seqnum) {
            if (my_tcp_data_callback) {
              funcptr fn = (funcptr)my_tcp_data_callback->next;
              return fn(udp_cell);
            }
          } // else duplicate packet received
        }
      }
    }
  }
}

/* taken from TCP/IP Illustrated Vol. 2(1995) by Gary R. Wright and W. Richard
   Stevens. Page 236 */

uint16_t cksum(uint16_t* ip, int len){
  long sum = 0;  /* assume 32 bit long, 16 bit short */

  while(len > 1){
    uint16_t word = swap16(*ip);
    sum += word;
    ip++;
    if(sum & 0x80000000) {  /* if high order bit set, fold */
      sum = (sum & 0xFFFF) + (sum >> 16);
    }
    len -= 2;
  }

  if(len) {      /* take care of left over byte */
    sum += swap16((uint16_t) *(unsigned char *)ip);
  }
  
  while(sum>>16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  
  return ~sum;
}

uint16_t transport_cksum(void* packet, uint16_t protocol, ipv4_packet* i4, uint16_t len) {
  long sum = 0;  /* assume 32 bit long, 16 bit short */
  uint16_t* data = (uint16_t*)packet;

  uint16_t i = len;
  while(i > 1){
    uint16_t word = swap16(*data);
    sum += word;
    data++;
    if(sum & 0x80000000)   /* if high order bit set, fold */
      sum = (sum & 0xFFFF) + (sum >> 16);
    i -= 2;
  }

  if (i) {      /* take care of left over byte */
    uint16_t word = swap16((uint16_t) *(unsigned char *)data);
    sum += word;
  }

  // add pseudo header

  sum += swap16(((uint16_t*)i4->src_ip)[0]);
  sum += swap16(((uint16_t*)i4->src_ip)[1]);
  
  sum += swap16(((uint16_t*)i4->dest_ip)[0]);
  sum += swap16(((uint16_t*)i4->dest_ip)[1]);
  
  sum += protocol;
  sum += len;
  
  while(sum>>16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  
  return ~sum;
}

Cell* machine_send_udp(Cell* data_cell) {
  if (!data_cell || (data_cell->tag!=TAG_BYTES && data_cell->tag!=TAG_STR)) return alloc_error(ERR_INVALID_PARAM_TYPE);

  int len = data_cell->size;
  uint8_t* data = (uint8_t*)data_cell->addr;
  
  eth_header* te = (eth_header*)tx_packet;
  memcpy(te->dest_mac, their_mac, 6);
  memcpy(te->src_mac,  my_mac, 6);
  te->type = swap16(ETH_TYPE_IP);
      
  ipv4_packet* i4 = (ipv4_packet*)(((uint8_t*)te)+14);
  i4->version = (4<<4) | 5; // ipv4 + ihl5 (5*4 bytes header)
  i4->services = 0;
  i4->size = swap16(20+8+len); // patch later i4+udp
  i4->id = swap16(0xbeef);
  i4->flags = 0x40; // don't fragment
  i4->frag_offset = 0; // cargo cult
  i4->ttl = 64;
  i4->proto = PROTO_IP_UDP;
  i4->checksum = 0;

  memcpy(i4->src_ip, my_ip, 4);
  memcpy(i4->dest_ip, their_ip, 4);
  
  i4->checksum = swap16(cksum((uint16_t*)i4, 20));
  
  udp_packet* u = (udp_packet*)(&i4->data);
  u->dest_port = swap16(4001);
  u->src_port = swap16(4000);
  u->size = swap16(len+8);
  u->checksum = 0; // fixme

  if (data && len) {
    memcpy(&u->data, data, len);
  }
  
  int packet_len = len+8+20+14;  // data + udp + i4 + eth

  u->checksum = swap16(transport_cksum(u, PROTO_IP_UDP, i4, len+8));

  printf("sending udp packet.\n");
  b_ethernet_tx(tx_packet, packet_len);

  return alloc_int(1);
}

Cell* machine_bind_tcp(Cell* port_cell, Cell* fn_cell) {
}

void send_tcp_packet(int srcport, int port, uint8_t flags, uint32_t seqnum, uint32_t acknum, uint8_t* data, uint16_t size) {
  memset(tx_packet, 0, 64*1024);
  
  int len = size;
  
  eth_header* te = (eth_header*)tx_packet;
  memcpy(te->dest_mac, their_mac, 6);
  memcpy(te->src_mac,  my_mac, 6);
  te->type = swap16(ETH_TYPE_IP);
      
  ipv4_packet* i4 = (ipv4_packet*)(((uint8_t*)te)+14);
  i4->version = (4<<4) | 5; // ipv4 + ihl5 (5*4 bytes header)
  i4->services = 0;
  i4->size = swap16(20+20+len); // patch later i4+tcp
  i4->id = swap16(0xbeef);
  i4->flags = 0x40; // don't fragment
  i4->frag_offset = 0;
  i4->ttl = 64;
  i4->proto = PROTO_IP_TCP;
  i4->checksum = 0;

  memcpy(i4->src_ip, my_ip, 4);
  memcpy(i4->dest_ip, their_ip, 4);
  
  i4->checksum = swap16(cksum((uint16_t*)i4, 20));
  
  tcp_packet* t = (tcp_packet*)(&i4->data);
  t->dest_port = swap16(port);
  t->src_port = swap16(srcport);
  t->seqnum = swap32(seqnum);
  t->acknum = swap32(acknum);
  t->data_offset = (5<<4);
  t->flags = flags;
  t->window = swap16(0x7210);
  t->urg_pointer = 0;
  t->checksum = 0;

  memcpy(&t->data, data, len);
  
  t->checksum = swap16(transport_cksum(t, PROTO_IP_TCP, i4, len+5*4));
  
  //printf("sending tcp packet.\n");
  int packet_len = len+5*4+20+14;  // data + tcp + i4 + eth
  
  b_ethernet_tx(tx_packet, packet_len);
}

static int their_tcp_port = 8000;

Cell* machine_connect_tcp(Cell* host_cell, Cell* port_cell, Cell* connected_fn_cell, Cell* data_fn_cell) {
  if (!host_cell || (host_cell->tag!=TAG_BYTES && host_cell->tag!=TAG_STR)) return alloc_error(ERR_INVALID_PARAM_TYPE);
  if (!port_cell || (port_cell->tag!=TAG_INT)) return alloc_error(ERR_INVALID_PARAM_TYPE);
  
  my_tcp_connected_callback = connected_fn_cell;
  my_tcp_data_callback = data_fn_cell;

  their_tcp_port = port_cell->value;

  memcpy(their_ip,host_cell->addr,4);

  my_seqnum++;
  send_tcp_packet(my_tcp_port,port_cell->value,TCP_RST,my_seqnum,0,NULL,0);
  my_tcp_port++;
  my_seqnum+=10;
  send_tcp_packet(my_tcp_port,port_cell->value,TCP_SYN,my_seqnum,0,NULL,0);
  
  return alloc_int(1);
}

Cell* machine_send_tcp(Cell* data_cell) {
  if (!data_cell || (data_cell->tag!=TAG_BYTES && data_cell->tag!=TAG_STR)) return alloc_error(ERR_INVALID_PARAM_TYPE);

  send_tcp_packet(my_tcp_port,their_tcp_port,TCP_PSH|TCP_ACK,my_seqnum,their_seqnum,data_cell->addr,data_cell->size);
  my_seqnum+=data_cell->size;
  
  return alloc_int(1);
}


void init_bm_eth() {
  //b_system_config(networkcallback_set, (unsigned long int)eth_receive);
}

void memset64(void * dest, uint32_t val32, uintptr_t size)
{
  uint64_t value = (((uint64_t)val32)<<32) | ((uint64_t)val32);
  uintptr_t i;
  for(i = 0; i < (size & (~7)); i+=8)
  {
    memcpy(((char*)dest) + i, &value, 8);
  }
}

int machine_video_set_pixel(uint32_t x, uint32_t y, uint32_t color) {
  if (x>=bm_screen_w || y>=bm_screen_h) return 0;
  uint32_t offset;
  
  if (bm_screen_bpp == 24) {
    offset = (y*bm_screen_w+x) * 3;
    bm_framebuffer[offset] = color&0xff;
    bm_framebuffer[offset+1] = (color&0xff00)>>8;
    bm_framebuffer[offset+2] = (color&0xff0000)>>16;
  } else {
    offset = (y*bm_screen_w+x);
    ((uint32_t*)bm_framebuffer)[offset] = color;
  }

  return 1;
}

int machine_video_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
  uint32_t y1=y;
  uint32_t y2=y+h;
  uint32_t x2=x+w;
  
  for (; y1<y2; y1++) {
    uint32_t x1=x;
    for (; x1<x2; x1++) {
      machine_video_set_pixel(x1,y1,color);
    }
  }
}

int machine_video_flip() {
  memcpy(bm_vram,(uint32_t*)bm_framebuffer,bm_screen_w*bm_screen_h*(bm_screen_bpp/8));
  return 1;
}

static int kb_shift = 0;
static int kb_meta = 0;
static int kb_alt = 0;
static int kb_ctrl = 0;
static int kb_caps = 0;

int machine_get_key(int modifiers) {
  if (modifiers) {
    int map = kb_shift;
    if (kb_ctrl) map += 512;
    if (kb_meta) map += 1024;
    if (kb_alt)  map += 256;
    
    return map;
  } else {
    if (inportbyte(0x64) & 1) {
      
      int key = 0;
      key = inportbyte(0x60);
        
      //printf("key: %d\n",key);
        
      if (key == 0x2a) { kb_shift = 1; return 0; }
      if (key == 0xaa) { kb_shift = 0; return 0; }

      // right shift
      if (key == 54)  { kb_shift = 1; return 0; }
      if (key == 182) { kb_shift = 0; return 0; }

      if (key == 29)  { kb_meta = 1; return 0; }
      if (key == 157) { kb_meta = 0; return 0; }
        
      if (key == 56)  { kb_alt = 1; return 0; }
      if (key == 184) { kb_alt = 0; return 0; }
        
      if (key == 29)  { kb_ctrl = 1; return 0; }
      if (key == 219) { kb_ctrl = 0; return 0; }
        
      if (key == 58)  { kb_caps = 1; return 0; }
      if (key == 186) { kb_caps = 0; return 0; }
      
      if (key>127) return 0;
      
      // convert to SDL scancodes
      if (key!=0) {
        switch (key) {
          case 75:
            key = 80; break; // left
          case 77:
            key = 79; break; // right
          case 72:
            key = 82; break; // top
          case 80:
            key = 81; break; // bottom
          case 14:
            key = 42; break; // backspace
          case 57: 
            key = 29; break; // space
          case 51: 
            key = 41; break; // escape
          default:
            break;
        }
      }
      
      return key;
    }
    return 0;
  }
}

Cell* machine_poll_udp() {
  eth_task();
  return udp_cell;
}

Cell* machine_save_file(Cell* cell, char* path) {
  /*printf("about to save: %s\n",path);
  if (cell->tag == TAG_STR || cell->tag == TAG_BYTES) {
    FILE* f = fopen(path,"wb");
    if (!f) return alloc_error(ERR_FORBIDDEN);
    
    fwrite(cell->addr, 1, cell->size, f);
    fclose(f);
    return alloc_int(1);
  } else {
    printf("error: cannot save tag %d\n",cell->tag);
  }*/
  return alloc_int(0);
}

static char sysfs_tmp[1024];

Cell* machine_load_file(char* path) {
  //printf("about to load: %s\n",path);
  //struct stat st;
  //if (stat(path, &st)) return alloc_error(ERR_NOT_FOUND);

  //if (st.st_size < 1) return alloc_bytes(); // zero-byte file

  // sysfs
  if (!strcmp(path,"/sys/mem")) {
    MemStats* mst = alloc_stats();
    sprintf(sysfs_tmp, "(%d %d)", mst->stack_bytes_used, mst->stack_bytes_max);
    return read_string(sysfs_tmp);
  }

  uint32_t f = b_file_open(path);

  if (f) {
    Cell* result_cell = alloc_num_bytes(2*1024*1024); // st.st_size
  
    uint32_t res = b_file_read(f, result_cell->addr, 2*1024*1024);
    return result_cell;
  } else {
    return alloc_error(ERR_FORBIDDEN);
  }       
}

static struct timeval tm1;

static inline void start_clock()
{
  gettimeofday(&tm1, NULL);
}

static inline void stop_clock()
{
  struct timeval tm2;
  gettimeofday(&tm2, NULL);

  unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
  //printf("%llu ms\n", t);
}

static char* _qwertzuiop = "qwertzuiop"; // 0x10-0x1c
static char* _asdfghjkl = "asdfghjkl";
static char* _yxcvbnm = "yxcvbnm";
static char* _num = "1234567890+";
static char* _num_shifted = "!\"+-%&/()=-";

uint8_t keyboard_to_ascii(uint8_t key)
{
  if (key == 0x1C) return '\n';
  if (key == 0x39) return ' ';
  if (key == 0xE)  return '\n';
  if (key == 0xB4) return '.';
  if (key == 0xB5) return '/';

  if (key == 0x2a) { kb_shift = 1; return 0; }
  if (key == 0xaa) { kb_shift = 0; return 0; }
  
  if (key >= 0x2 && key <= 0xc) {
    if (kb_shift) {
      return _num_shifted[key - 0x2];
    }
    else return _num[key - 0x2];
  }
  if (key >= 0x10 && key <= 0x1C)
  {
    return _qwertzuiop[key - 0x10];
  }
  else if (key >= 0x1E && key <= 0x26)
  {
    return _asdfghjkl[key - 0x1E];
  }
  else if (key >= 0x2C && key <= 0x32)
  {
    return _yxcvbnm[key - 0x2C];
  }
  return 0;
}

static char last_c = 0;
void poll_key(char* inbuf) {
  int i = 0;
  while(1) {
    if (inportbyte(0x64) & 1) {
      char c = keyboard_to_ascii(inportbyte(0x60));
      //printf("%x\n",c);
      //c=0;
      if (c>0) {
        printf("%c",c);
        fflush(stdout);
        last_c = c;
        
        inbuf[i++] = c;
        inbuf[i] = 0;

        if (c=='\n') {
          return;
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  Cell* ptest = NULL;
  char* boot_buffer = malloc(2*1024*1024);
  char* in_line = malloc(2*1024*1024);
  char* in_buffer = malloc(2*1024*1024);
  int len = 0;
  
  printf("boot...\n");
  //init_bm_eth();

  rx_packet = malloc(1500);
  tx_packet = malloc(1500);
  
  init_bm_video();
  init_compiler();
  
  udp_cell = alloc_num_bytes(65535);
  
  printf("welcome to sledge/minilisp x86/64 (c)2015 mntmn.\n");

  long count = 0;  
  int fullscreen = 0;
  
  int in_offset = 0;
  int parens = 0;

  int jit_inited = 0;

  int booted = 0;
  int last_boot_idx = 0;
  int boot_idx = 0;
  int linec = 0;

  while (1) {
    ptest = NULL;
    
    //printf("sledge> ");
    fflush(stdout);
    //getdelim(&in_line, &len, '\n', stdin);

    if (booted<2) {
      if (booted<1) {
        uint32_t bootf = b_file_open("editor.l");

        printf("editor.l: %lx\n",bootf);
        if (bootf) {
          uint32_t res = b_file_read(bootf, boot_buffer, 4000);
          printf("editor.l read: %ld\n",res);
          
          b_file_close(bootf);
        } else {
          //fgets(in_line, 10240, stdin);
        }
        booted++;
      }

      int boot_done = 0;

      // compile boot buffer line per line until done
      last_boot_idx = boot_idx;
      while (boot_buffer[boot_idx]!=10) {
        if (boot_buffer[boot_idx] == 0) {
          boot_done = 1;
          break;
        }
        boot_idx++;
      }
      boot_idx++;
      
      if (boot_done) {
        booted++;
        printf("boot.l file read.\n");
      } else {
        strncpy(in_line, boot_buffer+last_boot_idx, boot_idx-last_boot_idx);
        in_line[boot_idx-last_boot_idx] = 0;
      }
    } else {
      //poll_key(in_line);
    }
    
    len = strlen(in_line);

    // recognize parens
    
    int i;
    for (i=0; i<len; i++) {
      if (in_line[i] == '(') {
        parens++;
      } else if (in_line[i] == ')') {
        parens--;
      }
    }

    //printf("parens: %d in_offset: %d\n",parens,in_offset);

    if (len>1) {
      strncpy(in_buffer+in_offset, in_line, len-1);
      in_buffer[in_offset+len-1] = 0;
      
      //printf("line: '%s' (%d)\n",in_buffer,strlen(in_buffer));
      
      linec++;
      //if (linec>10) while (1) {};
    }
    
    if (parens>0) {
      //printf("...\n");
      in_offset+=len-1;
    } else {
      if (len>1) {
        ptest = read_string(in_buffer);
        in_offset=0;
      }
    }
    //printf("parens: %d offset: %d\n",parens,in_offset);
    
    jit_node_t  *in;
    funcptr     compiled;
    
    //printf("ptest: %p\n",ptest);

    if (ptest) {

      if (!jit_inited) { 
        init_jit("");
        jit_inited = 1;
      }

      _jit = jit_new_state();
      
      jit_prolog();
      
      stack_ptr = stack_base = jit_allocai(1024 * sizeof(int)); //malloc(1024*sizeof(unsigned long long)); //
      //printf("stack_ptr: %x\n",stack_ptr);
    
      int success = compile_arg(JIT_R0, ptest, TAG_ANY);

      if (!success) {
        printf("<HALT\n");
        while(1) {};
      }
      
      jit_retr(JIT_R0);
  
      compiled = jit_emit();
      //jit_clear_state();

      //jit_disassemble();

      //start_clock();

      //printf("compiled: %p\n",compiled);

      Cell* res = (Cell*)compiled();

      // TODO: move to write op
      if (!res) {
        printf("null\n");
      } else {
        char out_buf[1024*10];
        lisp_write(res, out_buf, 1024*10);
        printf("%s\n",out_buf);
      }
      
      //stop_clock();
      
      jit_clear_state();
      jit_destroy_state();
    } else {
    }

    //sdl_mainloop();
  }
  finish_jit();
  return 0;
}
