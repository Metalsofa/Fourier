#pragma once
#include "players.h"
#include "time.h"

using namespace std;
class Enemy {
	combatant data;
	int behavior;
	Enemy(combatant& d, int b):data(d), behavior(b) {
		srand(unsigned int(time(NULL)));
	}

	void act() {
		switch (behavior) {
		case 1:
			return behave1();
		case 2:
			return behave2();
		case 3:
			return behave3();
		case 4:
			return behave4();
		}
		cerr << "ERROR: INVALID BEHAVIOR" << endl;
	}

	void behave1() { 

	}
	void behave2() {

	}
	void behave3() {

	}
	void behave4() {

	}

	void move(point& dir, int mag) {
	
	}

	void aim(point& dir) {

	}

	void shoot(point& dir) {

	}
};