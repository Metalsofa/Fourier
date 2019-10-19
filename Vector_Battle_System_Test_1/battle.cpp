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

//Param: None
//Return: vector of every intersection point formed by the walls in the battlefield
//Notes: None
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

//Param: A single point, dot
//Return: Intersection closest to dot
//Notes: None
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
//Param: float width, float height
//Return: None
//Notes: Sets the dimensions of this battlefield to the parameters
void battlefield::setSize(float wide, float high) {
	width = wide;
	height = high;
}

//Param: None
//Return: width member variable
//Notes: None
float battlefield::getWidth() {
	return width;
}

//Param: None
//Return: height member variable
//Notes: None
float battlefield::getHeight() {
	return height;
}

//Param: None
//Return: Number of walls on the battlefield
//Notes: None
size_t battlefield::wallCount() { return walls.size(); }

//Param: ID for a wall
//Return: a wall by reference from the walls vector
//Notes: None
wall& battlefield::getWall(int wallID) {
	return walls[wallID];
}

//Param: None
//Return: A reference to the vector of walls member variable
//Notes: None
vector<wall>& battlefield::getWalls() {
	return walls;
}

//Param: Wall to add to battlefield
//Return: None
//Notes: None
void battlefield::addWall(wall& newWall) { //Consider overloading this to take a wall or to take multiple args
	walls.push_back(newWall);
}

//Param: ID of wall to remove(Must be valid)
//Return: None
//Notes: Order of walls vector is not perserved unlike delWall, and is more efficient than delWall
void battlefield::popWall(int wallID) {
	swap(walls[wallID], walls.back());
	return walls.pop_back();
}

//Param: ID of wall to remove
//Return: None
//Notes: Order of walls vector is perserved unlike popWall, and is less efficient than popWall
void battlefield::delWall(int wallID) {
	walls.erase(walls.begin() + wallID);
}

///////////////////////////////////////////////////////////
//
//           Class battlePreset DEFINITION
//
//			This class is a pre-set battle that can be read
//			from a file or written to one.
//
///////////////////////////////////////////////////////////

//Param: None
//Return: None
//Notes: Default constructor (Unimplemenented)
battlePreset::battlePreset() {

}

//Param: Height and Width for the map
//Return: None
//Notes: Constructor
battlePreset::battlePreset(float width, float height) {
	map.setSize(width, height);
}

//Param: filename of file with battle preset data
//Return: None
//Notes: Constructor
battlePreset::battlePreset(std::string filename) {
	readFromFile(filename);
}

//Param: filename of file to write this battlepreset to
//Return: None
//Notes: Write this battle preset to a file (Unimplemented)
void battlePreset::writeToFile(std::string filename) {

}

//Param: filename of a file to read a preset from 
//Return: None
//Notes: Overwrites current settings with whatever is read from file (Unimplemented)
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


//Param: None
//Return: Width of the battlefield
//Notes: Mostly for convienience
float battlestate::boardWidth() {
	return float(map.getWidth());
}

//Param: None
//Return: Height of battlefield
//Notes: Mostly for convienience
float battlestate::boardHeight() {
	return float(map.getHeight());
}

//Param: Width and height of battlefield
//Return: None
//Notes: Overwrites current width and height with parameters
void battlestate::setmapsize(float& wideness, float& highness) {
	map.setSize(wideness, highness);
}

//Param: Wall by reference
//Return: None
//Notes: Check battlefield::addWall for more
void battlestate::constructWall(wall& newWall) {
	map.addWall(newWall);
}

//Param: id of wall to remove
//Return: None
//Notes: Check battlefield::popWall for more
void battlestate::destroyWall(int wallID) {
	map.popWall(wallID);
}

//Param: Another battlepreset by reference
//Return: None
//Notes: Copy constructor
battlestate::battlestate(battlePreset& preset) {
	map = preset.map;
}


//Param: None
//Return: None
//Notes: Sets up the characters in a way used by the devs for testing
void battlestate::initChars() {
	//Setup battle: Initialize combatants
	player plyr1("teststat1.txt", point(0, 6), graphic("alpha.fgr"));
	plyr1.AddSpell(Spell(wallConst(BASIC_REFLECTIVE, true, 0)), 20);
	plyr1.AddSpell(Spell(portalConst(BASIC_REFLECTIVE)), 50);
	plyr1.AddSpell(Spell(rayConst(metastat(255, 255, 255), 5, 1, 1)), 1);
	protags.push_back(plyr1);

	player plyr2("teststat2.txt", point(10, 6), graphic("beta.fgr"));
	plyr2.AddSpell(rayConst(metastat(255, 255, 255), 5, 3, 1), 5);
	protags.push_back(plyr2);

	player plyr3("teststat3.txt", point(10, 0), graphic("gamma.fgr"));
	plyr3.AddSpell(portalConst(BASIC_REFLECTIVE), 20);
	protags.push_back(plyr3);

	player plyr4("teststat4.txt", point(0, 0), graphic("delta.fgr"));
	plyr4.AddSpell(rayConst(metastat(255, 255, 255), 5, 1, 1), 5);
	protags.push_back(plyr4);

	enemy e1(1, 3);
	e1.position = point(7, 5);
	e1.width = .4f;
	e1.addWaypoint(point(6, 5));
	e1.addWaypoint(point(5, 5));
	e1.addWaypoint(point(4, 5));
	antags.push_back(e1);

	enemy e2(1, 3);
	e2.position = point(7, 3);
	e2.width = .4f;
	e2.addWaypoint(point(6, 3));
	e2.addWaypoint(point(5, 3));
	e2.addWaypoint(point(4, 3));
	antags.push_back(e2);

	enemy e3(1, 3);
	e3.position = point(7, 4);
	e3.width = .4f;
	e3.addWaypoint(point(6, 4));
	e3.addWaypoint(point(5, 4));
	e3.addWaypoint(point(4, 4));
	antags.push_back(e3);


}


//Param: None
//Return: The number of traveling rays on the battlefield
//Notes: None
std::size_t battlestate::rayCount() {
	return rays.size();
}


//Param: Ray by reference to add to battlefield
//Return: None
//Notes: This method has an overloaded version where the parameter is const
void battlestate::spawnRay(ray& spawnthis) {
	rays.push_back(spawnthis);
}

//Param: Ray by const reference to add to the battlefield
//Return: None
//Notes: This method has an overloaded version where the parameter is not const
void battlestate::spawnRay(const ray& spawnthis) {
	rays.push_back(spawnthis);
}

//Param: A time unit to iterate all the rays by
//Return: None
//Notes: Moves all the rays in accordance to how they should move (Bounce off walls, go through portals, etc.)
void battlestate::iterateRay(float inc) {
	for (unsigned int i = 0; i < rays.size(); i++) {
		rays[i].advance(inc);
		//Now we check for collisions
		bool term = false;
		for (int j = 0; j < protags.size(); j++) {
			int hit = checkcollision(rays[i], protags[j]);
			if (hit) {
				rays[i].terminate(rays[i].bits[hit - 1]); //http://mathworld.wolfram.com/Circle-LineIntersection.html
				term = true;
				//if (protags[j].hit(rays[i].color, 1)) { protags.erase(protags.begin() + j); }	//FUTURE: change 1 to level of ray or change function, death animation or script(possibly)
				break;
			}
		}
		if (!term) {
			for (int j = 0; j < antags.size(); j++) {
				int hit = checkcollision(rays[i], antags[j]);
				if (hit) {
					rays[i].terminate(rays[i].bits[hit - 1]); //http://mathworld.wolfram.com/Circle-LineIntersection.html
					term = true;
					//if (antags[j].hit(rays[i].color, 1)) { antags.erase(antags.begin() + j); }	//FUTURE: change 1 to level of ray or change function, death animation or script(possibly)
					break;
				}
			}
		}
		unsigned int j = 0;	//Portal index
		for (portal& surface : map.portals) {
			if (rays[i].checkcollision(surface.getbody())) {
				term = true;
				if (rays[i].terminating) { rays[i].terminate(rays[i].bits[0]); break; }
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
					while (k < map.portals.size()) {
						segment comparator = map.portals[k].getbody();
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
						while (k < map.portals.size()) {
							segment comparator = map.portals[k].getbody();
							if (distancetoseg(previnter, comparator) <= distancetoseg(previnter, serf)) {
								secondclosestID = closestID;
								closestID = k;
							}
							k++;
						}
						if (closestID == secondclosestID)
							k++;
						if (closestID != -1 && secondclosestID != -1) {
							if (distancetoseg(previnter, map.portals[closestID].getbody()) ==
								distancetoseg(previnter, map.portals[secondclosestID].getbody())
								&& closestID != secondclosestID) {
								equidist = true;
							}
						}
						/* So now j is the ID for the wall inters is on, and closestID is the ID for
						the closest wall to previnter*/
						float err = 0.01f;
						int prevwallID = closestID;
						int currentwallID = j;
						if (prevwallID == currentwallID
							&& difference(inters, previnter).magnitude() < err)
							shouldbounce = false;
					}
					if (surface.pairInd != -1) {
						portal* pair = &map.portals[surface.pairInd];														//Paired portal(b) to portal(a) that is currently being hit
						point originHit = rays[i].wherehit(surface.getbody());												//Where the ray connects with portal a
						point pairHit = transfer(originHit, surface.body, pair->body);
						rays.push_back(ray(rays[i]));																		//Duplicates the ray
						while (rays.back().bits.size() > 2) {
							rays.back().bits.pop_back();
						}
						segment surface2(difference(serf.p1, serf.midpoint()), difference(serf.p2, serf.midpoint()));
						segment surface3(difference(pair->body.p1, pair->body.midpoint()), difference(pair->body.p2, pair->body.midpoint()));
						//point surfRay = unitvector(reflection(rays[i].direction, surface2));								
						//point seg = unitvector(serf.p2 - serf.p1);															
						//float ang = surfRay.angle() - seg.angle();															//Angle that new direction makes with portal a
						//float pairAng = (pair->body.p2 - pair->body.p1).angle();											//Angle that new direction makes with portal b if b is horizontal
						//ang += pairAng;																						//Adjusts for angle of portal b
						//rays.back().direction = point(cos(ang), sin(ang));
						rays.back().direction = reflection(rays[i].direction, surface2, surface3);
						rays.back().bits[0] = rays.back().bits[1] = pairHit + rays.back().direction * .01;					//Moves bits of new ray to come from portal b

						rays[i].terminate(rays[i].bits[0]);

					}
				}
				if (permit == 0 && !rays[i].terminating)
					rays[i].terminate(rays[i].bits[0]);	//NOTE: should terminate on the bit instead of the intersection for a smoother stop, also need to terminate again after 1 advance
				break;
			}
			j++;
		}

		j = 0;	//wall index
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
					if (closestID != j)
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
						float err = 0.01f;
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
								} else if (closestIDb == -1 || distanceComparator <
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
}

//Param: Enemy that is shooting, current wall that is being tested, which player is the target, recursion depth, 
//		 position that enemy is shooting from(mathematically), color of the ray(to test for permeation)
//Return: Point that an enemy at pos should shoot at to hit the player at playerInd, will return e.position if no such value can be found
//Notes: (DEPRECATED, USE recursiveReflexiveAim) 
//		 This is a recursive function that should be perfect AI to hit a player given the walls on the battlefield 
point battlestate::recursiveReflectiveAimWall(enemy& e, int wallInd, int playerInd, int depth, point pos, const metastat& shotColor) {
	//If there are no walls, just take a straight shot for the player
	if (!map.getWalls().size()) {
		return protags[playerInd].position;
	}
	//Return [invalid point] if no potential paths have been found
	bool breakP = false;
	if (depth == -1) { return e.position; }
	//If not at the base case
	if (wallInd != -1) {
		//Draw a line from here to the target
		segment s(protags[playerInd].position, pos);
		//Check if this line intersects the given wall
		if (isintersect(map.getWall(wallInd).body, s)) {
			//Draw a line from the intersection to the target
			segment trace(intersection(map.getWall(wallInd).body, s), s.p1);
			//Check that no other walls intersect with this segment
			bool reCurse = false;
			for (int i = 0; i < map.getWalls().size(); i++) {
				if (i != wallInd) {
					if (isintersect(trace, map.getWall(i).body)) {
						reCurse = true;
						break;
					}
				}
			}
			//If the recursion flag has been set to false, return
			if (!reCurse) {
				return intersection(map.getWall(wallInd).body, s);
			}
		}
	}
	//Check every other wall
	for (int i = 0; i < map.getWalls().size(); i++) {
		//Make sure not to twice consider this wall
		if (wallInd != i && permitted(shotColor, map.getWall(i).material.getPermittivitySpells())) {
			//Check if the considered wall is visable
			segment segIa(pos, map.getWall(i).body.p1);
			segment segIb(pos, map.getWall(i).body.p2);
			bool contin = false;
			for (int j = 0; j < map.getWalls().size(); j++) {
				if (j != i && isintersect(segIa, map.getWall(j).body) && isintersect(segIb, map.getWall(j).body)) {
					contin = true;
					break;
				}
			}
			if (contin) { continue; }

			//Recall this function on walls[i], after reflecting 'pos' across that wall
			point reticle(recursiveReflectiveAimWall(e, i, playerInd, depth - 1, reflection(pos, map.getWall(i).body), shotColor));
			//Continue if nothing valid is found
			if (reticle == e.position)
				continue;
			//Draw a line from here to the target
			segment trace(reticle, pos);
			segment s(intersection(trace, map.getWall(i).body), (wallInd == -1
				? pos
				: intersection(trace, map.getWall(wallInd).body)
				)
			);
			bool cont = false;
			for (int j = 0; j < map.getWalls().size(); j++) {
				if (j != i && j != wallInd && isintersect(s, map.getWall(j).body)) {
					cont = true;
					break;
				}
				//Check if this line intersects the given wall
				else if (j == wallInd && wallInd != -1 && !isintersect(map.getWall(wallInd).body, trace)) {
					cont = true;
					break;
				}
			}
			if (cont) {
				continue;
			}
			//If we're still considering a direct line, don't reflect it
			if (wallInd == -1)
				return reticle;
			//Othersize, reflect that point over walls[wallInd], which is the last one, and return the reflected point
			return reflection(reticle, map.getWall(wallInd).body);
		}
	}
	//If no valid solutions are found, return enemies position
	return e.position;
}

//Param: Enemy that is shooting, current wall that is being tested, which player is the target, recursion depth, 
//		 position that enemy is shooting from(mathematically), color of the ray(to test for permeation), 
//		 and whether we are testing a portal(1) or a wall(0)
//Return: Point that an enemy at pos should shoot at to hit the player at playerInd, will return e.position if no such value can be found
//Notes: This is a recursive function that should be perfect AI to hit a player given the walls and portals on the battlefield 
point battlestate::recursiveReflectiveAim(enemy& e, int wallInd, int playerInd, int depth, point pos, const metastat& shotColor, int portOrWall = 0) {
	//If there are no walls, just take a straight shot for the player
	if (!(map.getWalls().size() + map.portals.size())) {
		return protags[playerInd].position;
	}
	if (portOrWall != 0 && wallInd != -1 && map.portals[wallInd].pairInd == -1) {
		return e.position;
	}
	//Return [invalid point] if no potential paths have been found
	bool breakP = false;
	if (depth == -1) { return e.position; }
	//If not at the base case
	if (wallInd != -1) {
		//Draw a line from here to the target
		segment s(protags[playerInd].position, pos);
		//Check if this line intersects the given wall
		if ((portOrWall == 0 && isintersect(map.getWall(wallInd).body, s)) || (portOrWall == 1 && isintersect(map.portals[wallInd].body, s))) {
			//Draw a line from the intersection to the target
			segment trace(intersection((portOrWall == 0 ? map.getWall(wallInd).body : map.portals[wallInd].body), s), s.p1);
			//Check that no other walls intersect with this segment
			bool reCurse = false;
			for (int i = 0; i < map.getWalls().size(); i++) {
				if (i != wallInd || portOrWall != 0) {
					if (isintersect(trace, map.getWall(i).body)) {
						reCurse = true;
						break;
					}
				}
			}
			for (int i = 0; i < map.portals.size(); i++) {
				if (i != wallInd || portOrWall == 0) {
					if (map.portals[i].pairInd != -1 && isintersect(trace, map.portals[i].body)) {
						reCurse = true;
						break;
					}
				}
			}

			//If the recursion flag has been set to false, return
			if (!reCurse) {
				if (portOrWall == 0) {
					return intersection(map.walls[wallInd].body, s);
				} else {
					return reflection(intersection(map.portals[wallInd].body, s), map.portals[wallInd].body, map.portals[map.portals[wallInd].pairInd].body);
				}
			}
		}
	}
	//Check every other wall
	for (int i = 0; i < map.getWalls().size(); i++) {
		//Make sure not to twice consider this wall
		if ((wallInd != i || portOrWall != 0) && permitted(shotColor, map.getWall(i).material.getPermittivitySpells())) {
			//Check if the considered wall is visable
			segment segIa(pos, map.getWall(i).body.p1);
			segment segIb(pos, map.getWall(i).body.p2);
			bool contin = false;
			for (int j = 0; j < map.getWalls().size(); j++) {
				if (j != i && isintersect(segIa, map.getWall(j).body) && isintersect(segIb, map.getWall(j).body)) {
					contin = true;
					break;
				}
			}
			if (contin) { continue; }
			for (int j = 0; j < map.portals.size(); j++) {
				if (isintersect(segIa, map.portals[j].body) && isintersect(segIb, map.portals[j].body)) {
					contin = true;
					break;
				}
			}
			if (contin) { continue; }
			//Recall this function on walls[i], after reflecting 'pos' across that wall
			point reticle(recursiveReflectiveAim(e, i, playerInd, depth - 1, reflection(pos, map.getWall(i).body), shotColor));
			//Continue if nothing valid is found
			if (reticle == e.position)
				continue;
			//Draw a line from here to the target
			segment trace(reticle, pos);
			segment s(intersection(trace, map.getWall(i).body), (
				wallInd == -1 ? pos
				: intersection(trace, (portOrWall == 0 ? map.getWall(wallInd).body : map.portals[wallInd].body))
				)
			);
			bool cont = false;
			for (int j = 0; j < map.walls.size(); j++) {
				if (j != i && (j != wallInd || portOrWall != 0) && isintersect(s, map.walls[j].body)) {
					cont = true;
					break;
				}
				//Check if this line intersects the given wall
				else if (j == wallInd && portOrWall == 0 && !isintersect(map.walls[wallInd].body, trace)) {
					cont = true;
					break;
				}
			}
			if (cont) {
				continue;
			}
			for (int j = 0; j < map.portals.size(); j++) {
				if (map.portals[j].pairInd != -1 && isintersect(s, map.portals[j].body)) {
					cont = true;
					break;
				}//Check if this line intersects the given wall
				else if (j == wallInd && portOrWall != 0 && !isintersect(map.portals[wallInd].body, trace)) {
					cont = true;
					break;
				}
			}
			if (cont) {
				continue;
			}
			if (cont) {
				continue;
			}
			//If we're still considering a direct line, don't reflect it
			if (wallInd == -1)
				return reticle;
			//Othersize, reflect that point over walls[wallInd], which is the last one, and return the reflected point
			//If the last wall was a wall reflect it over that, otherwise reflect it over the portal
			if (portOrWall == 0) {
				return reflection(reticle, map.getWall(wallInd).body);
			}
			return reflection(reticle, map.portals[wallInd].body, map.portals[map.portals[wallInd].pairInd].body);

		}
	}
	//Check every other portal
	for (int i = 0; i < map.portals.size(); i++) {
		//Make sure not to twice consider this portal
		if ((wallInd != i || portOrWall == 0) && permitted(shotColor, map.portals[i].material.getPermittivitySpells()) && map.portals[i].pairInd != -1) {
			//Check if the considered wall is visable
			segment segIa(pos, map.portals[i].body.p1);
			segment segIb(pos, map.portals[i].body.p2);
			bool contin = false;
			for (int j = 0; j < map.walls.size(); j++) {
				if (isintersect(segIa, map.getWall(j).body) && isintersect(segIb, map.getWall(j).body)) {
					contin = true;
					break;
				}
			}
			if (contin) { continue; }
			for (int j = 0; j < map.portals.size(); j++) {
				if (j != i && map.portals[j].pairInd != -1 && isintersect(segIa, map.portals[j].body) && isintersect(segIb, map.portals[j].body)) {
					contin = true;
					break;
				}
			}
			if (contin) { continue; }

			//Recall this function on portals[i], after reflecting 'pos' across that portal
			point reticle(recursiveReflectiveAim(e, map.portals[i].pairInd, playerInd, depth - 1, reflection(pos, map.portals[i].body, map.portals[map.portals[i].pairInd].body), shotColor, 1));
			//Continue if nothing valid is found
			if (reticle == e.position)
				continue;
			//Draw a line from here to the target
			segment trace(reticle, pos);
			segment s(intersection(trace, map.portals[i].body), (wallInd == -1
				? pos
				: intersection(trace, (portOrWall == 0 ? map.getWall(wallInd).body : map.portals[wallInd].body))
				)
			);
			bool cont = false;
			for (int j = 0; j < map.portals.size(); j++) {
				if (j != i && (j != wallInd || portOrWall == 0) && isintersect(s, map.portals[j].body)) {
					cont = true;
					break;
				}
				//Check if this line intersects the given wall
				else if (j == wallInd && portOrWall != 0 && wallInd != -1 && !isintersect(map.portals[wallInd].body, trace)) {
					cont = true;
					break;
				}
			}
			if (cont) {
				continue;
			}
			for (int j = 0; j < map.walls.size(); j++) {
				if (isintersect(s, map.walls[j].body)) {
					cont = true;
					break;
				}//Check if this line intersects the given wall
				else if (j == wallInd && portOrWall == 0 && !isintersect(map.walls[wallInd].body, trace)) {
					cont = true;
					break;
				}
			}
			if (cont) {
				continue;
			}
			//If we're still considering a direct line, don't reflect it
			if (wallInd == -1)
				return reticle;
			//Othersize, reflect that point over walls[wallInd], which is the last one, and return the reflected point
			//If the last wall was a wall reflect it over that, otherwise reflect it over the portal
			if (portOrWall == 0) {
				return reflection(reticle, map.getWall(wallInd).body);
			}
			point ret = reflection(reticle, map.portals[wallInd].body, map.portals[map.portals[wallInd].pairInd].body);
			return ret;
		}
	}
	//If no valid solutions are found, return enemies position
	return e.position;
}

//Movement-Behavior function pointer: Just follows the path
//Param: Enemy by reference
//Return: None
//Notes: Moves the enemy to follow the path;
//		 Ex: Path = {A,B,C} enemy moves in order A, B, C, B, A, B...
void battlestate::enemymB1(enemy& e) {
	if (e.path.size() == 0) { return; } else if (e.path.size() == 1) {
		if ((e.path.front() - e.position).magnitude() > .05) { //.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
			move(e.path.front());
		}
		return;
	}
	if (e.ind == 0) {
		e.ind++;
		e.dir = true;
		e.move();
	} else if (e.ind == (e.path.size() - 1)) {
		e.ind--;
		e.dir = false;
		e.move();
	} else {
		e.ind += (e.dir ? 1 : -1);
		e.move();
	}
	return;
}


//Param: Enemy by reference
//Return: None
//Notes: Moves the enemy to follow the path in a loop fation;
//		 Ex: Path = {A,B,C} enemy moves in order A, B, C, A, B, C ...
void battlestate::enemymB1b(enemy& e) {
	if (e.path.size() == 0) { return; } else if (e.path.size() == 1) {
		if ((e.path.front() - e.position).magnitude() > .05) { //.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
			move(e.path.front());
		}
		return;
	}
	e.ind++;
	e.ind %= e.path.size();
	e.move();
	return;
}

//Param: Enemy by reference
//Return: None
//Notes: Enemy will shoot in one of the 8 cardinal and intercardinal directions
void battlestate::enemysBRand8(enemy& e) {
	int direction = rand() % 4;
	if (rand() % 2 == 0) {	//Shoot in a cardinal direction
		if (direction % 2 == 0) {
			e.aimAt(point(direction - 1.0f, 0.0f) + e.position);
		} else {
			e.aimAt(point(0.0f, direction - 2.0f) + e.position);
		}
	} else { //Shoot in a diagonal
		switch (direction) {
		case 0:
			e.aimAt(point(-1, -1) + e.position);
			break;
		case 1:
			e.aimAt(point(-1, 1) + e.position);
			break;
		case 2:
			e.aimAt(point(1, -1) + e.position);
			break;
		case 3:
			e.aimAt(point(1, 1) + e.position);
			break;
		}
	}
	spawnRay(e.shoot());
}

//Param: Enemy by reference
//Return: None
//Notes: Enemy will shoot if there are no walls in the way (Does not account for portals)
void battlestate::enemysB1(enemy& e) {	//Just shoots if there are no walls in the way of enemy and player
	bool shot = true;
	for (player& p : protags) {
		shot = true;
		for (wall& w : map.getWalls()) {
			segment s(p.position, e.position);
			if (isintersect(w.body, s)) {
				shot = false;
				break;
			}
		}
		if (shot) {
			spawnRay(e.shoot(metastat(clWhite.getLevel('r'), clWhite.getLevel('g'), clWhite.getLevel('b')), p.position));
			return;
		}
	}
	return;
}


//Param: Enemy by reference
//Return: None
//Notes: Uses the perfect recursive aim function 
void battlestate::enemysB4(enemy& e) {
	for (int i = 0; i < protags.size(); i++) {
		point aimDot = recursiveReflectiveAim(e, -1, 0, 3, e.position, metastat(clWhite.getLevel('r'), clWhite.getLevel('g'), clWhite.getLevel('b')));
		if (aimDot != e.position) {
			rays.size();
			spawnRay(e.shoot(metastat(255, 255, 255), aimDot));
			rays.size();
			return;
		}
	}
}

//Param: Enemy by reference
//Return: None
//Notes: Moves the enemy in some path(Unimplemented)
void battlestate::enemymB2(enemy& e) {

}

//Param: Enemy by reference
//Return: None
//Notes: Moves the enemy in some path(Unimplemented)
void battlestate::enemymB3(enemy& e) {

}

//Param: Enemy by reference
//Return: None
//Notes: Moves the enemy in some path(Unimplemented)
void battlestate::enemymB4(enemy& e) {

}

//Param: Time unit
//Return: None
//Notes: Iterates the enemy with their appropriate behaviors 
void battlestate::iterateAI(float inc) {
	for (enemy& e : antags) {
		if (e.moving) {
			point dire = (e.dest - e.position);
			if (dire.magnitude() < .05) {	//.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
				e.moving = false;
			} else {
				e.position += unitvector(dire) * 1.0f * inc;	//Future 1 is a speed multiplier, inc keeps it consistent with the number of frames being put out

				//enemysB4(e);

			}
			continue;		//Future remove return when we want enemies to be able to move and shoot at the same time
		} else {
			switch (e.moveB) {		//Picking a move behavior
			case 1:
				enemymB1(e);
				break;
			case 2:
				enemymB1b(e);
				break;
			case 3:
				enemymB2(e);
				break;
			case 4:
				enemymB3(e);
				break;
			default:
				break;
			}
			switch (e.shootB) {	//Picking a shoot behavior
			case 1:
				enemysB1(e);
				break;
			case 2:
				enemysBRand8(e);
				break;
			case 3:
				enemysB4(e);
				break;
			case 4:
				break;
			default:
				break;
			}
			continue;
		}
		//cerr << "ERROR: INVALID BEHAVIOR" << endl;
	}
}

//Param: time unit of imcrement
//Return: None
//Notes: Adds energy to player
void battlestate::iteratePlayer(float inc) {	//Used to iterate the players stats frame by frame
	for (auto& p : protags) {
		if (p.energy < p.energyCap) {
			p.energy += (10 * inc);	//FUTURE: update to match agility or speed of player
		}
	}
}


/*This function is called every frame during battle unless the battle is paused; even then, It may still be best
to call it and simply because certain animations might look cool cycling in the background when the battle is //DP: If u want to do that we should isolate the animation from movement
awaiting user input. All iterative battle behaviour and logic goes in here, or is called from in here.*/

//Param: time unit of increment
//Return: None
//Notes: iterates rays, ai, and players
void battlestate::iterate(float inc /*incremental time*/) {
	//Iterate Rays
	iterateRay(inc);

	//Iterate AI:
	iterateAI(inc);

	//Iterate players:
	iteratePlayer(inc);
}


//Param: Index for player
//Return: None
//Notes: Creates the spell that the player wants to create
void battlestate::playerAct(int playerInd) {
	const Spell* s = protags[playerInd].act();
	if (!s)
		return;
	switch (s->type) {
	case sRay: {
		spawnRay(ray(protags[playerInd].position + protags[playerInd].direction * .5, protags[playerInd].position + protags[playerInd].direction, *(s->r)));
		return;
	}
	case sWall: {
		segment seg(protags[playerInd].position, protags[playerInd].position + protags[playerInd].direction);
		seg = rotate90about(0, seg);
		seg.p1 += (seg.p1 - seg.p2);
		wall w(*(s->w), seg);
		constructWall(w);
		return;
	}
	case sPortal: {
		segment seg(protags[playerInd].position, protags[playerInd].position + protags[playerInd].direction);
		seg = rotate90about(0, seg);
		seg.p1 += (seg.p1 - seg.p2);
		int lastPortalInd = protags[playerInd].lastPortal;
		portal p(*(s->p), seg, lastPortalInd);
		//p.material.c
		map.portals.push_back(p);
		if (lastPortalInd == -1) {
			protags[playerInd].lastPortal = map.portals.size() - 1;
		} else {
			map.portals[lastPortalInd].pairInd = map.portals.size() - 1;
			protags[playerInd].lastPortal = -1;
		}
		return;
	}

	}
	//cout << "NOT ENOUGH ENERGY";
	return;
}
