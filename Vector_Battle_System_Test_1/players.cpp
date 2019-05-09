#include "globals.h"
#include "players.h"
#include "customGL.h"
#include "materials.h"

using namespace std;
using namespace fgr;

//Accessor for enumerated player stats
metastat& statblock::getStat(statNum which) {
	switch (which) {
	case stMaxHP:
		return maxHP;
	case stMaxFP:
		return maxFP;
	case stHP:
		return HP;
	case stFP:
		return FP;
	case stVitality:
		return vitality;
	case stSensitivity:
		return sensitivity;
	case stAgility:
		return agility;
	case stPower:
		return power;
	case stResilience:
		return resilience;
	case stStability:
		return stability;
	default:
		return HP;
	}
}

//Statblock default constructor
statblock::statblock() {

}

//Constuct a statblock from a text file
statblock::statblock(const std::string& filename) {
	std::ifstream source(filename);
	if (source.is_open()) {
		while (readline(source));
		source.close();
	}
	return;
}

//Interprets a string as an enumerated stat
statNum stringToStatNum(const std::string& word) {
	switch (word[0]) {
	case 'm':
		if (word == "maxHP")
			return stMaxHP;
		return stMaxFP;
	case 'H':
		return stHP;
	case 'F':
		return stFP;
	case 'v':
		return stVitality;
	case 'a':
		return stAgility;
	case 'p':
		return stPower;
	case 'r':
		return stResilience;
	case 's':
		if (word == "sensitivity")
			return stSensitivity;
		return stStability;
	default:
		return stNULL;
	}
}

//Read in a line from an input stream and set the appropriate stat
bool statblock::readline(std::istream& source) {
	std::string spec;
	if (source >> spec) {
		getStat(stringToStatNum(spec)) = metastat(source);
		return true;
	}
	return false;
}

//Combatant default constructor
combatant::combatant() {
	position = point(-1, -1);
	direction = point(1, 0);
	width = 0;
}

//Construct a combatant from a stat-text file
combatant::combatant(const std::string& statfile) : combatant() {
	stats = statblock(statfile);
}

//Changes direction based on angle(in radians)
void combatant::turn(float angle) {
	direction = unitfromangle(angle);
}

bool combatant::hit(const metastat& m, int level) {
	(stats.HP -= (m * ((float)level / 100)).bind()).bind();
	if (stats.HP == metastat(0,0,0)) {
		//TODO: have combatant removed
		return true;
	}
	return false;
}

//Enemy default constructor
enemy::enemy() : combatant() {

}

//Construct enemy from statblock file
enemy::enemy(const std::string& statfile) : combatant(statfile) {

}

//Add a waypoint to the enemy's motion path
void enemy::addWaypoint(const point& p, int i) { //Adds to path vector at index
	if (i == -1) { ind = 0;  return path.push_back(p); }
	path.insert(path.begin() + i, p);
}

//void enemy::act(battlestate& b) {	//invokes moving and shooting functions appropriately
//	if (moving) {
//		point dire = (dest - position);
//		if (dire.magnitude() < .05) {	//.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
//			moving = false;
//		} else {
//			position += unitvector(dire)*.02f;	//.02 is a speed multiplier
//		}
//		return;
//	} else if (moveB) {
//		invoke(moveB, *this, b);		//USING THIS BECAUSE BEHAVIOR IS SCOPED TO MEMBER FUNCTION
//		invoke(shootB, *this, b);
//		return;
//	}
//	cerr << "ERROR: INVALID BEHAVIOR" << endl;
//	return;
//}
//
////Movement-Behavior function pointer: Just follows the path
//void enemy::mB1(battlestate& b) {	
//	if (path.size() == 0) { return; } 
//	else if (path.size() == 1) { 
//		if ((path.front() - position).magnitude() > .05) { //.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
//			move(path.front());
//		}
//		return; 
//	}
//	if (ind == 0) {
//		ind++;
//		dir = true;
//		move(path[ind]);
//	} 
//	else if(ind == (path.size()-1)){
//		ind--;
//		dir = false;
//		move(path[ind]);
//	} 
//	else{
//		ind += (dir ? 1 : -1);
//		move(path[ind]);
//	}
//	return;
//}
//
////Similar to mB1, loops through the path
//void enemy::mB1b(battlestate& b) {
//	if (path.size() == 0) { return; } else if (path.size() == 1) {
//		if ((path.front() - position).magnitude() > .05) { //.05 can be decreased for more precise movement, or increased for more stable movement and prevent overshoot
//			move(path.front());
//		}
//		return;
//	}
//	ind++;
//	ind %= path.size();
//	move(path[ind]);
//	return;
//}
//
//void enemy::sBRand8(battlestate& b) {
//	int direction = rand() % 4;
//	if (rand() % 2 == 0) {	//Shoot in a cardinal direction
//		if (direction % 2 == 0) {
//			aimAt(point(direction - 1.0f, 0.0f) + position);
//		} else {
//			aimAt(point(0.0f, direction - 2.0f) + position);
//		}
//	} else { //Shoot in a diagonal
//		switch (direction) {
//		case 0:
//			aimAt(point(-1, -1) + position);
//			break;
//		case 1:
//			aimAt(point(-1, 1) + position);
//			break;
//		case 2:
//			aimAt(point(1, -1) + position);
//			break;
//		case 3:
//			aimAt(point(1,1) + position);
//			break;
//		}
//	}
//	shoot(b);
//}
//
////Shooting-behaviour function pointer: Just shoot each player if there are no walls in the way
//void enemy::sB1(battlestate& b) {	//Just shoots if there are no walls in the way of enemy and player
//	bool shot = true;
//	for (player& p : b.protags) {
//		shot = true;
//		for (wall& w : b.map.getWalls()) {
//			segment s(p.position, position);
//			if (isintersect(w.body, s)) {
//				shot = false;
//				break;
//			}
//		}
//		if (shot) {
//			shoot(clWhite, p.position,b);
//			return;
//		}
//	}
//	return;
//}
//
//
////Shooting-behavior function pointer: Makes simple use of the recursive-reflective aiming function
//void enemy::sB4(battlestate& b) {
//	//for (int i = 0; i < b.protags.size(); i++) {
//		point aimDot = recursiveReflectiveAim(b, -1, 0, 3, position, clWhite);
//		if (aimDot != position)
//			shoot(metastatfromID(0 + 1), aimDot, b);
//	//}
//}
//
//void enemy::mB2(battlestate& b) {
//
//}
//void enemy::mB3(battlestate& b) {
//
//}
//
//void enemy::mB4(battlestate& b) {
//
//}

//Recursive function to aim by bouncing off walls


void enemy::move(const point& dire) { //Sets the enemy to move to this point
	dest = dire;
	moving = true;
}

void enemy::move(int index){
	dest = path[index];
	moving = true;
}

void enemy::move(){
	dest = path[ind];
	moving = true;
}

void enemy::aimAt(const point& dire) {	//Sets the enemy to aim at this point
	aim = dire;
}

ray enemy::shoot() { //Shoots where aiming
	return shoot(clWhite, aim);
}

ray enemy::shoot(const metastat& col, const point& dire) { //Shoots at a point
	aim = dire;
	return ray(col,  unitvector(dire - position) * 0.6f + position, dire, 2.0f, 6.0f, 2);
	
}

//Player default constructor
player::player() : combatant() {
	energy = energyCap = 0;
	tog = false;
}

//Construct a player from a statblock text file
player::player(const std::string& statfile) : combatant(statfile) {
	energy = energyCap = 0;
	tog = false;
}

player::player(const std::string& filename, const fgr::point& pos, const graphic& sprit, float ang, bool toggle, float wid, float energ, float energCap): player(filename){
	position = pos;
	turn(ang);
	tog = toggle;
	sprite = sprit;
	width = wid;
	energy = energ;
	energyCap = energCap;
}



//
wall player::makeWall(int mat) const {
	segment s(position, position + direction);
	s = rotate90about(0, s);
	s.p1 += (s.p1-s.p2);
	return wall(s, mat, true);
}

ray player::shoot(const metastat & col) const {
	return ray(col, (position + direction*.3f), position + direction + direction, 2.0f, 6.0f, 2);
}

//Returns a pointer to the appropriate spell for this player to cast
const Spell* player::act() {
	for (int i = arsenal.size() - 1; i >= 0; i--) {
		if (arsenal[i].cost < energy) { 
			energy -= arsenal[i].cost; 
			return &arsenal[i]; 
		}
	}
	cout << "NOT ENOUGH ENERGY";
	return nullptr;
}


void player::toggle() { //Flips tog
	for (shape& sh : sprite) { sh.lineThickness = (tog ? 1.0f : 2.0f); }
	tog = !tog;
}


int checkcollision(const ray& r, const combatant& c) {
	float dist0 = (r.bits[0] - c.position).magnitude();
	float dist1 = (r.bits[1] - c.position).magnitude();
	if (dist0 < c.width / 2) { return 1; } //.25 NEEDS TO BE CHANGED IF PLAYER SIZE CHANGES
	if (dist1 < c.width / 2) { return 2; }
	return 0;
}