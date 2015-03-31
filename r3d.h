#ifndef R3D_H
#define R3D_H
#include <stdint.h>

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

void r3d_init(uint32_t clear_color);
void r3d_init_frame();
void r3d_triangles(int num_triangles, nv_vertex_t* triangles);
void r3d_render_frame();
void r3d_debug_gpu();

#endif
