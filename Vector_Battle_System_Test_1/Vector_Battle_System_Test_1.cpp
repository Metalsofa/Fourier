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

using namespace std;
using win32::Stopwatch;

//Global Constants
///const float PI = 3.14159265358978f;

//Global Variables for detecting mode
bool overworld_mode = false;
bool battle_mode = true;
bool design_mode = true;

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

//Global Variables for BD_MAKE_SHAPES
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

//Global Key Detections
const char keys[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
'W', 'X', 'Y', 'Z', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_',
'+', '=', '{', '}', '[', ']', '\\', '|', ';', ':', '"', '\'', ',', '<', '.', '>', '?', '\/', '`', '~' };
map<char, bool> normal_keysdown;

///Special and other keys
bool up_down = false; //DP: Instead of _down, _press might be less confusing
bool left_down = false;
bool down_down = false;
bool right_down = false;
int up_buf;
int down_buf;
int left_buf;
int right_buf;
const int keyBuf = 2;
bool esc_down = false;
bool enter_down = false;
bool space_down = false;
bool backspace_down = false;
///For Numerals in Particular

//Global MouseButton Detections
bool leftclicking = false;
bool rightclicking = false;
bool middleclicking = false;
segment clickdragtrail(0.0f, 0.0f, 0.0f, 0.0f);
segment rightclicktrail(0.0f, 0.0f, 0.0f, 0.0f);

Stopwatch st;

//Be careful; certain headers should only be included after global declaration
#include "customGL.h"
#include "console.h"

void changeSize(int width, int height) {
	//To avoid divide by zero:
	if (height == 0)
		height = 1;

	float ratio = 1.0f * width / height;

	//Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	//Reset Matrix
	glLoadIdentity();

	//Set viewport to be the entire window
	glViewport(0, 0, width, height);

	//Set the correct perspective
	gluPerspective(45, ratio, 1, 100);

	//Get back to the modelview
	glMatrixMode(GL_MODELVIEW);

}

void feedkeyboardinput(string &field) {
	for (char c : keys) { //Not sure how to eliminate this for loop
		if (normal_keysdown[c]) {
			normal_keysdown[c] = false;
			field += c;
		}
	}
	if (backspace_down) {
		backspace_down = false;
		if (field.size() > 0)
			field.pop_back();
	}
}


void renderScene(void) { //The custom function that tells openGL what to do when it refreshes
	//Sleep(int((increment * 1000.0f * gamma)));
	while (st.ElapsedMilliseconds() < double(increment * 1000.0f * gamma)) {}
	if (st.ElapsedMilliseconds() > 12.0f)
		cout << "@frame " << int(timer) << " - TIME: " << st.ElapsedMilliseconds() << endl;
	timer += increment * 100;
	st.Stop();
	st.Start();
	
	if (up_buf && !up_down) { up_buf--; }
	if (down_buf && !down_down) { down_buf--; }
	if (left_buf && !left_down) { left_buf--; }
	if (right_buf && !right_down) { right_buf--; }

	// Clear Color and Depth Buffers
	ClearScreen();

	// Reset transformations
	glLoadIdentity();

	// Set the camera to look at the appropriate spot (usually center of the board)
	definecamera();

	//Axis drawing code
	drawaxes();

	//Moving players:
	if (up_down || down_down || right_down || left_down) {
		float dY = 0;
		float dX = 0;
		if (up_down || (up_buf && !normal_keysdown['q'])) { dY += .01; }
		if (down_down || (down_buf && !normal_keysdown['q'])) { dY -= .01; }
		if (right_down || (right_buf && !normal_keysdown['q'])) { dX += .01; }
		if (left_down || (left_buf && !normal_keysdown['q'])) { dX -= .01; }
		if (normal_keysdown['q']) {
			currentbattle.fighters[0].position.y += dY;
			currentbattle.fighters[0].position.x += dX;
		}
		else if (dY != 0 || dX != 0) {
			float dir = atan2f(dY, dX);
			currentbattle.fighters[0].turn(dir);
		}
	}
	


	//Debug: Show timer
	if (show_timer) {
		rendertext(point(0.0f, 0.0f), to_string(timer));
	}

	//Draw Battlefield Geometry
		//Draw Walls
	int walliterator = 0;
	while (walliterator < currentbattle.map.wallcount()) { //DP: Do you have to redraw everything every render? Is it possible to only do it when a wall is added or deleted?, applies for other things rendered here
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
	if (esc_down) {
		exit(0);
	}
	if (normal_keysdown['`']) {//The availability of the console won't always be there, but it'll stay until a user-friendly menu is available
		//show_console = !show_console;
		string inp;
		while (true) {
			if (inp == "q" || inp == "b" || inp == "back") {
				break;
			}
			getline(cin, inp);
			handle_input(inp);
		}
		normal_keysdown['`'] = false;
	}
	//Controls for BATTLEFIELD_DESIGN_MODE
	if (design_mode) {
		//Quickly switch between design functions
		if (normal_keysdown['h'] && !show_console) {
			if (normal_keysdown['1'])
				DESIGN_FUNCTION = BD_CREATE_WALLS;
			if (normal_keysdown['2'])
				DESIGN_FUNCTION = BD_MAKE_RAYS;
		}
		if (normal_keysdown['k']) {
			key_mode = !key_mode;
			normal_keysdown['k'] = false;
		}
		//Controls based on design function
		switch (DESIGN_FUNCTION) {
		case BD_CREATE_WALLS: //These are the controls for if 'making walls' is the current design function
			if (!key_mode) {
				if (clickdragtrail.length() != 0) {
					if (!leftclicking) {
						wall new_wall(clickdragtrail, SELECTED_MATERIAL, true);
						clickdragtrail = segment(0, 0, 0, 0);
						currentbattle.construct_wall(new_wall);
					}
					else {
						wall new_wall(clickdragtrail, SELECTED_MATERIAL, true);
						drawwall(new_wall);
					}
				}
				if (rightclicktrail.length() != 0) {
					if (!rightclicking) {
						segment eraser = rightclicktrail;
						rightclicktrail = segment(0, 0, 0, 0);
						int wallID = 0;
						for (int i = 0; i < currentbattle.map.walls.size(); i++) {
							if (isintersect(eraser, currentbattle.map.walls[i].body))
								currentbattle.destroy_wall(i--);
						}
					}
					else {
						wall new_wall(rightclicktrail, ERASER, true);
						drawwall(new_wall);
					}
				}
			} else {

			}
			
			break;
		case BD_ERASE_WALLS: //No controls implemented for erasing walls; these controls are in 'make walls'
			break;
		case BD_MAKE_RAYS: //Design controls for making rays
			if (!key_mode) {
				if (clickdragtrail.length() != 0) {
					if (!leftclicking) {
						ray new_ray(randomhue(), clickdragtrail.p1, clickdragtrail.p2, clickdragtrail.length(),
							6.0f, 2);
						clickdragtrail = segment(0, 0, 0, 0);
						currentbattle.spawn_ray(new_ray);
					}
					else {
						wall new_wall(clickdragtrail, SELECTED_MATERIAL, true);
						drawwall(new_wall);
					}
				}
			} else {
				if (normal_keysdown[' ']) {
					ray new_ray(colorfromID(rain++ % 12 + 1), currentbattle.fighters[0].position, currentbattle.fighters[0].position + currentbattle.fighters[0].direction, 2.0f,
						6.0f, 2);
					currentbattle.spawn_ray(new_ray);
					normal_keysdown[' '] = false;
				}
			}
			
			break;
		case BD_MAKE_SHAPES: //Design controls for making graphics. Deceptively named!
			if (rightclicking) {
				for (int i = 0; i < art.pieces[Gindex].vertices.size(); i++) {
					if (difference(mouse.Position, art.pieces[Gindex].vertices[i]).magnitude() < 0.05) {
						art.pieces[Gindex].vertices.erase(art.pieces[Gindex].vertices.begin() + i);
						i--;
					}
				}
			}
			if (normal_keysdown['z']) { //Hold 'z' to move a point
				for (int i = 0; i < art.pieces[Gindex].vertices.size(); i++) {
					if (difference(mouse.Position, art.pieces[Gindex].vertices[i]).magnitude() < 0.05) {
						dragdot = i;
					}
				}
				if (dragdot != -1)
					art.pieces[Gindex].vertices[dragdot] = mouse.Position;
			}
			else
				dragdot = -1;
			//Add a point by releasing the mouse
			if (clickdragtrail.length() != 0 || clickdragtrail.p1.x != 0.0f || clickdragtrail.p1.y != 0.0f) {
				if (!leftclicking && !normal_keysdown['z']) {
					art.pieces[Gindex].vertices.emplace_back(clickdragtrail.p2);
					clickdragtrail = segment(0, 0, 0, 0);
					draw_art_GUI();
				}
				else {
					if (dragdot != -1)
						dragdot = -1;
					art.pieces[Gindex].vertices.emplace_back(clickdragtrail.p2);
					draw_art_GUI();
					art.pieces[Gindex].vertices.pop_back();
				}
			}
			else {
				draw_art_GUI();
			}
			break;

		}
	}

	//Draw mouse
	if (design_mode && !key_mode) {
		drawcursor(mouse);
		mouse.Rot += (mouse.RotSpeed / mouse.Spread) * increment;
	}

	//Draw console
	if (show_console)
		draw_console();

	glutSwapBuffers(); //This is the function that refreshes the canvas and implements everythiing we've drawn

	//Process Iterative Behaviour 
	//if (down_down) //DP: COMMENTED THESE OUT BECAUSE I NEED THE ARROW KEYS
	//	PerspectiveRise -= 0.1f;
	//if (up_down)
	//	PerspectiveRise += 0.1f;
	currentbattle.iterate(increment);
}

void ProcessNormalKeys(unsigned char key, int x, int y) {
	if (key == 27)
		esc_down = true;
	//if (key == 32)
		//space_down = true;
	if (key == 13)
		enter_down = true;
	if (key == 8)
		backspace_down = true;
	normal_keysdown[key] = true;
}

void ReleaseNormalKeys(unsigned char key, int x, int y) {
	if (key == 27)
		esc_down = false;
	//if (key == 32)
		//space_down = false;
	if (key == 13)
		enter_down = false;
	if (key == 8)
		backspace_down = false;
	normal_keysdown[key] = false;
}

void ReleaseSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		up_down = false;
		break;
	case GLUT_KEY_LEFT:
		left_down = false;
		break;
	case GLUT_KEY_DOWN:
		down_down = false;
		break;
	case GLUT_KEY_RIGHT:
		right_down = false;
		break;
	}
}

void ProcessSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		up_buf = keyBuf;
		up_down = true;
		break;
	case GLUT_KEY_DOWN:
		down_buf = keyBuf;
		down_down = true;
		break;
	case GLUT_KEY_RIGHT:
		right_buf = keyBuf;
		right_down = true;
		break;
	case GLUT_KEY_LEFT:
		left_buf = keyBuf;
		left_down = true;
		break;
	}
}

void MouseClick(int button, int state, int x, int y) { //Note that this is good for button press OR release
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftclicking = true;
			clickdragtrail.p1 = mouse.Position;
			clickdragtrail.p2 = mouse.Position;
		}
		else
		{
			leftclicking = false;
		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			rightclicking = true;
			rightclicktrail.p1 = mouse.Position;
		}
		else
			rightclicking = false;
	}
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN)
			middleclicking = true;
		else
			middleclicking = false;
	}
	if (button == 3 && state == GLUT_DOWN) { //4 means scrolling down
		/*
		if (show_console) {
			console_scroll++;
			if (console_scroll > console_history.size() - 1)
				console_scroll = console_history.size() - 1;
		}
		*/
	}
	if (button == 4 && state == GLUT_DOWN) { //3 means scrolling up
		/*
		if (show_console) {
			console_scroll--;
			if (console_scroll < 0)
				console_scroll = 0;
		}
		*/
	}
}

void ActiveMouseMove(int x, int y) { //DP: something I didn't put for the draw fighter function, even though it is void, when we optimize, it will break if we don't return (I think)

}

void PassiveMouseMove(int x, int y) {
	int mousebase = 300;
	mouse.Snap = 0;
	if (normal_keysdown['1'])
		mouse.Snap = 1.0f;
	else if (normal_keysdown['2'])
		mouse.Snap = 1.0f / 2.0f;
	else if (normal_keysdown['3'])
		mouse.Snap = 1.0f / 4.0f;
	else if (normal_keysdown['4'])
		mouse.Snap = 1.0f / 8.0f;
	float deltax = (x - mousebase) * mouse.Sensitivity / 30;
	float deltay = (mousebase - y) * mouse.Sensitivity / 30;
	if (mouse.Snap != 0) {
		deltax *= 2;
		deltay *= 2;
	}
	if (mouse.Snap != 0) {
		mouse.Position.x = roundf(mouse.Position.x / mouse.Snap) * mouse.Snap;
		mouse.Position.y = roundf(mouse.Position.y / mouse.Snap) * mouse.Snap;
	}
	if (!(middleclicking && enable_persprot)) {
		mouse.Position.x += deltax * cos(PerspectiveOrbit) - deltay * sin(PerspectiveOrbit);
		mouse.Position.y += deltay * cos(PerspectiveOrbit) + deltax * sin(PerspectiveOrbit);
		if (mouse.Position.x < 0)
			mouse.Position.x = 0;
		if (mouse.Position.x > currentbattle.BoardWidth())
			mouse.Position.x = currentbattle.BoardWidth();
		if (mouse.Position.y < 0)
			mouse.Position.y = 0;
		if (mouse.Position.y > currentbattle.BoardHeight())
			mouse.Position.y = currentbattle.BoardHeight();
	}
	if (middleclicking && enable_persprot) {
		PerspectiveOrbit -= deltax;
		PerspectiveRise -= deltay;
	}
	if (leftclicking) {
		clickdragtrail.p2 = mouse.Position;
	}
	if (rightclicking) {
		rightclicktrail.p2 = mouse.Position;
	}
	glutWarpPointer(mousebase, mousebase);
}

//main function
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
			design_mode = true;
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