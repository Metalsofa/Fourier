#pragma once

#ifndef __fcolor_h
#define __fcolor_h

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
		if (R > 1.0f)
			R = 1.0f;
		if (R < 0.0f)
			R = 0.0f;
		if (G > 1.0f)
			G = 1.0f;
		if (G < 0.0f)
			G = 0.0f;
		if (B > 1.0f)
			B = 1.0f;
		if (B < 0.0f)
			B = 0.0f;
		if (A > 1.0f)
			A = 1.0f;
		if (A < 0.0f)
			A = 0.0f;
		// DP: Can do it the way below:
		/*R = fmod(R, 1.0);
		G = fmod(G, 1.0);
		B = fmod(B, 1.0);
		A = fmod(A, 1.0);*/

	}
public:
	fcolor() { //DP: Simplified initalizer below
		R = G = B = A = 0.0f;
	}
	fcolor(float red, float green, float blue, float alpha) {
		R = red;
		G = green;
		B = blue;
		A = alpha;
		caplevels();
	}
	fcolor(float red, float green, float blue) {
		R = red;
		G = green;
		B = blue;
		A = 1.0f;
		caplevels();
	}
	//Accepted components are 'r', 'g', 'b', and 'a'.
	float getlevel(char component) {
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
	void setlevel(char component, float level) {
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
	void alterlevel(char component, float level) {
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
	void invert() {
		R = 1 - R;
		G = 1 - G;
		B = 1 - G;
	}

};

fcolor fcolor_inverse(fcolor col) { //DP: Pass by reference?
	col.invert();
	return col;
}

//This function is presently incomplete
fcolor fcolor_fromHSV(float hue, float saturation, float value) {
	fcolor col;
	//Incomplete
	return col;
}

#endif