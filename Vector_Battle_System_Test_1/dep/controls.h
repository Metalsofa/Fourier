/*All player controls, and some debug ones, go in here.*/
#pragma once
#ifndef __controls_h__
#define __controls_h__

#include "battle.h"
#include "materials.h"


bool keyMode = false; //Whether using keyboard controls (BETA)
void exteriorConsole();
//Global Variables for if in overworldMode
///I hear cactuses hurt to step on

//Global Variables for if in battleMode
battlePreset testpreset(11, 7); //Width, Height
battlestate currentbattle(testpreset); //This holds the information about the current battle scene

const int BoardDepth = 0; //Not sure about this. Probably will never change from 0.
float timer = 0; //We'll see if this ends up being necessary
float increment = float(1.0f / (60.0f + .01f) );//0.01f; //Incremental time in seconds, the denominator is nominal FPS
float gamma = 1.0f; //Time dilation, from the viewer's refrence frame
int rain = 0; //This is really just for fun; good for iterating through a rainbow for no good reason

//Global Variables for battlefieldDesignMode
int DESIGN_FUNCTION = BD_MAKE_RAYS;
metastat CHOSEEN_COLOR = clCyan;
Material SELECTED_MATERIAL = DEFAULT;



//Global Key Detections
map<char, bool> normalKeysdown;

///Special and other keys
bool upPress = false; 
bool leftPress = false;
bool downPress = false;
bool rightPress = false;
int upBuf;
int downBuf;
int leftBuf;
int rightBuf;
const int keyBuf = 5;
bool escPress = false;
bool enterPress = false;
bool spacePress = false;
bool backspacePress = false;
///For Numerals in Particular

//Global MouseButton Detections
bool leftclicking = false;
bool rightclicking = false;
bool middleclicking = false;
segment clickdragtrail(0.0f, 0.0f, 0.0f, 0.0f);
segment rightclicktrail(0.0f, 0.0f, 0.0f, 0.0f);

#include "customGL.h"

using namespace std;

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