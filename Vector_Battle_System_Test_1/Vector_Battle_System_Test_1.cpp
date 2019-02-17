/*Main .cpp file for this RPG. Mainly handles graphics and iterative callbacks.
The main function is rather short; the function 'renderscene' is the real 'main'
function.*/
//Test commit comment line

#include "gl/glut.h"
#include "Stopwatch.h" 
#include "players.h"
#include "battle.h"
#include "camera.h"

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

bool key_mode = false; //Whether using keyboard controls (BETA)

//Global Variables for if in overworld_mode


//Global Variables for if in battle_mode
battle_preset testpreset(10, 6); //Width, Height
battlestate currentbattle(testpreset); //This holds the information about the current battle scene
const int BoardDepth = 0; //Not sure about this. Probably will never change from 0.
float timer = 0; //We'll see if this ends up being necessary
float increment = 0.01f; //Incremental time in seconds
float gamma = 1.0f; //Time dilation, from the viewer's refrence frame
int rain = 0;


//Global Variables for BATTLEFIELD_DESIGN_MODE
int DESIGN_FUNCTION = BD_MAKE_RAYS;
metastat CHOSEEN_COLOR = cl_cyan;
Material SELECTED_MATERIAL = BtoG;

//Global Variables for the console
bool show_console = false;
//vector<string> console_history;
string user_input = "";
int console_scroll = 0;

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


//Global Camera Position Data
float LookPointX = float(currentbattle.BoardWidth()) / 2.0f;
float LookPointY = float(currentbattle.BoardHeight()) / 2.0f;
float LookPointZ = BoardDepth / 2.0f;
const float PerspectiveRiseMax = PI / 2.0f - 0.0005f;
float PerspectiveOrbit = 0.0f;
float PerspectiveRise = PerspectiveRiseMax;
float PerspectiveDist = 10.0f; //7.5f;

//Global Custom mouse. Properties and Position
cursor mouse = cursor();

//Global stopwatch
Stopwatch st;

//Be careful; certain headers should only be included after global declaration
#include "customGL.h"
#include "console.h"
#include "controls.h"


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

//Called every time the screen refreshes; also just so happens to handle all iterative things.
void renderScene(void) { 

	//Make sure we aren't dropping frames unneccesarily
	clocksync();

	// Clear Color and Depth Buffers
	ClearScreen();

	// Reset transformations
	glLoadIdentity();

	// Set the camera to look at the appropriate spot (usually center of the board)
	definecamera();

	//Axis drawing code
	drawaxes();
	
	//Debug: Show timer
	if (show_timer) {
		rendertext(point(0.0f, 0.0f), to_string(timer));
	}

	//Draw Battlefield Geometry
		//Draw Walls
	int walliterator = 0;
	while (walliterator < currentbattle.map.wallcount()) { 
		drawwall(currentbattle.map.getwalls()[walliterator]);
		walliterator++;
	}
	
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

	//Draw Combatants
	for (int i = 0; i < currentbattle.fighters.size(); i++) {
		drawfighter(currentbattle.fighters[i]);
	}

	//Draw Spells

	//Draw Rays
	int i = 0;
	while (i < currentbattle.raycount()) {
		drawray(currentbattle.rays[i]);
		i++;
	}

	//////////----Control Handling----//////////

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

	//Draw mouse
	if (battlefield_design_mode && !key_mode) {
		drawcursor(mouse);
		mouse.Rot += (mouse.RotSpeed / mouse.Spread) * increment;
	}

	//Draw console //Not sure we really do that anymore
	if (show_console)
		draw_console();

	//This is the function that refreshes the canvas and implements everything we've 'drawn'
	glutSwapBuffers(); 

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
	srand(time(NULL));

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
	glutIdleFunc(renderScene); //Callback for when the programs is idle (this will happen continuousCameraLookingY)
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