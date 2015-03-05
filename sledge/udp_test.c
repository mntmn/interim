#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#define ETH_TYPE_ARP 0x0806
#define ETH_TYPE_IP 0x0800
#define PROTO_IP 0x800
#define ARP_REQUEST 1
#define ARP_REPLY 2
#define PROTO_IP_UDP 0x11

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

uint16_t swap16(uint16_t in) {
  uint16_t out = in<<8 | ((in&0xff00)>>8);
  return out;
}

char broadcast_mac[] = {0xff,0xff,0xff,0xff,0xff,0xff};
char my_mac[] = {0x52,0x54,0xde,0xad,0xbe,0xef};
char their_mac[] = {0xb6,0xc0,0x8e,0x06,0x82,0xdf,0x52,0x54};

char my_ip[] = {10,0,0,4};
char their_ip[] = {10,0,0,1};

char* tx_packet;


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


uint16_t udp_cksum(udp_packet* udp, ipv4_packet* i4, uint16_t len){
  long sum = 0;  /* assume 32 bit long, 16 bit short */
  uint16_t* data = (uint16_t*)udp;

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
  
  sum += 0x11;
  sum += len;
  
  while(sum>>16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  
  return ~sum;
}



void machine_send_udp(char* sdata) {
  int len = strlen(sdata);
  uint8_t* data = (uint8_t*)sdata;
  
  eth_header* te = (eth_header*)tx_packet;
  memcpy(te->dest_mac, their_mac, 6);
  memcpy(te->src_mac,  my_mac, 6);
  te->type = swap16(ETH_TYPE_IP);
      
  ipv4_packet* i4 = (ipv4_packet*)(((uint8_t*)te)+14);
  i4->version = (4<<4) | 5; // ipv4 + ihl5 (5*4 bytes header)
  i4->services = 0;
  i4->size = swap16(20+8+len); // patch later i4+udp
  i4->id = swap16(0x0000);
  i4->flags = 0x40; // don't fragment
  i4->frag_offset = 0; // 
  i4->ttl = 0xff;
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

  memcpy(&u->data, data, len);
  
  int packet_len = len+8+20+14;  // data + udp + i4 + eth

  u->checksum = udp_cksum(u, i4, len+8);

  printf("sending udp packet.\n");
  //b_ethernet_tx(tx_packet, packet_len);

  printf("checksum: %d %x\n",u->checksum,u->checksum);

  for (int i=0; i<4; i++) {
    uint8_t* b1 = ((uint8_t*)tx_packet)+i*16;
    uint8_t* b2 = ((uint8_t*)tx_packet)+i*16+8;
  
    printf("%02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x\n",
           b1[0],b1[1],b1[2],b1[3],b1[4],b1[5],b1[6],b1[7],
           b2[0],b2[1],b2[2],b2[3],b2[4],b2[5],b2[6],b2[7]);
  }
}

void main() {
  tx_packet = malloc(1024);
  machine_send_udp("hello world");
}
