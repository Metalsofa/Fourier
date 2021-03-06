/*Main .cpp file for this RPG. Mainly handles graphics and iterative callbacks.
The main function is rather short; the function 'renderscene' is the real 'main'
function. It may be prudent to later change what it is called.*/

//Include global variables
#include "globals.h"

//Custom header includes
//#include "geometry.h"
#include "crypt.h"
#include "gl/glut.h"
#include "Stopwatch.h"
#include "graphics.h"

//STL/etc. includes
#include <Windows.h>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <map>
#include <time.h>
#include <vector>

//Namespaces
using namespace std;
using namespace fgr;
using win32::Stopwatch;


//Be careful; certain headers should only be included after global declaration
#include "controls.h"
#include "battle.h"
#include "console.h"
#include "camera.h"
#include "customGL.h"
#include "players.h"


//Utilizes a timer to keep te clocks spinning at the right pace
void clocksync() {
	//Sleep(int((increment * 1000.0f * gamma)));
	if (st.ElapsedMilliseconds() > increment * 1.2f * 1000.0f * gamma)
		cout << "@frame " << int(timer) << " - TIME: " << st.ElapsedMilliseconds() << endl;
	int timeToNextItr = int((double(increment * 1000.0f * gamma) - st.ElapsedMilliseconds()));
	if (timeToNextItr > 0) {
		Sleep(timeToNextItr);
	}
	//while (st.ElapsedMilliseconds() < double(increment * 1000.0f * gamma)) {}
	FPS = int(1000 / st.ElapsedMilliseconds());
	timer += increment * 100;
	st.Stop();
	st.Start();
}

void controlsInit() {	//initializes instMap
	instMap[' '] = "action";
	instMap['w'] = "moveUp";
	instMap['a'] = "moveLeft";
	instMap['s'] = "moveDown";
	instMap['d'] = "moveRight";
	instMap['i'] = "aimUp";
	instMap['j'] = "aimLeft";
	instMap['k'] = "aimDown";
	instMap['l'] = "aimRight";
	//instMap['`'] = "showConsole";
	instMap['0'] = "keyModeTog";
	instMap['1'] = "1";
	instMap['2'] = "2";
	instMap['3'] = "3";
	instMap['4'] = "4";
	instMap[27] = "escape";
	instMap[13] = "enter";
	instMap[8] = "backspace";
	instMap['h'] = "artModifier";
	instMap[GLUT_KEY_UP - 110] = "upPress";
	instMap[GLUT_KEY_DOWN - 110] = "downPress";
	instMap[GLUT_KEY_LEFT - 110] = "leftPress";
	instMap[GLUT_KEY_RIGHT - 110] = "rightPress";
}

void handleControls() {

	//Some controls will always be available, for now, like closing the program.
	evergreenKeychecks();

	//Controls for battle mode
	if (battleMode) {
		battleKeychecks();
	}

	//Controls for battlefield design mode
	if (battlefieldDesignMode) {
		battlefieldDesignKeychecks();
	}

	//Controls for art mode
	if (artMode) {
		artKeychecks();
	}

	//Controls for debug mode
	if (debugMode) {
		debugKeychecks();
	}

}

//Contains all gl-code; there should be no need to have any outside of this function
void renderScene(void) {
	//Screen-cleanup
	// Clear Color and Depth Buffers
	ClearScreen();
	// Reset transformations
	glLoadIdentity();
	// Set the camera to look at the appropriate spot (usually center of the board)
	definecamera();

	//////Call all of our screen-rendering functions
	////Draw Battlefield Geometry
	if (battleMode) {
		drawaxes();
		//Draw Walls
		int walliterator = 0;
		while (walliterator < int(currentbattle.map.wallCount())) {
			drawwall(currentbattle.map.getWalls()[walliterator]);
			walliterator++;
		}
		//Draw Portals
		for (portal& p : currentbattle.map.portals) {
			drawportal(p);
		}

		//Draw Spells

		//Draw Rays
		for (unsigned int i = 0; i < currentbattle.rayCount(); i++) {
			drawray(currentbattle.rays[i]);
		}
		////Draw Combatants
		if (battleMode && !artMode) {
			for (combatant& c : currentbattle.protags) {
				drawCombatant(c);
			}
			for (combatant& c : currentbattle.antags) {
				drawCombatant(c);
			}
		}
		//TODO: DRAW HUD, player stats, etc.
	}
	////Debug-only drawing
	//Debug: Show the number of objects
	if (showCorners) {
		for (unsigned int i = 0; i < currentbattle.map.intersections().size(); i++) {
			fgr::drawPoint(currentbattle.map.intersections()[i]);
			glColor3f(1, 1, 1);
		}
	}
	//Debug: object counts
	const bool showObjectCounts = false;
	if (showObjectCounts) {
		rendertext(point(0.0f, 4.0f), to_string(currentbattle.map.wallCount()) + " walls.");
		rendertext(point(0.0f, 5.0f), to_string(currentbattle.rayCount()) + " rays.");
	}
	//Debug: Show timer
	if (showTimer) {
		rendertext(point(0.0f, 0.0f), to_string(timer));
	}

	if (showFPS) {
		rendertext(point(0.0f, -0.5), to_string(FPS) + " FPS");
	}
	////Draw console //Not sure we really do that anymore
	if (showConsole)
		drawConsole();
	////Draw art GUI if in art mode
	if (artMode) {
		artKeychecks();
	}
	////Draw mouse
	if ((battlefieldDesignMode && !keyMode) || artMode) {
		drawCursor(mouse);
		mouse.Rot += (mouse.rotSpeed / mouse.spread) * increment;
	}
	//This is the function that refreshes the canvas and implements everything we've 'drawn'
	glutSwapBuffers();
}

//This function is called every time the application is left idle, and handles all forms of iteration
void iterateGame() {

	//Make sure we aren't dropping frames unneccesarily
	clocksync();

	//Render everything
	renderScene();

	//Check all the controls deemed neccessary by global booleans
	handleControls();

	//Iterative behavior
	currentbattle.iterate(increment);
}

//main function; exists to set up a few things and then enter the glut-main-loop
int main(int argc, char **argv) {
	//For development purposes
	///Tumbleweeds
	//Console welcome
	outputConsole("Welcome to the Vector_Battle_System_Test_1.cpp Console");
	outputConsole("Type help for a list of commands.");

	//Initialize rand
	srand(unsigned int(time(NULL)));

	//Initialize GLUT
	glutInit(&argc, argv);

	//Choose some settings for our Window
	if (!debugMode)
		glutEnterGameMode();
	else {
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(900, 500);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

		//Create the Window
		glutCreateWindow("Fourier");
		glutFullScreen();
	}

	//Some settings
	glutIgnoreKeyRepeat(1);
	glutSetCursor(GLUT_CURSOR_NONE); //Hide the cursor

	//Initialize controls:
	controlsInit();

	//register callbacks
	glutDisplayFunc(renderScene); //Callback for when we refresh
	glutReshapeFunc(changeSize); //Callback for when window is resized
	glutIdleFunc(iterateGame); //Callback for when the programs is idle (this will happen continuously)
	glutKeyboardFunc(ProcessNormalKeys); //Callback pressing a "normal" key
	glutSpecialFunc(ProcessSpecialKeys); //Callback for a "special" key
	glutKeyboardUpFunc(ReleaseNormalKeys); //Callback for releasing "normal" keys
	glutSpecialUpFunc(ReleaseSpecialKeys); //Callback for releasing special keys
	glutMouseFunc(MouseClick); //Callback for mouse clicks
	glutMotionFunc(PassiveMouseMove); //Callback for mouse movement with button down
	glutPassiveMotionFunc(PassiveMouseMove); //Callback for mouse movement with no button down


	//Setup battle: Initialize combatants		//TODO: Move this to a playerinit func for battlemode
	player plyr1("teststat1.txt", point(0,6), graphic("alpha.fgr"));		
	plyr1.arsenal.push_back(wallConst(BASIC_REFLECTIVE,true,0));
	plyr1.arsenal[0].cost = 20;
	currentbattle.protags.push_back(plyr1);

	player plyr2("teststat2.txt", point(10, 6), graphic("beta.fgr"));
	plyr2.arsenal.push_back(rayConst(metastat(255, 255, 255), 5, 3, 1));
	plyr2.arsenal[0].cost = 5;
	currentbattle.protags.push_back(plyr2);

	player plyr3("teststat3.txt", point(10, 0), graphic("gamma.fgr"));
	plyr3.arsenal.push_back(portalConst(BASIC_REFLECTIVE));
	plyr3.arsenal[0].cost = 20;
	currentbattle.protags.push_back(plyr3);

	player plyr4("teststat4.txt", point(0,0), graphic("delta.fgr"));
	plyr4.arsenal.push_back(rayConst(metastat(255, 255, 255), 5, 1, 1));
	plyr4.arsenal[0].cost = 5;
	currentbattle.protags.push_back(plyr4);

	enemy e1(1, 3);
	e1.position = point(7, 3);
	e1.width = .4f;
	plyr4.sprite = (graphic("delta.fgr"));
	currentbattle.antags.push_back(e1);
	currentbattle.antags[0].addWaypoint(point(7, 2.5));
	currentbattle.antags[0].addWaypoint(point(7, 4));
	currentbattle.antags[0].addWaypoint(point(1, 3));
	currentbattle.antags[0].addWaypoint(point(3, 1));

	//enter GLUT event processing cycle
	st.Start();
	glutMainLoop();
	if (debugMode) {
		glutLeaveGameMode();
	}

	return 0;
}
