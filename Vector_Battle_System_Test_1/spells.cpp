//Spells implementation file
#include "globals.h"
#include "spells.h"


// SPELL CLASS IMPLEMENTATION

Spell::Spell() {
	name = "";
	level = 0;
	description = "";
	category = 0;
	cost = 0;
	r = nullptr;
	w = nullptr;
	type = sNULL;

}

Spell::Spell(const rayConst& ra) {
	name = "";
	level = 0;
	description = "";
	category = 0;
	cost = 0;
	r = new rayConst(ra);
	w = nullptr;
	type = sRay;

}

Spell::Spell(const wallConst& wa) {
	name = "";
	level = 0;
	description = "";
	category = 0;
	cost = 0;
	r = nullptr;
	w = new wallConst(wa);
	type = sWall;
}

Spell::Spell(const Spell& s) {
	name = s.name;
	level = s.level;
	description = s.description;
	category = s.category;
	cost = s.cost;
	type = s.type;
	r = (s.r) ? new rayConst(*(s.r)) : nullptr;
	w = (s.w) ? new wallConst(*(s.w)) : nullptr;
}

Spell::~Spell() {
	delete r;
	delete w;
}

//inline int Spell::cast(battlestate& b) {
//	return cost;
//}
