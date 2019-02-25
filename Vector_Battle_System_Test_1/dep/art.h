#pragma once
#ifndef __art_h
#define __art_h

#include "graphics.h"
#include <string>

//Global Variables for art mode
bool show_layers = true; ///Art: Show the intuitive shape to explain what layer is being edited?
bool show_dots = true; ///Art: Show the dots in the currently edited shape
animation animart; //It's called anime and it's art
graphic art;
int Gindex = 0;
int dragdot = -1;
string current_graphic_name;






#endif