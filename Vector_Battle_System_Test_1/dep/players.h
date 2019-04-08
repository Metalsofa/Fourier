/* This class, player, is not the one used in battle. It keeps track of metastats outside of battle.
It is possible that a joint purpose will be be served by it, but for now, this is an outside-of-battle
thing, to be called by the function that initiates battle */
//Looks like this file will just be a general thing for my game.
#ifndef __players_h__
#define __players_h__

//Header includes
#include "crypt.h"
#include "graphics.h"
#include "battle.h"

//STL includes
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <time.h>

using namespace std;
using namespace fgr;
class battlestate; //Forward declaring battlestate
class ray; //Forward declare ray

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
		for (shape& sh : sprite) { sh.lineThickness = (tog? 1.0f : 2.0f); }
		tog = !tog;
	}

	void makeWall(int mat, battlestate & b);
	void shoot(const metastat & col, battlestate & b);
};

class enemy : public combatant {	//Non controlled combatants with AI
	//Typedef 'behavior' as a function pointer to a void that takes a battlestate-
	typedef  void (enemy::*behavior)(battlestate&);
private:
	vector<point> path;	//Contains points on a path for enemy to follow
	int ind;	//Current index of path that enemy is at is coming from
	bool dir; //True if moving from index 0 to n of path, false if moving backwards

	bool moving;	//Whether the enemy is moving
	point dest;		//Where the enemy is going to
	point aim;		//Where the enemy is aiming
	behavior moveB;	//Function pointer that tells the enemy how to move
	behavior shootB;//Function pointer that tells the enemy how to shoot

public:


	enemy(int m = -1, int s = -1) : combatant() {
		init(m, s);
	}
	void init(int m, int s);

	void addWaypoint(const point& p, int i = -1);

	void act(battlestate& b);

	void mB1(battlestate& b);
	void mB1b(battlestate & b);
	void sBRand8(battlestate & b);
	void sB1(battlestate& b);
	void sB4(battlestate & b);
	void mB2(battlestate& b);
	void mB3(battlestate& b);
	void mB4(battlestate& b);

	point recursiveReflectiveAim(battlestate & b, int wallInd, int playerInd, int depth, point pos, const metastat & shotColor);

	void move(const point& dire);

	void aimAt(const point& dire);

	void shoot(battlestate& b);

	void shoot(const metastat & col, const point & dire, battlestate & b);

};

#endif