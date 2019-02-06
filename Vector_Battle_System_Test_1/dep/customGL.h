#pragma once

#ifndef __customGL_h
#define __customGL_h

/*Attention Developoers:
	This header file was developed for another project, so there are some namespace
	conflicts with some of the other files. We are in the process of reformatting the
	other files to make room for these names before we #include theme anywhere.*/

//Be sure to include this only after freeglut is already included
#include "geometry.h"
#include <string>
#include "GL/glut.h"
#include "fcolor.h"

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

void glVertexPoint(point dot) {//DP: Ref?
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

void draw_point(point &dot, float size, bool label) {
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

#endif