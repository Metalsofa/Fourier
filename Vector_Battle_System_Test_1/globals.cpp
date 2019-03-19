//Any declarations that need to be visible to all cpp files goes here
//NOTE: In principle, no header file should have to deal with importing
//This one. Every cpp document, on the other hand, should.


//Include global variables
#include "globals.h"

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
//extern enum designMode { BD_CREATE_WALLS, BD_ERASE_WALLS, BD_MAKE_RAYS, BD_MAKE_SHAPES };

//Global Variables for detecting mode
extern bool artMode = false;
extern bool overworldMode = false;
extern bool battleMode = true;
extern bool battlefieldDesignMode = true;
extern bool debugMode = true;

//Global settings (mostly debug)
extern bool showCorners = false; ///Debug: Draw the corners formed by wall intersections?
extern bool showTicks = false; ///Finally found those annoying cursor ticks and made a setting for showing them.
extern bool enablePersprot = false; ///fun: enable the user to rotate their perspective with the middle mouse button
extern bool showTimer = false; ///Debug: Show the timer in the lower left
extern bool showFPS = true;
extern int FPS = 0; ///Calculated and frequently-updated FPS
extern bool showConsole = false;

//Global stopwatch
extern Stopwatch st {};

//The old GL-console cared about these
extern string userInput = "";
extern int consoleScroll = 0;


///////////////////////////////////////////////////////////
//
//			Global Variables:
//				ART MODE
//
///////////////////////////////////////////////////////////

//Global Variables for art mode
extern bool showLayers = true; ///Art: Show the intuitive shape to explain what layer is being edited?
extern bool showDots = true; ///Art: Show the dots in the currently edited shape
extern animation animart = animation();
extern graphic art = graphic();
extern int editingLayer = 0;
extern int editingFrame = 0;
extern int dragdot = -1;
extern point layDispPos(-1, 4); ///Art: Where the 'Frames&layers' matrix gui goes
extern float ladDispSize = 0.7f; ///Art: How big should each cell in the editor matrix appear?
extern string currentGraphicName = "";


///////////////////////////////////////////////////////////
//
//			Global Variables:
//				BATTLE
//
///////////////////////////////////////////////////////////

//Global Variables for if in battleMode
extern battlePreset testpreset(11, 7); //Width, Height
extern battlestate currentbattle(testpreset); //This holds the information about the current battle scene

extern const int BoardDepth = 0; //Not sure about this. Probably will never change from 0.
extern float timer = 0; //We'll see if this ends up being necessary
extern float increment = float(1.0f / (60.0f + .01f) );//0.01f; //Incremental time in seconds, the denominator is nominal FPS
extern float gamma = 1.0f; //Time dilation, from the viewer's refrence frame
extern int rain = 0; //This is really just for fun; good for iterating through a rainbow for no good reason

//Global Variables for battlefieldDesignMode
extern int DESIGN_FUNCTION = BD_MAKE_RAYS;
extern metastat CHOSEEN_COLOR = clCyan;
extern Material SELECTED_MATERIAL = DEFAULT;


///////////////////////////////////////////////////////////
//
//			Global Variables:
//				CONTROLS
//
///////////////////////////////////////////////////////////

//Whether using keyboard controls (neccessary only in BETA)
extern bool keyMode = false;
extern void exteriorConsole();
//Global Variables for if in overworldMode
///I hear cactuses hurt to step on

//Global Key Detections
extern map<char, bool> normalKeysdown = map<char, bool>();

///Special and other keys
extern bool upPress = false;
extern bool leftPress = false;
extern bool downPress = false;
extern bool rightPress = false;
extern int upBuf = 0;
extern int downBuf = 0;
extern int leftBuf = 0;
extern int rightBuf = 0;
extern const int keyBuf = 5;
extern bool escPress = false;
extern bool enterPress = false;
extern bool spacePress = false;
extern bool backspacePress = false;
///For Numerals in Particular

//Global Mouse Class
extern cursor mouse = cursor();

//Global MouseButton Detections
extern bool leftclicking = false;
extern bool rightclicking = false;
extern bool middleclicking = false;
extern segment clickdragtrail(0.0f, 0.0f, 0.0f, 0.0f);
extern segment rightclicktrail(0.0f, 0.0f, 0.0f, 0.0f);


///////////////////////////////////////////////////////////
//
//			Global Variables:
//				CAMERA
//
///////////////////////////////////////////////////////////

//Global Camera Position Data
extern float LookPointX = float(currentbattle.boardWidth()) / 2.0f;
extern float LookPointY = float(currentbattle.boardHeight()) / 2.0f;
extern float LookPointZ = BoardDepth / 2.0f;
extern const float PerspectiveRiseMax = PI / 2.0f - 0.0005f;
extern float PerspectiveOrbit = 0.0f;
extern float PerspectiveRise = PerspectiveRiseMax;
extern float PerspectiveDist = 10.0f; //7.5f;


