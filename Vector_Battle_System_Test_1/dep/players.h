/* This class, player, is not the one used in battle. It keeps track of metastats outside of battle.
It is possible that a joint purpose will be be served by it, but for now, this is an outside-of-battle
thing, to be called by the function that initiates battle */
//Looks like this file will just be a general thing for my game.
#ifndef __players_h__
#define __players_h__

//Header includes
#include "crypt.h"
#include "graphics.h"
//#include "battle.h"
#include "spells.h"
#include "spellCore.h"

//STL includes
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <time.h>

using namespace std;
using namespace fgr;

//class battlestate; //Forward declaring battlestate
//class ray; //Forward declare ray
//class wall;
//
class Spell;
class raySpell;
class wallSpell;

//Enumerate the stats that a character has
enum statNum {
	stNULL,
	stMaxHP,
	stMaxFP,
	stHP,
	stFP,
	stVitality,
	stSensitivity,
	stAgility,
	stPower,
	stResilience,
	stStability,
	stCount
};

//Interprets a string as an enumerated stat
statNum stringToStatNum(const std::string& word);

//This version of stats is for overworld logic and battle initiation
class statblock {
public:
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
	// Universal accessor for just in case
	metastat& getStat(statNum which);

	bool readline(std::istream& source);
	graphic sprite; //DP: I think we only need one sprite in the combatant class

	//Default constructor
	statblock();

	//Construct this statblock from a file
	statblock (const std::string& filename);

	
};

//all players AND enemies are one of these
class combatant {
public:
	combatant();
	combatant(const std::string& statfile);

	statblock stats;
	point position;
	point direction; //Direction as a unit vector
	//Sprites
	graphic sprite;
	float width;


	//Each segment represents a hitbox; p1 is lower left, p2 is upper right.
	vector<segment> hitboxes;

	//Changes direction based on angle(in radians)
	void turn(float angle);

	//When a combatant is hit by a spell
	void hit(metastat m, int level);

};

//controlled players
class player: public combatant { 
public:
	// REPRESENTATION
	vector<Spell> arsenal;
	float energy;
	float energyCap;
	//Whether or not the player can currently be controlled
	bool tog; 

	// CONSTRUCTORS
	//Default constructor
	player();
	//Construct from a text file
	player(const std::string& filename);
	//MEGA constructor that has most stuff
	player(const std::string& filename, const fgr::point& pos, const graphic& sprit, 
		float ang = 0, bool toggle = false,  float wid = 0.4f, float energ = 100, float energCap = 100);

	// MEMBER FUNCTIONS
	//Flips tog
	void toggle();
	const Spell* act();
	wall makeWall(int mat) const;
	ray shoot(const metastat & col) const;
};

class enemy : public combatant {	//Non controlled combatants with AI
	//Typedef 'behavior' as a function pointer to a void that takes a battlestate-
	//typedef  void (enemy::*behavior)(battlestate&);
public:

	// REPRESENTATION
	vector<point> path;	//Contains points on a path for enemy to follow
	int ind;	//Current index of path that enemy is at is coming from
	bool dir; //True if moving from index 0 to n of path, false if moving backwards
	point aim;		//Where the enemy is aiming

	//behavior moveB;	//Function pointer that tells the enemy how to move
	//behavior shootB;//Function pointer that tells the enemy how to shoot

	bool moving;	//Whether the enemy is moving
	point dest;		//Where the enemy is going to
	int moveB;
	int shootB;

	
	// CONSTRUCTORS
	//Default constructor
	enemy();
	//Constructor from statblock file
	enemy(const std::string& statfile);

	// MEMBER FUNCTIONS
	enemy(int m = -1, int s = -1) : combatant() {
		srand(unsigned int(time(NULL)));
		ind = 0;
		dir = true;
		moving = false;
		moveB = m;
		shootB = s;
	}

	void addWaypoint(const point& p, int i = -1);


	void move(const point& dire);
	void move(int index);
	void move();

	void aimAt(const point& dire);

	ray shoot();

	ray shoot(const metastat & col, const point & dire);

};

int checkcollision(const ray& r, const combatant& c);

#endif