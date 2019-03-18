/* This header file defines the 'battle' datatype, as well as a the battlefield datatype to support it.
Also the datatypes for stage geometry*/
#pragma once
#ifndef __battle_h__
#define __battle_h__

//Header includes
#include "spells.h"
#include "materials.h"
#include "geometry.h"
//#include "crypt.h"

//STL includes
#include <vector>
#include <string>

using namespace std;

class wall {
public:
	segment body;
	materialtype material;
	bool fixed; //wave or no wave
	int shape; //Circular, linear
///public:
	segment& getbody() { return body; }
	materialtype getmaterial() { return material; }
	bool getfixed() { return fixed; }
	wall(segment definingsegment, Material wallmaterial, bool isfixed);
	wall(); //DP: You don't need default values?
};

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
	vector<point> intersections();
	//returns the point (-100, -100) if there are no intersections.
	point nearestintersection(point& dot);
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
	void iterate(float &inc /*incremental time*/);
	//Constructor to read a "battlePreset.txt" and define this battlestate accordingly
};

#endif