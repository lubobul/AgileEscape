
// functions to play audio background music and sound effects

#include "Main.h"
#include "Globals.h"

// the audio variables for loading and playing files
Mix_Music *bgr_music;
Mix_Chunk *fx_music, *jump_music;

void play_bgr(const char* music) // called when we switch to menu or start some level to play the corresponding background music
{
	bgr_music = Mix_LoadMUS(music); // load the music file from the path that we passed as parameter when calling the function
	Mix_PlayMusic(bgr_music, -1); // endlessly loop the background music
}

void play_fx(const char* music) // called when an action happens - for example player getting killed or finishing the level
{
	fx_music = Mix_LoadWAV(music);
	Mix_PlayChannel(-1, fx_music, 0); // play the short sound effect on some audio channel, so we don't interfere with the background music
}

void play_jump() // called when the player jumps, path to the jump sound file already set in the init function
{
	Mix_PlayChannel(-1, jump_music, 0);
}