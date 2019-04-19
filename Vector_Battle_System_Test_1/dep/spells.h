/*This header file contians the object typs that keep track of spell info, whether in battle through
physics or outside of battle.*/
#pragma once
#ifndef __spells_h__
#define __spells_h__
//#include "geometry.h"
#include "spellCore.h"

using namespace std;
using namespace fgr;

//move: Spell Object that can be part of a player's known spells (called a function in game)
	///string name
	///int level
	///int category (which of  he 12 categories it falls into)
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


class battlestate;//Forward declaring battlestate

class Spell {
public:
	string name;
	int level;
	string decription;
	int category;
	Spell() {

	}
	/*void cast(battlestate& b) {
	}*/
};

class raySpell : public Spell {
public:
	ray r;
	raySpell() : Spell() {
		r = ray(metastat(), point(0, 0), point(1, 0), 1.0f, 1.0f, 1.0f);
		category = 0;
	}
	raySpell(ray& ra) :Spell() {
		r = ra;
		category = 0;
	}
	/*void cast(battlestate& b) {
		b.spawnRay(r);
	}*/
};

class wallSpell : public Spell {
public:
	wall w;
	wallSpell() :Spell() {
		category = 1;
	}
	wallSpell(wall& wa) :Spell() {
		category = 1;
		w = wa;
	}
	/*void cast(battlestate& b) {
		b.constructWall(w);
	}*/

};

#endif