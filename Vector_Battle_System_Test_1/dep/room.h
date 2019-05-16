/*Overworld type: room. In order for the overworld to be rendered at all,
the player has to be in some sort of room.*/
#pragma once
#ifndef __room_h__
#define __map_h__
#include "fgrutils.h"

//These give life to the world
class prop : public fgr::spritesheet {
public:
	// REPRESENTATION
	//Position relative to the room's origin point
	fgr::point position;
	//Whether or not this prop can be moved.
	bool movable;
	//How many times this prop remembers being interacted with by the player previously
	int interactionCount;
	//This function will be called when the player interacts with this prop
	void(*interact_func)(int prevc, prop& object);

	// MEMBER FUNCTIONS
	//CONSTURCTORS
	prop();
	//Call this when the player interacts with the prop (confusing I know)
	void interact();
};


class room {
public:
	// REPRESENTATION


	
};

#endif