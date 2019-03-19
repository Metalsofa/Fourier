#include "fcolor.h"

using namespace std;

void fcolor::caplevels() {
	R = fminf(fabsf(R), 1.0);
	G = fminf(fabsf(G), 1.0);
	B = fminf(fabsf(B), 1.0);
	A = fminf(fabsf(A), 1.0);
	//Thanks DP~
}

fcolor::fcolor() { //DP: Simplified initalizer below
	R = G = B = A = 0.0f;
}
fcolor::fcolor(float red, float green, float blue, float alpha) {
	R = red;
	G = green;
	B = blue;
	A = alpha;
	caplevels();
}
fcolor::fcolor(float red, float green, float blue) {
	R = red;
	G = green;
	B = blue;
	A = 1.0f;
	caplevels();
}
//Accepted components are 'r', 'g', 'b', and 'a'.
float fcolor::getLevel(char component) {
	if (component == 'r')
		return R;
	if (component == 'g')
		return G;
	if (component == 'b')
		return B;
	if (component == 'a')
		return A;
	return A;
}
//Accepted components are 'r', 'g', 'b', and 'a'.
void fcolor::setLevel(char component, float level) {
	if (component == 'r')
		R = level;
	if (component == 'g')
		G = level;
	if (component == 'b')
		B = level;
	if (component == 'a')
		A = level;
	caplevels();
}
//Accepted components are 'r', 'g', 'b', and 'a'.
void fcolor::alterLevel(char component, float level) {
	if (component == 'r')
		R += level;
	if (component == 'g')
		G += level;
	if (component == 'b')
		B += level;
	if (component == 'a')
		A += level;
	caplevels();
}
void fcolor::invert() {
	R = 1 - R;
	G = 1 - G;
	B = 1 - G;
}

fcolor fcolorInverse(fcolor& col) {
	col.invert();
	return col;
}

//This function is presently incomplete
fcolor fcolorFromHSV(float hue, float saturation, float value) {
	fcolor col;
	//Incomplete
	return col;
}