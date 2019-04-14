/*This header file contians the object typs that keep track of spell info, whether in battle through
physics or outside of battle.*/
#pragma once
#ifndef __spells_h__
#define __spells_h__
//#include "geometry.h"
//#include "battle.h"
#include "spellCore.h"

using namespace std;
using namespace fgr;


//move: Spell Object that can be part of a player's known spells (called a function in game)
	///string name
	///int level
	///int category (which of the 12 categories it falls into)
	///string description
	///int
//projectiles: have graphics, position, velocity, behavior,

//portal:
	//position
	//radius

//raysinks:

//tethers:
	//to whom they are linked
	//

//field:
	//function that returns direction and magnitude, given x and y

//class battlestate;
enum SpellType {sNULL, sRay, sWall};


class Spell {
public:
	string name;
	int level;
	string description;
	int category;
	int cost;
	Spell();
	Spell(const Spell& s);
	SpellType type;
	
	ray* r;
	wall* w;

	Spell(const ray& ra);
	Spell(const wall& wa);
	~Spell();
};


#endif