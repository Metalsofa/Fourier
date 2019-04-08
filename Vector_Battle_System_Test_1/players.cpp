#include "globals.h"
#include "players.h"
#include "customGL.h"
#include "materials.h"

using namespace std;
using namespace fgr;

void enemy::init(int m, int s) {
	srand(unsigned int(time(NULL)));
	ind = 0;
	dir = true;
	moving = false;
	switch (m) {		//Picking a move behavior
	case 1:
		moveB = &enemy::mB1;
		break;
	case 2:
		moveB = &enemy::mB2;
		break;
	case 3:
		moveB = &enemy::mB3;
		break;
	case 4:
		moveB = &enemy::mB4;
		break;
	default:
		moveB = nullptr;
	}
	switch (s) {	//Picking a shoot behavior
	case 1:
		shootB = &enemy::sB1;
		break;
	case 2:
		shootB = &enemy::sB1;
		break;
	case 3:
		shootB = &enemy::sB1;
		break;
	case 4:
		shootB = &enemy::sB4;
		break;
	default:
		shootB = nullptr;
	}
}

void enemy::addWaypoint(const point& p, int i) { //Adds to path vector at index
	if (i == -1) { ind = 0;  return path.push_back(p); }
	path.insert(path.begin() + i, p);
}

void enemy::act(battlestate& b) {	//invokes moving and shooting functions appropriately
	if (moving) {
		point dire = (dest - position);
		if (dire.magnitude() < .05) {	//.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
			moving = false;
		} else {
			position += unitvector(dire)*.02f;	//.02 is a speed multiplier
		}
		return;
	} else if (moveB) {
		invoke(moveB, *this, b);		//USING THIS BECAUSE BEHAVIOR IS SCOPED TO MEMBER FUNCTION
		invoke(shootB, *this, b);
		return;
	}
	cerr << "ERROR: INVALID BEHAVIOR" << endl;
	return;
}

//Movement-Behavior function pointer: Just follows the path
void enemy::mB1(battlestate& b) {	
	if (path.size() == 0) { return; } 
	else if (path.size() == 1) { 
		if ((path.front() - position).magnitude() > .05) { //.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
			move(path.front());
		}
		return; 
	}
	if (ind == 0) {
		ind++;
		dir = true;
		move(path[ind]);
	} 
	else if(ind == (path.size()-1)){
		ind--;
		dir = false;
		move(path[ind]);
	} 
	else{
		ind += (dir ? 1 : -1);
		move(path[ind]);
	}
	return;
}

//Similar to mB1, loops through the path
void enemy::mB1b(battlestate& b) {
	if (path.size() == 0) { return; } else if (path.size() == 1) {
		if ((path.front() - position).magnitude() > .05) { //.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
			move(path.front());
		}
		return;
	}
	ind++;
	ind %= path.size();
	move(path[ind]);
	return;
}

void enemy::sBRand8(battlestate& b) {
	int direction = rand() % 4;
	if (rand() % 2 == 0) {	//Shoot in a cardinal direction
		if (direction % 2 == 0) {
			aimAt(point(direction - 1, 0) + position);
		} else {
			aimAt(point(0, direction - 2) + position);
		}
	} else { //Shoot in a diagonal
		switch (direction) {
		case 0:
			aimAt(point(-1, -1) + position);
			break;
		case 1:
			aimAt(point(-1, 1) + position);
			break;
		case 2:
			aimAt(point(1, -1) + position);
			break;
		case 3:
			aimAt(point(1,1) + position);
			break;
		}
	}
	shoot(b);
}

//Shooting-behaviour function pointer: Just shoot each player if there are no walls in the way
void enemy::sB1(battlestate& b) {	//Just shoots if there are no walls in the way of enemy and player
	bool shot = true;
	for (player& p : b.protags) {
		shot = true;
		for (wall& w : b.map.getWalls()) {
			segment s(p.position, position);
			if (isintersect(w.body, s)) {
				shot = false;
				break;
			}
		}
		if (shot) {
			shoot(clWhite, p.position,b);
			return;
		}
	}
	return;
}


//Shooting-behavior function pointer: Makes simple use of the recursive-reflective aiming function
void enemy::sB4(battlestate& b) {
	//for (int i = 0; i < b.protags.size(); i++) {
		point aimDot = recursiveReflectiveAim(b, -1, 0, 3, position, clWhite);
		if (aimDot != position)
			shoot(metastatfromID(0 + 1), aimDot, b);
	//}
}

void enemy::mB2(battlestate& b) {

}
void enemy::mB3(battlestate& b) {

}

void enemy::mB4(battlestate& b) {

}

//Recursive function to aim by bouncing off walls
point enemy::recursiveReflectiveAim(battlestate& b, int wallInd, int playerInd, int depth, point pos, const metastat& shotColor) {
	//If there are no walls, just take a straight shot for the player
	if (!b.map.getWalls().size()) {
		return b.protags[playerInd].position;
	}
	//Return [invalid point] if no potential paths have been found
	bool breakP = false;
	if (depth == -1) { return position; }
	//If not at the base case
	if (wallInd != -1) {
		//Draw a line from here to the target
		segment s(b.protags[playerInd].position, pos);
		//Check if this line intersects the given wall
		if (isintersect(b.map.getWall(wallInd).body, s)) {
			//Draw a line from the intersection to the target
			segment trace(intersection(b.map.getWall(wallInd).body, s), s.p1);
			//Check that no other walls intersect with this segment
			bool reCurse = false;
			for (int i = 0; i < b.map.getWalls().size(); i++) {
				if (i != wallInd) {
					if (isintersect(trace, b.map.getWall(i).body)) {
						reCurse = true;
						break;
					}
				}
			}
			//If the recursion flag has been set to false, return
			if (!reCurse) {
				return intersection(b.map.getWall(wallInd).body, s);
			}
		}
	}
	//Check every other wall
	for (int i = 0; i < b.map.getWalls().size(); i++) {
		//Make sure not to twice consider this wall
		if (wallInd != i && permitted(shotColor, b.map.getWall(i).material.getPermittivitySpells())) {
			//Check if the considered wall is visable
			segment segIa(pos,b.map.getWall(i).body.p1);
			segment segIb(pos, b.map.getWall(i).body.p2);
			bool contin = false;
			for (int j = 0; j < b.map.getWalls().size(); j++) {
				if (j != i && isintersect(segIa,b.map.getWall(j).body) && isintersect(segIb, b.map.getWall(j).body)){
					contin = true;
					break;
				}
			}
			if (contin) { continue; }

			//Recall this function on walls[i], after reflecting 'pos' across that wall
			point reticle(recursiveReflectiveAim(b, i, playerInd, depth - 1, reflection(pos, b.map.getWall(i).body), shotColor));
			//Continue if nothing valid is found
			if (reticle == position)
				continue;
			//Draw a line from here to the target
			segment trace(reticle, pos);
			segment s(intersection(trace, b.map.getWall(i).body), (wallInd == -1
					? pos 
					: intersection(trace, b.map.getWall(wallInd).body)
				)
			);
			bool cont = false;
			for (int j = 0; j < b.map.getWalls().size(); j++) {
				if (j != i && j != wallInd && isintersect(s, b.map.getWall(j).body)) {
					cont = true;
					break;
				}
				//Check if this line intersects the given wall
				else if (j == wallInd && wallInd != -1 && !isintersect(b.map.getWall(wallInd).body, trace)) {
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
			return reflection(reticle, b.map.getWall(wallInd).body);
		}
	}
	//If no valid solutions are found, return -1, -1
	return position;
}

void enemy::move(const point& dire) { //Sets the enemy to move to this point
	dest = dire;
	moving = true;
}

void enemy::aimAt(const point& dire) {	//Sets the enemy to aim at this point
	aim = dire;
}

void enemy::shoot(battlestate& b) { //Shoots where aiming
	shoot(clWhite, aim, b);
}

void enemy::shoot(const metastat& col, const point& dire, battlestate& b) { //Shoots at a point
	ray newRay(col,  unitvector(dire - position) * 0.6f + position, dire, 2.0f,
		6.0f, 2);
	b.spawnRay(newRay);
}

void player::makeWall(int mat, battlestate & b) {
	segment s(position, position + direction);
	s = rotate90about(0, s);
	//s.p1 += (s.p1-s.p2);
	wall a(s, mat, true);
	b.constructWall(a);
}

void player::shoot(const metastat & col, battlestate & b) {
	ray newRay(col, (position + direction*.3f), position + direction + direction, 2.0f,
		6.0f, 2);
	b.spawnRay(newRay);
}
