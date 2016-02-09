
// contains the main functions for working with levels - loading them from file and setting them up and for releasing their resources so we can go back to the menu on finish

#include "Main.h"
#include "Globals.h"

// declarations of the textures and their rectangles which hold coordinates information
SDL_Texture *texture_level_bgr, *texture_tiles[32][24];
SDL_Texture *swap_energy, *swap_blackhole;
SDL_Surface *level_bgr;
SDL_Rect rBgr, rGhost, rTiles[32][24];

// timer variables to control movement and traps based on time
SDL_TimerID timer_move, timer_bombs, timer_energy, timer_blackhole;

// holds the number code for the meaning of each tile
int tiles[32][24];

// delay to control times and integers to hold the player's checkpoint location
int delay_energy, delay_blackhole, delay_bombs, delay_move, death_x, death_y;

bool swap_e = false;
bool swap_b = false;

void set_tile(const char*, int, int); // function prototype

void load_level_bgr(const char* path) // called when switching to level in Menu.cpp
{
	level_bgr = IMG_Load(path);
	texture_level_bgr = SDL_CreateTextureFromSurface(render, level_bgr); // load and set and image as texture for that renderer
	SDL_FreeSurface(level_bgr);
	level_bgr = NULL;
	rBgr.h = 768; // set the texture's height as the image's height
	rBgr.w = 1024; // set the texture's width as the image's width
	// the rectangle that will be associated with the texture will have coordinates 0, 0 so it's origin will match the window's origin(and fill the whole window)
	rBgr.x = 0;
	rBgr.y = 0;
}

void load_level(const char* path) // called when switchint to level in Menu.cpp
{
	// initiate a ghost to show death effect
	rGhost.x = 0;
	rGhost.y = 0;
	// prepare to read bytes from a file
	ifstream level_file(path);
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			level_file >> tiles[i][j]; // save to variable the level's file information containing the 2D matrix-like order of the tiles and their codes
	level_file.close();
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
		{
			switch ( tiles[i][j] ) // set tiles according to the code in the file, function set_tile() right below
			{
				case 0:
					texture_tiles[i][j] = NULL; // will act as free space to move
					break;
				case 1:
					set_tile("res/gfx/wall_tile.png", i, j); // sets the tiles depending on the code in an texture array with the same index(so the arrays can be parallel)
					break;
				case 2:
					set_tile("res/gfx/brick_tile.png", i, j);
					break;
				case 3:
					set_tile("res/gfx/water_tile.jpg", i, j);
					break;
				case 4:
					set_tile("res/gfx/lava_tile.gif", i, j);
					break;
				case 5: case 6: case 7: case 8:
					set_tile("res/gfx/mine_tile.png", i, j);
					break;
				case 9:
					set_tile("res/gfx/checkpoint.jpg", i, j);
					break;
				case 10:
					set_tile("res/gfx/spike_tile.png", i, j);
					break;
				case 11:
					swap_e = true;
					set_tile("res/gfx/energy_tile.gif", i, j);
					break;
				case 13:
					swap_b = true;
					set_tile("res/gfx/blackhole_tile.png", i, j);
					break;
				case 14:
					set_tile("res/gfx/jump_tile.gif", i, j);
					break;
				default:
					texture_tiles[i][j] = NULL;
					break;
			}
		}
	// set the initial speed(remains keeped that way) of player, bomb moving and energy trap reappear
	delay_move = 50;
	delay_bombs = 200;
	delay_energy = 500;
	delay_blackhole = 750;
	// for checkpoint initialization
	death_x = 0;
	death_y = 736;
	// starts running the 3 timer functions, implemented in Game.cpp which will automatically run and update the state
	timer_move = SDL_AddTimer(delay_move, timing_move, NULL);
	timer_energy = SDL_AddTimer(delay_energy, timing_energy, NULL);
	timer_blackhole = SDL_AddTimer(delay_blackhole, timing_blackhole, NULL);
	timer_bombs = SDL_AddTimer(delay_bombs, timing_bombs, NULL);
	// bomb traps start moving
	move_bomb = true;
}

void set_tile(const char* path, int i, int j) // fill the texture and rectangle arrays with information about the same tile
{
	SDL_Surface *tile = IMG_Load(path);
	texture_tiles[i][j] = SDL_CreateTextureFromSurface(render, tile);
	if ( swap_e )
		swap_energy = texture_tiles[i][j];
	if ( swap_b )
		swap_blackhole = texture_tiles[i][j];
	swap_e = false;
	swap_b = false;
	tile = NULL;
	SDL_FreeSurface(tile);
	rTiles[i][j].h = 32;
	rTiles[i][j].w = 32;
	// by multiplying by 32, we make sure that the tiles fill the renderer in an equal matrix-like way
	// all tiles are 32x32 pixels, and the window resolution is 1024x768(that's why all loops are repeated 32x24)
	rTiles[i][j].x = i * 32;
	rTiles[i][j].y = j * 32;
}

void release_level() // called when exiting level, so we can safely proceed to the menu
{
	// reset basic gameplay variables and timers
	god_mode = false; // reset cheating/testing mode
	bomb_moves = 0;
	rPlayer.x = 0;
	rPlayer.y = 736;
	SDL_RemoveTimer(timer_move);
	SDL_RemoveTimer(timer_energy);
	SDL_RemoveTimer(timer_blackhole);
	SDL_RemoveTimer(timer_bombs);
	jump = dir_left = dir_right = false;
	jcounter = 0;
	// release the level's tiles
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
		{
			SDL_DestroyTexture(texture_tiles[i][j]);
			texture_tiles[i][j] = NULL;
			tiles[i][j] = 0;
		}
	SDL_DestroyTexture(texture_level_bgr);
	texture_level_bgr = NULL;
}