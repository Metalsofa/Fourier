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
#include "players.h"
//Global Variables for the console.
/*NOTE: Some of these go unusied right now. May be subject to future cleanup.*/
//vector<string> console_history;
string userInput = "";
int consoleScroll = 0;




void feedkeyboardinput(string &field);

void outputConsole(string output);

//Process an input string
void handleInput(string &input);

//Pause the game and activate the exterior console for user input.
void exteriorConsole();

void drawConsole();


#endif