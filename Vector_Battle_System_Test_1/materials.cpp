//This is the implementation file for 'materials.h'

//Include global variables
#include "globals.h"

//Header file inclusions
#include "graphics.h"
#include "materials.h"

//STL inclusions
#include <vector>
#include <string>
#include <cmath>

using namespace std;

///////////////////////////////////////////////////////////
//
//           Class materialtype DEFINITION
//
//				This class structures the properties
//				of a given wall.
//
///////////////////////////////////////////////////////////

//Get the speed at which waves would travel through this material
float materialtype::wavespeed() { return sqrt(tension / dens); }

//Accessor for the red component of this wall's color as a float
float materialtype::getRed() { return (float(color.som) / 255.0f); }

//Accessor for the green component of this wall's color as a float
float materialtype::getGreen() { return(float(color.emo) / 255.0f); }

//Accessor for the blue component of this wall's color as a float
float materialtype::getBlue() { return(float(color.cog) / 255.0f); }

//Accessor for the opacity component of this wall's color as a float
float materialtype::getAlpha() { return opacity; }

//Accessor for the line-thickness of this wall as a float
float materialtype::getThickness() { return thickness; }

//Accessor for the spell-permitivity of this material as a metastat
metastat materialtype::getPermittivitySpells() const { return permittivitySpells; }

//Always use this constructor; it takes an integer corresponding to a material.
materialtype::materialtype(Material MATERIAL_ID) {
	switch (MATERIAL_ID) {
	case DEFAULT:
		hitpoints.define(0, 0, 0);
		permittivitySpells.define(0, 0, 0);
		permittivityPlayers.define(0, 0, 0);
		permittivityEnemies.define(0, 0, 0);
		harmfulnessPlayers.define(0, 0, 0);
		harmfulnessEnemies.define(0, 0, 0);
		color.define(255, 255, 255);
		thickness = 4.0f;
		opacity = 1;
		tension = 1; //Workshop this to the perfect value
		dens = 1; //Workshop this to the perfect value
		break;
	case BASIC_REFLECTIVE:
		hitpoints.define(0, 0, 0);
		permittivitySpells.define(1, 1, 1);
		permittivityPlayers.define(0, 0, 0);
		permittivityEnemies.define(0, 0, 0);
		harmfulnessPlayers.define(0, 0, 0);
		harmfulnessEnemies.define(0, 0, 0);
		color.define(255, 255, 255);
		thickness = 2.0f;
		opacity = 1;
		tension = 1; //Workshop this to the perfect value
		dens = 1;
		break;
	case ERASER:
		hitpoints.define(0, 0, 0);
		permittivitySpells.define(1, 1, 1);
		permittivityPlayers.define(0, 0, 0);
		permittivityEnemies.define(0, 0, 0);
		harmfulnessPlayers.define(0, 0, 0);
		harmfulnessEnemies.define(0, 0, 0);
		color.define(130, 100, 100);
		thickness = 1.5f;
		opacity = 1;
		tension = 1; //Workshop this to the perfect value
		dens = 1;
		break;
	case BtoG:
		hitpoints.define(0, 0, 0);
		permittivitySpells.define(1, 2, 2);
		permittivityPlayers.define(0, 0, 0);
		permittivityEnemies.define(0, 0, 0);
		harmfulnessPlayers.define(0, 0, 0);
		harmfulnessEnemies.define(0, 0, 0);
		color.define(100, 200, 200);
		thickness = 2.0f;
		opacity = 1;
		tension = 1; //Workshop this to the perfect value
		dens = 1;
		break;

	}
}

//Default constructor for just in case.
materialtype::materialtype() {
	hitpoints.define(0, 0, 0);
	permittivitySpells.define(0, 0, 0);
	permittivityPlayers.define(0, 0, 0);
	permittivityEnemies.define(0, 0, 0);
	harmfulnessPlayers.define(0, 0, 0);
	harmfulnessEnemies.define(0, 0, 0);
	color.define(255, 255, 255);
	thickness = 2.0f;
	opacity = 1;
	tension = 1; //Workshop this to the perfect value
	dens = 1; //Workshop this to the perfect value
}
