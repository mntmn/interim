#include <stdio.h>
#include <stdint.h>
#include "sledge/minilisp.h"
#include "sledge/alloc.h"

// mini-ip is part of interim OS, written in 2015 by mntmn.

#define ETH_TYPE_ARP 0x0806
#define ETH_TYPE_IP 0x0800
#define PROTO_IP 0x800
#define ARP_REQUEST 1
#define ARP_REPLY 2
#define PROTO_IP_UDP 0x11
#define PROTO_IP_TCP 0x6
#define PROTO_IP_ICMP 0x1

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

typedef struct icmp_packet {
  uint8_t type;
  uint8_t code;
  uint16_t checksum;
  uint16_t identifier;
  uint16_t seqnum;
  uint8_t data;
} icmp_packet;

// no swapping on ARM
uint16_t swap16(uint16_t in) {
  uint16_t out = in<<8 | ((in&0xff00)>>8);
  return out;
  //return in;
}

uint32_t swap32(uint32_t in) {
  uint32_t out = in<<24 | ((in&0xff00)<<8) | ((in&0xff0000)>>8) | ((in&0xff000000)>>24);
  return out;
  //return in;
}

char broadcast_mac[] = {0xff,0xff,0xff,0xff,0xff,0xff};
char my_mac[] = {0xde,0xad,0xbe,0xef,0x02,0x42};
char their_mac[] = {0xff,0xff,0xff,0xff,0xff,0xff};

char my_ip[] = {192,168,1,242};
char their_ip[] = {192,168,1,1};
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

extern int  ethernet_rx(uint8_t* packet);
extern void ethernet_tx(uint8_t* packet, int len);

void send_tcp_packet(int srcport, int port, uint8_t flags, uint32_t seqnum, uint32_t acknum, uint8_t* data, uint16_t size);

void init_mini_ip() {
  //udp_cell = buffer_cell;
  rx_packet = malloc(64*1024);
  tx_packet = malloc(64*1024);
  my_tcp_connected_callback = NULL;
  my_tcp_data_callback = NULL;
  printf("-- init_mini_ip done.\r\n");
}

uint16_t read_word16(void* ptr) {
  uint8_t* p = ptr;
  return ((uint16_t)p[0])|(p[1]<<8);
}

uint32_t read_word32(void* ptr) {
  uint8_t* p = ptr;
  return ((uint32_t)p[0])|(p[1]<<8)|(p[2]<<16)|(p[3]<<24);
}

void write_word16(void* p, uint16_t value) {
  uint8_t* buf = (uint8_t*)p;
  *(buf)=value&0xff;
  *(buf+1)=(value&0xff00)>>8;
}

void write_word32(void* p, uint32_t value) {
  uint8_t* buf = (uint8_t*)p;
  *(buf)=value&0xff;
  *(buf+1)=(value&0xff00)>>8;
  *(buf+2)=(value&0xffff00)>>16;
  *(buf+3)=(value&0xffffff00)>>24; 
}

int compare_ip(uint8_t* a, uint8_t* b) {
  return 0;
}

/*uint32_t write32(uint8_t* p) {
  return (p[0])|(p[1]<<8)|(p[2]<<16)|(p[3]<<24);
  }*/

Cell* eth_task() {
  int len = 0;

  len = ethernet_rx(rx_packet);
  if (len) {
    printf("[eth_task] rx: %d\r\n",len);
    //b_system_misc(debug_dump_mem, rx_packet, len);
    //printf("\n");

    eth_header* e = (eth_header*)rx_packet;

    //printf("ETH type: %x\r\n",swap16(e->type));

    if (swap16(e->type) == ETH_TYPE_ARP) {
      arp_packet* arp = (arp_packet*)(((uint8_t*)e)+14);

      if (swap16(arp->opcode) == ARP_REQUEST) {
        uint8_t* sip = arp->sender_ip;
        uint8_t* tip = arp->target_ip;
        printf("arp opcode: %x %d.%d.%d.%d looking for: %d.%d.%d.%d\r\n",swap16(arp->opcode),
               sip[0],sip[1],sip[2],sip[3],
               tip[0],tip[1],tip[2],tip[3]);

        if (read_word32(tip) == read_word32(my_ip)) {
          //printf("[eth] (arp) they're talking to us.\r\n");
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

          //printf("sending arp reply.\r\n");
          ethernet_tx(tx_packet, 14+28);

          // their mac address is probably the router :3
          memcpy(their_mac, e->src_mac, 6);
        }
      } else {
        //printf("unhandled arp opcode: %x\n",swap16(arp->opcode));
      }
    }
    else if (swap16(e->type) == ETH_TYPE_IP) {
      ipv4_packet* i4 = (ipv4_packet*)(((uint8_t*)e)+14);
      printf("ip packet! proto: %x\n",i4->proto);

      if (i4->proto == PROTO_IP_UDP) {
        udp_packet* u = (udp_packet*)(&i4->data);
        uint8_t* sip = i4->src_ip;

        // TODO: we need an arp table
        memcpy(their_mac, e->src_mac, 6);

        int size = swap16(u->size)-8;
        printf("got UDP packet from %d.%d.%d.%d. size: %d\r\n",sip[0],sip[1],sip[2],sip[3], size);

        ((uint8_t*)&u->data)[size]=0;
        printf("> %s\r\n",&u->data);

        Cell* packet = alloc_num_bytes(size);
        memcpy(packet->ar.addr, &u->data, size);
        return packet;
      }
      else if (i4->proto == PROTO_IP_ICMP) {
        uint8_t* sip = i4->src_ip;
        uint8_t* dip = i4->dest_ip;
        printf("got ICMP packet from %d.%d.%d.%d -> %d.%d.%d.%d.\r\n",sip[0],sip[1],sip[2],sip[3],dip[0],dip[1],dip[2],dip[3]);

        icmp_packet* icmp = (icmp_packet*)(&i4->data);
        printf("ICMP type: %d id: seqnum: %d\r\n",icmp->type,icmp->identifier,icmp->seqnum);
      }
      else if (i4->proto == PROTO_IP_TCP) {
        tcp_packet* rxt = (tcp_packet*)(&i4->data);

        int payload_size = swap16(read_word16(&i4->size))-20-20;

        printf("got TCP packet, flags: %x, payload_size: %d\r\n",rxt->flags,payload_size);

        if (rxt->flags == (TCP_SYN|TCP_ACK)) {
          // reply to SYN ACK

          my_seqnum++;
          their_seqnum = swap32(read_word32(&rxt->seqnum))+1;
          printf("REPLY TO SYN ACK: %x\n",rxt->flags);
          send_tcp_packet(swap16(read_word16(&rxt->dest_port)),swap16(read_word16(&rxt->src_port)),TCP_ACK,my_seqnum,their_seqnum,NULL,0);
          
          /*if (my_tcp_connected_callback) {
            funcptr fn = (funcptr)my_tcp_connected_callback->next;
            fn();
            return;
            }*/
        }
        else if ((rxt->flags&TCP_FIN) == TCP_FIN) {
          send_tcp_packet(swap16(read_word16(&rxt->dest_port)),swap16(read_word16(&rxt->src_port)),TCP_RST,my_seqnum,their_seqnum,NULL,0);
          
          my_seqnum++;
        }
        else if (payload_size>0) {
          // receive and reply to data
          uint32_t old_seqnum = their_seqnum;
          their_seqnum = swap32(read_word32(&rxt->seqnum))+payload_size;
          //printf("REPLY TO PSH acknum: %d\r\n",their_seqnum);
          send_tcp_packet(swap16(read_word16(&rxt->dest_port)),swap16(read_word16(&rxt->src_port)),TCP_ACK,my_seqnum,their_seqnum,NULL,0);

          printf("got data, copying payload size %d\r\n",payload_size);
          
          Cell* packet = alloc_num_bytes(payload_size+1);
          memcpy(packet->ar.addr, &rxt->data, payload_size);
          *((uint8_t*)packet->ar.addr+payload_size) = 0;

          if (old_seqnum != their_seqnum) {
            /*if (my_tcp_data_callback) {
              funcptr fn = (funcptr)my_tcp_data_callback->next;
              fn(udp_cell);
              return;
              }*/
            return packet;
          } // else duplicate packet received
        }
      }
    }
  }
  return alloc_num_bytes(0);
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

  int len = data_cell->dr.size;
  uint8_t* data = (uint8_t*)data_cell->ar.addr;
  
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

  printf("sending udp packet.\r\n");
  ethernet_tx(tx_packet, packet_len);

  return alloc_int(1);
}

Cell* machine_bind_tcp(Cell* port_cell, Cell* fn_cell) {
}

void send_tcp_packet(int srcport, int port, uint8_t flags, uint32_t seqnum, uint32_t acknum, uint8_t* data, uint16_t size) {
  memset(tx_packet, 0, 64*1024);

  // hardcoded router
  their_mac[0] = 0x84;
  their_mac[1] = 0xa8;
  their_mac[2] = 0xe4;
  their_mac[3] = 0x67;
  their_mac[4] = 0x08;
  their_mac[5] = 0x12;
  
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
  write_word32(&t->seqnum, swap32(seqnum));
  write_word32(&t->acknum, swap32(acknum));
  t->data_offset = (5<<4);
  t->flags = flags;
  write_word16(&t->window, swap16(0x7210));
  t->urg_pointer = 0;
  t->checksum = 0;

  memcpy(&t->data, data, len);
  
  t->checksum = swap16(transport_cksum(t, PROTO_IP_TCP, i4, len+5*4));
  
  int packet_len = len+5*4+20+14;  // data + tcp + i4 + eth
  
  printf("sending tcp packet (%d).\n",packet_len);
  ethernet_tx(tx_packet, packet_len);
}

static int their_tcp_port = 8000;

int connect_tcp(char* host_ip, int port) {
  //if (!host_cell || (host_cell->tag!=TAG_BYTES && host_cell->tag!=TAG_STR)) return alloc_error(ERR_INVALID_PARAM_TYPE);
  //if (!port_cell || (port_cell->tag!=TAG_INT)) return alloc_error(ERR_INVALID_PARAM_TYPE);
  
  //my_tcp_connected_callback = connected_fn_cell;
  //my_tcp_data_callback = data_fn_cell;

  their_tcp_port = port;

  memcpy(their_ip,host_ip,4);

  my_seqnum++;
  send_tcp_packet(my_tcp_port,port,TCP_RST,my_seqnum,0,NULL,0);
  my_tcp_port++;
  my_seqnum+=10;
  send_tcp_packet(my_tcp_port,port,TCP_SYN,my_seqnum,0,NULL,0);
  
  return 1;
}

Cell* send_tcp(Cell* data_cell) {
  if (!data_cell || (data_cell->tag!=TAG_BYTES && data_cell->tag!=TAG_STR)) return alloc_error(ERR_INVALID_PARAM_TYPE);

  send_tcp_packet(my_tcp_port,their_tcp_port,TCP_PSH|TCP_ACK,my_seqnum,their_seqnum,data_cell->ar.addr,data_cell->dr.size);
  my_seqnum+=data_cell->dr.size;
  
  return alloc_int(1);
}

