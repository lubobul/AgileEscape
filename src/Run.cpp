
// some fundamental functions to init, release the game resources and run the game_loop() for the levels gameplay

#include "Main.h"
#include "Globals.h"

// pointers to the window and render structures, declared here and externed in Globals.h
SDL_Window *window;
SDL_Renderer *render;
SDL_Surface *menu_bgr, *player, *s_exit, *ghost;;
SDL_Texture *texture_menu_bgr, *texture_player, *texture_exit, *texture_ghost;
SDL_Rect rPlayer, rExit;

bool running = true; // so the game loop can run

void init_system() // we came here from main()
{
	// initiate the libraries
	SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_NOPARACHUTE); // initiate everything in the main library combined with debugging help flag
	Mix_Init;
	IMG_Init;
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);
	if ( mode.h > 768 )
		window = SDL_CreateWindow("Agile Escape", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN);
	else
		window = SDL_CreateWindow("Agile Escape", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_FULLSCREEN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create the renderer to show graphics in this window
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024); // initiate audio output
	menu_bgr = IMG_Load("res/gfx/menu_bgr.jpg"); // load the image in the path to the surface
	texture_menu_bgr = SDL_CreateTextureFromSurface(render, menu_bgr); // create the texture from the image in the surface for our renderer
	jump_music = Mix_LoadWAV("res/audio/jump.wav"); // loads the file for the jump effect music
	// load the textures for the player, exit, and ghost(death effect), process described in Menu.cpp
	player = IMG_Load("res/gfx/player.png");
	texture_player = SDL_CreateTextureFromSurface(render, player);
	rPlayer.h = player->h;
	rPlayer.w = player->w;
	// set the player's starting position
	rPlayer.x = 0;
	rPlayer.y = 736;
	s_exit = IMG_Load("res/gfx/exit.png");
	texture_exit = SDL_CreateTextureFromSurface(render, s_exit);
	rExit.h = s_exit->h;
	rExit.w = s_exit->w;
	// set the exit's starting position
	rExit.x = 992;
	rExit.y = 0;
	ghost = IMG_Load("res/gfx/ghost.png");
	texture_ghost = SDL_CreateTextureFromSurface(render, ghost);
	rGhost.h = ghost->h;
	rGhost.w = ghost->w;
	menu_loop(); // located in Menu.cpp - start the menu loop where we wait for user's choise
}

void game_loop() // called when we switch the game state to some level
{
	while ( running )
	{
		game_input(); // try to get user input and handle it
		collision_detect(); // try to check for collision with traps or other objects so we can react to that
		game_rendering(); // renders the current state of the game to the window
		SDL_Delay(10); // short delay for stability reasons
	}
	// if we choose to quit the game from some level(not just return to the menu_loop()), the running variable for the loop will be set to false
	release_level(); // located in Level.cpp - releases the resources of the level so we can proceed to quit
	release_system(); // located just below - releases the main resources and quis the game
}

void release_system() // can be called from the game loop or the menu loop - depends where we choose to quit
{
	// destroy the renderer and window objecs
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	// uninitialize the libaries
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	// quit the program successfully(same as "return 0;" in the main function)
	exit(0);
	// nothing else will be executed
}