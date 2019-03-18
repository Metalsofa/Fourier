/* This class, player, is not the one used in battle. It keeps track of metastats outside of battle.
It is possible that a joint purpose will be be served by it, but for now, this is an outside-of-battle
thing, to be called by the function that initiates battle */
//Looks like this file will just be a general thing for my game.
#ifndef __players_h__
#define __players_h__

//Header includes
#include "crypt.h"
#include "art.h"
#include "graphics.h"

//STL includes
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <list> 
#include <time.h>

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

class player: public combatant { //controlled players
public:
	bool tog; //Whether or not the player can currently be controlled
	void toggle() { //Flips tog
		for (shape& sh : sprite.pieces) { sh.lineThickness = (tog? 1.0f : 2.0f); }
		tog = !tog;
	}
	
};

class enemy : public combatant {	//Non controlled combatants with AI
	//typedef  void (enemy::*behavior)(battlestate&);
private:
	vector<point> path;	//Contains points on a path for enemy to follow
	int ind;	//Current index of path that enemy is at is coming from
	bool dir; //True if moving from index 0 to n of path, false if moving backwards

	bool moving;	//Whether the enemy is moving
	point dest;		//Where the enemy is going to
	point aim;		//Where the enemy is aiming
	//behavior moveB;	//Function pointer that tells the enemy how to move
	//behavior shootB;//Function pointer that tells the enemy how to shoot

public:


	enemy(int m = -1, int s = -1) : combatant() {
		init(m, s);
	}
	void init(int m, int s) {
		srand(unsigned int(time(NULL)));
		ind = 0;
		dir = true;
		moving = false;
		//switch (m) {		//Picking a move behavior
		//case 1:
		//	moveB = &enemy::mB1;
		//	break;
		//case 2:
		//	moveB = &enemy::mB2;
		//	break;
		//case 3:
		//	moveB = &enemy::mB3;
		//	break;
		//case 4:
		//	moveB = &enemy::mB4;
		//	break;
		//default:
		//	moveB = nullptr;
		//}
		//switch (s) {	//Picking a shoot behavior
		//case 1:
		//	shootB = &enemy::sB1;
		//	break;
		//case 2:
		//	shootB = &enemy::sB1;
		//	break;
		//case 3:
		//	shootB = &enemy::sB1;
		//	break;
		//case 4:
		//	shootB = &enemy::sB1;
		//	break;
		//default:
		//	shootB = nullptr;
		//}
	}

	void addWaypoint(const point& p, int i = 0) { //Adds to path vector at index
		if (i == -1) { ind = 0;  return path.push_back(p); }
		path.insert(path.begin() + i, p);
	}

	//void act(battlestate& b) {	//invokes moving and shooting functions appropriately
	//	if (moving) {
	//		point dire = (dest - position);
	//		if (dire.magnitude() < .05) {	//.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
	//			moving = false;
	//		} else {
	//			position += unitvector(dire)*.02f;	//.02 is a speed multiplier
	//		}
	//		return;
	//	} else if (moveB) {
	//		invoke(moveB, *this, b);		//USING THIS BECAUSE BEHAVIOR IS SCOPED TO MEMBER FUNCTION
	//		return;
	//	}
	//	cerr << "ERROR: INVALID BEHAVIOR" << endl;
	//	return;
	//}

	//void mB1(battlestate& b) {	//Just follows the path 
	//	if (path.size() == 0) { return; } 
	//	else if (path.size() == 1) { 
	//		if ((path.front() - position).magnitude() > .05) { //.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
	//			move(path.front());
	//		}
	//		return; 
	//	}
	//	if (ind == 0) {
	//		ind++;
	//		dir = true;
	//		move(path[ind]);
	//	} 
	//	else if(ind == (path.size()-1)){
	//		ind--;
	//		dir = false;
	//		move(path[ind]);
	//	} 
	//	else{
	//		ind += (dir ? 1 : -1);
	//		move(path[ind]);
	//	}
	//	return;
	//}
	//void sB1(battlestate& b) {	//Just shoots if there are no walls in the way of enemy and player
	//	bool shot = true;
	//	for (player& p : b.protags) {
	//		shot = true;
	//		for (wall& w : b.map.getWalls()) {				//Currently commented out due to not being able to recognize currentbattle, ray, etc.
	//			segment s(p.position, position);
	//			if (isintersect(w.body, s)) {
	//				shot = false;
	//				break;
	//			}
	//		}
	//		if (shot) {
	//			shoot(p.position,b);
	//			return;
	//		}
	//	}
	//	return;
	//}
	//void mB2(battlestate& b) {

	//}
	//void mB3(battlestate& b) {

	//}
	//void mB4(battlestate& b) {

	//}

	void move(const point& dire) { //Sets the enemy to move to this point
		dest = dire;
		moving = true;
	}

	void aimAt(const point& dire) {	//Sets the enemy to aim at this point
		aim = dire;
	}

	//void shoot(battlestate& b) { //Shoots where aiming
	//	shoot(aim, b);
	//}

	//void shoot(const point& dire, battlestate& b) { //Shoots at a point
		//ray newRay(colorfromID(rain++ % 12 + 1), (dire-position)*.3f, dire, 2.0f,		//Currently commented out due to not being able to recognize currentbattle, ray, etc.
		//	6.0f, 2);
		//b.spawnRay(newRay);
	//}
};

#endif