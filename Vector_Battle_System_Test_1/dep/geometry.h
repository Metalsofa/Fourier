//We'll include things like triangles and other fun geometric functions in this one.
#pragma once
#define geometry
#include <cmath>
#include <string>
#include <vector>
using namespace std;

const float  PI = 3.14159265358979f;

template <class number>
number mean(number &num1, number &num2) {
	number meen = num1 + num2;
	meen /= 2;
	return meen;
	//DP: Why not:
	//return (num1 + num2) / 2;
	//It would save the creation of 1 var every time u call this function
}

float pyth(float &a, float &b) { //DP: Again, no need to create a var
	float len;
	len = sqrt(pow(a, 2) + pow(b, 2));
	return len;
}

float pyth(const float &a, const float &b) { //DP: Again, no need to create a var
	float len;
	len = sqrt(pow(a, 2) + pow(b, 2));
	return len;
}

//Boolean for whether X lies between A and B. Think A < X < B.
bool between(float A, float X, float B) {
	if (B < A) {
		float oldA = A;
		A = B;
		B = oldA;
	}
	if (A < X && X < B)
		return true;
	else
		return false;
	//DP: You can use or:
	//return ((A < X && X < B) || (B < X && X < A));
	//or...
	//return (min(A,B) < X && X < max(A,B)); 
	//The second one looks better, but the first one might be barely faster due to a function call
}

class point {
public:
	float x;
	float y;
	void define(float xpos, float ypos) {
		x = xpos;
		y = ypos;
	}
	point(float xpos, float ypos) {
		x = xpos;
		y = ypos;
	}
	point() {
		x = 0;
		y = 0;
	}
	float magnitude() {
		return pyth(x, y);
	}
	//The angle formed by this point's vector from the origin with respect to the x-axis
	float angle() {
		return atan2(y, x);
	}
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
	point retp(pointe.x * coefficient, pointe.y * coefficient);
	return retp;
}

point scalarproduct(const point &pointe, const float &coefficient) {//DP: Again, no need to create a var
	point retp(pointe.x * coefficient, pointe.y * coefficient);
	return retp;
}

float dotproduct(point r1, point r2) {
	return (r1.x * r2.x + r1.y * r2.y);
}

//Returns a float, since this product will by definition be in the z-direction
float flatcrossproduct(point r1, point r2) { //DP: Again, no need to create a var
	float det = r1.x * r2.y - r1.y * r2.x;
	return det;
}

point unitvector(point po) {//DP: Again, no need to create a var
	if (po.magnitude() == 0)
		return point(1.0f, 0.0f);
	point uni;
	float recip;
	recip = 1 / po.magnitude();
	uni = scalarproduct(po, recip);
	return uni;
	//DP: Like below
	// return scalarproduct(po, (1/po.magnitude()));
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
point difference(point &point1, point &point2) {//DP: Again, no need to create a var
	point diff;
	diff.x = point1.x - point2.x;
	diff.y = point1.y - point2.y;
	return diff;
}

point difference(const point &point1, const point &point2) {//DP: Again, no need to create a var
	point diff;
	diff.x = point1.x - point2.x;
	diff.y = point1.y - point2.y;
	return diff;
}

//DP: I don't get what the point of the const versions from above are, but just overload the +, -, +=, and -= operator;

point rotate90(point poi) {//DP: Again, no need to create a var
	point pot(0 - poi.y, poi.x);
	return pot;
}

point point_sum(vector<point> &points) {
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
	void define(point point1, point point2) { //DP: Why not create constructors? + Pass by ref
		p1 = point1;
		p2 = point2;
	}
	point midpoint() {//DP: Again, no need to create a var
		float mx = (p1.x + p2.x) / 2;
		float my = (p1.y + p2.y) / 2;
		point mid(mx, my);
		return mid;
	}
	void define(float x1, float  y1, float  x2, float y2) { //DP: Constructors?
		p1.define(x1, y1);
		p2.define(x2, y2);
	}
	float length() {
		float leng;
		leng = pyth(p1.x - p2.x, p1.y - p2.y);
		return leng;
	}
	segment() {
		p1.define(0.0f,0.0f);
		p2.define(0.0f,0.0f);
	}
	segment(float x1, float y1, float x2, float y2) {
		p1.define(x1, y1);
		p2.define(x2, y2);
	}
	segment(point P1, point P2) { //DP: Pass by ref?
		p1 = P1;
		p2 = P2;
	}
};

//DP: If you don't use the defines outside of the class, there is no point in them, as they are redundant, but if you want to use them outside the class, why aren't the constructors calling them?

segment rotate90about(int pointID, segment seg) { //DP: pass by ref 
	segment nseg; // segment nseg = (pointID == 1)?seg:segment(seg.p2, seg.p1);
	if (pointID = 1) //DP: ALWAYS TRUE????
		nseg = seg;
	else
		nseg.define(seg.p2, seg.p1);
	point analog;
	analog.x = nseg.p2.x - nseg.p1.x;
	analog.y = nseg.p2.y - nseg.p1.y;
	analog = rotate90(analog);
	nseg.p2 = combine(nseg.p1, analog);
	return nseg;


}

segment equilateral_bisector(segment seg) { //Bisector protrudes from left, if p1 is bottom and p2 is top DP: pass by ref 
	segment perp;
	perp.p1 = seg.midpoint();
	point diffp = difference(perp.p2, perp.p1);
	perp.p2 = combine(perp.p1, scalarproduct(unitvector(difference(rotate90about(1, seg).p2, seg.p1)), sqrt(3.0f) / 2));
	return perp;
}

point intersection(segment sega, segment segb) { //Figured this out using Cramer's Rule DP: pass by ref 
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
	point solution;
	solution.define(X, Y);
	return solution;
}

//This is showing some REALLY weird behavior when one of the segments is perfectly vertical or horizontal
bool isintersect(segment sega, segment segb) { //DP: Pass by reference?
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
	return eval;
}

bool isperpintersect(point p, segment v) {//DP: pass by ref 
	point diff_vp = difference(v.p1, p);
	point diff_vv = difference(v.p2, v.p1);
	float numerator = dotproduct(diff_vp, diff_vv);
	float denominator = dotproduct(diff_vv, diff_vv);
	float t = -numerator / denominator;
	if (0 <= t && t <= 1)
		return true;
	else
		return false;
	//DP: return  (0 <= t && t <= 1);
}

float distancetoline(point p, segment v) { //DP: pass by ref 
	point diff_pv = difference(v.p1, p);
	point diff_vv = difference(v.p2, v.p1);
	float numerator = abs(flatcrossproduct(diff_vv, diff_pv));
	float denominator = diff_vv.magnitude();
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
	}
	//Otherwise we find the perpendicular path to the segment
	float r = distancetoline(dot, seg);
	return r;
}

//Returns the given point after refection about an axis defined by a given segment
point reflection(point &dot, segment &mirror) {
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
segment reflective_bisector(point &dot, segment &sega, segment &segb) {
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
	float bisector_angle_1 = mean(angleA, angleB);
	bool use_angle1 = !between(angleA, angleC, angleB);
	angleB += PI;
	if (angleB > PI * 2.0f)
		angleB -= PI * 2.0f;
	float bisector_angle_2 = mean(angleA, angleB);
	if (bisector_angle_1 > PI)
		bisector_angle_1 -= PI;
	if (bisector_angle_2 > PI)
		bisector_angle_2 -= PI;
	float diff1 = abs(bisector_angle_1 - angleC);
	float diff2 = abs(bisector_angle_2 - angleC);
	float proper_angle;
	if (diff1 > diff2)
		proper_angle = bisector_angle_1;
	else
		proper_angle = bisector_angle_2;
	//New logic
	if (use_angle1)
		proper_angle = bisector_angle_1;
	else
		proper_angle = bisector_angle_2;
	point bisector_unit = unitfromangle(proper_angle);
	point bisector_p1 = bisector_unit;
	point bisector_p2 = scalarproduct(bisector_unit, -1.0f);
	bisector_p1 = combine(bisector_p1, corner);
	bisector_p2 = combine(bisector_p2, corner);
	segment bisector(bisector_p1, bisector_p2);
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
	circle(point centre, float rad) {
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
		point A = equilateral_bisector(s1()).p2;
		point B = equilateral_bisector(s2()).p2;
		point C = equilateral_bisector(s3()).p2;
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