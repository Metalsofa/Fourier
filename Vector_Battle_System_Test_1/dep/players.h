/* This class, player, is not the one used in battle. It keeps track of metastats outside of battle.
It is possible that a joint purpose will be be served by it, but for now, this is an outside-of-battle
thing, to be called by the function that initiates battle */
//Looks like this file will just be a general thing for my game.
#ifndef _Players_h
#define _Players_h
#include <vector>
#include <fstream>
#include "crypt.h"
#include <sstream>
#include <map>
#include <list> 

#include "geometry.h"
#include "art.h"

using namespace std;


//This version of stats is for overworld logic and battle initiation
class stats {
private:
	//Player metastats cap out at 255
	metastat maxHP; //maximum HP
	metastat maxFP; //maximum FP
	metastat HP; //present HP
	metastat FP; //present FP
	metastat vitality; //like constitution
	metastat sensitivity; //Inverse of DEF
	metastat agility; //DEX
	metastat power; //ATK
	metastat resilience; //Benefit from healing
	metastat stability; //Reduces rate of life loss

	//Abilities also need to somehow appear
public:
	graphic sprite; //DP: I think we only need one sprite in the combatant class
	int Hitpoints(int comp)					{ return HP.component(comp); }
	int MaxHitpoints(int comp)				{ return maxHP.component(comp); }
	int HitpointsFraction(int comp)			{ return Hitpoints(comp) / MaxHitpoints(comp); }
	int Functionpoints(int comp)			{ return FP.component(comp); }
	int MaxFunctionpoints(int comp)			{ return maxFP.component(comp); }
	int FunctionpointsFraction(int comp)	{ return Functionpoints(comp) / MaxFunctionpoints(comp); }
	int Vitality(int comp)					{ return vitality.component(comp); }
	int Sensitivity(int comp)				{ return sensitivity.component(comp); }
	int Agility(int comp)					{ return agility.component(comp); }
	int Power(int comp)						{ return power.component(comp); }
	int Resilience(int comp)				{ return resilience.component(comp); }
	int Stability(int comp)					{ return stability.component(comp); }
};

//all players AND enemies are one of these
class combatant {
public:
	combatant() {

	}
	stats statblock;
	point position;
	point direction; //Direction as a unit vector
	//Sprites
	graphic sprite;
	float width;


	//Each segment represents a hitbox; p1 is lower left, p2 is upper right.
	vector<segment> hitboxes;

	void turn(float angle) { //Changes direction based on angle(in radians)
		direction = unitfromangle(angle);
	}
};

class player: public combatant {
public:
	bool tog;
	void toggle() {
		for (shape& sh : sprite.pieces) { sh.lineThickness = (tog? 1 : 2); }
		tog = !tog;
	}
};

class Enemy : public combatant {
public:
	list<point> path;
	list<point>::iterator itr;
	bool dir; //True if moving from index 0 to n of path, false if moving backwards

	bool moving;
	point dest;

	int behavior;
	Enemy(int b) : combatant(), behavior(b) {
		srand(unsigned int(time(NULL)));
		itr = path.begin();
		dir = true;
		moving = false;
	}

	void addWaypoint(const point& p) { //Adds to path vector at end
		return addWaypoint(p, -1);
		itr = path.begin();
	}

	void addWaypoint(const point& p, int ind) { //Adds to path vector at index
		if (ind == -1) { return path.push_back(p); }
		list<point>::iterator iter;
		if (ind < path.size() / 2) {
			iter = path.begin();
			for (int i = 0; i < ind; i++) { iter++; }
		} else {
			iter = path.end();
			for (int i = 0; i < (path.size() - ind); i++) { iter--; }
		}
		path.insert(iter, p);
	}

	void act() {	//Decides which AI to implement
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
		else if (path.size() == 1) { 
			if ((path.front() - position).magnitude() > .5) {
				move(path.front());
			}
			return; 
		}

		if (itr == path.begin()) {
			itr++;
			dir = true;
			move(*itr);
		} /*else if(itr != path.end()){
			if (++itr == path.end()) {
				itr--;
				itr--;
				dir = false;
				move(*itr);
			}
		}*/
		else {
			if (dir) { itr++; } else { itr--; }
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


//enum Color {clWhite = 0xFFFFFF, clBlack = 0x0, clRed = 0xFF0000, clOrange = 0xFF7F00, clYellow = 0xFFFF00,
//	clLime = 0x7FFF00, clGreen = 0x00FF, clTeal = 0x00FF7F, clCyan = 0x00FFFF, clIndigo = 0x007FFF, clBlue = 0x0000FF,
//	clPurple = 0x7F00FF, clMagenta = 0xFF00FF, clViolet = 0xFF007F
//};

#define clWhite metastat(255, 255, 255) /*White*/
#define clBlack metastat(0, 0, 0) /*Black*/
#define clRed metastat(255,0,0) /*Red*/
#define domainRed "action"
#define clOrange metastat(255,127,0) /*Orange*/
#define domainOrange "artistry"
#define clYellow metastat(255,255,0) /*Yellow*/
#define domainYellow "expression"
#define clLime metastat(127,255,0) /*Lime*/
#define domainLime "evocation"
#define clGreen metastat(0,255,0) /*Green*/
#define domainGreen "emotion"
#define clTeal metastat(0,255,127) /*Teal*/
#define domainTeal "introspection"
#define clCyan metastat(0,255,255) /*Cyan*/
#define domainCyan "belief"
#define clIndigo metastat(0,127,255) /*Indigo*/
#define domainIndigo "heuristic"
#define clBlue metastat(0,0,255) /*Blue*/
#define domainBlue "cognition"
#define clPurple metastat(127,0,255) /*Purple*/
#define domainPurple "synthesis"
#define clMagenta metastat(255,0,255) /*Magenta*/
#define domainMagenta "application"
#define clViolet metastat(255,0,127) /*Violet*/
#define domainViolet "operation"

//DP: Use constexpr over macro: https://stackoverflow.com/questions/42388077/constexpr-vs-macros

#endif