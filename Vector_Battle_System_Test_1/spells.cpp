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
	p = nullptr;
	type = sNULL;
}

Spell::Spell(const rayConst& ra) {
	name = "";
	level = 0;
	description = "";
	category = 0;
	cost = 10;
	r = new rayConst(ra);
	w = nullptr;
	p = nullptr;
	type = sRay;

}

Spell::Spell(const wallConst& wa) {
	name = "";
	level = 0;
	description = "";
	category = 0;
	cost = 20;
	r = nullptr;
	w = new wallConst(wa);
	p = nullptr;
	type = sWall;
}

Spell::Spell(const portalConst& pC) {
	name = "";
	level = 0;
	description = "";
	category = 0;
	cost = 20;
	r = nullptr;
	w = nullptr;
	p = new portalConst(pC);
	type = sPortal;
}

//Assignment operator
Spell& Spell::operator=(const Spell& other) {
	delete r;
	delete w;
	name = other.name;
	level = other.level;
	description = other.description;
	category = other.category;
	cost = other.cost;
	type = other.type;
	r = (other.r) ? new rayConst(*(other.r)) : nullptr;
	w = (other.w) ? new wallConst(*(other.w)) : nullptr;
	p = (other.p) ? new portalConst(*(other.p)) : nullptr;
	return *this;
}

//Copy constructor
Spell::Spell(const Spell& s) {
	name = s.name;
	level = s.level;
	description = s.description;
	category = s.category;
	cost = s.cost;
	type = s.type;
	r = (s.r) ? new rayConst(*(s.r)) : nullptr;
	w = (s.w) ? new wallConst(*(s.w)) : nullptr;
	p = (s.p) ? new portalConst(*(s.p)) : nullptr;
}

Spell::~Spell() {
	delete r;
	delete w;
}

