/*All player controls, and some debug ones, go in here.*/
#pragma once
#ifndef __controls_h
#define __controls_h


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
void evergreenKeychecks() {
	//Closing the application
	if (escPress) {
		exit(0);
	}

}

//These controls are active only in debug mode
void debugKeychecks() {

	//Opening the console
	if (normalKeysdown['`']) {
		exteriorConsole();
		//showConsole = !showConsole; //relic of a time long past
		normalKeysdown['`'] = false;
	}

	//Quickly switch between design functions
	if (normalKeysdown['h'] && !showConsole) {
		if (normalKeysdown['1'])
			DESIGN_FUNCTION = BD_CREATE_WALLS;
		if (normalKeysdown['2'])
			DESIGN_FUNCTION = BD_MAKE_RAYS;
		if (normalKeysdown['3']) {
			normalKeysdown['3'] = false;
			artMode = !artMode;
			battlefieldDesignMode = !battlefieldDesignMode;
		}
	}

}

//These controls are active only in battlefield-design mode
void battlefieldDesignKeychecks() {

	//Entering/leaving 'keyMode'
	if (normalKeysdown['0']) {
		keyMode = !keyMode;
		normalKeysdown['0'] = false;
	}

	//Controls based on design function
	switch (DESIGN_FUNCTION) {
	case BD_CREATE_WALLS: //These are the controls for if 'making walls' is the current design function
		if (!keyMode) {
			if (clickdragtrail.length() != 0) {
				if (!leftclicking) {
					wall newWall(clickdragtrail, SELECTED_MATERIAL, true);
					clickdragtrail = segment(0, 0, 0, 0);
					currentbattle.constructWall(newWall);
				}
				else {
					wall newWall(clickdragtrail, SELECTED_MATERIAL, true);
					drawwall(newWall);
				}
			}
			if (rightclicktrail.length() != 0) {
				if (!rightclicking) {
					segment eraser = rightclicktrail;
					rightclicktrail = segment(0, 0, 0, 0);
					int wallID = 0;
					for (int i = 0; i < currentbattle.map.walls.size(); i++) {
						if (isintersect(eraser, currentbattle.map.walls[i].body))
							currentbattle.destroyWall(i--);
					}
				}
				else {
					wall newWall(rightclicktrail, ERASER, true);
					drawwall(newWall);
				}
			}
		}
		break;
	case BD_MAKE_RAYS: //Design controls for making rays
		if (!keyMode) {
			if (clickdragtrail.length() != 0) {
				if (!leftclicking) {
					ray newRay(randomhue(), clickdragtrail.p1, clickdragtrail.p2, clickdragtrail.length(),
						6.0f, 2);
					clickdragtrail = segment(0, 0, 0, 0);
					currentbattle.spawnRay(newRay);
				}
				else {
					wall newWall(clickdragtrail, SELECTED_MATERIAL, true);
					drawwall(newWall);
				}
			}
		}
		else {
			if (normalKeysdown[' ']) {
				for (player& b : currentbattle.protags) {
					if (b.tog) {
						ray newRay(colorfromID(rain++ % 12 + 1), (b.position + b.direction*.3f), b.position + b.direction + b.direction, 2.0f,
							6.0f, 2);
						currentbattle.spawnRay(newRay);
					}
				}
				normalKeysdown[' '] = false;
			}
		}
		break;
	}
}

//These controls are active only in art mode
void artKeychecks() {
	if (rightclicking) {
		for (int i = 0; i < animart[editingFrame].pieces[editingLayer].vertices.size(); i++) {
			if (difference(mouse.Position, animart[editingFrame].pieces[editingLayer].vertices[i]).magnitude() < 0.05) {
				animart[editingFrame].pieces[editingLayer].vertices.erase(animart[editingFrame].pieces[editingLayer].vertices.begin() + i);
				i--;
			}
		}
	}
	if (normalKeysdown['H']) {
		normalKeysdown['H'] = false;
		animart.frames.insert(animart.frames.begin() + editingFrame++, graphic());
		while (animart[editingFrame - 1].pieces.size() < animart[editingFrame].pieces.size()) {
			animart[editingFrame - 1].pieces.push_back(shape());
		}
	}
	if (normalKeysdown['L']) {
		normalKeysdown['L'] = false;
		animart.frames.insert(animart.frames.begin() + editingFrame + 1, graphic());
		while (animart[editingFrame + 1].pieces.size() < animart[editingFrame].pieces.size()) {
			animart[editingFrame + 1].pieces.push_back(shape());
		}
	}
	if (normalKeysdown['J']) {
		normalKeysdown['J'] = false;
		for (unsigned int i = 0; i < animart.frames.size(); i++) {
			animart[i].pieces.insert(animart[i].pieces.begin() + editingLayer + 1, shape());
		}
	}
	if (normalKeysdown['K']) {
		normalKeysdown['K'] = false;
		editingLayer++;
		for (unsigned int i = 0; i < animart.frames.size(); i++) {
			animart[i].pieces.insert(animart[i].pieces.begin() + editingLayer, shape());
		}
	}
	if (normalKeysdown['h']) {
		normalKeysdown['h'] = false;
		if (editingFrame > 0)
			editingFrame--;
	}
	if (normalKeysdown['l']) {
		normalKeysdown['l'] = false;
		if (editingFrame < animart.frames.size() - 1) {
			editingFrame++;
		}
	}
	if (normalKeysdown['k']) {
		normalKeysdown['k'] = false;
		if (editingLayer > 0)
			editingLayer--;
	}
	if (normalKeysdown['j']) {
		normalKeysdown['j'] = false;
		if (editingLayer < animart[editingFrame].pieces.size() - 1) {
			editingLayer++;
		}
	}
	if (normalKeysdown['z']) { //Hold 'z' to move a point
		for (int i = 0; i < animart[editingFrame].pieces[editingLayer].vertices.size(); i++) {
			if (difference(mouse.Position, animart[editingFrame].pieces[editingLayer].vertices[i]).magnitude() < 0.05) {
				dragdot = i;
			}
		}
		if (dragdot != -1)
			animart[editingFrame].pieces[editingLayer].vertices[dragdot] = mouse.Position;
	}
	else
		dragdot = -1;
	//Add a point by releasing the mouse
	if (clickdragtrail.length() != 0 || clickdragtrail.p1.x != 0.0f || clickdragtrail.p1.y != 0.0f) {
		if (!leftclicking && !normalKeysdown['z']) {
			animart[editingFrame].pieces[editingLayer].vertices.push_back(clickdragtrail.p2);
			clickdragtrail = segment(0, 0, 0, 0);
			drawArtGUI();
		}
		else {
			if (dragdot != -1)
				dragdot = -1;
			animart[editingFrame].pieces[editingLayer].vertices.push_back(clickdragtrail.p2);
			drawArtGUI();
			animart[editingFrame].pieces[editingLayer].vertices.pop_back();
		}
	}
	else {
		drawArtGUI();
	}

}

//These controls are active only in battle mode
void battleKeychecks() {

	//Handle key-depression buffer decrement
	if (upBuf && !normalKeysdown['i']) { upBuf--; }
	if (downBuf && !normalKeysdown['k']) { downBuf--; }
	if (leftBuf && !normalKeysdown['j']) { leftBuf--; }
	if (rightBuf && !normalKeysdown['l']) { rightBuf--; }

	//Moving players (presently only handles one player):
	//
	point d(0.0f, 0.0f); //The differential of this player's movement
	if (normalKeysdown['w']) { d.y += increment; }
	if (normalKeysdown['a']) { d.x -= increment; }
	if (normalKeysdown['s']) { d.y -= increment; }
	if (normalKeysdown['d']) { d.x += increment; }
	for (player& x : currentbattle.protags) {
		if (x.tog) {
			x.position.y += d.y;
			x.position.x += d.x;
		}
	}
	if (normalKeysdown['i'] || normalKeysdown['j'] || normalKeysdown['k'] || normalKeysdown['l']) {
		d.y = d.x = 0;
		if (normalKeysdown['i'] || upBuf) { d.y += increment; }
		if (normalKeysdown['k'] || downBuf) { d.y -= increment; }
		if (normalKeysdown['l'] || rightBuf) { d.x += increment; }
		if (normalKeysdown['j'] || leftBuf) { d.x -= increment; }
		if (d.y != 0 || d.x != 0) {
			for (unsigned int i = 0; i < 4; i++) {
				if (currentbattle.protags[i].tog) { currentbattle.protags[i].turn(d.angle()); }
			}
		}
	}

}

//These controls are active only in overworld mode
void overworldKeychecks() {

}



////////////////GLUT key/mouse action callback functions////////////////

//Called when a 'normal' key becomes pressed
void ProcessNormalKeys(unsigned char key, int x, int y) {
	if (key == 27)
		escPress = true;
	//if (key == 32)
		//spacePress = true;
	if (key == 13)
		enterPress = true;
	if (key == 8)
		backspacePress = true;
	if (key == 'i') {
		upBuf = keyBuf;
	} else if (key == 'j') {
		leftBuf = keyBuf;
	} else if (key == 'k') {
		downBuf = keyBuf;
	} else if (key == 'l') {
		rightBuf = keyBuf;
	}
	if (key >= '1' && key <= '4') {
		currentbattle.protags[key - '1'].toggle();
	}
	normalKeysdown[key] = true;
}

//Called when a 'normal' key becomes no longer pressed
void ReleaseNormalKeys(unsigned char key, int x, int y) {
	if (key == 27)
		escPress = false;
	//if (key == 32)
		//spacePress = false;
	if (key == 13)
		enterPress = false;
	if (key == 8)
		backspacePress = false;
	normalKeysdown[key] = false;
}

//Called when a 'special' key becomes pressed
void ProcessSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		upPress = true;
		break;
	case GLUT_KEY_DOWN:
		downPress = true;
		break;
	case GLUT_KEY_RIGHT:
		rightPress = true;
		break;
	case GLUT_KEY_LEFT:
		leftPress = true;
		break;
	}
}

//Called when a 'special' key is no longer pressed
void ReleaseSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		upPress = false;
		break;
	case GLUT_KEY_LEFT:
		leftPress = false;
		break;
	case GLUT_KEY_DOWN:
		downPress = false;
		break;
	case GLUT_KEY_RIGHT:
		rightPress = false;
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
		if (showConsole) {
			consoleScroll++;
			if (consoleScroll > console_history.size() - 1)
				consoleScroll = console_history.size() - 1;
		}
		*/
	}
	if (button == 4 && state == GLUT_DOWN) { //3 means scrolling up
		/*
		if (showConsole) {
			consoleScroll--;
			if (consoleScroll < 0)
				consoleScroll = 0;
		}
		*/
	}
}

void ActiveMouseMove(int x, int y) { 
	return;
}

void PassiveMouseMove(int x, int y) {
	int mousebase = 300;
	mouse.Snap = 0;
	if (normalKeysdown['1'])
		mouse.Snap = 1.0f;
	else if (normalKeysdown['2'])
		mouse.Snap = 1.0f / 2.0f;
	else if (normalKeysdown['3'])
		mouse.Snap = 1.0f / 4.0f;
	else if (normalKeysdown['4'])
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
	if (!(middleclicking && enablePersprot)) {
		mouse.Position.x += deltax * cos(PerspectiveOrbit) - deltay * sin(PerspectiveOrbit);
		mouse.Position.y += deltay * cos(PerspectiveOrbit) + deltax * sin(PerspectiveOrbit);
		if (mouse.Position.x < 0)
			mouse.Position.x = 0;
		if (mouse.Position.x > currentbattle.boardWidth())
			mouse.Position.x = currentbattle.boardWidth();
		if (mouse.Position.y < 0)
			mouse.Position.y = 0;
		if (mouse.Position.y > currentbattle.boardHeight())
			mouse.Position.y = currentbattle.boardHeight();
	}
	if (middleclicking && enablePersprot) {
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


#endif