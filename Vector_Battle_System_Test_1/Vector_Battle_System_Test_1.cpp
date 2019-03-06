/*Main .cpp file for this RPG. Mainly handles graphics and iterative callbacks.
The main function is rather short; the function 'renderscene' is the real 'main'
function. It may be prudent to later change what it is called.*/

#include "gl/glut.h"
#include "Stopwatch.h"
#include "graphics.h"
#include "art.h"
#include "players.h"

#include <Windows.h>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <map>
#include <time.h>
#include <vector>

//Namespaces
using namespace std;
using win32::Stopwatch;


//Global Variables for detecting mode
bool artMode = false;
bool overworldMode = false;
bool battleMode = true;
bool battlefieldDesignMode = true;
bool debugMode = true;


//Global settings (mostly debug)
bool showCorners = false; ///Debug: Draw the corners formed by wall intersections?
bool showTicks = false; ///Finally found those annoying cursor ticks and made a setting for showing them.
bool enablePersprot = false; ///fun: enable the user to rotate their perspective with the middle mouse button
bool showTimer = false; ///Debug: Show the timer in the lower left
bool showFPS = true;
int FPS = 0; ///Calculated and frequently-updated FPS

//Global stopwatch
Stopwatch st;

//Be careful; certain headers should only be included after global declaration
#include "art.h"
#include "console.h"
#include "controls.h"
#include "battle.h"
#include "camera.h"
#include "customGL.h"


//Utilizes a timer to keep te clocks spinning at the right pace
void clocksync() {
	//Sleep(int((increment * 1000.0f * gamma)));
	if (st.ElapsedMilliseconds() > increment * 1.2f * 1000.0f * gamma)
		cout << "@frame " << int(timer) << " - TIME: " << st.ElapsedMilliseconds() << endl;
	while (st.ElapsedMilliseconds() < double(increment * 1000.0f * gamma)) {}
	FPS = 1000 / st.ElapsedMilliseconds();
	timer += increment * 100;
	st.Stop();
	st.Start();
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

		//Draw Spells

		//Draw Rays
		for (unsigned int i = 0; i < currentbattle.rayCount(); i++) {
			drawray(currentbattle.rays[i]);
		}
		////Draw Combatants
		if (battleMode && !artMode) {
			for (int i = 0; i < currentbattle.fighters.size(); i++) {
				drawCombatant(currentbattle.fighters[i]);
			}
		}
	}
	////Debug-only drawing
	//Debug: Show the number of objects
	if (showCorners) {
		for (unsigned int i = 0; i < currentbattle.map.intersections().size(); i++) {
			drawPoint(currentbattle.map.intersections()[i]);
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

	//If this program was opened with arguments:
	if (argc > 1) {
		string filename = argv[1];
		string extention = "";
		bool ext = false;
		//Get the file extention
		for (unsigned int c = 0; c < filename.size(); c++) {
			if (ext) {
				extention.push_back(filename[c]);
			}
			if (filename[c] == '.') {
				ext = true;
			}
		}
		for (unsigned int i = 0; i <= extention.size(); i++) {
			filename.pop_back();
		}
		if (extention == "fgr") {
			artMode = true;
			art = graphic(filename + '.' + extention);
			currentGraphicName = filename;
			battlefieldDesignMode = true;
			battleMode = true;
			overworldMode = false;
			DESIGN_FUNCTION = BD_MAKE_SHAPES;
			outputConsole("Opened graphic file " + filename);
		}
	}

	//DP: Initialize rand
	srand(unsigned int(time(NULL)));

	//Initialize GLUT
	glutInit(&argc, argv);

	//Choose some settings for our Window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(900, 500);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//Create the Window
	glutCreateWindow("Fourier");
	glutFullScreen();

	//Some settings
	glutIgnoreKeyRepeat(1);
	glutSetCursor(GLUT_CURSOR_NONE); //Hide the cursor

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


	//Setup battle: Initialize combatants
	combatant plyr1;
	plyr1.position = point(6, 3);
	plyr1.turn(0);
	plyr1.tog = false;
	plyr1.sprite = graphic("alpha.fgr");
	plyr1.width = .4;
	currentbattle.fighters.push_back(plyr1);

	combatant plyr2;
	plyr2.position = point(5, 4);
	plyr2.turn(0);
	plyr2.tog = false;
	plyr2.sprite = graphic("beta.fgr");
	plyr2.width = .4;
	currentbattle.fighters.push_back(plyr2);

	combatant plyr3;
	plyr3.position = point(4, 3);
	plyr3.turn(0);
	plyr3.tog = false;
	plyr3.sprite = graphic("gamma.fgr");
	plyr3.width = .4;
	currentbattle.fighters.push_back(plyr3);

	combatant plyr4;
	plyr4.position = point(5, 2);
	plyr4.turn(0);
	plyr4.tog = false;
	plyr4.sprite = graphic("delta.fgr");
	plyr4.width = .4;
	currentbattle.fighters.push_back(plyr4);

	//enter GLUT event processing cycle
	st.Start();
	glutMainLoop();

	return 0;
}