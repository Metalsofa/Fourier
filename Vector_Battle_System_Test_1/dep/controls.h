/*All player controls, and some debug ones, go in here.*/
#pragma once
#ifndef __controls_h__
#define __controls_h__

#include "battle.h"
#include "materials.h"



#include "customGL.h"

using namespace std;
using namespace fgr;

/*A 'keycheck' function contains code for checking AND interpreting the presently pressed keys
under a given scenario, calling the appropriate functions if neccessary.*/




//These controls will always be active
void evergreenKeychecks();

//These controls are active only in debug mode
void debugKeychecks();

//These controls are active only in battlefield-design mode
void battlefieldDesignKeychecks();

//These controls are active only in art mode
void artKeychecks();

//These controls are active only in battle mode
void battleKeychecks();

//These controls are active only in overworld mode
void overworldKeychecks();


////////////////GLUT key/mouse action callback functions////////////////

//Called when a 'normal' key becomes pressed
void ProcessNormalKeys(unsigned char key, int x, int y);

//Called when a 'normal' key becomes no longer pressed
void ReleaseNormalKeys(unsigned char key, int x, int y);

//Called when a 'special' key becomes pressed
void ProcessSpecialKeys(int key, int x, int y);

//Called when a 'special' key is no longer pressed
void ReleaseSpecialKeys(int key, int x, int y);

void MouseClick(int button, int state, int x, int y);

void ActiveMouseMove(int x, int y);

void PassiveMouseMove(int x, int y);


#endif