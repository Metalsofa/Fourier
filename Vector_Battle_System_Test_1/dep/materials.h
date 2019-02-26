//Define the constants for material names, and define each of those materials
#pragma once
#ifndef __materials_h
#define __materials_h

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
	float wavespeed() { return sqrt(tension / dens); }
	float getRed() { return (float(color.som) / 255.0f); }
	float getGreen() { return(float(color.emo) / 255.0f); }
	float getBlue() { return(float(color.cog) / 255.0f); }
	float getAlpha() { return opacity; }
	float getThickness() { return thickness; }
	metastat getPermittivitySpells() { return permittivitySpells; }
	//Always use this constructor; it takes an integer corresponding to a material.
	materialtype(Material MATERIAL_ID);
	materialtype() {
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
};

//#define MATERIALS_DEFAULT 1
//#define MATERIALS_BASIC_REFLECTIVE 2
//#define MATERIALS_ERASER 3
//DP: I like the lightbulbs idea of making these contexpr, or just make these constants, or enums. Preprocessor stuff scares me.

materialtype::materialtype(Material MATERIAL_ID) { //DP: Might want to pull out the things that all 3 have in common so it is more clear what separates these things ex: permittivitySpells, color, thickness, etc.
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

#endif