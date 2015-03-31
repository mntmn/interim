#include <stdint.h>

#include "raspi.h"
#include "vc4.h"
#include "vc4_control_list.h"
#include "r3d.h"

static uint8_t* r3d_bin_address;
static uint8_t* r3d_bin_base;

static uint8_t* r3d_render_ctl_list;
static uint8_t* r3d_render_ctl_list_end;

static uint8_t* r3d_overspill_mem;

static uint8_t* r3d_gouraud_shader;

static uint8_t cl_idx;

static int tile_rows;
static int tile_cols;

// clear_color is RGBA8888
void r3d_init(uint32_t clear_color) {
  // RENDER CONTROL LIST -------------------------------------------------

  r3d_bin_address = (uint8_t*)aligned_alloc(128, R3D_BIN_SIZE);
  r3d_bin_base = (uint8_t*)aligned_alloc(128, R3D_BIN_SIZE);
  
  r3d_render_ctl_list = (uint8_t*)aligned_alloc(128, CTL_BLOCK_SIZE * 10); // FIXME: enough?
  uint8_t* cl = r3d_render_ctl_list;

  printf(buf, "-- render control list at %p\r\n",r3d_render_ctl_list);
  
  cl += vc4_clear_colors(cl, clear_color, clear_color, 0, 0, 0);
  cl += vc4_tile_rendering_mode_conf(cl, (uint32_t)FB, 1920, 1080, vc4_frame_buffer_color_format_rgba8888);
  cl += vc4_tile_coordinates(cl, 0,0);
  cl += vc4_store_tile_buffer_general(cl, 0,0,0);

  // these are optional instructions for hardware thread synchronization
  //cl += vc4_wait_on_semaphore(cl);
  //cl += vc4_reserved2(cl); // from bcm ghw_composer_impl.cpp createRendList(), supposed to be a MARK

  // assumes 1920x1080
  tile_rows = 16;
  tile_cols = 30;

  for (int y = 0; y < tile_rows; y++) {
    for (int x = 0; x < tile_cols; x++) {
      cl += vc4_tile_coordinates(cl, x,y);
      cl += vc4_branch_to_sublist(cl, (uint32_t)bin_address + ((y * tile_cols + x) * 32)); // sublists are 32 bytes long
      if (y!=tile_rows-1 || x!=tile_cols-1) {
        cl += vc4_store_multi_sample(cl);
      } else {
        cl += vc4_store_multi_sample_end(cl); // end of frame
      }
    }
  }

  r3d_render_ctl_list_end = cl;

  r3d_overspill_mem = malloc(R3D_OVERSPILL_SIZE);

  r3d_bin_ctl_lists = (uint8_t*)aligned_alloc(128, CTL_BLOCK_SIZE * NUM_CTL_LISTS);
  r3d_vertex_lists = (uint8_t*)aligned_alloc(128, CTL_BLOCK_SIZE * NUM_CTL_LISTS);
  r3d_shader_states = (uint8_t*)aligned_alloc(128, CTL_BLOCK_SIZE * NUM_CTL_LISTS);

  vc4_gouraud_shader(r3d_gouraud_shader);

  cl_idx = 0;
}

static uint8_t* next_shader_state;

void r3d_init_frame() {
  // advance lists index (cycle through lists)
  cl_idx++;
  if (cl_idx>(NUM_CTL_LISTS-1)) cl_idx = 0;
  printf("~~ cl_idx: %d\r\n",cl_idx);

  next_shader_state = shader_states+cl_idx*CTL_BLOCK_SIZE;
}

static nv_vertex_t* next_triangles;
static int next_num_triangles;

void r3d_triangles(int num_triangles, nv_vertex_t* triangles) {
  next_triangles = triangles;
  next_num_triangles = num_triangles;
  
  //vc4_shader_state_record(flat_shader_state, flat_shader);
  vc4_nv_shader_state_record(next_shader_state, r3d_gouraud_shader, 3, 6*4, (uint8_t*)next_triangles);
}

void r3d_render_frame() {
  // reset binning and frame flush counters
  *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_BFC)) = 1;
  *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_RFC)) = 1;
    
  // reset and stop binning and render threads
  *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_CT0CS)) = (1<<15) | (1<<5);
  *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_CT1CS)) = (1<<15) | (1<<5);

  
  // get next binning control list
  uint8_t* cl = control_lists_bin+cl_idx*CTL_BLOCK_SIZE;
  uint8_t* control_list_bin = cl;

  // TODO: does the whole list have to be recreated every time?
  
  cl += vc4_tile_binning_mode_conf(cl, (uint32_t)r3d_bin_address, R3D_BIN_SIZE, (uint32_t)r3d_bin_base,
                                   r3d_tile_cols, r3d_tile_rows, vc4_auto_initialise_tile_state_data_array);
  cl += vc4_start_tile_binning(cl);
  cl += vc4_clip_window(cl, 0,0,1920,1080);
  cl += vc4_configuration_bits(cl, vc4_enable_forward_facing_primitive + vc4_enable_reverse_facing_primitive, vc4_early_z_updates_enable);
  cl += vc4_viewport_offset(cl, 0,0);
  cl += vc4_nv_shader_state(cl, (uint32_t)next_shader_state);
  //cl += vc4_coordinate_array_primitives(cl, vc4_primitives_type_triangles, 3*NUM_TRIS, (uint32_t)triangles);
  cl += vc4_vertex_array_primitives(cl, vc4_mode_triangles, 3 * next_num_triangles, 0);
  cl += vc4_flush(cl);
  uint8_t* control_list_bin_end = cl;
  
  // submit binning list
    
  *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_CT0CA)) = (uint32_t)control_list_bin;
  *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_CT0EA)) = (uint32_t)control_list_bin_end;

  do {
    bfc = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_BFC));
  } while (bfc==1);
    
  // submit render
    
  *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_CT1CA)) = (uint32_t)control_list_render;
  *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_CT1EA)) = (uint32_t)control_list_render_end;
    
  do {
    rfc = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_RFC));
  } while (rfc==1);
}

void r3d_debug_gpu() {
  uint32_t dbge, fdbgo, fdbgr, fdbgs, bfc, errstat, pcs, status0, status1, rfc;

  dbge = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_DBGE));
  fdbgo = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_FDBGO));
  fdbgr = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_FDBGR));
  fdbgs = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_FDBGS));
  bfc = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_BFC));
  rfc = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_RFC));
  errstat = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_ERRSTAT));
  pcs = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_PCS));
  status0 = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_CT0CS));
  status1 = *((volatile uint32_t*)(peripheral_base + V3D_BASE + V3D_CT1CS));
    
  printf(buf, "-- BFC: 0x%x RFC: 0x%x PCS: 0x%x ERRST: 0x%x DBGE: 0x%x DBGO: 0x%x DBGR: 0x%x DBGS: 0x%x ST0: 0x%x ST1: 0x%x\r\n",bfc,rfc,pcs,errstat,dbge,fdbgo,fdbgr,fdbgs,status0,status1);
}
