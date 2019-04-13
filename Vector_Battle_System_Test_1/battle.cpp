//This is the implementation file for every class in 'battle.h'

//Header file inclusions
//#include "geometry.h"
#include "battle.h"
//#include "crypt.h"

//STL inclusions
//#include <vector>

using namespace std;
using namespace fgr;



///////////////////////////////////////////////////////////
//
//           Class battlefield DEFINITION
//
//			This class is for structuring battlefield
//			geometry and other aspects of terrain
//
///////////////////////////////////////////////////////////


//Returns a vector of every intersection point formed by the walls in the battlefield
vector<point> battlefield::intersections() const {
	vector<point> returnthis;
	for (int i = 0; i < walls.size(); i++) {
		for (int j = i + 1; j < walls.size(); j++) {
			if (isintersect(walls[i].getbody(), walls[j].getbody())) {
				point ints = intersection(walls[i].getbody(), walls[j].getbody());
				bool addIt = true;
				for (int k = 0; k < returnthis.size(); k++) {
					if (converges(returnthis[k], ints)) {
						addIt = false;
					}
				}
				if (addIt)
					returnthis.push_back(ints);
			}
		}
	}
	return returnthis;
}

//Returns the point (-100, -100) if there are no intersections.
point battlefield::nearestintersection(const point& dot) const {
	point nearest(-100, -100);
	for (unsigned int i = 0; i < intersections().size(); i++) {
		point comp = intersections()[i];
		float compdist = difference(comp, dot).magnitude();
		float neardist = difference(nearest, dot).magnitude();
		if (compdist <= neardist)
			nearest = comp;
	}
	return nearest;
}

//Set the dimensions of this battlefield object
void battlefield::setSize(float wide, float high) {
	width = wide;
	height = high;
}

//Accessor for the width of this battlefield
float battlefield::getWidth() {
	return width;
}

//Accessor for the height of this battlefield
float battlefield::getHeight() {
	return height;
}

//Return the number of walls
size_t battlefield::wallCount() { return walls.size(); }

//Get a particular wall
wall& battlefield::getWall(int wallID) {
	return walls[wallID];
}

//Get a refrence to the entire vector of walls
vector<wall>& battlefield::getWalls() {
	return walls;
}

//Add a wall to the vector of walls
void battlefield::addWall(wall& newWall) { //Consider overloading this to take a wall or to take multiple args
	walls.push_back(newWall);
}

//Pop a wall from the vector of walls
void battlefield::popWall(int wallID) {
	swap(walls[wallID], walls.back());
	return walls.pop_back();
}


///////////////////////////////////////////////////////////
//
//           Class battlePreset DEFINITION
//
//			This class is a pre-set battle that can be read
//			from a file or written to one.
//
///////////////////////////////////////////////////////////

//Default constructor
battlePreset::battlePreset() {

}

//Construct from height and width
battlePreset::battlePreset(float width, float height) {
	map.setSize(width, height);
}

//Construct this battle preset using a file
battlePreset::battlePreset(std::string filename) {
	readFromFile(filename);
}

//Write this battle preset to a file
void battlePreset::writeToFile(std::string filename) {

}

//Read in a battle preset from a file
void battlePreset::readFromFile(std::string filename) {

}


///////////////////////////////////////////////////////////
//
//           Class battlestate DEFINITION
//
//			This class keeps track of current data about
//			the battle, not meant to be written to files
//
///////////////////////////////////////////////////////////


//Returns a float of the battlefield's width, just for convenience
float battlestate::boardWidth() {
	return float(map.getWidth());
}

//Returns a FLOAT of the battlefield's height, just for convineince.
float battlestate::boardHeight() {
	return float(map.getHeight());
}

//Pass this function two integers to change the size of the map.
void battlestate::setmapsize(float& wideness, float& highness) {
	map.setSize(wideness, highness);
}

void battlestate::constructWall(wall& newWall) {
	map.addWall(newWall);
}

void battlestate::destroyWall(int wallID) {
	map.popWall(wallID);
}

//Constructor for battlestate; takes a battlePreset
battlestate::battlestate(battlePreset& preset) { //Pass by ref? K
	map = preset.map;
}

//Returns the number of travelling rays in existence
std::size_t battlestate::rayCount() {
	return rays.size();
}

//Add a ray to the vector or travelling rays 
void battlestate::spawnRay(ray& spawnthis) {
	rays.push_back(spawnthis);
}

//Oh look there's another version of the previous function that works with const
void battlestate::spawnRay(const ray& spawnthis) {
	rays.push_back(spawnthis);
}

/*This function is called every frame during battle unless the battle is paused; even then, It may still be best
to call it and simply because certain animations might look cool cycling in the background when the battle is //DP: If u want to do that we should isolate the animation from movement
awaiting user input. All iterative battle behaviour and logic goes in here, or is called from in here.*/
void battlestate::iterate(float &inc /*incremental time*/) {
	//Iterate Rays
	for (unsigned int i = 0; i < rays.size(); i++) {
		rays[i].advance(inc);
		//Now we check for collisions
		unsigned int j = 0;
		bool term = false;
		for (combatant& x : protags) {
			int hit = rays[i].checkcollision(x);
			if (hit) {
				rays[i].terminate(rays[i].bits[hit-1]); //http://mathworld.wolfram.com/Circle-LineIntersection.html
				term = true;
			}
		}
		for (combatant& x : antags) {
			int hit = rays[i].checkcollision(x);
			if (hit) {
				rays[i].terminate(rays[i].bits[hit - 1]); //http://mathworld.wolfram.com/Circle-LineIntersection.html
				term = true;
			}
		}

		for (wall& surface : map.walls) {
			if (rays[i].checkcollision(surface.getbody())) {
				term = true;
				int permit = rays[i].permitted(surface.getmaterial().getPermittivitySpells());
				if (permit == 1) {
					bool shouldbounce = true;
					bool equidist = false; //True if the nearest two walls are equidistant to bits[1]
					segment frontseg(rays[i].getbits()[0], rays[i].bits[1]);
					segment serf = surface.getbody();
					point inters = intersection(frontseg, serf);
					unsigned int k = 0;
					unsigned int closestID = 0; //The ID of the closest wall
					//DP: Might want to store the distance of the closest so it doensn't have to keep recalculating
					while (k < map.getWalls().size()) {
						segment comparator = map.getWall(k).getbody();
						if (distancetoseg(inters, comparator) <= distancetoseg(inters, serf))
							closestID = k;
						k++;
					}
					if (!closestID == j)
						shouldbounce = false; /* From now on it's fair to assume that j is the ID of
											  the closest wall to inters */

					/*The following logic is for judging whether the current intersection is actually
					the same as the previous */
					if (rays[i].getbits().size() > 2) {
						point previnter = rays[i].bits[1];

						/* The following sequence returns a closestID value representing the closest
						wall to the previous intersection; ie, the wall on which the previous
						collision took place.*/
						//Also it's breaking wall collisions on walls with certain indices
						unsigned int k = 0;
						int closestID = -1; //DP: Didn't you already calculate the closest above? also known as j? This is pretty confusing, you'll have to explain it to me
						int secondclosestID = -1;
						//DP: Might want to store the distance of the first and second closest so it doensn't have to keep recalculating
						while (k < map.getWalls().size()) {
							segment comparator = map.getWall(k).getbody();
							if (distancetoseg(previnter, comparator) <= distancetoseg(previnter, serf)) {
								secondclosestID = closestID;
								closestID = k;
							}
							k++;
						}
						if (closestID == secondclosestID)
							k++;
						if (closestID != -1 && secondclosestID != -1) {
							if (distancetoseg(previnter, map.getWall(closestID).getbody()) ==
								distancetoseg(previnter, map.getWall(secondclosestID).getbody())
								&& closestID != secondclosestID) {
								equidist = true;
							}
						}
						/* So now j is the ID for the wall inters is on, and closestID is the ID for
						the closest wall to previnter*/
						float err = 0.01f; //Used to be 0.01f;
						int prevwallID = closestID;
						int currentwallID = j;
						if (prevwallID == currentwallID
							&& difference(inters, previnter).magnitude() < err)
							shouldbounce = false;
					}
					if (shouldbounce) {
						//Check if at a corner
						bool atcorner = false;
						if (converges(inters, map.nearestintersection(inters))) {
							atcorner = true;
						}
						//The following IF statement is triggering as it should, but the
						//Corner logic is causing the ray to pass through corners
						if (difference(map.nearestintersection(inters), inters).magnitude() < 0.03) {
							atcorner = true;
						}
						if (equidist)
							atcorner = true;
						//If in a corner, turn serf into the appropriate bisector
						if (atcorner) {
							//Determine point dot, the location of the previous intersection
							point dot = rays[i].getbits()[1];
							point head = rays[i].getbits()[0];
							point corner = map.nearestintersection(inters);
							//Determine the closest walls: WALL_A and WALL_B (really just their indices)
							int closestIDa = -1;
							int closestIDb = -1;
							unsigned int i = 0;
							vector<wall> wals = map.getWalls();
							while (i < wals.size()) {
								if (closestIDa == -1) {
									closestIDa = i++;
									continue;
								}
								segment comparator = wals[i].getbody();
								segment currentclosest = wals[closestIDa].getbody();
								float distanceComparator = distancetoseg(head, comparator);
								float distanceCurrentClosest = distancetoseg(head, currentclosest);
								/*This if statement compares the current furthest wall to the comparator,
								and sets the new closest if it beats the current one. The old closest
								is set to the second closest.*/
								if (distanceComparator < distanceCurrentClosest) {
									closestIDb = closestIDa;
									closestIDa = i;
								}
								else if (closestIDb == -1 || distanceComparator <
									distancetoseg(head, wals[closestIDb].getbody())) {
									closestIDb = i;
								}
								i++;
							}
							//Extract sega and segb from WALL_A and WALL_B
							segment sega = wals[closestIDa].getbody();
							segment segb = wals[closestIDb].getbody();
							//Find the appropriate bisector given sega, segb, and dot
							segment cornermirror = reflectiveBisector(dot, sega, segb);
							//Turn serf into that bisector
							serf = cornermirror;
						}
						rays[i].bounce(serf);
					}
				}
				if (permit == 0 && !rays[i].terminating)
					rays[i].terminate(rays[i].wherehit(surface.getbody()));
			}
			j++;
		}
		if (!term) { rays[i].terminating = false; }
		//Erase this ray if it's out of bounds or dead
		if (abs(rays[i].getbits().back().x() - map.getWidth() / 2) > map.getWidth()
			|| abs(rays[i].getbits().back().y() - map.getHeight() / 2) > map.getHeight() || rays[i].deathtime()) {
			rays.erase(rays.begin() + i);
			i--; //Appropriately adjust our iterator
		}
	}

	//Iterate AI:
	for (enemy& e : antags) {
		e.act(*this);
	}
}
