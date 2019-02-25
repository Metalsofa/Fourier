/*All player controls, and some debug ones, go in here.*/
#pragma once
#ifndef __controls_h
#define __controls_h


bool key_mode = false; //Whether using keyboard controls (BETA)
void exterior_console();
//Global Variables for if in overworld_mode
///I hear cactuses hurt to step on

//Global Variables for if in battle_mode
battle_preset testpreset(10, 6); //Width, Height
battlestate currentbattle(testpreset); //This holds the information about the current battle scene
const int BoardDepth = 0; //Not sure about this. Probably will never change from 0.
float timer = 0; //We'll see if this ends up being necessary
float increment = 0.01f; //Incremental time in seconds
float gamma = 1.0f; //Time dilation, from the viewer's refrence frame
int rain = 0; //This is really just for fun; good for iterating through a rainbow for no good reason

//Global Variables for BATTLEFIELD_DESIGN_MODE
int DESIGN_FUNCTION = BD_MAKE_RAYS;
metastat CHOSEEN_COLOR = cl_cyan;
Material SELECTED_MATERIAL = BtoG;



//Global Key Detections
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
const int keyBuf = 5;
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

#include "customGL.h"

using namespace std;

/*A 'keycheck' function contains code for checking AND interpreting the presently pressed keys
under a given scenario, calling the appropriate functions if neccessary.*/




//These controls will always be active
void evergreen_keychecks() {
	//Closing the application
	if (esc_down) {
		exit(0);
	}

}

//These controls are active only in debug mode
void debug_keychecks() {

	//Opening the console
	if (normal_keysdown['`']) {
		exterior_console();
		//show_console = !show_console; //relic of a time long past
		normal_keysdown['`'] = false;
	}

}

//These controls are active only in battlefield-design mode
void battlefield_design_keychecks() {
	//Quickly switch between design functions
	if (normal_keysdown['h'] && !show_console) {
		if (normal_keysdown['1'])
			DESIGN_FUNCTION = BD_CREATE_WALLS;
		if (normal_keysdown['2'])
			DESIGN_FUNCTION = BD_MAKE_RAYS;
		if (normal_keysdown['s']) {
			art_mode = !art_mode;
			battlefield_design_mode = !battlefield_design_mode;
		}
	}

	//Entering/leaving 'key_mode'
	if (normal_keysdown['0']) {
		key_mode = !key_mode;
		normal_keysdown['0'] = false;
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
		}
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
		}
		else {
			if (normal_keysdown[' ']) {
				for (combatant& b : currentbattle.fighters) {
					if (b.tog) {
						ray new_ray(colorfromID(rain++ % 12 + 1), b.position, b.position + b.direction, 2.0f,
							6.0f, 2);
						currentbattle.spawn_ray(new_ray);
					}
				}
				normal_keysdown[' '] = false;
			}
		}
		break;
	}
}

//These controls are active only in art mode
void art_keychecks() {
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

}

//These controls are active only in battle mode
void battle_keychecks() {

	//Handle key-depression buffer decrement
	if (up_buf && !normal_keysdown['i']) { up_buf--; }
	if (down_buf && !normal_keysdown['k']) { down_buf--; }
	if (left_buf && !normal_keysdown['j']) { left_buf--; }
	if (right_buf && !normal_keysdown['l']) { right_buf--; }

	//Moving players (presently only handles one player):
	//
	float dY = 0;
	float dX = 0;
	if (normal_keysdown['w']) { dY += .01f; }
	if (normal_keysdown['a']) { dX -= .01f; }
	if (normal_keysdown['s']) { dY -= .01f; }
	if (normal_keysdown['d']) { dX += .01f; }
	for (combatant& x : currentbattle.fighters) {
		if (x.tog) {
			x.position.y += dY;
			x.position.x += dX;
		}
	}
	if (normal_keysdown['i'] || normal_keysdown['j'] || normal_keysdown['k'] || normal_keysdown['l']) {
		dY = dX = 0;
		if (normal_keysdown['i'] || up_buf) { dY += .01f; }
		if (normal_keysdown['k'] || down_buf) { dY -= .01f; }
		if (normal_keysdown['l'] || right_buf) { dX += .01f; }
		if (normal_keysdown['j'] || left_buf) { dX -= .01f; }
		if (dY != 0 || dX != 0) {
			float dir = atan2f(dY, dX);
			for (combatant &x : currentbattle.fighters) { 
				if (x.tog) { x.turn(dir); }
			}
		}
	}

}

//These controls are active only in overworld mode
void overworld_keychecks() {

}



////////////////GLUT key/mouse action callback functions////////////////

//Called when a 'normal' key becomes pressed
void ProcessNormalKeys(unsigned char key, int x, int y) {
	if (key == 27)
		esc_down = true;
	//if (key == 32)
		//space_down = true;
	if (key == 13)
		enter_down = true;
	if (key == 8)
		backspace_down = true;
	if (key == 'i') {
		up_buf = keyBuf;
	} else if (key == 'j') {
		left_buf = keyBuf;
	} else if (key == 'k') {
		down_buf = keyBuf;
	} else if (key == 'l') {
		right_buf = keyBuf;
	}
	if (key >= '1' && key <= '4') {
		currentbattle.fighters[key - '1'].tog = !(currentbattle.fighters[key - '1'].tog);
	}
	normal_keysdown[key] = true;
}

//Called when a 'normal' key becomes no longer pressed
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

//Called when a 'special' key becomes pressed
void ProcessSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		up_down = true;
		break;
	case GLUT_KEY_DOWN:
		down_down = true;
		break;
	case GLUT_KEY_RIGHT:
		right_down = true;
		break;
	case GLUT_KEY_LEFT:
		left_down = true;
		break;
	}
}

//Called when a 'special' key is no longer pressed
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


#endif