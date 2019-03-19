#pragma once

#ifndef __fcolor_h__
#define __fcolor_h__

#include <cmath>

/*Attention Developoers:
	This header file was developed for another project, so there are some namespace
	conflicts with some of the other files. We are in the process of reformatting the
	other files to make room for these names before we #include theme anywhere.*/

class fcolor { //In this class, all levels are a float from 0 to 1.
private:
	float R;
	float G;
	float B;
	float A;
	void caplevels() {
		R = fminf(fabsf(R), 1.0);
		G = fminf(fabsf(G), 1.0);
		B = fminf(fabsf(B), 1.0);
		A = fminf(fabsf(A), 1.0);
		//Thanks DP~
	}
public:
	fcolor();
	fcolor(float red, float green, float blue, float alpha);
	fcolor(float red, float green, float blue);
	//Accepted components are 'r', 'g', 'b', and 'a'.
	float getLevel(char component);
	//Accepted components are 'r', 'g', 'b', and 'a'.
	void setLevel(char component, float level);
	//Accepted components are 'r', 'g', 'b', and 'a'.
	void alterLevel(char component, float level);
	void invert();

};

fcolor fcolorInverse(fcolor& col);

//This function is presently incomplete
fcolor fcolorFromHSV(float hue, float saturation, float value);

#endif