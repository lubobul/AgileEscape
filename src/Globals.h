
// some externed global variables to be used in different files

// to avoid double inclusion in some cases
#ifndef GLOBALS_H
#define GLOBALS_H

// variables that are externed so they can be used in multiple source files
extern SDL_Window *window; // the game's window object
extern SDL_Renderer *render; // to display graphics on the window
extern SDL_Texture *texture_menu_bgr, *texture_level_bgr, *texture_player, *texture_exit, *texture_ghost, *texture_tiles[32][24], *swap_energy, *swap_blackhole; // hold the actual graphics
extern SDL_Surface *menu_bgr, *level_bgr, *player, *s_exit, *ghost;// holds the images
extern SDL_Rect rBgr, rPlayer, rExit, rGhost, rTiles[32][24]; // rectangles for the textures, which set their coordinates to display them in particular position on the renderer
extern Mix_Music *bgr_music;
extern Mix_Chunk *fx_music, *jump_music;

extern bool running, jump, dir_left, dir_right, move_bomb, god_mode; // to control the state and logic of moveable objects
extern int delay_move, delay_energy, delay_blackhole, delay_bombs, bomb_moves, jcounter, death_x, death_y; // to control timers of movable objects
extern int tiles[32][24]; // hold the code for each tile - some numbers mean walls, some traps, 0 means no tile(free space to move)

#endif