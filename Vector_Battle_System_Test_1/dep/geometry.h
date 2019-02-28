//We'll include things like triangles and other fun geometric functions in this one.
#pragma once
#ifndef __geometry_h
#define __geometry_h
#include <cmath>
#include <string>
#include <vector>
using namespace std;

const float  PI = 3.14159265358979f;

template <class number>
number mean(number &num1, number &num2) {
	//DP: Why not:
	return (num1 + num2) / 2;
	//It would save the creation of 1 var every time u call this function
	//DM: Lol yeah 
	///Implemented suggested fix 1/31/1/19
}

float pyth(float &a, float &b) { //DP: Again, no need to create a var DM: K
	return sqrt(pow(a, 2) + pow(b, 2));
}

float pyth(const float &a, const float &b) { //DP: Again, no need to create a var DM: K
	return sqrt(pow(a, 2) + pow(b, 2));
}


//Boolean for whether X lies between A and B. Think A < X < B.
bool between(float A, float X, float B) {
	//return ((A < X && X < B) || (B < X && X < A));
	//or...
	return (fminf(A,B) < X && X < fmaxf(A,B)); 
	//The second one looks better, but the first one might be barely faster due to a function call
}

class point {
private:
	//Remembers its value to make some calculations faster
	float angCache;
	float magCache;
	float xCache;
	float yCache;
	//Checks if the cache-values for x and y are up-to-date, updates them if not
	void updateCache() {
		if (xCache != x || yCache != y) {
			xCache = x;
			yCache = y;
			angCache = atan2(y, x);
			magCache = pyth(x, y);
		}
	}
public:
	float x;
	float y;
	point(float xpos, float ypos) {
		x = xpos;
		y = ypos;
	}
	point() {
		x = 0;
		y = 0;
	}
	//Time to overload some operators!
	// += overload
	point& operator +=(const point& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	// + overload
	friend point operator +(point lhs, const point& rhs) {
		lhs += rhs;
		return lhs;
	}
	// -= overload
	point& operator-= (const point& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	// - overload
	friend point operator- (point lhs, const point& rhs) {
		lhs -= rhs;
		return lhs;
	}
	// *= overload
	point& operator*= (const float& rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}
	// * overload
	friend point operator* (point lhs, const float& rhs) {
		lhs *= rhs;
		return lhs;
	}
	float magnitude() {
		updateCache();
		return magCache;
	}
	//The angle formed by this point's vector from the origin with respect to the x-axis
	float angle() {
		updateCache();
		return angCache;
	}
	//Returns a nice label for this point for debug/gui purposes
	string label() {
		return "(" + to_string(x) + "," + to_string(y) + ")";
	}
};

bool converges(point PointA, point PointB) { //DP: Overwrite == and != for a point, but actually it might be already overloaded to do what ur doing, but I can't find anything online about it
	if (PointA.x == PointB.x && PointA.y == PointB.y)
		return true;
	else
		return false;
}

point scalarproduct(point &pointe, float &coefficient) { //DP: Again, no need to create a var
	return pointe * coefficient;
}

point scalarproduct(const point &pointe, const float &coefficient) {//DP: Again, no need to create a var
	return pointe * coefficient;
}

float dotproduct(point r1, point r2) {
	return (r1.x * r2.x + r1.y * r2.y);
}

//Returns a float, since this product will by definition be in the z-direction
float flatcrossproduct(point r1, point r2) { //DP: Again, no need to create a var DM: K
	return r1.x * r2.y - r1.y * r2.x;
}

point unitvector(point po) {//DP: Again, no need to create a var
	if (po.magnitude() == 0)
		return point(1.0f, 0.0f);
	return scalarproduct(po, ( 1 / po.magnitude() ) );
	//DP: Like below
	// return scalarproduct(po, (1/po.magnitude())); //THanks implemented :>
}

//Returns the unit vector corresponding to an angle in radians.
point unitfromangle(float &angle) {//DP: Again, no need to create a var
	point unit;
	unit.x = cos(angle);
	unit.y = sin(angle);
	unit = unitvector(unit); //Just to be sure.
	return unit;
}

point combine(point &point1, point &point2) {//DP: Again, no need to create a var
	point comb;
	comb.x = point1.x + point2.x;
	comb.y = point1.y + point2.y;
	return comb;
}

point combine(const point &point1, const point &point2) {//DP: Again, no need to create a var 
	point comb;
	comb.x = point1.x + point2.x;
	comb.y = point1.y + point2.y;
	return comb;
}

//Point 1 - Point 2
point difference(point& point1, point& point2) {//DP: Again, no need to create a var
	return point1 - point2;
}
point difference(const point& point1, const point& point2) {//DP: Again, no need to create a var
	return point1 - point2;
}

//DP: I don't get what the point of the const versions from above are, but just overload the +, -, +=, and -= operator;
//DM: Yeah I wrote these functions long before I learned how to overload operators. I'll implement them now.

point rotate90(point poi) {//DP: Again, no need to create a var
	point pot(0 - poi.y, poi.x);
	return pot;
}

point pointSum(vector<point> &points) {
	point result = point(0.0f,0.0f);
	for (point bit : points) {
		result = combine(result, bit);
	}
	return result;
}

//Accepts two points and an angle, returns the first point rotated by the specified angle about the second.
point rotateabout(point &arg, point &axis, float &angle) {
	arg = difference(arg, axis);
	float theta1 = atan2(arg.y, arg.x);
	float length = arg.magnitude();
	float theta2 = theta1 + angle;
	float x2 = length * cos(theta2);
	float y2 = length * sin(theta2);
	point newp(x2, y2);
	newp = combine(newp, axis);
	return newp;
}

class segment {
public:
	point p1;
	point p2;
	void define(point& point1, point& point2) { //DP: Why not create constructors? + Pass by ref
		p1 = point1;
		p2 = point2;
	}
	const point midpoint() const {//DP: Again, no need to create a var
		return point(mean(p1.x,p2.x),mean(p1.y,p2.y));
	}
	float length() {
		float leng;
		leng = pyth(p1.x - p2.x, p1.y - p2.y);
		return leng;
	}
	//Height of box bounded by this segment. Can be negative.
	const float height() const {
		return (p2.y - p1.y);
	}
	//Width of box bounded by this segment. Can be negative.
	const float width() const {
		return (p2.x - p1.x);
	}
	segment() {
		p1 = point(0.0f,0.0f);
		p2 = point(0.0f,0.0f);
	}
	segment(float x1, float y1, float x2, float y2) {
		p1 = point(x1, y1);
		p2 = point(x2, y2);
	}
	segment(point P1, point P2) { //DP: Pass by ref?
		p1 = P1;
		p2 = P2;
	}
};

//DP: If you don't use the defines outside of the class, there is no point in them, as they are redundant, but if you want to use them outside the class, why aren't the constructors calling them?
//DM: Because, if you'll believe it, I didn't know what a 'constructor' was when I made these functions back in november, then I implemented some when I learned about them. But yeah it would be best to cut them out now. I'm trying to clean it up now

segment rotate90about(int pointID, segment& seg) { //DP: pass by ref 
	segment nseg; // segment nseg = (pointID == 1)?seg:segment(seg.p2, seg.p1);
	if (pointID == 1) //DP: ALWAYS TRUE???? DM:CHanged...
		nseg = seg;
	else
		nseg.define(seg.p2, seg.p1);
	//point analog; //UNcomment if fermatpoint stops working
	//analog.x = nseg.p2.x - nseg.p1.x; //UNcomment if fermatpoint stops working
	//analog.y = nseg.p2.y - nseg.p1.y; //UNcomment if fermatpoint stops working
	point analog = nseg.p2 - nseg.p1; //comment OUT if fermatpoint stops working
	analog = rotate90(analog);
	nseg.p2 = combine(nseg.p1, analog);
	return nseg;


}

segment equilateralBisector(segment seg) { //Bisector protrudes from left, if p1 is bottom and p2 is top DP: pass by ref 
	segment perp;
	perp.p1 = seg.midpoint();
	point diffp = difference(perp.p2, perp.p1);
	perp.p2 = combine(perp.p1, scalarproduct(unitvector(difference(rotate90about(1, seg).p2, seg.p1)), sqrt(3.0f) / 2));
	return perp;
}


point intersection(segment& sega, segment& segb) { //Figured this out using Cramer's Rule DP: pass by ref 
	float dxa = sega.p2.x - sega.p1.x;
	float dxb = segb.p2.x - segb.p1.x;
	float dya = sega.p2.y - sega.p1.y;
	float dyb = segb.p2.y - segb.p1.y;
	float xa = sega.p1.x;
	float xb = segb.p1.x;
	float ya = sega.p1.y;
	float yb = segb.p1.y;
	float pa = dxa * ya - dya * xa;
	float pb = dxb * yb - dyb * xb;
	float det = dya * dxb - dyb * dxa;
	float dety = dya * pb - dyb * pa;
	float detx = dxa * pb - dxb * pa;
	float Y = dety / det;
	float X = detx / det;
	point solution(X, Y);
	return solution;
}

//This is showing some REALLY weird behavior when one of the segments is perfectly vertical or horizontal
bool isintersect(segment& sega, segment& segb) { //DP: Pass by reference?
	point ints = intersection(sega, segb);
	bool eval = true;
	bool xaeval = (ints.x < sega.p1.x) == (ints.x > sega.p2.x);
	if (sega.p1.x == sega.p2.x)
		xaeval = false;
	bool xbeval = (ints.x < segb.p1.x) == (ints.x > segb.p2.x);
	if (segb.p1.x == segb.p2.x)
		xbeval = false;
	bool yaeval = (ints.y < sega.p1.y) == (ints.y > sega.p2.y);
	if (sega.p1.y == sega.p2.y)
		yaeval = false;
	bool ybeval = (ints.y < segb.p1.y) == (ints.y > segb.p2.y);
	if (segb.p1.y == segb.p2.y)
		ybeval = false;
	if (!xaeval && !yaeval)
		eval = false;
	if (!xbeval && !ybeval)
		eval = false;
	//if (!yaeval)
	//	eval = false;
	//if (!ybeval)
	//	eval = false;
	/*if (eval) { 
		cerr << "SOMETHING IS WRONG" << endl
			<< (eval? "TRUE" : "FALSE") << endl
			<< ((onSegment(sega.p1, ints, sega.p2) && onSegment(segb.p1, ints, segb.p2)) ? "TRUE" : "FALSE") << endl
			<< "Seg 1: (" << sega.p1.x << ", " << sega.p1.y << "), ("  << sega.p2.x << ", " << sega.p2.y << ")" << endl
			<< "Seg 2: (" << segb.p1.x << ", " << segb.p1.y << "), (" << segb.p2.x << ", " << segb.p2.y << ")" << endl;
		exit(1);
	}*/
	return eval;
}



bool isperpintersect(point p, segment v) {//DP: pass by ref 
	point diffVp = difference(v.p1, p);
	point diffVv = difference(v.p2, v.p1);
	float numerator = dotproduct(diffVp, diffVv);
	float denominator = dotproduct(diffVv, diffVv);
	float t = -numerator / denominator;
	if (0 <= t && t <= 1)
		return true;
	else
		return false;
	//DP: return  (0 <= t && t <= 1);
}

float distancetoline(point p, segment v) { //DP: pass by ref 
	point diffPv = difference(v.p1, p);
	point diffVv = difference(v.p2, v.p1);
	float numerator = abs(flatcrossproduct(diffVv, diffPv));
	float denominator = diffVv.magnitude();
	float distance = numerator / denominator;
	return distance;
}

//Returns a position vector representing the shortest path from a point to a segment
float distancetoseg(point dot, segment seg) { //DP: pass by ref 
	//First we see if the perpendicular line to the segment even hits it.
	if (!isperpintersect(dot, seg)) {
		point r1 = difference(dot, seg.p1);
		point r2 = difference(dot, seg.p2);
		float shortest = (r1.magnitude() < r2.magnitude()) ? r1.magnitude() : r2.magnitude();
		return shortest;
		//DP: conscise version:
		//return min(difference(dot, seg.p1).magnitude(), difference(dot, seg.p2).magnitude());
	}
	//Otherwise we find the perpendicular path to the segment
	float r = distancetoline(dot, seg);
	return r;
}

//Returns the given point after refection about an axis defined by a given segment
point reflection(point &dot, segment &mirror) { //DP: Don't have to create all these vars
	point transform = mirror.midpoint(); //Bring the process to the origin
	dot = difference(dot, transform);
	point mir = difference(mirror.p1,transform);
	float dang = mir.angle() - dot.angle();
	float nang = dot.angle() + dang * 2;
	float nx = dot.magnitude() * cos(nang);
	float ny = dot.magnitude() * sin(nang);
	point reflect(nx, ny);
	reflect = combine(reflect, transform);
	return reflect;
}

/*Returns the appropriate reflective bisector at a given intersection that reflects an incedent ray as if
it reflected at that corner, given a point that lies on the incedent ray.*/
segment reflectiveBisector(point &dot, segment &sega, segment &segb) {
	point corner = intersection(sega, segb);
	point A = sega.p1;
	point B = segb.p1;
	point C = dot;
	A = difference(A, corner);
	B = difference(B, corner);
	C = difference(C, corner);
	A = unitvector(A);
	B = unitvector(B);
	C = unitvector(C);
	float angleA = A.angle();
	if (angleA < 0)
		angleA += PI * 2.0f;
	if (angleA > PI)
		angleA -= PI;
	float angleB = B.angle();
	if (angleB < 0)
		angleB += PI * 2.0f;
	if (angleB > PI)
		angleB -= PI;
	float angleC = C.angle();
	if (angleC < 0)
		angleC += PI * 2.0f;
	if (angleC > PI)
		angleC -= PI;
	float bisectorAngle1 = mean(angleA, angleB);
	bool useAngle1 = !between(angleA, angleC, angleB);
	angleB += PI;
	if (angleB > PI * 2.0f)
		angleB -= PI * 2.0f;
	float bisectorAngle2 = mean(angleA, angleB);
	if (bisectorAngle1 > PI)
		bisectorAngle1 -= PI;
	if (bisectorAngle2 > PI)
		bisectorAngle2 -= PI;
	float diff1 = abs(bisectorAngle1 - angleC);
	float diff2 = abs(bisectorAngle2 - angleC);
	float properAngle;
	if (diff1 > diff2)
		properAngle = bisectorAngle1;
	else
		properAngle = bisectorAngle2;
	//DP: properAngle = (diff1>diff2)? bisectorAngle1: bisectorAngle2;
	//New logic
	if (useAngle1)
		properAngle = bisectorAngle1;
	else
		properAngle = bisectorAngle2;
	//DP: properAngle = (useAngle1)? bisectorAngle1: bisectorAngle2;
	point bisectorUnit = unitfromangle(properAngle);
	point bisectorP1 = bisectorUnit;
	point bisectorP2 = scalarproduct(bisectorUnit, -1.0f);
	bisectorP1 = combine(bisectorP1, corner);
	bisectorP2 = combine(bisectorP2, corner);
	segment bisector(bisectorP1, bisectorP2);
	return bisector;
}

class circle {
public:
	point center;
	float radius;
	circle() {
		center = point(0, 0);
		radius = 0.0f;
	}
	circle(point centre, float rad) { //DP: Pass by ref?
		center = centre;
		radius = rad;
	}
	float circumfrence() { return PI * radius * 2; }
	float area() { return PI * radius * radius; }
};

class triangle {
public:
	point p1;
	point p2;
	point p3;
	segment s1() {
		segment seg1(p2, p3);
		return seg1;
	}
	segment s2() {
		segment seg1(p3, p1);
		return seg1;
	}
	segment s3() {
		segment seg1(p1, p2);
		return seg1;
	}
	triangle(point a, point b, point c) {
		p1 = a;
		p2 = b;
		p3 = c;
	}
	//1, 2, or 3 for specific estimates, anything else for the average of all 3.
	point fermatpoint(int which) {
		point fermA;
		point fermB;
		point fermC;
		point fermM;
		point A = equilateralBisector(s1()).p2;
		point B = equilateralBisector(s2()).p2;
		point C = equilateralBisector(s3()).p2;
		segment finder1;
		finder1.define(p1, A);
		segment finder2;
		finder2.define(p2, B);
		segment finder3;
		finder3.define(p3, C);
		fermA = intersection(finder2, finder3);
		fermB = intersection(finder3, finder1);
		fermC = intersection(finder1, finder2);
		fermM.x = (fermA.x + fermB.x + fermC.x) / 3;
		fermM.y = (fermA.y + fermB.y + fermC.y) / 3;
		if (which == 1)
			return fermA;
		else if (which == 2)
			return fermB;
		else if (which == 3)
			return fermC;
		else
			return fermM;
	}
};


#endif