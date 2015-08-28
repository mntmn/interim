#include <stdint.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

#include "raspi.h"
#include "vc4.h"
#include "vc4_control_list.h"
#include "r3d.h"

#define aligned_alloc memalign
#define R3D_ALIGN 256

static uint8_t* r3d_bin_address;
static uint8_t* r3d_bin_base;

static uint8_t* r3d_bin_ctl_lists;
static uint8_t* r3d_render_ctl_lists;
static uint8_t* r3d_vertex_lists;
static uint8_t* r3d_shader_states;

static uint8_t* r3d_overspill_mem;

static uint8_t* r3d_gouraud_shader;

static uint8_t cl_idx;

static int r3d_tile_rows;
static int r3d_tile_cols;

static uint32_t* FB;

static uint8_t* control_list_bin;
static uint8_t* control_list_bin_end;

static uint8_t* control_list_render;
static uint8_t* control_list_render_end;

static uint8_t* next_shader_state;
static nv_vertex_t* next_triangles;
static int next_num_triangles;

// linux driver: https://github.com/anholt/linux/blob/vc4-kms-v3d/drivers/gpu/drm/vc4/vc4_gem.c
// mesa: http://cgit.freedesktop.org/mesa/mesa/tree/src/gallium/drivers/vc4/vc4_draw.c

void r3d_write_binning_list(uint8_t* cl) {
  // get next binning control list ---------------------------------------------------------------
  
  control_list_bin = cl;
  
  cl += vc4_tile_binning_mode_conf(cl, (uint32_t)r3d_bin_address, R3D_BIN_SIZE, (uint32_t)r3d_bin_base,
                                   r3d_tile_cols, r3d_tile_rows, vc4_auto_initialise_tile_state_data_array);
  cl += vc4_start_tile_binning(cl);
  cl += vc4_clip_window(cl, 0,0,1920,1080);
  cl += vc4_configuration_bits(cl, vc4_enable_forward_facing_primitive + vc4_enable_reverse_facing_primitive, 0); // vc4_early_z_updates_enable
  cl += vc4_viewport_offset(cl, 0,0);
  
  //cl += vc4_coordinate_array_primitives(cl, vc4_primitives_type_triangles, 3*NUM_TRIS, (uint32_t)triangles);

  // confirmed that this order is correct
  cl += vc4_nv_shader_state(cl, (uint32_t)next_shader_state);
  cl += vc4_vertex_array_primitives(cl, vc4_mode_triangles, 3 * next_num_triangles, 0); 

  cl += vc4_flush_all_state(cl);
  control_list_bin_end = cl;
}

void r3d_write_render_list(uint8_t* cl) {
  control_list_render = cl;
  uint32_t clear_color = 0xffffffff;
  
  cl += vc4_clear_colors(cl, clear_color, clear_color, 0, 0, 0);
  cl += vc4_tile_rendering_mode_conf(cl, (uint32_t)FB, 1920, 1080, vc4_frame_buffer_color_format_rgba8888);

  // these are optional instructions for hardware thread synchronization
  //cl += vc4_wait_on_semaphore(cl);
  
  // unclear if necessary
  //cl += vc4_nv_shader_state(cl, (uint32_t)next_shader_state);
  //cl += vc4_vertex_array_primitives(cl, vc4_mode_triangles, 3 * next_num_triangles, 0);
  
  cl += vc4_tile_coordinates(cl, 0,0);
  cl += vc4_store_tile_buffer_general(cl, 0, 0, 0); // disable double buffer swap 1<<4 in second zero arg

  //cl += vc4_reserved2(cl); // from bcm ghw_composer_impl.cpp createRendList(), supposed to be a MARK

  // assumes 1920x1080
  r3d_tile_rows = 16;
  r3d_tile_cols = 30;

  for (int y = 0; y < r3d_tile_rows; y++) {
    for (int x = 0; x < r3d_tile_cols; x++) {
      cl += vc4_tile_coordinates(cl, x,y);
      cl += vc4_branch_to_sublist(cl, (uint32_t)r3d_bin_address + ((y * r3d_tile_cols + x) * 32)); // sublists are 32 bytes long
      if (y!=r3d_tile_rows-1 || x!=r3d_tile_cols-1) {
        cl += vc4_store_multi_sample(cl);
      } else {
        //cl += vc4_store_multi_sample(cl);
      }
    }
  }
  cl += vc4_store_multi_sample_end(cl); // end of frame
  control_list_render_end = cl;
}

// clear_color is RGBA8888
void r3d_init(uint32_t* fb) {
  FB = fb;
  
  // RENDER CONTROL LIST -------------------------------------------------

  r3d_bin_address = (uint8_t*)aligned_alloc(R3D_ALIGN, R3D_BIN_SIZE);
  r3d_bin_base = (uint8_t*)aligned_alloc(R3D_ALIGN, R3D_BIN_SIZE);
  
  //r3d_render_ctl_list = (uint8_t*)aligned_alloc(R3D_ALIGN, CTL_BLOCK_SIZE * 10); // FIXME: enough?
  //uint8_t* cl = r3d_render_ctl_list;

  printf("-- bin address at %p\r\n",r3d_bin_address);
  printf("-- bin base at %p\r\n",r3d_bin_base);
  //printf("-- render control list at %p\r\n",r3d_render_ctl_list);
  
  r3d_overspill_mem = aligned_alloc(R3D_ALIGN,R3D_OVERSPILL_SIZE);

  r3d_bin_ctl_lists = (uint8_t*)aligned_alloc(R3D_ALIGN, CTL_BLOCK_SIZE * NUM_CTL_LISTS);
  r3d_render_ctl_lists = (uint8_t*)aligned_alloc(R3D_ALIGN, CTL_BLOCK_SIZE * NUM_CTL_LISTS);
  r3d_vertex_lists = (uint8_t*)aligned_alloc(R3D_ALIGN, CTL_BLOCK_SIZE * NUM_CTL_LISTS);
  r3d_shader_states = (uint8_t*)aligned_alloc(R3D_ALIGN, CTL_BLOCK_SIZE * NUM_CTL_LISTS);
  
  r3d_gouraud_shader = (uint8_t*)aligned_alloc(R3D_ALIGN, CTL_BLOCK_SIZE * 1);
  vc4_gouraud_shader(r3d_gouraud_shader);

  r3d_write_render_list(r3d_render_ctl_lists);

  cl_idx = -1;
}

nv_vertex_t* r3d_init_frame() {  
  // reset and stop binning and render threads
  //*((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT0CS)) = (1<<15) | (1<<5);
  //*((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT1CS)) = (1<<15) | (1<<5);

  // advance lists index (cycle through lists)
  cl_idx++;
  if (cl_idx>(NUM_CTL_LISTS-1)) cl_idx = 0;
  //printf("~~ cl_idx: %d\r\n",cl_idx);

  next_shader_state = r3d_shader_states+cl_idx*CTL_BLOCK_SIZE;
  next_triangles = (nv_vertex_t*)(r3d_vertex_lists+cl_idx*CTL_BLOCK_SIZE);
  return next_triangles;
}

void r3d_triangles(int num_triangles, nv_vertex_t* triangles) {
  next_triangles = triangles;
  next_num_triangles = num_triangles;
  
  //vc4_shader_state_record(flat_shader_state, flat_shader);
  vc4_nv_shader_state_record(next_shader_state, r3d_gouraud_shader, 3, 6*4, (uint8_t*)next_triangles);
}

extern void khrn_hw_full_memory_barrier(void);

void r3d_render_frame(uint32_t clear_color) {
  arm_dmb();
  arm_dsb();
  arm_isb();
  
  r3d_write_binning_list(r3d_bin_ctl_lists+cl_idx*CTL_BLOCK_SIZE);
  
  arm_invalidate_data_caches();
  arm_dmb();
  //khrn_hw_full_memory_barrier();

  // reset binning and frame flush counters
  *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_BFC)) = 1;
  *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_RFC)) = 1;
  
  
  // submit binning list
  *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT0CA)) = (uint32_t)control_list_bin;
  *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT0EA)) = (uint32_t)control_list_bin_end;
  
  //khrn_hw_full_memory_barrier();

  arm_dmb();
  //printf("~~ submitted binning list %p-%p\r\n",control_list_bin,control_list_bin_end);
  
  //cl = r3d_render_ctl_lists+cl_idx*CTL_BLOCK_SIZE;
  
  // submit render
  
  *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT1CA)) = (uint32_t)control_list_render;
  *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT1EA)) = (uint32_t)control_list_render_end;

  arm_dmb();
  //printf("~~ submitted rendering list %p-%p\r\n",control_list_render,control_list_render_end);
  
  uint32_t bfc = 0;
  /*do {
    //printf("bfc loop\r\n");
    arm_dmb();
    r3d_debug_gpu();
    bfc = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_BFC));
  } while (bfc==0);
  */
  uint32_t rfc = 0;
  /*do {
    //printf("rfc loop\r\n");
    arm_dmb();
    r3d_debug_gpu();
    rfc = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_RFC));
    } while (rfc==0);*/

  uint32_t ct1cs = 0x20;

  int timeout = 0;
  do {
    timeout++;
    if (timeout>1000*1000) break;
    ct1cs = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT1CS));
  } while (ct1cs & 0x20);
  
  //printf("~~ r3d timeout: %d\r\n",timeout);
}

void r3d_debug_gpu() {
  uint32_t dbge, fdbgo, fdbgr, fdbgs, bfc, errstat, pcs, status0, status1, rfc;

  arm_dmb();
  arm_dsb();
  
  dbge = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_DBGE));
  fdbgo = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_FDBGO));
  fdbgr = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_FDBGR));
  fdbgs = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_FDBGS));
  bfc = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_BFC));
  rfc = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_RFC));
  errstat = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_ERRSTAT));
  pcs = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_PCS));
  status0 = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT0CS));
  status1 = *((volatile uint32_t*)(PERIPHERAL_BASE + V3D_BASE + V3D_CT1CS));
    
  //printf("-- BFC: 0x%x RFC: 0x%x PCS: 0x%x ERRST: 0x%x DBGE: 0x%x DBGO: 0x%x DBGR: 0x%x DBGS: 0x%x ST0: 0x%x ST1: 0x%x\r\n",bfc,rfc,pcs,errstat,dbge,fdbgo,fdbgr,fdbgs,status0,status1);
}
