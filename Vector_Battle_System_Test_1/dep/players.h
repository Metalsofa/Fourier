/* This class, player, is not the one used in battle. It keeps track of metastats outside of battle.
It is possible that a joint purpose will be be served by it, but for now, this is an outside-of-battle
thing, to be called by the function that initiates battle */
//Looks like this file will just be a general thing for my game.
#ifndef __players_h
#define __players_h
#include <vector>
#include <fstream>
#include "crypt.h"
#include <sstream>
#include <map>

#include "geometry.h"
#include "art.h"

using namespace std;





//This version of player is for overworld logic and battle initiation
class player {
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
	graphic sprite;
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

	bool tog;

	player statblock;
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
//enum Color {cl_white = 0xFFFFFF, cl_black = 0x0, cl_red = 0xFF0000, cl_orange = 0xFF7F00, cl_yellow = 0xFFFF00,
//	cl_lime = 0x7FFF00, cl_green = 0x00FF, cl_teal = 0x00FF7F, cl_cyan = 0x00FFFF, cl_indigo = 0x007FFF, cl_blue = 0x0000FF,
//	cl_purple = 0x7F00FF, cl_magenta = 0xFF00FF, cl_violet = 0xFF007F
//};

#define cl_white metastat(255, 255, 255) /*White*/
#define cl_black metastat(0, 0, 0) /*Black*/
#define cl_red metastat(255,0,0) /*Red*/
#define domain_red "action"
#define cl_orange metastat(255,127,0) /*Orange*/
#define domain_orange "artistry"
#define cl_yellow metastat(255,255,0) /*Yellow*/
#define domain_yellow "expression"
#define cl_lime metastat(127,255,0) /*Lime*/
#define domain_lime "evocation"
#define cl_green metastat(0,255,0) /*Green*/
#define domain_green "emotion"
#define cl_teal metastat(0,255,127) /*Teal*/
#define domain_teal "introspection"
#define cl_cyan metastat(0,255,255) /*Cyan*/
#define domain_cyan "belief"
#define cl_indigo metastat(0,127,255) /*Indigo*/
#define domain_indigo "heuristic"
#define cl_blue metastat(0,0,255) /*Blue*/
#define domain_blue "cognition"
#define cl_purple metastat(127,0,255) /*Purple*/
#define domain_purple "synthesis"
#define cl_magenta metastat(255,0,255) /*Magenta*/
#define domain_magenta "application"
#define cl_violet metastat(255,0,127) /*Violet*/
#define domain_violet "operation"

//DP: Use constexpr over macro: https://stackoverflow.com/questions/42388077/constexpr-vs-macros

#endif