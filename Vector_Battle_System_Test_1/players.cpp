#include "globals.h"
#include "players.h"

using namespace std;

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
		shootB = &enemy::sB1;
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

void enemy::mB1(battlestate& b) {	//Just follows the path 
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
void enemy::sB1(battlestate& b) {	//Just shoots if there are no walls in the way of enemy and player
	bool shot = true;
	for (player& p : b.protags) {
		shot = true;
		for (wall& w : b.map.getWalls()) {				//Currently commented out due to not being able to recognize currentbattle, ray, etc.
			segment s(p.position, position);
			if (isintersect(w.body, s)) {
				shot = false;
				break;
			}
		}
		if (shot) {
			shoot(p.position,b);
			return;
		}
	}
	return;
}
void enemy::mB2(battlestate& b) {

}
void enemy::mB3(battlestate& b) {

}
void enemy::mB4(battlestate& b) {

}

//Recursive function to aim by bouncing off walls
point enemy::advancedAim(battlestate& b, int wallInd, int playerInd, int depth, point pos) {
	if (depth = 0) { return point(-1,-1); }
	segment s(b.protags[playerInd].position, pos);
	if (isintersect(b.map.getWall(wallInd).body, s)) {
		return intersection(b.map.getWall(wallInd).body, s);
	}
	for (int i = 0; i < b.map.getWalls().size(); i++) {
		if (wallInd != i) {
			//return advancedAim(b, i, playerInd, depth - 1, reflect(pos, b.map.getWall(i)));	//Write reflect function
		}
	}

}

void enemy::move(const point& dire) { //Sets the enemy to move to this point
	dest = dire;
	moving = true;
}

void enemy::aimAt(const point& dire) {	//Sets the enemy to aim at this point
	aim = dire;
}

void enemy::shoot(battlestate& b) { //Shoots where aiming
	shoot(aim, b);
}

void enemy::shoot(const point& dire, battlestate& b) { //Shoots at a point
	ray newRay(clWhite, (dire - position)* .1 + position, dire, 2.0f,		//Currently commented out due to not being able to recognize currentbattle, ray, etc.
		6.0f, 2);
	b.spawnRay(newRay);
}

