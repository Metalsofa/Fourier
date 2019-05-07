/* This header file defines the 'battle' datatype, as well as a the battlefield datatype to support it.
Also the datatypes for stage geometry*/
#pragma once
#ifndef __battle_h__
#define __battle_h__

//Header includes
#include "materials.h"
#include "players.h"
#include "spellCore.h"
//#include "geometry.h"
#include "fgrutils.h"
//#include "crypt.h"

//STL includes
#include <vector>
#include <string>

using namespace std;
using namespace fgr;



//A battlefield is one of the members of battlePreset and battlestate; it contains the "terrain" of the battle, if you will.
class battlefield { 
public:
	float width;
	float height;
	vector<wall> walls;
	//Other vectors containg other sets of battlefield objects
	//sinks
	//portals
	//background animations
///public:
	//Get a vector of all intersection points of this wall
	vector<point> intersections() const;
	//returns the point (-100, -100) if there are no intersections.
	point nearestintersection(const point& dot) const;
	//Set the width and height of this battlefield
	void setSize(float wide, float high);
	//Accessor for the width of this battlefield
	float getWidth();
	//Accessor for the height of this battlefield
	float getHeight();
	//Get the number of walls
	size_t wallCount();
	//Access a refrence to a particular wall
	wall& getWall(int wallID);
	//Access the entire vector of walls by refrence
	vector<wall>& getWalls();
	//Add a wall to the vector
	void addWall(wall& newWall);
	//Pop a wall from the vector
	void popWall(int wallID);
};


/*pre-defined battles, ready to be called on when battle is initiated. These can be read from files
by the game when it plays, or written to files by me when I make battles in my battle design interface*/
class battlePreset {
public:
	battlefield map;
	//Enemies and their positions
	//Background
///public:
	///battlefield getmap() {
	///	return map;
	///}
	//Default constructor
	battlePreset();
	//Height-and-width constructor
	battlePreset(float width, float height);
	//Construct directly from a file
	battlePreset(std::string filename);
	//Write this battle preset to a file
	void writeToFile(std::string filename);
	//Read in a battlepreset from a file
	void readFromFile(std::string filename);
};


/*This is the dynamic battlestate where all the present "gobal" variables live. 
The best way to construct this is to feed it a preset.*/
class battlestate { 
public:
	//-----INTERNAL REPRESENTATION-----
	//The mutable battlefield that combat takes place on
	battlefield map;
	//Vector of all travelling rays
	vector<ray> rays;
	//Other spells
	//vector of sinks/sources
	vector<player> protags;
	vector<enemy> antags;
	//vector of portals
	///PROTOTYPE
	//-----METHODS-----
	//Returns a read-only copy of the map in use.
	///battlefield getmap() {
	///	return map;
	///}
	//Returns a FLOAT of the battlefield's width, just for convineince.
	float boardWidth();
	//Returns a FLOAT of the battlefield's height, just for convineince.
	float boardHeight();
	//Pass this function two integers to change the size of the map.
	void setmapsize(float& wideness, float& highness);
	//Add a new wall
	void constructWall(wall& newWall);
	//Destroy a wall!
	void destroyWall(int wallID);
	//Constructor for battlestate; takes a battlePreset
	battlestate(battlePreset& preset);
	//Returns the number of travelling rays in existence
	size_t rayCount();
	//Add a ray to the vector or travelling rays (this is a relic of when I had private members. I miss those days)
	void spawnRay(ray& spawnthis);
	//Oh look there's another version of the previous function that works with const
	void spawnRay(const ray& spawnthis);
	/*This function is called every frame during battle unless the battle is paused; even then, It may still be best
	to call it and simply because certain animations might look cool cycling in the background when the battle is //DP: If u want to do that we should isolate the animation from movement
	awaiting user input. All iterative battle behaviour and logic goes in here, or is called from in here.*/

	//Moves the Rays on the battlefield
	void iterateRay(float inc);

	point recursiveReflectiveAim(enemy& e, int wallInd, int playerInd, int depth, point pos, const metastat& shotColor);

	void enemymB1(enemy& e);

	void enemymB1b(enemy& e);

	void enemysBRand8(enemy& e);

	void enemysB1(enemy& e);

	void enemysB4(enemy& e);

	void enemymB2(enemy& e);

	void enemymB3(enemy& e);

	void enemymB4(enemy& e);

	//Moves the Enemies on the battlefield
	void iterateAI(float inc);

	void iteratePlayer(float inc);

	void iterate(float &inc /*incremental time*/);
	//Constructor to read a "battlePreset.txt" and define this battlestate accordingly

	void playerAct(int playerInd);

};


#endif