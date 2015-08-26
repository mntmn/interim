
/*
int machine_video_flip() {
  nv_vertex_t* triangles = r3d_init_frame();

  Cell* c_x1 = lookup_global_symbol("tx1");
  Cell* c_x2 = lookup_global_symbol("tx2");
  Cell* c_y1 = lookup_global_symbol("ty1");
  Cell* c_y2 = lookup_global_symbol("ty2");
  
  int x1 = c_x1->value*16;
  int y1 = c_y1->value*16;
  int x2 = c_x2->value*16;
  int y2 = c_y2->value*16;

  triangles[0].x = x1;
  triangles[0].y = y1;
  triangles[0].z = 1.0f;
  triangles[0].w = 1.0f;
  triangles[0].r = 1.0f;
  triangles[0].g = 0.0f;
  triangles[0].b = 1.0f;
  
  triangles[1].x = x2;
  triangles[1].y = y1;
  triangles[1].z = 1.0f;
  triangles[1].w = 1.0f;
  triangles[1].r = 1.0f;
  triangles[1].g = 1.0f;
  triangles[1].b = 1.0f;
  
  triangles[2].x = x1;
  triangles[2].y = y2;
  triangles[2].z = 1.0f;
  triangles[2].w = 1.0f;
  triangles[2].r = 1.0f;
  triangles[2].g = 1.0f;
  triangles[2].b = 1.0f;
  
  triangles[3].x = x2;
  triangles[3].y = y1;
  triangles[3].z = 1.0f;
  triangles[3].w = 1.0f;
  triangles[3].r = 1.0f;
  triangles[3].g = 1.0f;
  triangles[3].b = 1.0f;
  
  triangles[4].x = x2;
  triangles[4].y = y2;
  triangles[4].z = 1.0f;
  triangles[4].w = 1.0f;
  triangles[4].r = 1.0f;
  triangles[4].g = 0.0f;
  triangles[4].b = 1.0f;
  
  triangles[5].x = x1;
  triangles[5].y = y2;
  triangles[5].z = 1.0f;
  triangles[5].w = 1.0f;
  triangles[5].r = 1.0f;
  triangles[5].g = 1.0f;
  triangles[5].b = 1.0f;
  
  r3d_triangles(2, triangles);
  r3d_render_frame(0xffffffff);
  
  //memset(FB_MEM, 0xffffff, 1920*1080*4);
  //r3d_debug_gpu();
  
  return 0;
}
*/
