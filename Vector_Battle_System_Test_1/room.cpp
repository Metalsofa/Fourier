//Implementation file for certain overworld classes

#include "room.h"

// WORLD CLASS IMPLEMENTATION

//Default constructor
world::world() {
	for (unsigned int i = 0; i < max_rooms; ++i) {
		rooms[i].empty = true;
		rooms[i].worldhook = this;
	}
	return;
}

//Free up any room slots that are no longer needed
void world::cleanup() {
	setDistances();
	for (unsigned int i = 0; i < max_rooms; ++i) {
		if (!rooms[i].empty && rooms[i].traversalDistance > maxdistance) {
			rooms[i].freeme();
		}
	}
	return;
}

//Recursive component of traversing the world and setting distances
void world::setDistances(room* travis, unsigned short dist) {
	assert(travis->traversalDistance == 0);
	travis->traversalDistance = dist;
	for (unsigned int i = 0; i < travis->gateways.size(); ++i) {
		if (travis->traversalDistance > dist) {
			setDistances(travis, dist + 1);
		}

	}
	return;
}

//Traverse the world and mark each room with how many steps it took to get there
void world::setDistances() {
	//Reset the distances to 0
	for (unsigned int i = 0; i < max_rooms; ++i) {
		rooms[i].traversalDistance = 0;
	}
	//Traverse the world
	setDistances(currentRoom, 1);
	//After this proccess, there should be no zeroes. They would be unreachable.
	return;
}

//Find space for a new room in the room array
unsigned int world::findspace() {
	int furthest = 0;
	for (unsigned int i = 0; i < max_rooms; ++i) {
		//If there's a free space, just return that
		if (rooms[i].empty) return i;
		if (rooms[i].traversalDistance > rooms[furthest].traversalDistance)
			furthest = i;
	}
	//Otherwise we need to make room (assume distances are already set)
	rooms[furthest].freeme();
	return furthest;
}

//Loads a room file into the world and returns a pointer to it
room* world::loadroom(const std::string& filepath) {
	//Search for if the room is already present
	for (unsigned int i = 0; i < max_rooms; ++i) {
		if (rooms[i].name == filepath) {
			rooms[i].recover();
			return &(rooms[i]);
		}
	}
	//Otherwise find room for it
	room* slot = rooms + findspace();
	*slot = room(filepath);
	return slot;
}

// PROP CLASS IMPLEMENTATION
prop::prop() : fgr::spritesheet() {
	movable = false;
	interact_func = NULL;
	interactionCount = 0;
}

void prop::interact() {
	interact_func(interactionCount, *this);
	interactionCount++;
}

// DOOR CLASS IMPLEMENTATION

//Default constructor
door::door() {
	destination = NULL;
	contiguous = false;
	maypass = true;
}

//Dereference operator
room& door::operator*() const {
	return *destination;
}

// ROOM CLASS IMPLEMENTATION

//Default constructor
room::room() {
	empty = true;
	worldhook = NULL;
}

//Construct from room file path
room::room(const std::string& filepath) {

}

//Disconnect this room from any contiguous ones, and then free it up.
void room::freeme() {
	//Unlink any rooms connected to this one.
	for (unsigned int i = 0; i < gateways.size(); ++i) {
		gateways[i].destination->gateways[gateways[i].destdoor].destination = NULL;
		gateways[i].destination = NULL;
	}
	empty = true;
}

//Get back a room that has been freed.
void room::recover() {
	loadneighbors(1, -1);
}

//Load the neighbors of this room, recursively possibly loading the neighbors as well
void room::loadneighbors(int depth, int ignore) {
	//Iterate through every door
	door* mydoor;
	door* theirdoor;
	for (unsigned int i = 0; i < gateways.size(); ++i) {
		//Make sure to ignore the one we want to ignore
		if (i == ignore) continue;
		mydoor = &gateways[i];
		if (depth > 0 || mydoor->contiguous) {
			if (!mydoor->destination) {
				mydoor->destination = worldhook->loadroom(mydoor->filepath);
				theirdoor = &mydoor->destination->gateways[mydoor->destdoor];
				theirdoor -> destination = this;
				theirdoor -> destdoor = i;
			}
			(**mydoor).loadneighbors(depth - 1, mydoor->destdoor);
		}
	}
	return;
}

