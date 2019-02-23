/*Main .cpp file for this RPG. Mainly handles graphics and iterative callbacks.
The main function is rather short; the function 'renderscene' is the real 'main'
function. It may be prudent to later change what it is called.*/

#include "gl/glut.h"
#include "Stopwatch.h"
#include "players.h"
#include "battle.h"

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
bool art_mode = false;
bool overworld_mode = false;
bool battle_mode = true;
bool battlefield_design_mode = true;


//Global settings (mostly debug)
bool show_corners = false; ///Debug: Draw the corners formed by wall intersections?
bool show_ticks = false; ///Finally found those annoying cursor ticks and made a setting for showing them.
bool enable_persprot = false; ///fun: enable the user to rotate their perspective with the middle mouse button
bool show_timer = false; ///Debug: Show the timer in the lower left

//Global Variables for art mode
bool show_layers = true; ///Art: Show the intuitive shape to explain what layer is being edited?
bool show_dots = true; ///Art: Show the dots in the currently edited shape
graphic art;
int Gindex = 0;
int dragdot = -1;
string current_graphic_name;

//Global stopwatch
Stopwatch st;

//Be careful; certain headers should only be included after global declaration
#include "console.h"
#include "controls.h"
#include "camera.h"
#include "customGL.h"


//Utilizes a timer to keep te clocks spinning at the right pace
void clocksync() {
	//Sleep(int((increment * 1000.0f * gamma)));
	while (st.ElapsedMilliseconds() < double(increment * 1000.0f * gamma)) {}
	if (st.ElapsedMilliseconds() > 12.0f)
		cout << "@frame " << int(timer) << " - TIME: " << st.ElapsedMilliseconds() << endl;
	timer += increment * 100;
	st.Stop();
	st.Start();
}

void handle_controls() {

	//Some controls will always be available, for now, like closing the program.
	evergreen_keychecks();

	//Controls for battle mode
	if (battle_mode) {
		battle_keychecks();
	}

	//Controls for battlefield design mode
	if (battlefield_design_mode) {
		battlefield_design_keychecks();
	}

	//Controls for art mode
	if (art_mode) {
		art_keychecks();
	}

}

//Contains all gl-code; there should be no need to have any outside of this function
void renderScene(void) {

	////Screen-cleanup
	// Clear Color and Depth Buffers
	ClearScreen();
	// Reset transformations
	glLoadIdentity();
	// Set the camera to look at the appropriate spot (usually center of the board)
	definecamera();

	//////Call all of our screen-rendering functions
	////Draw Battlefield Geometry
	drawaxes();
	//Draw Walls
	int walliterator = 0;
	while (walliterator < int(currentbattle.map.wallcount())) {
		drawwall(currentbattle.map.getwalls()[walliterator]);
		walliterator++;
	}
	////Draw Combatants
	for (int i = 0; i < currentbattle.fighters.size(); i++) {
		draw_combatant(currentbattle.fighters[i]);
	}

	//Draw Spells

	//Draw Rays
	for (unsigned int i = 0; i < currentbattle.raycount(); i++) {
		drawray(currentbattle.rays[i]);
	}
	////Debug-only drawing
	//Debug: Show the number of objects
	if (show_corners) {
		for (unsigned int i = 0; i < currentbattle.map.intersections().size(); i++) {
			drawpoint(currentbattle.map.intersections()[i]);
			glColor3f(1, 1, 1);
		}
	}
	//Debug: object counts
	const bool show_objectcounts = false;
	if (show_objectcounts) {
		rendertext(point(0.0f, 4.0f), to_string(currentbattle.map.wallcount()) + " walls.");
		rendertext(point(0.0f, 5.0f), to_string(currentbattle.raycount()) + " rays.");
	}
	//Debug: Show timer
	if (show_timer) {
		rendertext(point(0.0f, 0.0f), to_string(timer));
	}
	////Draw console //Not sure we really do that anymore
	if (show_console)
		draw_console();
	////Draw mouse
	if (battlefield_design_mode && !key_mode) {
		drawcursor(mouse);
		mouse.Rot += (mouse.RotSpeed / mouse.Spread) * increment;
	}
	//This is the function that refreshes the canvas and implements everything we've 'drawn'
	glutSwapBuffers();
}

//This function is called every time the application is left idle, and handles all forms of iteration
void iterate_game() {

	//Make sure we aren't dropping frames unneccesarily
	clocksync();

	//Render everything
	renderScene();

	//Check all the controls deemed neccessary by global booleans
	handle_controls();

	//Iterative behavior
	currentbattle.iterate(increment);

}

//main function; exists to set up a few things and then enter the glut-main-loop
int main(int argc, char **argv) {
	//For development purposes
	///Tumbleweeds
	//Console welcome
	output_console("Welcome to the Vector_Battle_System_Test_1.cpp Console");
	output_console("Type help for a list of commands.");

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
			art = graphic(filename + '.' + extention);
			current_graphic_name = filename;
			battlefield_design_mode = true;
			battle_mode = true;
			overworld_mode = false;
			DESIGN_FUNCTION = BD_MAKE_SHAPES;
			output_console("Opened graphic file " + filename);
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
	glutIdleFunc(iterate_game); //Callback for when the programs is idle (this will happen continuously)
	glutKeyboardFunc(ProcessNormalKeys); //Callback pressing a "normal" key
	glutSpecialFunc(ProcessSpecialKeys); //Callback for a "special" key
	glutKeyboardUpFunc(ReleaseNormalKeys); //Callback for releasing "normal" keys
	glutSpecialUpFunc(ReleaseSpecialKeys); //Callback for releasing special keys
	glutMouseFunc(MouseClick); //Callback for mouse clicks
	glutMotionFunc(PassiveMouseMove); //Callback for mouse movement with button down
	glutPassiveMotionFunc(PassiveMouseMove); //Callback for mouse movement with no button down


	//Setup battle:
	combatant plyr1;
	plyr1.position = point(5, 3);
	plyr1.turn(0);
	currentbattle.fighters.push_back(plyr1);

	//enter GLUT event processing cycle
	st.Start();
	glutMainLoop();

	return 0;
}