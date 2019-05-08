//Include global variables
#include "globals.h"

#include "controls.h"

using namespace std;
using namespace fgr;


void evergreenKeychecks() {
	//Closing the application
	if (instDown["escape"]) {
		exit(0);
	}

}

//void evergreenKeychecks() {
//	//Closing the application
//	if (escPress) {
//		exit(0);
//	}
//
//}

//These controls are active only in debug mode

//This version is for the new map
void debugKeychecks() {

	//Opening the console
	if (instDown["showConsole"]) {
		exteriorConsole();
		//showConsole = !showConsole; //relic of a time long past
		instDown["showConsole"] = false;
	}

	//Quickly switch between design functions
	if (instDown["artModifier"] && !showConsole) {
		if (instDown["1"])
			DESIGN_FUNCTION = BD_CREATE_WALLS;
		if (instDown["2"])
			DESIGN_FUNCTION = BD_MAKE_RAYS;
		if (instDown["3"]) {
			instDown["3"] = false;
			artMode = !artMode;
			battlefieldDesignMode = !battlefieldDesignMode;
		}
	}

}

//void debugKeychecks() {
//
//	//Opening the console
//	if (normalKeysdown['`']) {
//		exteriorConsole();
//		//showConsole = !showConsole; //relic of a time long past
//		normalKeysdown['`'] = false;
//	}
//
//	//Quickly switch between design functions
//	if (normalKeysdown['h'] && !showConsole) {
//		if (normalKeysdown['1'])
//			DESIGN_FUNCTION = BD_CREATE_WALLS;
//		if (normalKeysdown['2'])
//			DESIGN_FUNCTION = BD_MAKE_RAYS;
//		if (normalKeysdown['3']) {
//			normalKeysdown['3'] = false;
//			artMode = !artMode;
//			battlefieldDesignMode = !battlefieldDesignMode;
//		}
//	}
//
//}

//These controls are active only in battlefield-design mode

//This version is for the new map
void battlefieldDesignKeychecks() {

	//Entering/leaving 'keyMode'
	if (instDown["keyModeTog"]) {
		keyMode = !keyMode;
		instDown["keyModeTog"] = false;
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
				} else {
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
				} else {
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
				} else {
					wall newWall(clickdragtrail, SELECTED_MATERIAL, true);
					drawwall(newWall);
				}
			}
		} else {
			
		}
		break;
	}
}


//void battlefieldDesignKeychecks() {
//
//	//Entering/leaving 'keyMode'
//	if (normalKeysdown['0']) {
//		keyMode = !keyMode;
//		normalKeysdown['0'] = false;
//	}
//
//	//Controls based on design function
//	switch (DESIGN_FUNCTION) {
//	case BD_CREATE_WALLS: //These are the controls for if 'making walls' is the current design function
//		if (!keyMode) {
//			if (clickdragtrail.length() != 0) {
//				if (!leftclicking) {
//					wall newWall(clickdragtrail, SELECTED_MATERIAL, true);
//					clickdragtrail = segment(0, 0, 0, 0);
//					currentbattle.constructWall(newWall);
//				} else {
//					wall newWall(clickdragtrail, SELECTED_MATERIAL, true);
//					drawwall(newWall);
//				}
//			}
//			if (rightclicktrail.length() != 0) {
//				if (!rightclicking) {
//					segment eraser = rightclicktrail;
//					rightclicktrail = segment(0, 0, 0, 0);
//					int wallID = 0;
//					for (int i = 0; i < currentbattle.map.walls.size(); i++) {
//						if (isintersect(eraser, currentbattle.map.walls[i].body))
//							currentbattle.destroyWall(i--);
//					}
//				} else {
//					wall newWall(rightclicktrail, ERASER, true);
//					drawwall(newWall);
//				}
//			}
//		}
//		break;
//	case BD_MAKE_RAYS: //Design controls for making rays
//		if (!keyMode) {
//			if (clickdragtrail.length() != 0) {
//				if (!leftclicking) {
//					ray newRay(randomhue(), clickdragtrail.p1, clickdragtrail.p2, clickdragtrail.length(),
//						6.0f, 2);
//					clickdragtrail = segment(0, 0, 0, 0);
//					currentbattle.spawnRay(newRay);
//				} else {
//					wall newWall(clickdragtrail, SELECTED_MATERIAL, true);
//					drawwall(newWall);
//				}
//			}
//		} else {
//			if (normalKeysdown[' ']) {
//				for (int i = 0; i < currentbattle.protags.size(); i++) {
//					if (currentbattle.protags[i].tog) {
//						currentbattle.playerAct(i);
//					}
//				}
//				normalKeysdown[' '] = false;
//			}
//		}
//		break;
//	}
//}

//These controls are active only in art mode
void artKeychecks() {
	if (rightclicking) {
		for (int i = 0; i < animart[editingFrame][editingLayer].size(); i++) {
			if (difference(mouse.Position, animart[editingFrame][editingLayer][i]).magnitude() < 0.05) {
				auto itr = animart[editingFrame][editingLayer].begin();
				for (int a = 0; a < i; a++) { itr++; }
				animart[editingFrame][editingLayer].erase(itr);
				i--;
			}
		}
	}
	if (normalKeysdown['H']) {
		normalKeysdown['H'] = false;
		animart.insert(animart.begin() + editingFrame++, frame());
		while (animart[editingFrame - 1].size() < animart[editingFrame].size()) {
			animart[editingFrame - 1].push_back(shape());
		}
	}
	if (normalKeysdown['L']) {
		normalKeysdown['L'] = false;
		animart.insert(animart.begin() + editingFrame + 1, frame());
		while (animart[editingFrame + 1].size() < animart[editingFrame].size()) {
			animart[editingFrame + 1].push_back(shape());
		}
	}
	if (normalKeysdown['J']) {
		normalKeysdown['J'] = false;
		for (unsigned int i = 0; i < animart.size(); i++) {
			animart[i].insert(animart[i].begin() + editingLayer + 1, shape());
		}
	}
	if (normalKeysdown['K']) {
		normalKeysdown['K'] = false;
		editingLayer++;
		for (unsigned int i = 0; i < animart.size(); i++) {
			animart[i].insert(animart[i].begin() + editingLayer, shape());
		}
	}
	if (normalKeysdown['h']) {
		normalKeysdown['h'] = false;
		if (editingFrame > 0)
			editingFrame--;
	}
	if (normalKeysdown['l']) {
		normalKeysdown['l'] = false;
		if (editingFrame < animart.size() - 1) {
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
		if (editingLayer < animart[editingFrame].size() - 1) {
			editingLayer++;
		}
	}
	if (normalKeysdown['z']) { //Hold 'z' to move a point
		for (int i = 0; i < animart[editingFrame][editingLayer].size(); i++) {
			if (difference(mouse.Position, animart[editingFrame][editingLayer][i]).magnitude() < 0.05) {
				dragdot = i;
			}
		}
		if (dragdot != -1)
			animart[editingFrame][editingLayer][dragdot] = mouse.Position;
	} else
		dragdot = -1;
	//Add a point by releasing the mouse
	if (clickdragtrail.length() != 0 || clickdragtrail.p1.x() != 0.0f || clickdragtrail.p1.y() != 0.0f) {
		if (!leftclicking && !normalKeysdown['z']) {
			animart[editingFrame][editingLayer].push_back(clickdragtrail.p2);
			clickdragtrail = segment(0, 0, 0, 0);
			drawArtGUI();
		} else {
			if (dragdot != -1)
				dragdot = -1;
			animart[editingFrame][editingLayer].push_back(clickdragtrail.p2);
			drawArtGUI();
			animart[editingFrame][editingLayer].pop_back();
		}
	} else {
		drawArtGUI();
	}

}

//These controls are active only in battle mode

//This version is for the new map
void battleKeychecks() {
	
	//Toggling the players from enabled to disabled
	if (instDown["1"]) { currentbattle.protags[0].toggle(); instDown["1"] = false; }
	if (instDown["2"]) { currentbattle.protags[1].toggle(); instDown["2"] = false; }
	if (instDown["3"]) { currentbattle.protags[2].toggle(); instDown["3"] = false; }
	if (instDown["4"]) { currentbattle.protags[3].toggle(); instDown["4"] = false; }

	//Handle key-depression buffer decrement
	if (upBuf && !instDown["aimUp"]) { upBuf--; }
	if (downBuf && !instDown["aimDown"]) { downBuf--; }
	if (leftBuf && !instDown["aimLeft"]) { leftBuf--; }
	if (rightBuf && !instDown["aimRight"]) { rightBuf--; }

	//Moving players (presently only handles one player):
	//
	point d(0.0f, 0.0f); //The differential of this player's movement
	if (instDown["moveUp"]) { d.yinc(increment); }
	if (instDown["moveLeft"]) { d.xdec(increment); }
	if (instDown["moveDown"]) { d.ydec(increment); }
	if (instDown["moveRight"]) { d.xinc(increment); }
	//Iterate through the protagonists and move them
	for (auto& x : currentbattle.protags) {
		if (x.tog) {
			x.position += d;
		}
	}
	//Iterate through the protagonists and aim them
	if (instDown["aimUp"] || instDown["aimLeft"] || instDown["aimDown"] || instDown["aimRight"]) {
		d.y(0.0f);
		d.x(0.0f);
		if (instDown["aimUp"] || upBuf) { d.yinc(increment); }
		if (instDown["aimDown"] || downBuf) { d.ydec(increment); }
		if (instDown["aimRight"] || rightBuf) { d.xinc(increment); }
		if (instDown["aimLeft"] || leftBuf) { d.xdec(increment); }
		if (d.y() != 0 || d.x() != 0) {
			for (auto& x : currentbattle.protags) {
				if (x.tog) { x.turn(d.angle()); }
			}
		}
	}
	if (instDown["action"]) {
		for (int i = 0; i < currentbattle.protags.size(); i++) {
			if (currentbattle.protags[i].tog) {
				currentbattle.playerAct(i);
			}
		}
		instDown["action"] = false;
	}
}

//void battleKeychecks() {
//
//	//Handle key-depression buffer decrement
//	if (upBuf && !normalKeysdown['i']) { upBuf--; }
//	if (downBuf && !normalKeysdown['k']) { downBuf--; }
//	if (leftBuf && !normalKeysdown['j']) { leftBuf--; }
//	if (rightBuf && !normalKeysdown['l']) { rightBuf--; }
//
//	//Moving players (presently only handles one player):
//	//
//	point d(0.0f, 0.0f); //The differential of this player's movement
//	if (normalKeysdown['w']) { d.yinc(increment); }
//	if (normalKeysdown['a']) { d.xdec(increment); }
//	if (normalKeysdown['s']) { d.ydec(increment); }
//	if (normalKeysdown['d']) { d.xinc(increment); }
//	//Iterate through the protagonists and move them
//	for (unsigned int x = 0; x < currentbattle.protags.size(); x++) {
//		if (currentbattle.protags[x].tog) {
//			currentbattle.protags[x].position += d;
//		}
//	}
//	//Iterate through the protagonists and aim them
//	if (normalKeysdown['i'] || normalKeysdown['j'] || normalKeysdown['k'] || normalKeysdown['l']) {
//		d.y(0.0f);
//		d.x(0.0f);
//		if (normalKeysdown['i'] || upBuf) { d.yinc(increment); }
//		if (normalKeysdown['k'] || downBuf) { d.ydec(increment); }
//		if (normalKeysdown['l'] || rightBuf) { d.xinc(increment); }
//		if (normalKeysdown['j'] || leftBuf) { d.xdec(increment); }
//		if (d.y() != 0 || d.x() != 0) {
//			for (unsigned int i = 0; i < 4; i++) {
//				if (currentbattle.protags[i].tog) { currentbattle.protags[i].turn(d.angle()); }
//			}
//		}
//	}
//
//}

//These controls are active only in overworld mode
void overworldKeychecks() {

}



////////////////GLUT key/mouse action callback functions////////////////

//Called when a 'normal' key becomes pressed

//This is the version for the new map
void ProcessNormalKeys(unsigned char key, int x, int y) {
	instDown[instMap[key]] = true;
	
}

//void ProcessNormalKeys(unsigned char key, int x, int y) {
//	if (key == 27)
//		escPress = true;
//	//if (key == 32)
//		//spacePress = true;
//	if (key == 13)
//		enterPress = true;
//	if (key == 8)
//		backspacePress = true;
//	if (key == 'i') {
//		upBuf = keyBuf;
//	} else if (key == 'j') {
//		leftBuf = keyBuf;
//	} else if (key == 'k') {
//		downBuf = keyBuf;
//	} else if (key == 'l') {
//		rightBuf = keyBuf;
//	}
//	if (key >= '1' && key <= '4') {
//		currentbattle.protags[key - '1'].toggle();
//	}
//	normalKeysdown[key] = true;
//}

//Called when a 'normal' key becomes no longer pressed

//This is the version for the new map
void ReleaseNormalKeys(unsigned char key, int x, int y) {
	instDown[instMap[key]] = false;
}

//void ReleaseNormalKeys(unsigned char key, int x, int y) {
//	if (key == 27)
//		escPress = false;
//	//if (key == 32)
//		//spacePress = false;
//	if (key == 13)
//		enterPress = false;
//	if (key == 8)
//		backspacePress = false;
//	normalKeysdown[key] = false;
//}

////Called when a 'special' key becomes pressed
//void ProcessSpecialKeys(int key, int x, int y) {	//TODO MOVE THESE TO THE MAP BUT THEY COINCIDE WITH KEYS
//	switch (key) {
//	case GLUT_KEY_UP:
//		upPress = true;
//		break;
//	case GLUT_KEY_DOWN:
//		downPress = true;
//		break;
//	case GLUT_KEY_RIGHT:
//		rightPress = true;
//		break;
//	case GLUT_KEY_LEFT:
//		leftPress = true;
//		break;
//	}
//}

void ProcessSpecialKeys(int key, int x, int y) {	//TODO MOVE THESE TO THE MAP BUT THEY COINCIDE WITH KEYS
	if (key <= GLUT_KEY_DOWN && key >= GLUT_KEY_LEFT) {
		instDown[instMap[key - 110]] = true;
	}
}

////Called when a 'special' key is no longer pressed
//void ReleaseSpecialKeys(int key, int x, int y) {
//	switch (key) {
//	case GLUT_KEY_UP:
//		upPress = false;
//		break;
//	case GLUT_KEY_LEFT:
//		leftPress = false;
//		break;
//	case GLUT_KEY_DOWN:
//		downPress = false;
//		break;
//	case GLUT_KEY_RIGHT:
//		rightPress = false;
//		break;
//	}
//}

void ReleaseSpecialKeys(int key, int x, int y) {	//TODO MOVE THESE TO THE MAP BUT THEY COINCIDE WITH KEYS
	if (key <= GLUT_KEY_DOWN && key >= GLUT_KEY_LEFT) {
		instDown[instMap[key - 110]] = false;
	}
}

void MouseClick(int button, int state, int x, int y) { //Note that this is good for button press OR release
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftclicking = true;
			clickdragtrail.p1 = mouse.Position;
			clickdragtrail.p2 = mouse.Position;
		} else {
			leftclicking = false;
		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			rightclicking = true;
			rightclicktrail.p1 = mouse.Position;
		} else
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
		mouse.Position.x(roundf(mouse.Position.x() / mouse.Snap) * mouse.Snap);
		mouse.Position.y(roundf(mouse.Position.y() / mouse.Snap) * mouse.Snap);
	}
	if (!(middleclicking && enablePersprot)) {
		mouse.Position.xinc(deltax * cos(PerspectiveOrbit) - deltay * sin(PerspectiveOrbit));
		mouse.Position.yinc(deltay * cos(PerspectiveOrbit) + deltax * sin(PerspectiveOrbit));
		if (mouse.Position.x() < 0)
			mouse.Position.x(0.0f);
		if (mouse.Position.x() > currentbattle.boardWidth())
			mouse.Position.x(currentbattle.boardWidth());
		if (mouse.Position.y() < 0)
			mouse.Position.y(0.0f);
		if (mouse.Position.y() > currentbattle.boardHeight())
			mouse.Position.y(currentbattle.boardHeight());
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