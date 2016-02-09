
// contains main gameplay functions for getting input, collision detection, timers and rendering

#include "Main.h"
#include "Globals.h"

bool dir_left = false, dir_right = false, jump = false; // initiate moving booleans
bool move_bomb = true; // start moving bombs
bool show_energy = false; // to initiate the energy trap
bool show_blackhole = false; // to initiate the black hole trap
bool god_mode = false; // user for cheating or testing

int jcounter = 0; // initiate the counter for the lenght of the jump
int bomb_moves = 0; // initiate the counter for the lenght of the distance the bombs move

// prototypes of functions for collision detection
bool detect_right();
bool detect_left();
bool detect_up();
bool detect_down();
bool detect_ceil();

void game_input() // called from game_loop() in Run.cpp - checks for user input
{
	// works the same way as in Menu.cpp
	SDL_Event game_kprs;
	while ( SDL_PollEvent(&game_kprs) )
	{
		switch ( game_kprs.type )
		{
			case SDL_KEYDOWN: // when we press down a button
			{
				switch ( game_kprs.key.keysym.sym ) // Note: jumping and moving are handled in the timer functions below
				{
					case SDLK_ESCAPE:
						running = false; // stops the game loop
						release_level(); // free the resources used in the level
						menu_loop(); // go back to menu - located in Menu.cpp
						break;
					case SDLK_UP: case SDLK_w: case SDLK_SPACE:
						jump = true; // allow for a jump
						play_jump(); // play audio effect
						break;
					case SDLK_LEFT: case SDLK_a:
						dir_left = true; // allow for a move on the left
						break;
					case SDLK_RIGHT: case SDLK_d:
						dir_right = true; // allow for a move on the right
						break;
					case SDLK_F6: // switch on and off cheating/testing mechanism
						if ( !god_mode )
							god_mode = true;
						else
							god_mode = false;
						break;
					default:
						break;
				}
			// case KEYDOWN break
				break;
			}
			case SDL_KEYUP: // when we release a key from down state
			{
				switch ( game_kprs.key.keysym.sym ) // Note: jumping and moving are handled in the timer functions below
				{
					case SDLK_UP: case SDLK_w: case SDLK_SPACE:
						jump = true; // continue jumping when pressing the jump keys
						break;
					// control the movement variables so the player does not automove:
					case SDLK_LEFT: case SDLK_a:
						dir_left = false;
						break;
					case SDLK_RIGHT: case SDLK_d:
						dir_right = false;
						break;
					default:
						break;
				}
			// case KEYUP break
				break;
			}
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
		}
	}
}

// all timing functions below are called automatically and initiated in the load_level() function in Level.cpp
// all collision detect functions are below

Uint32 timing_move(Uint32 delay_move, void *) // moves the player at the set speed and controls jumping
{
	if ( ( rPlayer.y < 736 ) && ( !jump ) ) // if the player is above the window bottom and is not jumping he should fall down 
	{
		bool move_down = detect_down(); // get information if the player is allowed to fall down without leaving window boundaries
		if ( move_down ) rPlayer.y += 32; // if the player is allowed, fall down
	}
	if ( jcounter > 3 ) // if already jumped 3 steps(tile heights)
	{
		jump = false; // stop jumping until jump button activates it again
		jcounter = 0; // reset the counter of the steps jumped
	}
	if ( jump ) // if jump button is pressed
	{
		jcounter++; // count steps jumped
		bool move_up = detect_up(); // check to see if the player is allowed to move up
		bool ceiling = detect_ceil(); // check specifically if the player is hitting the top of the window
		if ( ( move_up ) && ( ceiling ) ) // if conditions allow it
			rPlayer.y -= 32; // move up one step
	}
	if ( ( rPlayer.x > 0 ) && ( dir_left ) ) // if player is set to move left and he is not at the leftmost edge
	{
		bool move_left = detect_left(); // is he allowed to move left
		if ( move_left ) rPlayer.x -= 32; // if so, move one step left
	}
	if ( ( rPlayer.x < 992 ) && ( dir_right ) ) // same as above
	{
		bool move_right = detect_right();
		if ( move_right ) rPlayer.x += 32;
	}
	return delay_move += delay_move; // keep the moving speed the same
}

// the 5 functions below return true if the player is free to move, and return false if there is a wall in the corresponding direction

bool detect_right()
{
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			if ( ( tiles[i][j] == 1 ) || ( tiles[i][j] == 2 ) ) // if a tile is a wall type
				if ( ( rPlayer.x + 32 == rTiles[i][j].x ) && ( rPlayer.y == rTiles[i][j].y ) ) // if the next step in the corresponding direction will collide with the wall
					// in other words, if the player's rectangle object and that tile's rectangle object will be at the same x, y coordinates
					return false;
	return true;
}

bool detect_left()
{
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			if ( ( tiles[i][j] == 1 ) || ( tiles[i][j] == 2 ) )
				if ( ( rPlayer.x - 32 == rTiles[i][j].x ) && ( rPlayer.y == rTiles[i][j].y ) )
					return false;
	return true;
}

bool detect_up()
{
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			if ( ( tiles[i][j] == 1 ) || ( tiles[i][j] == 2 ) )
				if ( ( rPlayer.x == rTiles[i][j].x ) && ( rPlayer.y - 32 == rTiles[i][j].y ) )
					return false;
	return true;
}

bool detect_down()
{
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			if ( ( tiles[i][j] == 1 ) || ( tiles[i][j] == 2 ) )
				if ( ( rPlayer.x == rTiles[i][j].x ) && ( rPlayer.y + 32 == rTiles[i][j].y ) )
					return false;
	return true;
}

bool detect_ceil() // short function to check if we are under the window's top so we can abort a jump that could get us off the screen
{
	if ( rPlayer.y <= 0 )
		return false;
	else
		return true;
}

Uint32 timing_energy(Uint32 delay_energy, void *) // controls the showing and hiding of energy trap
{
	if ( !show_energy ) // if the trap was hidden in the previous call
		show_energy = true; // make the trap appear for this call
	else show_energy = false; // otherwise if the trap appeared in the previous call, hide it for this call
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			if ( ( tiles[i][j] == 11 ) || ( tiles[i][j] == 12 ) ) // check the energy trap tiles
				if ( show_energy ) // if the trap is to be shown
				{
					tiles[i][j] = 11; // // set the code for an active trap
					texture_tiles[i][j] = swap_energy;
				}
				else // if the trap is to be hidden
				{
					tiles[i][j] = 12; // set the code for inactive trap(some unused in other tiles number)
					texture_tiles[i][j] = NULL;
				}
	return delay_energy += delay_energy; // show and hide the trap at regular intervals
}

Uint32 timing_blackhole(Uint32 delay_blackhole, void *) // same as timing_energy()
{
	if ( !show_blackhole )
		show_blackhole = true;
	else show_blackhole = false;
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			if ( ( tiles[i][j] == 13 ) || ( tiles[i][j] == 15 ) )
				if ( show_blackhole )
				{
					tiles[i][j] = 13;
					texture_tiles[i][j] = swap_blackhole;
				}
				else
				{
					tiles[i][j] = 15; // don't use 15 as tile code
					texture_tiles[i][j] = NULL;
				}
	return delay_blackhole += delay_blackhole;
}

Uint32 timing_bombs(Uint32 delay_bombs, void *) // control the movement of the bombs
{
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ ) // check for 4 tipes of bomb tiles, each for different moving direction
		{
			if ( tiles[i][j] == 5 )
			{
				if ( move_bomb ) // if we are to move the bombs
					rTiles[i][j].y += 32; // move them one step in the corresponding direction
				else
					rTiles[i][j].y -= 32; // oterwise move them one step back towards their starting location
			}
			if ( tiles[i][j] == 6 )
			{
				if ( move_bomb )
					rTiles[i][j].x += 32;
				else
					rTiles[i][j].x -= 32;
			}
			if ( tiles[i][j] == 7 )
			{
				if ( move_bomb )
					rTiles[i][j].x -= 32;
				else
					rTiles[i][j].x += 32;
			}
			if ( tiles[i][j] == 8 )
			{
				if ( move_bomb )
					rTiles[i][j].y -= 32;
				else
					rTiles[i][j].y += 32;
			}
		}
	bomb_moves++; // count how many times the bombs were moves
	if ( bomb_moves == 3 ) // if 3 moves were made, stop moving them in that direction(and move them backwards)
		move_bomb = false;
	if ( bomb_moves == 6 ) // if 3 backwards move wre made
	{
		move_bomb = true; // start moving them again
		bomb_moves = 0; // reset the move counter
	}
	return delay_bombs += delay_bombs; // move the bombs in the same speed
}

void collision_detect() // called from game_loop() in Run.cpp - check for basic collision detection
{
	if ( ( rPlayer.x == rExit.x ) && ( rPlayer.y == rExit.y ) ) // if the player coordinates meet the exit coordinates
	{
		play_fx("res/audio/finish.wav"); // play finish level sound effect
		running = false; // stop the game loop
		SDL_Delay(3000); // 3 seconds delay for the audio effect
		release_level(); // free the level resources
		menu_loop(); // reinitialize the menu
	}
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			if ( ( rPlayer.x == rTiles[i][j].x ) && ( rPlayer.y == rTiles[i][j].y ) ) // check the tile to which the player's coordinates equal
			{
				if ( tiles[i][j] == 9 ) // if the current tile is a checkpoint
				{
					// player will respawn at the checkpoint's coordinates
					death_x = rTiles[i][j].x;
					death_y = rTiles[i][j].y;
				}
				if ( tiles[i][j] == 14 ) // if the current tile is a jump pad
				{
					play_fx("res/audio/jump_pad.wav");
					for ( int i = 0; i < 8; i++ )
					{
						rPlayer.y -= 32; // jump high
						for ( int i = 0; i < 32; i++ ) // unoptimized check for traps
							for ( int j = 0; j < 24; j++ )
								if ( ( rPlayer.x == rTiles[i][j].x ) && ( rPlayer.y == rTiles[i][j].y ) )
									if ( ( ( ( tiles[i][j] == 3 ) || ( tiles[i][j] == 4 ) ) || ( ( tiles[i][j] == 5 ) || ( tiles[i][j] == 6 ) ) ) || ( ( ( tiles[i][j] == 7 ) || ( tiles[i][j] == 8 ) ) || ( ( tiles[i][j] == 10 ) || ( tiles[i][j] == 11 ) ) ) || ( tiles[i][j] == 13 ) )
										goto trap; // bad jump
					}
				}
				// if the tile is a trap(death situation):
				if ( ( ( ( tiles[i][j] == 3 ) || ( tiles[i][j] == 4 ) ) || ( ( tiles[i][j] == 5 ) || ( tiles[i][j] == 6 ) ) ) ||  ( ( ( tiles[i][j] == 7 ) || ( tiles[i][j] == 8 ) ) || ( ( tiles[i][j] == 10 ) || ( tiles[i][j] == 11 ) ) ) || ( tiles[i][j] == 13 ) )
					trap: // bad goto
					if ( !god_mode ) // if we are not in cheating/testing mode
					{
						ghost_fx(); // located below, shows a death effect texture
						// move the player either at the beginning of the level, either at some checkpoint if reached already
						rPlayer.x = death_x;
						rPlayer.y = death_y;
						// reset moving and jumping
						jump = dir_left = dir_right = false;
						jump = false;
						jcounter = 0;
						// shord delay for esthetic reasons
						SDL_Delay(300);
					}
			}
}

void ghost_fx() // called if the player collides with deathly tile
{
	// set the ghost texture's coordinates
	rGhost.x = rPlayer.x;
	rGhost.y = rPlayer.y;
	play_fx("res/audio/death.wav"); // play a daeth sound effect
	SDL_RenderCopy(render, texture_ghost, NULL, &rGhost); // display the death effect texture at it's rectangle location on the renderer
	SDL_RenderPresent(render); // show the updated screen
}

void game_rendering() // called from game_loop() in Run.cpp - displays the content of the renderer on the window
{
	SDL_RenderClear(render); // clear the renderer
	// load the renderer with the textures, each set at it's rectangle, which holds its height, width and display coordinates, and copy them to the renderer
	// the ordering of the functions correspondent to the ordering of the textures on screen(the latter the function call, the more textures it will overlap)
	SDL_RenderCopy(render, texture_level_bgr, NULL, &rBgr);
	SDL_RenderCopy(render, texture_player, NULL, &rPlayer);
	SDL_RenderCopy(render, texture_exit, NULL, &rExit);
	for ( int i = 0; i < 32; i++ )
		for ( int j = 0; j < 24; j++ )
			SDL_RenderCopy(render, texture_tiles[i][j], NULL, &rTiles[i][j]);
	SDL_RenderCopy(render, texture_player, NULL, &rPlayer);
	SDL_RenderPresent(render); // show the contents of the renderer on the window
}