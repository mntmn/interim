#include <SDL2/SDL.h>

void sdl_setpixel(Uint32 x, Uint32 y, Uint32 color);
void* sdl_init(int fullscreen);
int sdl_mainloop();

int sdl_get_key();
int sdl_get_modifiers();
void* sdl_get_fb();
long sdl_get_fbsize();

void sdl_mount_fbfs();
