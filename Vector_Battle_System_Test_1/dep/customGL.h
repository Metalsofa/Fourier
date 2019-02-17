
#ifndef __customGL_h
#define __customGL_h

/*Attention Developoers:
	This header file was developed for another project, so there are some namespace
	conflicts with some of the other files. We are in the process of reformatting the
	other files to make room for these names before we #include theme anywhere.*/


#include "geometry.h"
#include "GL/glut.h"
#include "fcolor.h"
#include "waves.h"
#include "players.h"

#include <string>

enum discrete_hue {red, orange, yellow, lime, green, teal, cyan, indigo, blue, purple, magenta, violet};


void setcolor(fcolor col) { //DP: Ref?
	glColor4f(col.get_level('r'), col.get_level('g'), col.get_level('b'), col.get_level('a'));
}

void draw_text(point location, string text) {//DP: Ref?
	point dot = location;
	glRasterPos2f(dot.x, dot.y);
	for (unsigned int i = 0; i < text.size(); i++) { //glutBitmapString() https://stackoverflow.com/questions/544079/how-do-i-use-glutbitmapstring-in-c-to-draw-text-to-the-screen
		//Draw each character    
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void glVertexPoint(point dot) {//DP: Ref? //DM: Cannot, for many of thsese
	glVertex2f(dot.x, dot.y);
}
void glVertexSegment(segment seg) {//DP: Ref?
	glVertexPoint(seg.p1);
	glVertexPoint(seg.p2);
}
void glVertexTriangle(triangle tri) {//DP: Ref?
	glVertexPoint(tri.p1);
	glVertexPoint(tri.p2);
	glVertexPoint(tri.p3);
}
void glVertexFermatPoint(triangle tri) {//DP: Ref?
	glVertexPoint(tri.fermatpoint(0));
}

//what is the point of glVertexPoint and glVertexFermatPoint, it just seems to be an unecessary function call, when u use these, you could just use that one line?

void draw_point(point dot, float size, bool label) {
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertexPoint(dot);
	glEnd();
	if (label)
		draw_text(dot, dot.label());
}

void draw_segment(segment &seg, float &thickness, bool &endpoints, bool &labels) {
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glVertexSegment(seg);
	glEnd();
	if (endpoints) {
		draw_point(seg.p1, thickness * 2.0f, labels);
		draw_point(seg.p2, thickness * 2.0f, labels);
	}

}

void draw_triangle(triangle &tri, bool filled, bool vertices, float pointsize, bool labels,
	bool fermat_point) {
	if (filled)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_LINE_LOOP);
	glVertexTriangle(tri);
	glEnd();
	if (vertices) {
		glBegin(GL_POINTS);
		glVertexTriangle(tri);
		glEnd();
		draw_point(tri.p1, pointsize, labels);
		draw_point(tri.p2, pointsize, labels);
		draw_point(tri.p3, pointsize, labels);
	}
	if (fermat_point) {
		draw_point(tri.fermatpoint(0),pointsize,labels);
	}
}

//bounds.p1 is bottom-left, bounds.p2 is top-right.
void draw_XYgrid(segment bounds, unsigned int levels) {
	unsigned int level = 1;
	while (level <= levels) {
		glLineWidth(float(levels) - float(level) + 1.0f);
		float lightness = 1.0f / float(level);
		glColor3f(lightness, lightness, lightness);
		glBegin(GL_LINES);
		for (unsigned int i = 1; i < level; i++) {
			bounds.p1.x *= 2.0f; bounds.p1.y *= 2.0f;
			bounds.p2.x *= 2.0f; bounds.p2.y *= 2.0f;
		}
		int xfirst, xlast, yfirst, ylast;
		if (bounds.p1.x < 0)
			xfirst = floor(bounds.p1.x);
		else
			xfirst = ceil(bounds.p1.x);
		if (bounds.p1.y < 0)
			yfirst = floor(bounds.p1.y);
		else
			yfirst = ceil(bounds.p1.y);
		if (bounds.p2.x > 0)
			xlast = floor(bounds.p2.x);
		else
			xlast = ceil(bounds.p2.x);
		if (bounds.p2.y > 0)
			ylast = floor(bounds.p2.y);
		else
			ylast = ceil(bounds.p2.y);
		for (unsigned int i = 1; i < level; i++) {
			bounds.p1.x /= 2; bounds.p1.y /= 2;
			bounds.p2.x /= 2; bounds.p2.y /= 2;
		}
		float lv = level;
		for (int i = xfirst; i <= xlast; i++) {
			float it = i;
			glVertex2f(it / pow(2, lv - 1), bounds.p1.y); glVertex2f(it / pow(2, lv - 1), bounds.p2.y);
		}
		for (int i = yfirst; i <= ylast; i++) {
			float it = i;
			glVertex2f(bounds.p1.x, it / pow(2, lv - 1)); glVertex2f(bounds.p2.x, it / pow(2, lv - 1));
		}
		glEnd();
		level++;
	}
}

//The following sections added after Feb 16 to clean up the main file

void drawpoint(point &dot) {
	glColor3f(1, 1, 1);
	glTranslatef(dot.x, dot.y, 0.0f);
	glutSolidSphere(0.05, 10, 10);
	glTranslatef(-dot.x, -dot.y, 0.0f);
	glColor3f(0, 0, 0);
	glTranslatef(dot.x, dot.y, 0.0f);
	glutSolidSphere(0.025, 10, 10);
	glTranslatef(-dot.x, -dot.y, 0.0f);
}

void drawsegment(segment &seg) {
	glBegin(GL_LINES);
	glVertex2f(seg.p1.x, seg.p1.y);
	glVertex2f(seg.p2.x, seg.p2.y);
	glEnd();
}

/*Always at (0,0) in the matrix, drawn along the x-axis, left bound is like the phase angle.
Resolution represents the number of points used to paint a single crest/trough. Appears clamped by bounds for aesthetic,
but only if passed TRUE for the clamping of that bound.*/


metastat colorfromID(int colorID) {
	switch (colorID) {
	case 0:
		return cl_black;
	case 1:
		return cl_red;
		break;
	case 2:
		return cl_orange;
	case 3:
		return cl_yellow;
	case 4:
		return cl_lime;
	case 5:
		return cl_green;
	case 6:
		return cl_teal;
	case 7:
		return cl_cyan;
	case 8:
		return cl_indigo;
	case 9:
		return cl_blue;
	case 10:
		return cl_purple;
	case 11:
		return cl_magenta;
	case 12:
		return cl_violet;
	case 13:
		return cl_white;
	}
	return cl_black;
}


metastat randomhue() {
	int hueID = rand() % 12 + 1; //DP: Not good random, should seed random in main, I did it on line 1065 and also included time.h
	return colorfromID(hueID);
}
void draw_wave(sinusoid &wave, float leftbound, float rightbound, int resolution, bool clamp_left, bool clamp_right) {
	if (leftbound > rightbound) {
		float temp = rightbound;
		rightbound = leftbound;
		leftbound = temp;
	}
	float cycles = wave.frequency * (rightbound - leftbound);
	int nodes = floor(2 * cycles * (1 + resolution));
	float spacing = ((1 / wave.frequency) / 2) / (resolution + 1);
	//float beginningoffset = 1.0f / wave.frequency - fmod(leftbound, 1.0f / wave.frequency);
	float beginningoffset = spacing - fmod(leftbound, spacing);
	if (beginningoffset = spacing)
		beginningoffset = 0;
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.0f, (wave.eval(leftbound) * !clamp_left));
	for (int i = 0; i < nodes; i++) {
		glVertex2f(float(i) * spacing + beginningoffset, wave.eval(leftbound + float(i) * spacing + beginningoffset));
	}
	glVertex2f(rightbound - leftbound, wave.eval(rightbound) * !clamp_right);
	glEnd();
}



//Like draw_wave but for a Fourier series //Seems done to me. Haven't worked on it in a while
void draw_series(vector<sinusoid> &series, float leftbound, float rightbound, int resolution, bool clamp_left, bool clamp_right) {
	if (leftbound > rightbound) {
		float temp = rightbound;
		rightbound = leftbound;
		leftbound = temp;
	}
	float cycles = series[0].frequency * (rightbound - leftbound);
	int nodes = floor(2 * cycles * (1 + resolution));
	float spacing = ((1 / series[0].frequency) / 2) / (resolution + 1);
	//float beginningoffset = 1.0f / wave.frequency - fmod(leftbound, 1.0f / wave.frequency);
	float beginningoffset = spacing - fmod(leftbound, spacing);
	if (beginningoffset = spacing)
		beginningoffset = 0;
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.0f, eval_series(series, leftbound) * !clamp_left);
	for (int i = 0; i < nodes; i++) {
		glVertex2f(float(i) * spacing + beginningoffset, eval_series(series, leftbound + float(i) * spacing + beginningoffset));
	}
	glVertex2f(rightbound - leftbound, eval_series(series, rightbound) * !clamp_right);
	glEnd();
}


#endif