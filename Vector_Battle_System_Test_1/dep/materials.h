//Define the constants for material names, and define each of those materials
#pragma once
#ifndef __materials_h__
#define __materials_h__

#include "graphics.h"

#include <cmath>


//Enumerate material presets
enum Material { DEFAULT, BASIC_REFLECTIVE, ERASER, BtoG };

class materialtype {
public:
	metastat hitpoints; //Values of 0 indicate the contstruct is indestructible with respect to that dimension DP: Might want to make that -1, as 0 hitpoints is entirely possible(broken)
	metastat permittivitySpells; //for permittivity, 0 = STOP, 1 = REFLECT, 2 = PASS
	metastat permittivityPlayers; //If the player has an impermissible component, it cannot pass
	metastat permittivityEnemies; //Same as above
	metastat harmfulnessPlayers; //How much it hurts players who touch it. 0 = Harmless
	metastat harmfulnessEnemies; //Like above
	metastat color; //Color this material is rendered in.
	float thickness;
	float opacity;
	float tension;
	float dens; //linear mass density

///public:
	float wavespeed();
	//Accessor for the red component of this wall's color as a float
	float getRed();
	//Accessor for the green component of this wall's color as a float
	float getGreen();
	//Accessor for the blue component of this wall's color as a float
	float getBlue();
	//Accessor for the opacity component of this wall's color as a float
	float getAlpha();
	//Accessor for the line-thickness of this wall as a float
	float getThickness();
	//Accessor for the spell-permitivity of this material as a metastat
	metastat getPermittivitySpells();
	//Always use this constructor; it takes an integer corresponding to a material.
	materialtype(Material MATERIAL_ID);
	//Default constructor (for just in case)
	materialtype();
};

#endif