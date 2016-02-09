
// main includes and function prototypes

// to avoid double inclusion in some cases
#ifndef MAIN_H
#define MAIN_H

// SDL libraries headers
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

// common standard library headers just in case if needed
#include <fstream>

using namespace std;

// prototypes of most of the functions to avoid calling fuctions that could be called before their definition in the text flow
void init_system();
void game_loop();
void menu_loop();
void release_system();
void play_bgr(const char*);
void play_fx(const char*);
void play_jump();
void load_level_bgr(const char*);
void load_level(const char*);
void game_input();
void game_rendering();
void release_level();
void collision_detect();
void ghost_fx();
Uint32 timing_move(Uint32, void *);
Uint32 timing_energy(Uint32, void *);
Uint32 timing_blackhole(Uint32, void *);
Uint32 timing_bombs(Uint32, void *);

#endif