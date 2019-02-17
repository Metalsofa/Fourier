/* This header file defines the 'battle' datatype, as well as a the battlefield datatype to support it.
Also the datatypes for stage geometry*/
#pragma once
#define battle
#include "spells.h"
#include "materials.h"


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
	wall (segment definingsegment, Material wallmaterial, bool isfixed) {
		body = definingsegment;
		materialtype defining_material(wallmaterial);
		material = defining_material;
		fixed = isfixed;
	}
	wall() {} //DP: You don't need default values?
};

//A battlefield is one of the members of battle_preset and battlestate; it contains the "terrain" of the battle, if you will.
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
	vector<point> intersections() {
		vector<point> returnthis;
		//Ded code
		/*
		while (i < walls.size()) {
			unsigned int j = 0;
			while (j < walls.size()) {
				if (i != j) {
					if (isintersect(walls[i].getbody(), walls[j].getbody())) {
						point ints = intersection(walls[i].getbody(), walls[j].getbody());
						bool add_it = true;
						unsigned int k = 0;
						while (k < returnthis.size()) {
							if (converges(returnthis[k], ints)) {
								add_it = false;
							}
							k++;
						}
						if (add_it)
							returnthis.emplace_back(ints);
					}
				}
				j++;
			}
			i++;
		}*/
		//DP: Below is a cleaner and slightly more efficient version of the loops you have above
		//DM:Implemented 1/31/19
		for (int i = 0; i < walls.size(); i++) {
			for (int j = i + 1; j < walls.size(); j++) {
				if (isintersect(walls[i].getbody(), walls[j].getbody())) {
					point ints = intersection(walls[i].getbody(), walls[j].getbody());
					bool add_it = true;
					for (int k = 0; k < returnthis.size(); k++) {
						if (converges(returnthis[k], ints)) {
							add_it = false;
						}
					}
					if (add_it)
						returnthis.emplace_back(ints);
				}
			}
		}
		return returnthis;
	}

	//returns the point (-100, -100) if there are no intersections.
	point nearestintersection(point& dot) {
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
	void setsize(float wide, float high) {
		width = wide;
		height = high;
	}
	float getwidth() {
		return width;
	}
	float getheight() {
		return height;
	}
	size_t wallcount() { return walls.size(); }
	wall getwall(int wallID) {
		return walls[wallID];
	}
	vector<wall> getwalls() {
		return walls;
	}
	void add_wall(wall& new_wall) { //Consider overloading this to take a wall or to take multiple args
		walls.push_back(new_wall);
	}
	void pop_wall(int wallID) { 
		swap(walls[wallID], walls.back());
		return walls.pop_back();
	}
};


/*pre-defined battles, ready to be called on when battle is initiated. These can be read from files
by the game when it plays, or written to files by me when I make battles in my battle design interface*/
class battle_preset {
public:
	battlefield map;
	//Enemies and their positions
	//Background

///public:
	///battlefield getmap() {
	///	return map;
	///}

	//constructor for testing purposes
	battle_preset(float width, float height) {
		map.setsize(width, height);
	}

	//Function to read a txt file and define this battle preset accordingly


	//Function to write this battle preset to a txt file

};


/*This is the dynamic battlestate where all the present "gobal" variables live. 
The best way to construct this is to feed it a preset.*/
class battlestate { 
public:
	//The mutable battlefield that combat takes place on
	battlefield map;
	//Vector of all travelling rays
	vector<ray> rays;
	//Other spells
	//vector of sinks/sources
	vector<combatant> fighters;
	//vector of portals



	//Returns a read-only copy of the map in use.
	///battlefield getmap() {
	///	return map;
	///}
	//Returns a FLOAT of the battlefield's width, just for convineince.
	float BoardWidth() {
		return float(map.getwidth());
	}
	//Returns a FLOAT of the battlefield's height, just for convineince.
	float BoardHeight() {
		return float(map.getheight());
	}
	//Pass this function two integers to change the size of the map.
	void setmapsize(float& wideness, float& highness) {
		map.setsize(wideness, highness);
	}
	void construct_wall(wall& new_wall) { //Pass by ref? K
		map.add_wall(new_wall);
	}
	void destroy_wall(int wallID) {
		map.pop_wall(wallID);
	}
	//Constructor for battlestate; takes a battle_preset
	battlestate (battle_preset& preset) { //Pass by ref? K
		map = preset.map;
	}
	//Returns the number of travelling rays in existence
	size_t raycount() {
		return rays.size();
	}
	//Add a ray to the vector or travelling rays (this is a relic of when I had private members. I miss those days)
	void spawn_ray(ray& spawnthis) {
		rays.push_back(spawnthis);
	}
	//Oh look there's another version of the previous function that works with const
	void spawn_ray(const ray& spawnthis) {
		rays.push_back(spawnthis);
	}
	/*This function is called every frame during battle unless the battle is paused; even then, It may still be best
	to call it and simply because certain animations might look cool cycling in the background when the battle is //DP: If u want to do that we should isolate the animation from movement
	awaiting user input. All iterative battle behaviour and logic goes in here, or is called from in here.*/
	void iterate(float &inc /*incremental time*/) {
		//Iterate Rays
		for (unsigned int i = 0; i < rays.size(); i++) {
			rays[i].advance(inc);
			//Now we check for collisions
			unsigned int j = 0;
			for (wall surface : map.walls) {
				if (rays[i].checkcollision(surface.getbody())) {
					int permit = rays[i].permitted(surface.getmaterial().getPermittivity_Spells());
					if (permit == 1) {
						bool shouldbounce = true;
						bool equidist = false; //True if the nearest two walls are equidistant to bits[1]
						segment frontseg(rays[i].getbits()[0], rays[i].bits[1]);
						segment serf = surface.getbody();
						point inters = intersection(frontseg, serf);
						unsigned int k = 0;
						unsigned int closestID = 0; //The ID of the closest wall
						//DP: Might want to store the distance of the closest so it doensn't have to keep recalculating
						while (k < map.getwalls().size()) {
							segment comparator = map.getwall(k).getbody();
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
							while (k < map.getwalls().size()) {
								segment comparator = map.getwall(k).getbody();
								if (distancetoseg(previnter, comparator) <= distancetoseg(previnter, serf)) {
									secondclosestID = closestID;
									closestID = k;
								}
								k++;
							}
							if (closestID == secondclosestID)
								k++;
							if (closestID != -1 && secondclosestID != -1) {
								if (distancetoseg(previnter, map.getwall(closestID).getbody()) ==
									distancetoseg(previnter, map.getwall(secondclosestID).getbody())
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
								int closestID_A = -1;
								int closestID_B = -1;
								unsigned int i = 0;
								vector<wall> wals = map.getwalls();
								while (i < wals.size()) {
									if (closestID_A == -1) {
										closestID_A = i++;
										continue;
									}
									segment comparator = wals[i].getbody();
									segment currentclosest = wals[closestID_A].getbody();
									float distance_comparator = distancetoseg(head, comparator);
									float distance_currentclosest = distancetoseg(head, currentclosest);
									/*This if statement compares the current furthest wall to the comparator,
									and sets the new closest if it beats the current one. The old closest
									is set to the second closest.*/
									if (distance_comparator < distance_currentclosest) {
										closestID_B = closestID_A;
										closestID_A = i;
									}
									else if (closestID_B == -1 || distance_comparator <
										distancetoseg(head, wals[closestID_B].getbody())) {
										closestID_B = i;
									}
									i++;
								}
								//Extract sega and segb from WALL_A and WALL_B
								segment sega = wals[closestID_A].getbody();
								segment segb = wals[closestID_B].getbody();
								//Find the appropriate bisector given sega, segb, and dot
								segment cornermirror = reflective_bisector(dot, sega, segb);
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
			//Erase this ray if it's out of bounds or dead
			if (abs(rays[i].getbits().back().x - map.getwidth() / 2) > map.getwidth()
				|| abs(rays[i].getbits().back().y - map.getheight() / 2) > map.getheight() || rays[i].deathtime()) {
				rays.erase(rays.begin() + i);
				i--; //Appropriately adjust our iterator
			}
		}
	}
	//Constructor to read a "battle_preset.txt" and define this battlestate accordingly

};

//Battlefield design stuff
#define BD_CREATE_WALLS 1
#define BD_ERASE_WALLS 2
#define BD_MAKE_RAYS 3
#define BD_MAKE_SHAPES 4
