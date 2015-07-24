/*
VideoCore IV
Control ID Codes
*/

// we need these because we can't store words in odd addresses with pointer arithmetic, this will crash the device

#define str16(buf,value) *(buf)=value&0xff; *(buf+1)=(value&0xff00)>>8;
#define str32(buf,value) *(buf)=value&0xff; *(buf+1)=(value&0xff00)>>8; *(buf+2)=(value&0xffff00)>>16; *(buf+3)=(value&0xffffff00)>>24; 

#define vc4_halt_op 0x00
#define vc4_nop_op  0x01
#define vc4_reserved2_op 0x02 // MARKER
#define vc4_reserved3_op 0x03 // RESET_MARKER_COUNT
#define vc4_flush_op 0x04
#define vc4_flush_all_state_op 0x05
#define vc4_start_tile_binning_op 0x06
#define vc4_increment_semaphore_op 0x07
#define vc4_wait_on_semaphore_op 0x08

#define vc4_repeat_start_marker_op 0x19
#define vc4_repeat_from_start_marker_op 0x20

#define vc4_branch_op 0x10 // followed by address
#define vc4_branch_to_sublist_op 0x11 // followed by address

int vc4_nop(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_nop_op;
  return 1;
}

int vc4_reserved2(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_reserved2_op;
  return 1;
}

int vc4_flush(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_flush_op;
  return 1;
}

int vc4_flush_all_state(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_flush_all_state_op;
  return 1;
}

int vc4_start_tile_binning(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_start_tile_binning_op;
  return 1;
}

int vc4_increment_semaphore(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_increment_semaphore_op;
  return 1;
}

int vc4_wait_on_semaphore(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_wait_on_semaphore_op;
  return 1;
}

int vc4_branch(uint8_t* buf, uint32_t addr){
  *((uint8_t*) (buf+0))  = vc4_branch_op;
  str32(buf+1,addr);
  return 5;
}

int vc4_branch_to_sublist(uint8_t* buf, uint32_t addr){
  *((uint8_t*) (buf+0))  = vc4_branch_to_sublist_op;
  str32(buf+1,addr);
  return 5;
}

#define vc4_return_from_sublist_op 0x12
#define vc4_store_multi_sample_op 0x18
#define vc4_store_multi_sample_end_op 0x19

int vc4_store_multi_sample(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_store_multi_sample_op;
  return 1;
}

int vc4_store_multi_sample_end(uint8_t* buf){
  *((uint8_t*) (buf+0))  = vc4_store_multi_sample_end_op;
  return 1;
}

#define vc4_disable_color_buffer_write 0x01
#define vc4_disable_z_stencil_buffer_write 0x02
#define vc4_disable_clear_on_write 0x04
#define vc4_last_tile_of_frame 0x08
#define vc4_memory_base_address 0xfffffff0
#define vc4_store_full_resolution 0x1a // followed by address + data control id data record word: memory address of tile (in multiples of 16 bytes) (bit 4..31), data record (bit 0..3)

#define vc4_disable_color_buffer_read 0x01
#define vc4_disable_z_stencil_buffer_read 0x02

//#define Memory_Base_Address 0xFFFFFFF0

#define vc4_re_load_full_resolution 0x1b // followed by address +data control id data record word: memory address of tile (in multiples of 16 bytes) (bit 4..31), data record (bit 0..3)

// store_tile_buffer_general: store tile buffer general control id code data record description
#define vc4_store_none      0x0000 // store_tile_buffer_general: buffer to store = none
#define vc4_store_color     0x0001 // store_tile_buffer_general: buffer to store = color
#define vc4_store_z_stencil 0x0002 // store_tile_buffer_general: buffer to store = z/stencil
#define vc4_store_z_only    0x0003 // store_tile_buffer_general: buffer to store = z-only
#define vc4_store_vg_mask   0x0004 // store_tile_buffer_general: buffer to store = vg-mask
#define vc4_store_full_dump 0x0005 // store_tile_buffer_general: buffer to store = full dump
#define vc4_format_raster 0x0000 // store_tile_buffer_general: format = raster format
#define vc4_format_t      0x0010 // store_tile_buffer_general: format = t-format
#define vc4_format_lt     0x0020 // store_tile_buffer_general: format = lt-format
#define vc4_mode_sample       0x0000 // store_tile_buffer_general: mode = sample 
#define vc4_mode_decimate_4x  0x0040 // store_tile_buffer_general: mode = decimate 4x
#define vc4_mode_decimate_16x 0x0080 // store_tile_buffer_general: mode = decimate 16x
#define vc4_color_format_rgba8888         0x0000 // store_tile_buffer_general: pixel color format = rgba8888
#define vc4_color_format_bgr565_dithered  0x0100 // store_tile_buffer_general: pixel color format = bgr565 dithered
#define vc4_color_format_bgr565_no_dither 0x0200 // store_tile_buffer_general: pixel color format = bgr565 no dither
#define vc4_disable_double_buffer_swap     0x1000 // store_tile_buffer_general: disable double-buffer swap in double buffer mode
#define vc4_disable_color_buffer_clear     0x2000 // store_tile_buffer_general: disable color buffer clear on store/dump
#define vc4_disable_z_stencil_buffer_clear 0x4000 // store_tile_buffer_general: disable z/stencil buffer clear on store/dump
#define vc4_disable_vg_mask_buffer_clear   0x8000 // store_tile_buffer_general: disable vg-mask buffer clear on store/dump
#define vc4_disable_color_buffer_dump     0x00000001 // store_tile_buffer_general: disable color buffer dump
#define vc4_disable_z_stencil_buffer_dump 0x00000002 // store_tile_buffer_general: disable z/stencil buffer dump
#define vc4_disable_vg_mask_buffer_dump   0x00000004 // store_tile_buffer_general: disable vg-mask buffer dump
//#define last_tile_of_frame            0x00000008 // store_tile_buffer_general: last tile of frame
//#define memory_base_address           0xfffffff0 // store_tile_buffer_general: memory base address of frame/tile dump buffer (in multiples of 16 bytes)

/*
#define Store_Tile_Buffer_General data16, data32, address { // Control ID Code: Store Tile Buffer General (R)
  db $1C // Control ID Code Byte: ID Code #28
  dh data16 // Control ID Data Record Short: (Bit 0..15)
  dw address + data32 // Control ID Data Record Word: Memory Base Address Of Frame/Tile Dump Buffer (In Multiples Of 16 Bytes) (Bit 20..47), Data Record (Bit 16..19)
}
*/

int vc4_store_tile_buffer_general(uint8_t* buf, uint16_t data16, uint32_t data32, uint32_t address){
  *((uint8_t*) (buf+0))  = 0x1c;
  str16(buf+1,data16);
  str32(buf+3,address+data32); // FIXME mask + or?
  return 7;
}

// Load_Tile_Buffer_General: Load Tile Buffer General Control ID Code Data Record Description
#define vc4_load_none        0x0000 // load_tile_buffer_general: buffer to load = none
#define vc4_load_color       0x0001 // load_tile_buffer_general: buffer to load = color
#define vc4_load_z_stencil   0x0002 // load_tile_buffer_general: buffer to load = z/stencil
#define vc4_load_na          0x0003 // load_tile_buffer_general: buffer to load = n/a
#define vc4_load_vg_mask     0x0004 // load_tile_buffer_general: buffer to load = vg-mask
#define vc4_load_full_reload 0x0005 // load_tile_buffer_general: buffer to load = full reload
//#define color_format_rgba8888         0x0000 // load_tile_buffer_general: pixel color format = rgba8888
//#define color_format_bgr565_dithered  0x0100 // load_tile_buffer_general: pixel color format = bgr565 dithered
//#define color_format_bgr565_no_dither 0x0200 // load_tile_buffer_general: pixel color format = bgr565 no dither
#define vc4_disable_color_buffer_load     0x00000001 // load_tile_buffer_general: disable color buffer load
#define vc4_disable_z_stencil_buffer_load 0x00000002 // load_tile_buffer_general: disable z/stencil buffer load
#define vc4_disable_vg_mask_buffer_load   0x00000004 // load_tile_buffer_general: disable VG-Mask Buffer Load
//#define Memory_Base_Address           0xFFFFFFF0 // Load_Tile_Buffer_General: Memory Base Address Of Frame/Tile Dump Buffer (In multiples of 16 bytes)

/*#define Load_Tile_Buffer_General data16, data32, address { // Control ID Code: Load Tile Buffer General (R)
  db $1D // Control ID Code Byte: ID Code #29
  dh data16 // Control ID Data Record Short: (Bit 0..15)
  dw address + data32 // Control ID Data Record Word: Memory Base Address Of Frame/Tile Dump Buffer (In Multiples Of 16 Bytes) (Bit 20..47), Data Record (Bit 16..19)
}
*/

int vc4_load_tile_buffer_general(uint8_t* buf, uint16_t data16, uint32_t data32, uint32_t address){
  *((uint8_t*) (buf+0))  = 0x1d;
  str16(buf+1,data16);
  str32(buf+3,address+data32); // FIXME mask + or?
  return 7;
}

/*// Indexed_Primitive_List: Indexed Primitive List (OpenGL) Control ID Code Data Record Description
#define Mode_Points         0x00 // Indexed_Primitive_List: Primitive Mode = Points
#define Mode_Lines          0x01 // Indexed_Primitive_List: Primitive Mode = Lines
#define Mode_Line_Loop      0x02 // Indexed_Primitive_List: Primitive Mode = Line Loop
#define Mode_Line_Strip     0x03 // Indexed_Primitive_List: Primitive Mode = Line Strip
#define Mode_Triangles      0x04 // Indexed_Primitive_List: Primitive Mode = Triangles
#define Mode_Triangle_Strip 0x05 // Indexed_Primitive_List: Primitive Mode = Triangle Strip
#define Mode_Triangle_Fan   0x06 // Indexed_Primitive_List: Primitive Mode = Triangle Fan
#define Index_Type_8  0x00 // Indexed_Primitive_List: Index Type = 8-Bit
#define Index_Type_16 0x10 // Indexed_Primitive_List: Index Type = 16-Bit
#define Indices_Length       0xFFFFFFFF // Indexed_Primitive_List: Length (Number Of Indices)
#define Indices_List_Address 0xFFFFFFFF // Indexed_Primitive_List: Address Of Indices List
#define Maximum_Index        0xFFFFFFFF // Indexed_Primitive_List: Maximum Index (Primitives Using A Greater Index Will Cause Error)
*/
                        /*macro Indexed_Primitive_List data, length, address, maxindex { // Control ID Code: Indexed Primitive List (OpenGL)
  db $20 // Control ID Code Byte: ID Code #32
  db data     // Control ID Data Record Byte: (Bit 0..7)
  dw length   // Control ID Data Record Word: Length (Number Of Indices) (Bit 8..39)
  dw address  // Control ID Data Record Word: Address Of Indices List (Bit 40..71)
  dw maxindex // Control ID Data Record Word: Maximum Index (Bit 72..103)
  }*/

// Vertex_Array_Primitives: Vertex Array Primitives (OpenGL) Control ID Code Data Record Description
#define Mode_Points         0x00 // Vertex_Array_Primitives: Primitives Mode = Points
#define Mode_Lines          0x01 // Vertex_Array_Primitives: Primitives Mode = Lines
#define Mode_Line_Loop      0x02 // Vertex_Array_Primitives: Primitives Mode = Line Loop
#define Mode_Line_Strip     0x03 // Vertex_Array_Primitives: Primitives Mode = Line Strip
#define vc4_mode_triangles      0x04 // Vertex_Array_Primitives: Primitives Mode = Triangles
#define Mode_Triangle_Strip 0x05 // Vertex_Array_Primitives: Primitives Mode = Triangle Strip
#define Mode_Triangle_Fan   0x06 // Vertex_Array_Primitives: Primitives Mode = Triangle Fan
#define Vertices_Length    0xFFFFFFFF // Vertex_Array_Primitives: Length (Number Of Vertices)
#define First_Vertex_Index 0xFFFFFFFF // Vertex_Array_Primitives: Index Of First Vertex

/*macro Vertex_Array_Primitives data, length, index { // Control ID Code: Vertex Array Primitives (OpenGL)
  db $21 // Control ID Code Byte: ID Code #33
  db data   // Control ID Data Record Byte: (Bit 0..7)
  dw length // Control ID Data Record Word: Length (Number Of Vertices) (Bit 8..39)
  dw index  // Control ID Data Record Word: Index Of First Vertex (Bit 40..71)
  }*/

int vc4_vertex_array_primitives(uint8_t* buf, uint8_t data, uint32_t len, uint32_t index){
  *((uint8_t*) (buf+0))  = 0x21;
  *((uint8_t*) (buf+1))  = data;
  str32(buf+2,len);
  str32(buf+6,index);

  return 10;
}

// VG_Coordinate_Array_Primitives: VG Coordinate Array Primitives (Only For Use In VG Shader Mode) Control ID Code Data Record Description
#define Primitives_Type_RHT            0x01 // VG_Coordinate_Array_Primitives: Primitives Type = RHT
#define Primitives_Type_RHT_Strip      0x03 // VG_Coordinate_Array_Primitives: Primitives Type = RHT Strip
#define vc4_primitives_type_triangles      0x04 // VG_Coordinate_Array_Primitives: Primitives Type = Triangles
#define Primitives_Type_Triangle_Strip 0x05 // VG_Coordinate_Array_Primitives: Primitives Type = Triangle Strip
#define Primitives_Type_Triangle_Fan   0x06 // VG_Coordinate_Array_Primitives: Primitives Type = Triangle Fan
#define Triangle_Fan_Continuation_List 0xF0 // VG_Coordinate_Array_Primitives: Continuation List (For Triangle Fans Only)
#define Primitives_Length        0xFFFFFFFF // VG_Coordinate_Array_Primitives: Length (Number Of Primitives)
#define Coordinate_Array_Address 0xFFFFFFFF // VG_Coordinate_Array_Primitives: Address Of Coordinate Array (32-Bit X,Y Screen Coordinates) 

/*macro VG_Coordinate_Array_Primitives data, length, address { // Control ID Code: VG Coordinate Array Primitives (Only For Use In VG Shader Mode)
  db $29 // Control ID Code Byte: ID Code #41
  db data    // Control ID Data Record Byte: (Bit 0..7)
  dw length  // Control ID Data Record Word: Length (Number Of Primitives) (Bit 8..39)
  dw address // Control ID Data Record Word: Address Of Coordinate Array (Bit 40..71)
  }*/

int vc4_coordinate_array_primitives(uint8_t* buf, uint8_t data, uint32_t len, uint32_t addr){
  *((uint8_t*) (buf+0))  = 0x29;
  *((uint8_t*) (buf+1))  = data;
  str32(buf+2,len);
  str32(buf+6,addr);

  return 10;
}


// VG_Inline_Primitives: VG Inline Primitives (Only For Use In VG Shader Mode) Control ID Code Data Record Description
#define Primitives_Type_RHT            0x01 // VG_Inline_Primitives: Primitives Type = RHT
#define Primitives_Type_RHT_Strip      0x03 // VG_Inline_Primitives: Primitives Type = RHT Strip
#define Primitives_Type_Triangles      0x04 // VG_Inline_Primitives: Primitives Type = Triangles
#define Primitives_Type_Triangle_Strip 0x05 // VG_Inline_Primitives: Primitives Type = Triangle Strip
#define Primitives_Type_Triangle_Fan   0x06 // VG_Inline_Primitives: Primitives Type = Triangle Fan
#define Triangle_Fan_Continuation_List 0xF0 // VG_Inline_Primitives: Continuation List (For Triangle Fans Only)
#define Escape_Terminated_Coordinate_List 0xFFFFFFFF // VG_Inline_Primitives: Escape Terminated Uncompressed 32-Bit X,Y Coordinate List


/*macro VG_Inline_Primitives data { // Control ID Code: VG Inline Primitives (Only For Use In VG Shader Mode)
  db $2A // Control ID Code Byte: ID Code #42
  db data // Control ID Data Record Byte: (Bit 0..7)
  // Control ID Data Record Words: Escape Terminated Uncompressed 32-Bit X,Y Coordinate List (Bit 8..X)
}
*/

// Primitive_List_Format: Primitive List Format Control ID Code Data Record Description
#define vc4_primitive_type_points    0x00 // primitive list format: primitive type = points
#define vc4_primitive_type_lines     0x01 // primitive list format: primitive type = lines
#define vc4_primitive_type_triangles 0x02 // primitive list format: primitive type = triangles
#define vc4_primitive_type_rht       0x03 // primitive list format: primitive type = rht
#define vc4_data_type_index_16       0x10 // primitive list format: data type = 16-bit index
#define vc4_data_type_xy_32          0x30 // primitive list format: data type = 32-bit x/y

/*macro Primitive_List_Format data { // Control ID Code: Primitive List Format (R)
  db $38 // Control ID Code Byte: ID Code #56
  db data // Control ID Data Record Byte: (Bit 0..7)
}
*/

// GL_Shader_State: GL Shader State Control ID Code Data Record Description
#define Attribute_Arrays_0 0x00000000 // GL_Shader_State: Number Of Attribute Arrays = 0 (0 => All 8 Arrays) (Ignored For Extended Shader Record)
#define Attribute_Arrays_1 0x00000001 // GL_Shader_State: Number Of Attribute Arrays = 1 (Ignored For Extended Shader Record)
#define Attribute_Arrays_2 0x00000002 // GL_Shader_State: Number Of Attribute Arrays = 2 (Ignored For Extended Shader Record)
#define Attribute_Arrays_3 0x00000003 // GL_Shader_State: Number Of Attribute Arrays = 3 (Ignored For Extended Shader Record)
#define Attribute_Arrays_4 0x00000004 // GL_Shader_State: Number Of Attribute Arrays = 4 (Ignored For Extended Shader Record)
#define Attribute_Arrays_5 0x00000005 // GL_Shader_State: Number Of Attribute Arrays = 5 (Ignored For Extended Shader Record)
#define Attribute_Arrays_6 0x00000006 // GL_Shader_State: Number Of Attribute Arrays = 6 (Ignored For Extended Shader Record)
#define Attribute_Arrays_7 0x00000007 // GL_Shader_State: Number Of Attribute Arrays = 7 (Ignored For Extended Shader Record)
#define Extended_Shader_Record 0x00000008 // GL_Shader_State: Extended Shader Record (With 26-Bit Attribute Memory Stride)
#define Memory_Base_Address 0xFFFFFFF0 // GL_Shader_State: Memory Base Address Of Shader Record (In Multiples Of 16 Bytes)

/*macro GL_Shader_State data, address { // Control ID Code: GL Shader State
  db $40 // Control ID Code Byte: ID Code #64
  dw address + data // Control ID Data Record Word: Memory Address Of Shader Record (In Multiples Of 16 Bytes) (Bit 4..31), Data Record (Bit 0..3)
  }*/

// NV_Shader_State: NV Shader State (No Vertex Shading) Control ID Code Data Record Description

                   /*macro NV_Shader_State address { // Control ID Code: NV Shader State (No Vertex Shading)
  db $41 // Control ID Code Byte: ID Code #65
  dw address // Control ID Data Record Word: Memory Address Of Shader Record (16-Byte Aligned) (Bit 0..31)
  }*/

                   // VG_Shader_State: VG Shader State Control ID Code Data Record Description
                   /*macro VG_Shader_State address { // Control ID Code: VG Shader State
  db $42 // Control ID Code Byte: ID Code #66
  dw address // Control ID Data Record Word: Memory Address Of Shader Record (16-Byte Aligned) (Bit 0..31)
  }*/


int vc4_nv_shader_state(uint8_t* buf, uint32_t addr) {
  *((uint8_t*) (buf+0))  = 0x41;
  str32(buf+1,addr);

  return 5;
}

int vc4_vg_shader_state(uint8_t* buf, uint32_t addr) {
  *((uint8_t*) (buf+0))  = 0x42;
  str32(buf+1,addr);

  return 5;
}


// VG_Inline_Shader_Record: VG Inline Shader Record Control ID Code Data Record Description
#define Dual_Threaded_Fragment_Shader    0x00000000 // VG_Inline_Shader_Record: Dual Threaded Fragment Shader
#define Single_Threaded_Fragment_Shader  0x00000001 // VG_Inline_Shader_Record: Single Threaded Fragment Shader
#define Fragment_Shader_Code_Address     0xFFFFFFF8 // VG_Inline_Shader_Record: Fragment Shader Code Address (8-Byte Multiple)
#define Fragment_Shader_Uniforms_Address 0xFFFFFFFF // VG_Inline_Shader_Record: Fragment Shader Uniforms Address (4-Byte Aligned)
                           /*macro VG_Inline_Shader_Record data, addressc, addressu { // Control ID Code: VG Inline Shader Record
  db $43 // Control ID Code Byte: ID Code #67
  dw addressc + data // Control ID Data Record Word: Fragment Shader Code Address (8-Byte Multiple) (Bit 3..31), Data Record (Bit 0..2)
  dw addressu // Control ID Data Record Word: Fragment Shader Uniforms Address (4-Byte Aligned) (Bit 32..63)
}
                           */

// Configuration data -------------------------------------

// Configuration_bits: configuration bits control id code data record description
#define vc4_enable_forward_facing_primitive 0x01 // configuration_bits: enable forward facing primitive
#define vc4_enable_reverse_facing_primitive 0x02 // configuration_bits: enable reverse facing primitive
#define vc4_clockwise_primitives            0x04 // configuration_bits: clockwise primitives
#define vc4_enable_depth_offset             0x08 // configuration_bits: enable depth offset
#define vc4_antialiased_points_lines        0x10 // configuration_bits: antialiased points & lines (not actually supported)
#define vc4_coverage_read_type_level_4_8    0x00 // configuration_bits: coverage read type = 4*8-bit level
#define vc4_coverage_read_type_mask_16      0x20 // configuration_bits: coverage read type = 16-bit mask
#define vc4_rasteriser_oversample_mode_none 0x00 // configuration_bits: rasteriser oversample mode = none
#define vc4_rasteriser_oversample_mode_4x   0x40 // configuration_bits: rasteriser oversample mode = 4x
#define vc4_rasteriser_oversample_mode_16x  0x80 // configuration_bits: rasteriser oversample mode = 16x
#define vc4_coverage_pipe_select          0x0001 // configuration_bits: coverage pipe select
#define vc4_coverage_update_mode_non_zero 0x0000 // configuration_bits: coverage update mode = non zero
#define vc4_coverage_update_mode_odd      0x0002 // configuration_bits: coverage update mode = odd
#define vc4_coverage_update_mode_or       0x0004 // configuration_bits: coverage update mode = or
#define vc4_coverage_update_mode_zero     0x0006 // configuration_bits: coverage update mode = zero
#define vc4_coverage_read_mode_clear_on_read 0x0000 // configuration_bits: coverage read mode = clear on read
#define vc4_coverage_read_mode_leave_on_read 0x0008 // configuration_bits: coverage read mode = leave on read
#define vc4_depth_test_function_never  0x0000 // configuration_bits: depth-test function = never
#define vc4_depth_test_function_lt     0x0010 // configuration_bits: depth-test function = less than (lt)
#define vc4_depth_test_function_eq     0x0020 // configuration_bits: depth-test function = equal (eq)
#define vc4_depth_test_function_le     0x0030 // configuration_bits: depth-test function = less equal (le)
#define vc4_depth_test_function_gt     0x0040 // configuration_bits: depth-test function = greater than (gt)
#define vc4_depth_test_function_ne     0x0050 // configuration_bits: depth-test function = not equal (ne)
#define vc4_depth_test_function_ge     0x0060 // configuration_bits: depth-test function = greater equal (ge)
#define vc4_depth_test_function_always 0x0070 // configuration_bits: depth-test function = always
#define vc4_z_updates_enable       0x0080 // configuration_bits: z updates enable
#define vc4_early_z_enable         0x0100 // configuration_bits: early z enable
#define vc4_early_z_updates_enable 0x0200 // configuration_bits: early z updates enable

/*macro Configuration_Bits data8, data16 { // Control ID Code: Configuration Bits
  db $60 // Control ID Code Byte: ID Code #96
  db data8  // Control ID Data Record Byte: (Bit 0..7)
  dh data16 // Control ID Data Record Short: (Bit 8..23)
  }*/

int vc4_configuration_bits(uint8_t* buf, uint8_t data8, uint16_t data16) {
  *((uint8_t*) (buf+0))  = 0x60;
  *((uint8_t*) (buf+1))  = data8;
  *((uint16_t*)(buf+2))  = data16;

  return 4;
}

// Flat_Shade_Flags: Flat Shade Flags Control ID Code Data Record Description
#define Flat_Shade_Flags 0xFFFFFFFF // Flat_Shade_Flags: Flat-Shading Flags (32 X 1-Bit)

/*macro Flat_Shade_Flags flags { // Control ID Code: Flat Shade Flags
  db $61 // Control ID Code Byte: ID Code #97
  dw flags // Control ID Data Record Word: Flat-Shading Flags (32 x 1-Bit) (Bit 0..31)
  }*/

// Point_Size: Points Size Control ID Code Data Record Description
#define Points_Size 0xFFFFFFFF // Points_Size: Point Size (FLOAT32)

/*macro Point_Size size { // Control ID Code: Points Size
  db $62 // Control ID Code Byte: ID Code #98
  dw size // Control ID Data Record Word: Point Size (FLOAT32) (Bit 0..31)
  }*/

// Line_Width: Line Width Control ID Code Data Record Description
#define Line_Width 0xFFFFFFFF // Line_Width: Line Width (FLOAT32)

/*macro Line_Width width { // Control ID Code: Line Width
  db $63 // Control ID Code Byte: ID Code #99
  dw width // Control ID Data Record Word: Line Width (FLOAT32) (Bit 0..31)
  }*/

// RHT_X_Boundary: RHT X Boundary Control ID Code Data Record Description
#define RHT_Primitive_X_Boundary 0xFFFF // RHT_X_Boundary: RHT Primitive X Boundary (SINT16)

/*macro RHT_X_Boundary boundary { // Control ID Code: RHT X Boundary 
  db $64 // Control ID Code Byte: ID Code #100
  dh boundary // Control ID Data Record Short: RHT Primitive X Boundary (SINT16) (Bit 0..15)
  }*/

// Depth_Offset: Depth Offset Control ID Code Data Record Description
#define Depth_Offset_Factor 0xFFFF // Depth_Offset: Depth Offset Factor (FLOAT1-8-7)
#define Depth_Offset_Units  0xFFFF // Depth_Offset: Depth Offset Units (FLOAT1-8-7)

/*macro Depth_Offset factor, units { // Control ID Code: Depth Offset
  db $65 // Control ID Code Byte: ID Code #101
  dh factor // Control ID Data Record Short: Depth Offset Factor (FLOAT1-8-7) (Bit 0..15)
  dh units  // Control ID Data Record Short: Depth Offset Units (FLOAT1-8-7) (Bit 16..31)
  }*/

/*macro Clip_Window left, bottom, width, height { // Control ID Code: Clip Window
  db $66 // Control ID Code Byte: ID Code #102
  dh left   // Control ID Data Record Short: Clip Window Left Pixel Coordinate (UINT16) (Bit 0..15)
  dh bottom // Control ID Data Record Short: Clip Window Bottom Pixel Coordinate (UINT16) (Bit 16..31)
  dh width  // Control ID Data Record Short: Clip Window Width In Pixels (UINT16) (Bit 32..47)
  dh height // Control ID Data Record Short: Clip Window Height In Pixels (UINT16) (Bit 48..63)
  }*/

int vc4_clip_window(uint8_t* buf, uint16_t left, uint16_t bottom, uint16_t w, uint16_t h) {
  *((uint8_t*) (buf+0))  = 0x66;
  str16(buf+1,left);
  str16(buf+3,bottom);
  str16(buf+5,w);
  str16(buf+7,h);

  return 9;
}

/*macro Viewport_Offset x, y { // Control ID Code: Viewport Offset
  db $67 // Control ID Code Byte: ID Code #103
  dh x // Control ID Data Record Short: Viewport Centre X-Coordinate (SINT16) (Bit 0..15)
  dh y // Control ID Data Record Short: Viewport Centre Y-Coordinate (SINT16) (Bit 16..31)
  }*/

int vc4_viewport_offset(uint8_t* buf, uint16_t x, uint16_t y) {
  *((uint8_t*) (buf+0))  = 0x67;
  str16(buf+1,x);
  str16(buf+3,y);

  return 5;
}

// Z_Min_Max_Clipping_Planes: Z Min & Max Clipping Planes Control ID Code Data Record Description
#define Minimum_ZW 0xFFFFFFFF // Z_Min_Max_Clipping_Planes: Minimum ZW (FLOAT32)
#define Maximum_ZW 0xFFFFFFFF // Z_Min_Max_Clipping_Planes: Maximum ZW (FLOAT32)
/*macro Z_Min_Max_Clipping_Planes min, max { // Control ID Code: Z Min & Max Clipping Planes
  db $68 // Control ID Code Byte: ID Code #104
  dw min // Control ID Data Record Word: Minimum ZW (FLOAT32) (Bit 0..31)
  dw max // Control ID Data Record Word: Maximum ZW (FLOAT32) (Bit 32..63)
  }*/

// Clipper_XY_Scaling: Clipper XY Scaling Control ID Code Data Record Description
#define Viewport_Half_Width  0xFFFFFFFF // Clipper_XY_Scaling: Viewport Half-Width In 1/16th Of Pixel (FLOAT32)
#define Viewport_Half_Height 0xFFFFFFFF // Clipper_XY_Scaling: Viewport Half-Height In 1/16th Of pixel (FLOAT32)
/*macro Clipper_XY_Scaling width, height { // Control ID Code: Clipper XY Scaling (B)
  db $69 // Control ID Code Byte: ID Code #105
  dw width  // Control ID Data Record Word: Viewport Half-Width In 1/16th Of Pixel (FLOAT32) (Bit 0..31)
  dw height // Control ID Data Record Word: Viewport Half-Height In 1/16th Of pixel (FLOAT32) (Bit 32..63)
  }*/

// Clipper_Z_Scale_Offset: Clipper Z Scale & Offset Control ID Code Data Record Description
#define Viewport_Z_Scale  0xFFFFFFFF // Clipper_Z_Scale_Offset: Viewport Z Scale (ZC To ZS) (FLOAT32)
#define Viewport_Z_Offset 0xFFFFFFFF // Clipper_Z_Scale_Offset: Viewport Z Offset (ZC To ZS) (FLOAT32)
/*macro Clipper_Z_Scale_Offset scale, offset { // Control ID Code: Clipper Z Scale & Offset (B)
  db $6A // Control ID Code Byte: ID Code #106
  dw scale  // Control ID Data Record Word: Viewport Z Scale (ZC To ZS) (FLOAT32) (Bit 0..31)
  dw offset // Control ID Data Record Word: Viewport Z Offset (ZC To ZS) (FLOAT32) (Bit 32..63)
  }*/

// Tile_Binning_Mode_Configuration: Tile Binning Mode Configuration Control ID Code Data Record Description
//#define Multisample_Mode_4X   0x01 // Tile_Binning_Mode_Configuration: Multisample Mode (4X)
//#define Buffer_Color_Depth_64 0x02 // Tile_Binning_Mode_Configuration: Tile Buffer 64-Bit Color Depth
#define vc4_auto_initialise_tile_state_data_array  0x04 // tile_binning_mode_configuration: auto-initialise tile state data array
#define vc4_tile_allocation_initial_block_size_32  0x00 // tile_binning_mode_configuration: tile allocation initial block size = 32 bytes
#define vc4_tile_allocation_initial_block_size_64  0x08 // tile_binning_mode_configuration: tile allocation initial block size = 64 bytes
#define vc4_tile_allocation_initial_block_size_128 0x10 // tile_binning_mode_configuration: tile allocation initial block size = 128 bytes
#define vc4_tile_allocation_initial_block_size_256 0x18 // tile_binning_mode_configuration: tile allocation initial block size = 256 bytes
#define vc4_tile_allocation_block_size_32          0x00 // tile_binning_mode_configuration: tile allocation block size = 32 bytes
#define vc4_tile_allocation_block_size_64          0x20 // tile_binning_mode_configuration: tile allocation block size = 64 bytes
#define vc4_tile_allocation_block_size_128         0x40 // tile_binning_mode_configuration: tile allocation block size = 128 bytes
#define vc4_tile_allocation_block_size_256         0x60 // tile_binning_mode_configuration: Tile Allocation Block Size = 256 Bytes
//#define Double_Buffer_In_Non_MS_Mode           0x80 // Tile_Binning_Mode_Configuration: Double-Buffer In Non-MS Mode

/*macro Tile_Binning_Mode_Configuration address, size, baseaddress, width, height, data { // Control ID Code: Tile Binning Mode Configuration (B)
  db $70 // Control ID Code Byte: ID Code #112
  dw address     // Control ID Data Record Word: Tile Allocation Memory Address (Bit 0..31)
  dw size        // Control ID Data Record Word: Tile Allocation Memory Size (Bytes) (Bit 32..63)
  dw baseaddress // Control ID Data Record Word: Tile State Data Array Base Address (16-Byte Aligned, Size Of 48 Bytes * Num Tiles) (Bit 64..95)
  db width       // Control ID Data Record Byte: Width (In Tiles) (Bit 96..103)
  db height      // Control ID Data Record Byte: Height (In Tiles) (Bit 104..111)
  db data        // Control ID Data Record Byte: Data Record (Bit 112..119)
  }*/

int vc4_tile_binning_mode_conf(uint8_t* buf, uint32_t addr, uint32_t size, uint32_t baseaddr, uint8_t w, uint8_t h, uint8_t data) {
  *((uint8_t*) (buf+0))  = 0x70;
  str32(buf+1,addr);
  str32(buf+5,size);
  str32(buf+9,baseaddr);
  *((uint8_t*) (buf+13)) = w;
  *((uint8_t*) (buf+14)) = h;
  *((uint8_t*) (buf+15)) = data;

  return 16;
}

// Tile_Rendering_Mode_Configuration: Tile Rendering Mode Configuration Control ID Code Data Record Description
#define vc4_tile_rendering_memory_address 0xffffffff // tile_rendering_mode_configuration: memory address
#define vc4_tile_rendering_width  0xffff // tile_rendering_mode_configuration: width (pixels) (uint16)
#define vc4_tile_rendering_height 0xffff // tile_rendering_mode_configuration: height (pixels) (uint16)
#define vc4_multisample_mode_4x   0x0001 // tile_rendering_mode_configuration: multisample mode (4x)
#define vc4_buffer_color_depth_64 0x0002 // tile_rendering_mode_configuration: tile buffer 64-bit color depth (hdr mode)
#define vc4_frame_buffer_color_format_bgr565_dithered  0x0000 // tile_rendering_mode_configuration: non-hdr frame buffer color format = bgr565 dithered
#define vc4_frame_buffer_color_format_rgba8888         0x0004 // tile_rendering_mode_configuration: non-hdr frame buffer color format = rgba8888
#define frame_buffer_color_format_bgr565_no_dither 0x0008 // tile_rendering_mode_configuration: non-hdr frame buffer color format = bgr565 no dither
#define vc4_decimate_mode_1x  0x0000 // tile_rendering_mode_configuration: decimate mode = 1x
#define vc4_decimate_mode_4x  0x0010 // tile_rendering_mode_configuration: decimate mode = 4x
#define vc4_decimate_mode_16x 0x0020 // tile_rendering_mode_configuration: decimate mode = 16x
#define vc4_memory_format_linear    0x0000 // tile_rendering_mode_configuration: memory format = linear
#define vc4_memory_format_t_format  0x0040 // tile_rendering_mode_configuration: memory format = t-format
#define vc4_memory_format_lt_format 0x0080 // tile_rendering_mode_configuration: memory format = lt-format
#define vc4_enable_vg_mask_buffer 0x0100 // tile_rendering_mode_configuration: enable vg mask buffer
#define vc4_select_coverage_mode  0x0200 // tile_rendering_mode_configuration: select coverage mode
#define vc4_early_z_update_direction_lt_le 0x0000 // tile_rendering_mode_configuration: early-z update direction = lt/le
#define vc4_early_z_update_direction_gt_ge 0x0400 // tile_rendering_mode_configuration: early-z update direction = gt/ge
#define vc4_early_z_early_cov_disable    0x0800 // tile_rendering_mode_configuration: early-z/early-cov disable
#define vc4_double_buffer_in_non_ms_mode 0x1000 // tile_rendering_mode_configuration: double-buffer in non-ms mode

/*macro Tile_Rendering_Mode_Configuration address, width, height, data { // Control ID Code: Tile Rendering Mode Configuration (R)
  db $71 // Control ID Code Byte: ID Code #113
  dw address // Control ID Data Record Word: Memory Address (Bit 0..31)
  dh width   // Control ID Data Record Short: Width (Pixels) (UINT16) (Bit 32..47)
  dh height  // Control ID Data Record Short: Height (Pixels) (UINT16) (Bit 48..63)
  dh data    // Control ID Data Record Short: Data Record (Bit 64..79)
  }*/


int vc4_tile_rendering_mode_conf(uint8_t* buf, uint32_t addr, uint16_t w, uint16_t h, uint16_t data) {
  *((uint8_t*) (buf+0)) = 0x71;
  str32(buf+1,addr);
  str16(buf+5,w);
  str16(buf+7,h);
  str16(buf+9,data);

  return 11;
}


// Clear_Colors: Clear Colors Control ID Code Data Record Description
#define Clear_Color 0xFFFFFFFFFFFFFFFF // Clear_Colors: Clear Color (2X RGBA8888 Or RGBA16161616)
#define Clear_ZS      0x00FFFFFF // Clear_Colors: Clear ZS (UINT24)
#define Clear_VG_Mask 0xFF000000 // Clear_Colors: Clear VG Mask (UINT8)
#define Clear_Stencil 0xFF // Clear_Colors: Clear Stencil (UINT8)

/*macro Clear_Colors clearcolor, clearzs, clearvgmask, clearstencil { // Control ID Code: Clear Colors (R)
  db $72 // Control ID Code Byte: ID Code #114
  dd clearcolor                         // Control ID Data Record Double: Clear Color (2X RGBA8888 Or RGBA16161616) (Bit 0..63)
  dw (clearvgmask * $1000000) + clearzs // Control ID Data Record Word: Clear VG Mask (UINT8) (Bit 80..95), Clear ZS (UINT24) (Bit 64..79)
  db clearstencil                       // Control ID Data Record Byte: Clear Stencil (UINT8) (Bit 96..103)
  }*/

int vc4_clear_colors(uint8_t* buf, uint32_t cc1, uint32_t cc2, uint32_t clearzs, uint8_t clearvgmask, uint8_t clearstencil){
  *((uint8_t*) (buf+0))  = 0x72;
  str32(buf+1, cc1);
  str32(buf+5, cc2);
  str32(buf+9, (clearvgmask * 0x1000000) + clearzs);
  *((uint8_t*) (buf+13)) = clearstencil;
  return 14;
}

/*macro Tile_Coordinates column, row { // Control ID Code: Tile Coordinates (R)
  db $73 // Control ID Code Byte: ID Code #115
  db column // Control ID Data Record Byte: Tile Column Number (INT8) (Bit 0..7)
  db row    // Control ID Data Record Byte: Tile Row Number (INT8) (Bit 8..15)
}
*/

int vc4_tile_coordinates(uint8_t* buf, uint8_t col, uint8_t row) {
  *((uint8_t*)(buf+0)) = 0x73;
  *((uint8_t*)(buf+1)) = col;
  *((uint8_t*)(buf+2)) = row;

  return 3;
}

int vc4_shader_state_record(uint8_t* buf, uint8_t* frag_shader_addr) {
  *((uint8_t*) (buf+0))  = 0; // flags (0 = single threaded)
  *((uint8_t*) (buf+1))  = 0; // not used
  *((uint8_t*) (buf+2))  = 0; // num uniforms (not used?)
  *((uint8_t*) (buf+3))  = 0; // num varyings
  
  *((uint32_t*)(buf+4))  = (uint32_t)frag_shader_addr;
  *((uint32_t*)(buf+8))  = 0; // uniforms address
  
  return 12;
}
/*

align 16 ; 128-Bit Align
NV_SHADER_STATE_RECORD:
  db 0 ; Flag Bits: 0 = Fragment Shader Is Single Threaded, 1 = Point Size Included In Shaded Vertex Data, 2 = Enable Clipping, 3 = Clip Coordinates Header Included In Shaded Vertex Data
  db 6 * 4 ; Shaded Vertex Data Stride
  db 0 ; Fragment Shader Number Of Uniforms (Not Used Currently)
  db 3 ; Fragment Shader Number Of Varyings
  dw FRAGMENT_SHADER_CODE ; Fragment Shader Code Address
  dw 0 ; Fragment Shader Uniforms Address
  dw VERTEX_DATA ; Shaded Vertex Data Address (128-Bit Aligned If Including Clip Coordinate Header)
*/

int vc4_nv_shader_state_record(uint8_t* buf, uint8_t* frag_shader_addr, uint8_t num_varyings, uint8_t vertex_stride, uint8_t* vertex_addr) {
  *((uint8_t*) (buf+0))  = 0; // flags (0 = single threaded)
  *((uint8_t*) (buf+1))  = vertex_stride; // Shaded Vertex Data Stride
  *((uint8_t*) (buf+2))  = 0; // num uniforms (not used?)
  *((uint8_t*) (buf+3))  = num_varyings; // num varyings
  
  *((uint32_t*)(buf+4))  = (uint32_t)frag_shader_addr;
  *((uint32_t*)(buf+8))  = 0; // uniforms address (unused)
  *((uint32_t*)(buf+12)) = (uint32_t)vertex_addr;
  
  return 16;
}

int vc4_vertex(uint8_t* buf, uint16_t x, uint16_t y) {
  *((uint16_t*)(buf+0))  = x * 16;
  *((uint16_t*)(buf+2))  = y * 16;
  
  return 4;
}

int vc4_flat_shader(uint8_t* buf, uint32_t rgba) {
  *((uint32_t*)(buf+0))   = 0x009e7000;
  *((uint32_t*)(buf+4))   = 0x100009E7;

  *((uint32_t*)(buf+8))   = rgba;
  *((uint32_t*)(buf+12))  = 0xe0020ba7;
  *((uint32_t*)(buf+16))  = 0x009e7000;
  *((uint32_t*)(buf+20))  = 0x500009e7;
  *((uint32_t*)(buf+24))  = 0x009e7000;
  *((uint32_t*)(buf+28))  = 0x300009e7;

  *((uint32_t*)(buf+32))  = 0x009e7000;
  *((uint32_t*)(buf+36))  = 0x100009E7;
  *((uint32_t*)(buf+40))  = 0x009e7000;
  *((uint32_t*)(buf+44))  = 0x100009E7;

  return 12*4;
}

int vc4_gouraud_shader(uint8_t* buf) {
  *((uint32_t*)(buf+0))   = 0x958E0DBF;
  *((uint32_t*)(buf+4))   = 0xD1724823; // mov r0, vary// mov r3.8d, 1.0
  *((uint32_t*)(buf+8))   = 0x818E7176;
  *((uint32_t*)(buf+12))  = 0x40024821; // fadd r0, r0, r5// mov r1, vary
  *((uint32_t*)(buf+16))  = 0x818E7376;
  *((uint32_t*)(buf+20))  = 0x10024862; // fadd r1, r1, r5// mov r2, vary
  *((uint32_t*)(buf+24))  = 0x819E7540;
  *((uint32_t*)(buf+28))  = 0x114248A3; // fadd r2, r2, r5// mov r3.8a, r0
  *((uint32_t*)(buf+32))  = 0x809E7009;
  *((uint32_t*)(buf+36))  = 0x115049E3; // nop// mov r3.8b, r1
  *((uint32_t*)(buf+40))  = 0x809E7012;
  *((uint32_t*)(buf+44))  = 0x116049E3; // nop// mov r3.8c, r2
  *((uint32_t*)(buf+48))  = 0x159E76C0;
  *((uint32_t*)(buf+52))  = 0x30020BA7; // mov tlbc, r3// nop// thrend
  *((uint32_t*)(buf+56))  = 0x009E7000;
  *((uint32_t*)(buf+60))  = 0x100009E7; // nop// nop// nop
  *((uint32_t*)(buf+64))  = 0x009E7000;
  *((uint32_t*)(buf+68))  = 0x500009E7; // nop// nop// sbdone

  return 72;
}
