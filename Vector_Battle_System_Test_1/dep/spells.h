/*This header file contians the object typs that keep track of spell info, whether in battle through
physics or outside of battle.*/
#pragma once
#ifndef __spells_h__
#define __spells_h__
#include "geometry.h"
#include "waves.h"
#include "players.h"

//move: Spell Object that can be part of a player's known spells (called a function in game)
	///string name
	///int level
	///int category (which of  he 12 categories it falls into)
	///string description
	///int

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
	//Constructor for some custom ray
	ray(metastat col, point location, point heading, float leng, float fastness, float thickn) { //DP: Pass by ref
		bits.push_back(location);
		bits.push_back(location);
		//DP: bits = {location, location};
		direction = unitvector(difference(heading, location));
		speed = fastness;
		nominalLength = leng;
		thickness = thickn;
		color = col;
	}

	//Constructor that accepts a spell and returns the appropriate ray //maybee
	///Doesn't exist yet

	//Returns the vector representing the ray's velocity
	point movevector() {
		if (terminating)
			return scalarproduct(unitvector(difference(terminalpoint, bits[0])), speed);
		else
			return scalarproduct(unitvector(direction), speed);
		//DP: return scalarproduct(unitvector((
		//	(terminating)
		//		?difference(terminalpoint, bits[0])
		//		:direction)), 
		//	speed);
	}

	//Returns the actual length of the vector, calculated by counting individual segment lengths
	float length() {
		float cumulative = 0;
		int i = 0;
		while (i < int(bits.size()) - 1) {
			point p1 = bits[i];
			point p2 = bits[i + 1];
			segment meas(p1, p2);
			cumulative += meas.length();
			i += 1;
		}
		//DP:
		//for (int i = 0; i < int(bits.size()) - 1; i++) {
		//	cumulative += segment(bits[i], bits[i + 1]);
		//}
		return cumulative;
	}
	//Returns the length that this vector is supposed to have according to its definition
	float lengthNominal() { return nominalLength; }
	//Advance the motion of the ray in accordance to its properties
	void advance(float inc /*Incremental Time*/) { 
		if (!terminating) {
			point frontarb = combine(bits[0], scalarproduct(movevector(), inc));
			point unitback = unitvector(difference(bits[bits.size() - 2], bits.back()));
			bits[0] = frontarb;
			if (length() >= nominalLength)
				bits.back() = combine(bits.back(), scalarproduct(unitback, inc * speed));
		}
		else {
			point frontarb = combine(bits[0], scalarproduct(movevector(), inc));
			if (difference(terminalpoint, frontarb).magnitude() > difference(terminalpoint, bits[0]).magnitude())
				frontarb = terminalpoint;
			point diff = difference(bits[bits.size() - 2], bits.back());
			point unitback = unitvector(diff);
			point moveback = scalarproduct(unitback, inc * speed);
			if (speed * inc >= difference(terminalpoint, bits.back()).magnitude())
				bits.back() = terminalpoint;
			else
				bits.back() = combine(bits.back(), moveback);
			bits[0] = frontarb;
			nominalLength = length();
			if (converges(bits[0], bits.back()) && converges(bits[0], terminalpoint))
				killme = true;
		}
		
		//Terminate the last point if it catches up
		float forgiveness = 0.000005f;
		if (difference(bits.back(), bits[bits.size() - 2]).magnitude() < speed * inc && bits.size() > 2) {
			bits.pop_back();
		}
	}
	bool gotRed() {return !color.som == 0;} //Bool: Is there a RED component to this ray?
	bool gotGreen() { return !color.emo == 0; } //Bool: Is there a GREEN component to this ray?
	bool gotBlue() { return !color.cog == 0; } //Bool: Is there a BLUE component to this ray?
	bool deathtime() { return killme; }
	//Tell the ray it is now terminating, and tell it where to terminate.
	void terminate(point where) { //DP: Pass by ref
		terminating = true;
		terminalpoint = where;
	}
	int checkcollision(combatant& c) { 
		float dist0 = (bits[0] - c.position).magnitude();
		float dist1 = (bits[1] - c.position).magnitude();
		if (dist0 < c.width / 2) { return 1; } //.25 NEEDS TO BE CHANGED IF PLAYER SIZE CHANGES
		if (dist1 < c.width / 2) { return 2; }
		return 0;
	}
	bool checkcollision(segment surface) { //DP Pass by ref, no need to create var
		segment frontseg(bits[0], bits[1]);
		return isintersect(frontseg, surface);
	}
	point wherehit(segment surface) { //DP Pass by ref, no need to create var
		segment frontseg(bits[0], bits[1]);
		return intersection(frontseg, surface);
	}
	//Returns 0 (kill) 1 (bounce) or 2 (permit) based on this ray's compatability with a given material.
	int permitted(metastat permittivity) { //DP: Pass by ref
		//These 3 lines of code return 0 (for 'kill') if any component of the ray is not permitted.
		if (permittivity.som == 0 && gotRed()) return 0;
		if (permittivity.emo == 0 && gotGreen()) return 0;
		if (permittivity.cog == 0 && gotBlue()) return 0;
		//These 3 lines of cour return 1 (for 'bounce') if any component of the ray is to be bounced.
		if (permittivity.som == 1 && gotRed()) return 1;
		if (permittivity.emo == 1 && gotGreen()) return 1;
		if (permittivity.cog == 1 && gotBlue()) return 1;
		//If no return has happened by now, the ray is permitted by this permittivity.
		return 2;
		//DP:
		//if ((permittivity.som == 0 && gotRed()) ||
		//	(permittivity.emo == 0 && gotGreen()) ||
		//	(permittivity.cog == 0 && gotBlue())) return 0;
		//else if ((permittivity.som == 1 && gotRed()) ||
		//	(permittivity.emo == 1 && gotGreen()) ||
		//	(permittivity.cog == 1 && gotBlue())) return 1;
		//return 2;
	}

	//Doublechecks that there is a collision, then causes the ray to bounce off of the given surface
	void bounce(segment surface) { //DP: Pass by ref
		bool shouldbounce = true;
		segment frontseg(bits[0], bits[1]);
		point ints = intersection(frontseg, surface);
		point intdiff = difference(bits[1], ints);
		float intdist = intdiff.magnitude();
		float errr = 0.00f;
		if (bits.size() == 2) //DP: Merge into line above: float errr = (bits.size() == 2)? 0.01f : 0f;
			errr = 0.01f;
		//Debug:
		if (intdist < errr) {
			bool TAILINT_FLAG = true; //DP: ? Whats the purpose of this, because u created it within a scope, u can't use it anywhere else.
		}
		if (isintersect(frontseg, surface) && intdist > errr) {
			bits.insert((bits.begin() + 1), intersection(frontseg, surface));
			bits[0] = reflection(bits[0], surface);
			segment surface2;
			surface2.p1 = difference(surface.p1, surface.midpoint());
			surface2.p2 = difference(surface.p2, surface.midpoint());
			direction = reflection(direction, surface2);
		}
	}
	//Returns the thickness that glut should be using to render this ray.
	float getthickness() { return thickness; }
	float getRed() {return float(color.som) / 255.0f;} //Returns float from 0 to 1, for glut's sake
	float getGreen() {return float(color.emo) / 255.0f;} //Returns float from 0 to 1, for glut's sake
	float getBlue() { return float(color.cog) / 255.0f;} //Returns float from 0 to 1, for glut's sake
	//Returns a read-only copy of the vector containing every 'bit' (point) of this ray
	vector<point> getbits() {
		return bits;
	}
};

//projectiles: have graphics, position, velocity, behavior,

//portal:
	//position
	//radius

//raysinks:

//animation: is a series of graphics and 

//tethers:
	//to whom they are linked
	//

//field:
	//function that returns direction and magnitude, given x and y

#endif