
// functions to run the menu and switch the game state to some level depending on key pressed

#include "Main.h"
#include "Globals.h"

void game_state(); // function prototype

void menu_loop() // will be called also when exiting levels to initiate again the menu
{
	play_bgr("res/audio/menu.mp3"); // start playing the menu background looping music
	running = true; // can be removed, already set in Run.cpp
	while ( true ) // endlessy render the menu and wait for user input which switches to some game level
	{
		SDL_RenderClear(render); // clear the contents of the renderer
		SDL_RenderCopy(render, texture_menu_bgr, NULL, NULL); // copy the menu background texture to our renderer
		SDL_RenderPresent(render); // display the contents of the renderer
		game_state(); // located just below - checks for user input to change the game state to some level or quit the game
	}
}


void game_state() // called from the endless loop in menu_loop()
{
	// create an event object and check it for keypresses
	SDL_Event menu_kprs;
	// this loops checks for events and quits if none are handled, repeats itself with the menu loop
	while ( SDL_PollEvent(&menu_kprs) )
	{
		switch ( menu_kprs.type )
		{
			// for the case of pressed down keyboard button, we open a new switch to check for concrete buttons
			case SDL_KEYDOWN:
			{
				switch ( menu_kprs.key.keysym.sym )
				{
					case SDLK_ESCAPE: // if the press the Esc key we usually want to quit the program so we call the finishing function
						release_system(); // located in Run.cpp
						break;
					// from buttons 1 to 9 we start the 9 predefined levels
					// load_level_bgr(path) - located in Level.cpp - loads the image in the path as a background
					// play_bgr(path) - located in Audio.cpp - starts playing the music in the path as background music
					// load_level(path) - located in Level.cpp - loads the level data file in the path and sets the textures to be rendered for that level
					// game_loop() - located in Run.cpp - endless loop which repeats all gameplay functions
					case SDLK_1:
						load_level_bgr("res/gfx/lvl01.png");
						play_bgr("res/audio/lvl01.mp3");
						load_level("data/lvl01.dat");
						game_loop();
						break;
					case SDLK_2:
						load_level_bgr("res/gfx/lvl02.jpg");
						play_bgr("res/audio/lvl02.mp3");
						load_level("data/lvl02.dat");
						game_loop();
						break;
					case SDLK_3:
						load_level_bgr("res/gfx/lvl03.jpg");
						play_bgr("res/audio/lvl03.mp3");
						load_level("data/lvl03.dat");
						game_loop();
						break;
					case SDLK_4:
						load_level_bgr("res/gfx/lvl04.jpg");
						play_bgr("res/audio/lvl04.mp3");
						load_level("data/lvl04.dat");
						game_loop();
						break;
					case SDLK_5:
						load_level_bgr("res/gfx/lvl05.jpg");
						play_bgr("res/audio/lvl05.mp3");
						load_level("data/lvl05.dat");
						game_loop();
						break;
					case SDLK_6:
						load_level_bgr("res/gfx/lvl06.jpg");
						play_bgr("res/audio/lvl06.mp3");
						load_level("data/lvl06.dat");
						game_loop();
						break;
					case SDLK_7:
						load_level_bgr("res/gfx/lvl07.jpg");
						play_bgr("res/audio/lvl07.mp3");
						load_level("data/lvl07.dat");
						game_loop();
						break;
					case SDLK_8:
						load_level_bgr("res/gfx/lvl08.png");
						play_bgr("res/audio/lvl08.mp3");
						load_level("data/lvl08.dat");
						game_loop();
						break;
					case SDLK_9:
						load_level_bgr("res/gfx/lvl09.jpg");
						play_bgr("res/audio/lvl09.mp3");
						load_level("data/lvl09.dat");
						game_loop();
						break;
					// 0 to start the default custom level from the level editor
					case SDLK_0:
						load_level_bgr("res/gfx/editor_background.jpg");
						play_bgr("res/audio/lvl01.mp3");
						load_level("data/lvl_custom.dat");
						game_loop();
						break;
					default:
						break;
				}
				// the break for the first case(SDL_KEWDOWN)
				break;
			}
			case SDL_QUIT:
				// if we use normal ways to close the program, we release the opened resources and quit
				release_system(); // located in Run.cpp
				break;
			default:
				break;
		}
	}
}