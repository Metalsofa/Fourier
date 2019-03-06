#pragma once
#ifndef __art_h
#define __art_h

#include "graphics.h"
#include <string>

//Global Variables for art mode
bool showLayers = true; ///Art: Show the intuitive shape to explain what layer is being edited?
bool showDots = true; ///Art: Show the dots in the currently edited shape
animation animart;
graphic art;
int editingLayer = 0;
int editingFrame = 0;
int dragdot = -1;
point layDispPos(-1, 4); ///Art: Where the 'Frames&layers' matrix gui goes
float ladDispSize = 0.7f; ///Art: How big should each cell in the editor matrix appear?
string currentGraphicName;






#endif