#pragma once
#include "players.h"
#include <list> 
#include "time.h"

using namespace std;
class Enemy {
	combatant data;

	list<point> path;
	list<point>::iterator itr;
	bool dir; //True if moving from index 0 to n of path, false if moving backwards

	bool moving;
	point dest;

	int behavior;
	Enemy(combatant& d, int b):data(d), behavior(b) {
		srand(unsigned int(time(NULL)));
		itr = path.begin();
		dir = true;
		moving = false;
	}

	void addWaypoint(point& p, int ind) { //Adds to path vector at index

	}

	void act() {
		if (moving) { return; }
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

	void behave1() {	//Just follows the path
		if (!path.size()) { return; }
		else if (path.size() == 1) { move(path.front()); return; }

		if (*itr == path.back()) {
			itr --; 
			itr--;
			dir = false;
			move(*itr);
		} else if (itr == path.begin()) {
			itr++;
			dir = true;
			move(*itr);
		} else {
			if (dir) { itr++; }
			else { itr--; } 
			move(*itr);
		}
		return;
	}
	void behave2() {

	}
	void behave3() {

	}
	void behave4() {

	}

	void move(point& dir) {
		dest = dir;
		moving = true;
	}

	void aim(point& dir) {

	}

	void shoot(point& dir) {

	}
};