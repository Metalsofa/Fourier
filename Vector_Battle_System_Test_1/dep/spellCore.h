/*This header file contians the object typs that keep track of spell info, whether in battle through
physics or outside of battle.*/
#pragma once
#ifndef __spellCore_h__
#define __spellCore_h__
//#include "geometry.h"
#include "waves.h"
#include "materials.h"
//#include "players.h"

using namespace std;
using namespace fgr;
//move: Spell Object that can be part of a player's known spells (called a function in game)
	///string name
	///int level
	///int category (which of  he 12 categories it falls into)
	///string description
	///int
class wallConst {
public:
	materialtype material;
	bool fixed; //wave or no wave
	int shape = 0; //Circular, linear
	wallConst();
	wallConst(const materialtype& m, bool f, int s);
	wallConst(int wallmaterial, bool isfixed, int s);
	wallConst(const wallConst& w);
};

class wall {
public:
	segment body;
	materialtype material;
	bool fixed; //wave or no wave
	int shape = 0; //Circular, linear
///public:
	const segment& getbody() const { return body; }
	const materialtype& getmaterial() const { return material; }
	bool getfixed() { return fixed; }
	wall(const segment& definingsegment, const Material& wallmaterial, bool isfixed);
	wall(segment definingsegment, int wallmaterial, bool isfixed);
	wall(); 
	wall(const wall& w);
	wall(const wallConst& w, const segment& s);
};

class rayConst {
public:
	metastat color; //color
	float nominalLength; //The actuall length may change, but this is what it's supposed to be.
	float speed; //In battlefield units per second
	float thickness;
	rayConst(const metastat& c, float len, float spd, float thck);
	rayConst(const rayConst& r);
};

class ray {//all arrow-shaped travelling projectiles
public:
	vector<point> bits; /*Every piece of the ray and its tail. Entry [0] is the head, the last one is the
						tail.any other point along the way is a ricochet point and does not move,
						is deleted when the tail catches up to it.*/
	vector<int> disjoints; /*For each integer in this list, the line segment between the corresponding
						   point and the next one will not be rendered. eg, if the ray passes through a
						   portal */
	metastat color; //color
	int kindness; //Let's just do a scale of -10 to 10, from Cruel to Kind
	point direction; //This "point" is really functioning as a unit vector in two dimensions.
	float speed; //In battlefield units per second
	float nominalLength; //The actuall length may change, but this is what it's supposed to be.
	bool killme = false;
	bool terminating = false;
	float thickness;
	point terminalpoint;
	///public:

	ray();
	ray(const ray& r);
	//Constructor for some custom ray
	ray(const metastat& col, const point& location, const point& heading, float leng, float fastness, float thickn);
	ray(const point& loc, const point& head, const rayConst& r);

	//Constructor that accepts a spell and returns the appropriate ray //maybee
	///Doesn't exist yet
	//Returns the vector representing the ray's velocity
	point movevector() const;
	//Returns the actual length of the vector, calculated by counting individual segment lengths
	float length() const;
	//Returns the length that this vector is supposed to have according to its definition
	float lengthNominal() const;
	//Advance the motion of the ray in accordance to its properties
	void advance(float inc /*Incremental Time*/);
	bool gotRed() const;
	bool gotGreen() const;
	bool gotBlue() const;
	bool deathtime() const;
	//Tell the ray it is now terminating, and tell it where to terminate.
	void terminate(const point& where);
	//int checkcollision(const combatant& c) const;
	bool checkcollision(const segment& surface) const;
	point wherehit(const segment& surface) const;
	//Returns 0 (kill) 1 (bounce) or 2 (permit) based on this ray's compatability with a given material.
	int permitted(const metastat& permittivity) const;

	//Doublechecks that there is a collision, then causes the ray to bounce off of the given surface
	void bounce(const segment& surface);
	//Returns the thickness that glut should be using to render this ray.
	float getthickness() const;
	float getRed() const; //Returns float from 0 to 1, for glut's sake
	float getGreen() const; //Returns float from 0 to 1, for glut's sake
	float getBlue() const; //Returns float from 0 to 1, for glut's sake
	//Returns a read-only copy of the vector containing every 'bit' (point) of this ray
	const vector<point>& getbits() const;
};

//Check if a spell of a given color can pass through a given material
int permitted(const metastat& spellColor, const metastat& permittivity);

//projectiles: have graphics, position, velocity, behavior,

class portalConst {
public:
	materialtype material;
	portalConst(Material mat) : material(mat) {}
	portalConst(int mat) { material = (Material)mat; }
	portalConst(const portalConst& pC) : material(pC.material) {}
};

class portal {
public:
	segment body;
	materialtype material;
	portal* pair;

	const segment& getbody() const { return body; }
	const materialtype& getmaterial() const { return material; }
	portal(const segment& definingsegment, Material mat, portal* partner);
	portal(const segment& definingsegment, int wallmaterial, portal* partner);
	portal();
	portal(const portal& p);
	portal(const portalConst& pc, const segment& s, portal* partner);
};
//portal:
	//position
	//radius

//raysinks:

//tethers:
	//to whom they are linked
	//

//field:
	//function that returns direction and magnitude, given x and y

#endif