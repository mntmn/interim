
// Bomberjacket OS R3D Engine

#define R3D_OVERSPILL_SIZE 10*1024
#define NUM_CTL_LISTS 64
#define CTL_BLOCK_SIZE 4*1024
#define R3D_BIN_SIZE 0x80000

typedef struct nv_vertex_t {
  uint16_t x;
  uint16_t y;
  float z;
  float w;
  float r;
  float g;
  float b;
} nv_vertex_t;

