
//Any declarations that need to be visible to all cpp files goes here
//NOTE: In principle, no header file should have to deal with importing
//This one. Every cpp document, on the other hand, should.

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

//Naturally A lot of things have to be included
#include "geometry.h"
#include "crypt.h"
#include "gl/glut.h"
#include "Stopwatch.h"
#include "graphics.h"
#include "battle.h"
#include "camera.h"
#include <string>

//Namespaces
using namespace std;
using win32::Stopwatch;

///////////////////////////////////////////////////////////
//
//			Global Variables:
//				DEBUG/DEVELOPMENT
//
///////////////////////////////////////////////////////////

//Design function enums
enum designMode { BD_CREATE_WALLS, BD_ERASE_WALLS, BD_MAKE_RAYS, BD_MAKE_SHAPES };

//Global Variables for detecting mode
extern bool artMode;
extern bool overworldMode;
extern bool battleMode;
extern bool battlefieldDesignMode;
extern bool debugMode;

//Global settings (mostly debug)
extern bool showCorners;
extern bool showTicks;
extern bool enablePersprot;
extern bool showTimer;
extern bool showFPS;
extern int FPS;
extern bool showConsole;

//Global stopwatch
extern Stopwatch st;

//The old GL-console cared about these
extern string userInput;
extern int consoleScroll;


///////////////////////////////////////////////////////////
//
//			Global Variables:
//				ART MODE
//
///////////////////////////////////////////////////////////

//Global Variables for art mode
extern bool showLayers;
extern bool showDots;
extern animation animart;
extern graphic art;
extern int editingLayer;
extern int editingFrame;
extern int dragdot;
extern point layDispPos;
extern float ladDispSize;
extern string currentGraphicName;


///////////////////////////////////////////////////////////
//
//			Global Variables:
//				BATTLE
//
///////////////////////////////////////////////////////////

//Global Variables for if in battleMode
extern battlePreset testpreset; //Width, Height
extern battlestate currentbattle; //This holds the information about the current battle scene

extern const int BoardDepth;
extern float timer;
extern float increment;
extern float gamma;
extern int rain;

//Global Variables for battlefieldDesignMode
extern int DESIGN_FUNCTION;
extern metastat CHOSEEN_COLOR;
extern Material SELECTED_MATERIAL;


///////////////////////////////////////////////////////////
//
//			Global Variables:
//				CONTROLS
//
///////////////////////////////////////////////////////////

//Whether using keyboard controls (neccessary only in BETA)
extern bool keyMode;
void exteriorConsole();
//Global Variables for if in overworldMode
///I hear cactuses hurt to step on

//Global Key Detections
extern map<char, bool> normalKeysdown;

///Special and other keys
extern bool upPress;
extern bool leftPress;
extern bool downPress;
extern bool rightPress;
extern int upBuf;
extern int downBuf;
extern int leftBuf;
extern int rightBuf;
extern const int keyBuf;
extern bool escPress;
extern bool enterPress;
extern bool spacePress;
extern bool backspacePress;
///For Numerals in Particular

//Global Mouse Class
extern cursor mouse;

//Global MouseButton Detections
extern bool leftclicking;
extern bool rightclicking;
extern bool middleclicking;
extern segment clickdragtrail;
extern segment rightclicktrail;


///////////////////////////////////////////////////////////
//
//			Global Variables:
//				CAMERA
//
///////////////////////////////////////////////////////////

//Global Camera Position Data
extern float LookPointX;
extern float LookPointY;
extern float LookPointZ;
extern const float PerspectiveRiseMax;
extern float PerspectiveOrbit;
extern float PerspectiveRise;
extern float PerspectiveDist;

#endif
