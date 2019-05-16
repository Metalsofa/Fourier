//Spells implementation file
#include "globals.h"
#include "spellCore.h"

wallConst::wallConst() {
	fixed = false;
}
wallConst::wallConst(const materialtype& m, bool f, int s = 0) : material(m), fixed(f), shape(s) {}
wallConst::wallConst(int wallmaterial, bool isfixed, int s = 0): fixed(isfixed), shape(s) {
	materialtype definingMatarial((Material)wallmaterial);
	material = definingMatarial;
}
wallConst::wallConst(const wallConst& w) : fixed(w.fixed), shape(w.shape), material(w.material) {}

///////////////////////////////////////////////////////////
//
//           Class wall DEFINITION
//
//				This class structures the properties
//				of a given wall.
//
///////////////////////////////////////////////////////////



//Know-it-all constructor
wall::wall(const segment& definingsegment, const Material& wallmaterial, bool isfixed) {
	body = definingsegment;
	materialtype definingMatarial(wallmaterial);
	material = definingMatarial;
	fixed = isfixed;
}

wall::wall(segment definingsegment, int wallmaterial, bool isfixed) {
	body = definingsegment;
	materialtype definingMatarial((Material)wallmaterial);
	material = definingMatarial;
	fixed = isfixed;
}

//Default constructor
wall::wall() {
	fixed = false;
}

wall::wall(const wall& w) {
	body = w.body;
	material = w.material;
	fixed = w.fixed;
	shape = w.shape;
}
wall::wall(const wallConst& w, const segment& s) {
	body = segment(s);
	material = w.material;
	fixed = w.fixed;
	shape = w.shape;
}

//Check if a spell of a given color can pass through a given material
int permitted(const metastat& spellColor, const metastat& permittivity) { //DP: Pass by ref
	//These 3 lines of code return 0 (for 'kill') if any component of the ray is not permitted.
	if (permittivity.som == 0 && spellColor.som) return 0;
	if (permittivity.emo == 0 && spellColor.emo) return 0;
	if (permittivity.cog == 0 && spellColor.cog) return 0;
	//These 3 lines of cour return 1 (for 'bounce') if any component of the ray is to be bounced.
	if (permittivity.som == 1 && spellColor.som) return 1;
	if (permittivity.emo == 1 && spellColor.emo) return 1;
	if (permittivity.cog == 1 && spellColor.cog) return 1;
	//If no return has happened by now, the ray is permitted by this permittivity.
	return 2;
}



///////////////////////////////////////////////////////////
//
//           Class Portal DEFINITION
//
//				Structures information about portals
//				on the battlefield.	
//
///////////////////////////////////////////////////////////

portal::portal(const segment& definingsegment, Material mat, portal* partner = nullptr) {
	body = definingsegment;
	material = mat;
	pair = partner;
}

portal::portal(const segment& definingsegment, int wallmaterial, portal* partner = nullptr) {
	body = definingsegment;
	material = (Material)wallmaterial;
	pair = partner;
}
portal::portal() {
	pair = nullptr;
}
portal::portal(const portal& p) {
	body = p.body;
	material = p.material;
	pair = p.pair;
}

portal::portal(const portalConst& pc, const segment& s, portal* partner = nullptr) {
	body = s;
	material = pc.material;
	pair = partner;
}





rayConst::rayConst(const metastat& c = metastat(255,255,255), float len = 1, float spd = 1, float thck = 1) 
	: color(c), nominalLength(len), speed(spd), thickness(thck){}
rayConst::rayConst(const rayConst& r) 
	: color(r.color), nominalLength(r.nominalLength), speed(r.speed), thickness(r.thickness) {}

///////////////////////////////////////////////////////////
//
//           Class ray DEFINITION
//
//				Child class of projectile, meant to look and
//				act like rays of light.
//
///////////////////////////////////////////////////////////

//move: Spell Object that can be part of a player's known spells (called a function in game)
	///string name
	///int level
	///int category (which of  he 12 categories it falls into)
	///string description
	///int

ray::ray() {
}

ray::ray(const ray& r) {
	bits = r.bits;
	disjoints = r.disjoints;
	color = r.color;
	kindness = r.kindness;
	direction = r.direction;
	speed = r.speed;
	nominalLength = r.nominalLength;
	killme = r.killme;
	terminating = r.terminating;
	thickness = r.thickness;
	terminalpoint = r.terminalpoint;
}

//Constructor for some custom ray
ray::ray(metastat col, point location, point heading, float leng, float fastness, float thickn) { //DP: Pass by ref
	bits.push_back(location);
	bits.push_back(location);
	//DP: bits = {location, location};
	direction = unitvector(difference(heading, location));
	speed = fastness;
	nominalLength = leng;
	thickness = thickn;
	color = col;
}
ray::ray(point loc, point head, const rayConst& r) {
	bits.push_back(loc);
	bits.push_back(loc);
	direction = unitvector(difference(head, loc));
	speed = r.speed;
	nominalLength = r.nominalLength;
	thickness = r.thickness;
	color = r.color;
}

//Constructor that accepts a spell and returns the appropriate ray //maybee
///Doesn't exist yet

//Returns the vector representing the ray's velocity
point ray::movevector() const {
	if (terminating)
		return scalarproduct(unitvector(difference(terminalpoint, bits[0])), speed);
	else
		return scalarproduct(unitvector(direction), speed);
	//DP: return scalarproduct(unitvector((
	//	(terminating)
	//		?difference(terminalpoint, bits[0])
	//		:direction)), 
	//	speed);
}

//Returns the actual length of the vector, calculated by counting individual segment lengths
float ray::length() const {
	float cumulative = 0;
	int i = 0;
	while (i < int(bits.size()) - 1) {
		point p1 = bits[i];
		point p2 = bits[i + 1];
		segment meas(p1, p2);
		cumulative += meas.length();
		i += 1;
	}
	//DP:
	//for (int i = 0; i < int(bits.size()) - 1; i++) {
	//	cumulative += segment(bits[i], bits[i + 1]);
	//}
	return cumulative;
}

//Returns the length that this vector is supposed to have according to its definition
float ray::lengthNominal() const { return nominalLength; }

//Advance the motion of the ray in accordance to its properties
void ray::advance(float inc /*Incremental Time*/) {
	if (!terminating) {
		point frontarb = combine(bits[0], scalarproduct(movevector(), inc));
		point unitback = unitvector(difference(bits[bits.size() - 2], bits.back()));
		bits[0] = frontarb;
		if (length() >= nominalLength)
			bits.back() = combine(bits.back(), scalarproduct(unitback, inc * speed));
	}
	else {
		point frontarb = combine(bits[0], scalarproduct(movevector(), inc));
		if (difference(terminalpoint, frontarb).magnitude() > difference(terminalpoint, bits[0]).magnitude())
			frontarb = terminalpoint;
		point diff = difference(bits[bits.size() - 2], bits.back());
		point unitback = unitvector(diff);
		point moveback = scalarproduct(unitback, inc * speed);
		if (speed * inc >= difference(terminalpoint, bits.back()).magnitude())
			bits.back() = terminalpoint;
		else
			bits.back() = combine(bits.back(), moveback);
		bits[0] = frontarb;
		nominalLength = length();
		if (converges(bits[0], bits.back()) && converges(bits[0], terminalpoint))
			killme = true;
	}
	//Terminate the last point if it catches up
	float forgiveness = 0.000005f;
	if (difference(bits.back(), bits[bits.size() - 2]).magnitude() < speed * inc && bits.size() > 2) {
		bits.pop_back();
	}
}

bool ray::gotRed() const { return color.som != 0; } //Bool: Is there a RED component to this ray?
bool ray::gotGreen() const { return color.emo != 0; } //Bool: Is there a GREEN component to this ray?
bool ray::gotBlue() const { return color.cog != 0; } //Bool: Is there a BLUE component to this ray?
bool ray::deathtime() const { return killme; }

//Tell the ray it is now terminating, and tell it where to terminate.
void ray::terminate(const point& where) { //DP: Pass by ref
	terminating = true;
	terminalpoint = where;
}


bool ray::checkcollision(const segment & surface) const {
	return isintersect(surface,segment(bits[0], bits[1])) == 1; //&& intersection(segment(bits[0],bits[1]),surface) != bits[1];
}
point ray::wherehit(const segment& surface) const {
	return intersection(segment(bits[0], bits[1]), surface);
}
//Returns 0 (kill) 1 (bounce) or 2 (permit) based on this ray's compatability with a given material.
int ray::permitted(const metastat & permittivity) const {
	//These 3 lines of code return 0 (for 'kill') if any component of the ray is not permitted.
	if ((permittivity.som == 0 && gotRed()) ||
		(permittivity.emo == 0 && gotGreen()) ||
		(permittivity.cog == 0 && gotBlue())) return 0;
	//These 3 lines of cour return 1 (for 'bounce') if any component of the ray is to be bounced.
	else if ((permittivity.som == 1 && gotRed()) ||
		(permittivity.emo == 1 && gotGreen()) ||
		(permittivity.cog == 1 && gotBlue())) return 1;
	//If no return has happened by now, the ray is permitted by this permittivity.
	return 2;
}

//Doublechecks that there is a collision, then causes the ray to bounce off of the given surface
void ray::bounce(const segment& surface) {
	bool shouldbounce = true;
	segment frontseg(bits[0], bits[1]);
	point ints = intersection(frontseg, surface);
	point intdiff = difference(bits[1], ints);
	float intdist = intdiff.magnitude();
	float errr = (bits.size() == 2)? 0.01f : 0.00f;
	if (isintersect(frontseg, surface) == 1 && intdist > errr) {
		bits.insert((bits.begin() + 1), intersection(frontseg, surface));
		bits[0] = reflection(bits[0], surface);	//Why bits[0]?
		segment surface2;
		surface2.p1 = difference(surface.p1, surface.midpoint());
		surface2.p2 = difference(surface.p2, surface.midpoint());
		direction = reflection(direction, surface2);
	}
}

//Returns the thickness that glut should be using to render this ray.
float ray::getthickness() const { return thickness; }
float ray::getRed() const { return float(color.som) / 255.0f; } //Returns float from 0 to 1, for glut's sake
float ray::getGreen() const { return float(color.emo) / 255.0f; } //Returns float from 0 to 1, for glut's sake
float ray::getBlue() const { return float(color.cog) / 255.0f; } //Returns float from 0 to 1, for glut's sake

//Returns a read-only copy of the vector containing every 'bit' (point) of this ray
const vector<point>& ray::getbits() const {
	return bits;
}


