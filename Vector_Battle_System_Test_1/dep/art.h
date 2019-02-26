#pragma once
#ifndef __art_h
#define __art_h

#include "graphics.h"
#include <string>

//Global Variables for art mode
bool showLayers = true; ///Art: Show the intuitive shape to explain what layer is being edited?
bool showDots = true; ///Art: Show the dots in the currently edited shape
animation animart; //It's called anime and it's art
graphic art;
int Gindex = 0;
int dragdot = -1;
string currentGraphicName;






#endif