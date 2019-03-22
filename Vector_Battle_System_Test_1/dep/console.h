/*This header file is home to a lot of console, design, and debug functionality.
In theory, nothing that the player will need during gameplay should be in here.*/
#pragma once
#ifndef __console_h__
#define __console_h__

#include <string>
#include "gl/glut.h"
#include "battle.h"
#include "controls.h"
#include "customGL.h"
//#include "players.h"


//vector<string> console_history;



//Put into the string any characters being pressed on the keyboard
void feedkeyboardinput(string &field);

//Send a message to the console
void outputConsole(string output);

//Process an input string
void handleInput(string &input);

//Pause the game and activate the exterior console for user input.
void exteriorConsole();

//Deprecated code for rendering the console
void drawConsole();


#endif