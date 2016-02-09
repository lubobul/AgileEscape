
// contains solely main function - application starts here

// Agile Escape game entry point
// Author: Konstantin V. Hadzhiev
// Contact: kvhadzhiev@gmail.com

#include "Main.h"
#include "resource.h" // for the game executable's icon

int main(int argc, char *argv[]) // start the program
{
	// set up our graphics systems and continue from there
	init_system(); // located in Run.cpp - initiates all libraries and creates the window
	// we actually exit the game at different place and never return to the main function
	return 0;
}