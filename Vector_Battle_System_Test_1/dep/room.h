/*Overworld type: room. In order for the overworld to be rendered at all,
the player has to be in some sort of room.*/
#pragma once
#ifndef __room_h__
#define __room_h__
#include "fgrutils.h"

//Forward declaration
class world;
class prop;
class door;
class room;

//The maximum number of rooms that can be loaded in at once.
const unsigned int max_rooms = 16;

///////////////////////////////////////////////////////////
//
//           Class room DEFINITION
//
//				This class structures the properties
//				of a given room.
//				Used as a node in the 'world' data
//				structure.
//
///////////////////////////////////////////////////////////

//A single room in a world, loaded in all at once
class room {
public:
	// REPRESENTATION
	//If this is true, something could be loaded on top of this room
	bool empty;
	//Hook to the world class that this room is a part of 
	world* worldhook;
	//Identical to the filepath this was loaded from
	std::string name;
	//The image representing the background
	fgr::graphic backround;
	//The portals from this room to other rooms, and which room they go to
	std::vector<door> gateways;
	//How far from the player's location we have found this room to be
	unsigned short traversalDistance;
	// FUNCTIONS
	//CONSTRUCTORS
	//Default constructor
	room();
	//Construct from a filepath
	room(const std::string& path);
	//Load neighbors of this room
	void loadneighbors(int depth, int ignore);
	//Free the slot this room takes and disconnect it from any rooms that connect to it
	void freeme();
	//Recover a freed room
	void recover();
};

///////////////////////////////////////////////////////////
//
//           Class world DEFINITION
//
//			This class structures the rooms
//			that are loaded in the game.
//			This implementation uses a graph
//			structure that keeps the members
//			in a vector. This makes garbage
//			collection a lot easier.
//
///////////////////////////////////////////////////////////

//A data structure for organizing rooms in a linked graph
class world {
public:
	// REPERSENTATION
	//The rooms are kept in this array
	room rooms[max_rooms];
	//The 'current' room
	room* currentRoom;
	//How far from a room we are willing to be before despawning it automatically.
	unsigned short maxdistance;
	// FUNCTIONS
	//Traverse he world and set the stepcount for the rooms
		//Driver funciton
	void setDistances();
		//For recursive use
	void setDistances(room* travis, unsigned short dist);
	//Load a room file into the room array automatically
	room* loadroom(const std::string& filepath);
	//Finds space in the room array for a new room to be put in there
	unsigned int findspace();
	// FUNCTIONS
	//CONSTRUCTORS
	//Default constructor
	world();
	//Iterates through the room array and frees cells that are no longer needed
	void cleanup();
};

///////////////////////////////////////////////////////////
//
//           Class prop DEFINITION
//
//				These are objects that can be interacted
//				with (or not) in the world
//
///////////////////////////////////////////////////////////

//These give life to the world
class prop : public fgr::spritesheet {	//DP: I think the prop should have a spritesheet, rather than be a spritesheet
public:
	// REPRESENTATION
	//Identical to the filepath this was loaded from
	std::string name;
	//Position relative to the room's origin point
	fgr::point position;
	//Whether or not this prop can be moved.
	bool movable;
	//How many times this prop remembers being interacted with by the player previously
	int interactionCount;
	//This function will be called when the player interacts with this prop
	void(*interact_func)(int prevc, prop& object);	//DP: Does a prop need to be passed in?

	// MEMBER FUNCTIONS
	//CONSTURCTORS
	prop();
	//Call this when the player interacts with the prop (confusing I know)
	void interact();
};

///////////////////////////////////////////////////////////
//
//           Class door DEFINITION
//
//				These act as pointers in the
//				'world' data structure.
//
///////////////////////////////////////////////////////////

class door {
public:
	//The box the player must walk into in order to pass through the door
	fgr::segment hitbox;
	//The file path to the room that this door leads to
	std::string filepath;
	//The identity of the room within this floor that this leads to
	room* destination;
	//The index of the door in the destination room that this one sends your out of
	unsigned int destdoor;
	//Whether the room it points to should be loaded as soon as this one is (part of the same contiguous reigon, in other words)
	bool contiguous;
	//Whether the player may go through this door, otherwise it is 'locked' and causes no interaction
	bool maypass;
	// FUNCTIONS
	//CONSTRUCTORS
	//Default constructor
	door();
	//The derefrence operator returns a refrence to the room this door points to
	room& operator*() const;
};
#endif