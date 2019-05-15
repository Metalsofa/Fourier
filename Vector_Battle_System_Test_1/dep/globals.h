
//Any declarations that need to be visible to all cpp files goes here
//NOTE: In principle, no header file should have to deal with importing
//This one. Every cpp document, on the other hand, should.

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

//Naturally A lot of things have to be included
//#include "geometry.h"
#include "fgrutils.h"
#include "crypt.h"
#include "gl/glut.h"
#include "Stopwatch.h"
#include "graphics.h"
#include "spells.h"
#include "battle.h"
#include "camera.h"
#include <string>

//Namespaces
using namespace std;
using namespace fgr;
using win32::Stopwatch;

///////////////////////////////////////////////////////////
//
//			Global Variables:
//				DEBUG/DEVELOPMENT
//
///////////////////////////////////////////////////////////

//Design function enums
enum designMode { BD_CREATE_WALLS, BD_ERASE_WALLS, BD_MAKE_RAYS, BD_MAKE_SHAPES };

//enum Color {clWhite = 0xFFFFFF, clBlack = 0x0, clRed = 0xFF0000, clOrange = 0xFF7F00, clYellow = 0xFFFF00,
//	clLime = 0x7FFF00, clGreen = 0x00FF, clTeal = 0x00FF7F, clCyan = 0x00FFFF, clIndigo = 0x007FFF, clBlue = 0x0000FF,
//	clPurple = 0x7F00FF, clMagenta = 0xFF00FF, clViolet = 0xFF007F
//};

#define clWhite metastat(255, 255, 255) /*White*/
#define clBlack metastat(0, 0, 0) /*Black*/
#define clRed metastat(255,0,0) /*Red*/
#define domainRed "action"
#define clOrange metastat(255,127,0) /*Orange*/
#define domainOrange "artistry"
#define clYellow metastat(255,255,0) /*Yellow*/
#define domainYellow "expression"
#define clLime metastat(127,255,0) /*Lime*/
#define domainLime "evocation"
#define clGreen metastat(0,255,0) /*Green*/
#define domainGreen "emotion"
#define clTeal metastat(0,255,127) /*Teal*/
#define domainTeal "introspection"
#define clCyan metastat(0,255,255) /*Cyan*/
#define domainCyan "belief"
#define clIndigo metastat(0,127,255) /*Indigo*/
#define domainIndigo "heuristic"
#define clBlue metastat(0,0,255) /*Blue*/
#define domainBlue "cognition"
#define clPurple metastat(127,0,255) /*Purple*/
#define domainPurple "synthesis"
#define clMagenta metastat(255,0,255) /*Magenta*/
#define domainMagenta "application"
#define clViolet metastat(255,0,127) /*Violet*/
#define domainViolet "operation"


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
//				OVERWORLD
//
///////////////////////////////////////////////////////////



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
extern map<char, bool> normalKeysdown;	//TODO: remove this when art and console controls are removed
extern map<char, string> instMap;	//EDIT THIS TO CHANGE KEYBINDINGS, reference controlsInit() for default keybindings
extern map<string, bool> instDown;	//Is edited by the program, determines which buttons are being pressed


///Special and other keys
//extern bool upPress;		//No need for these, Use instDown["upPress"] instead
//extern bool leftPress;
//extern bool downPress;
//extern bool rightPress;
extern int upBuf;
extern int downBuf;
extern int leftBuf;
extern int rightBuf;
extern const int keyBuf;
//extern bool escPress;		//No need for these, use instDown[escape]
//extern bool enterPress;	//instDown["enter"]
//extern bool spacePress;	//instDown["action"]
//extern bool backspacePress;	//instDown["backspace"]
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
