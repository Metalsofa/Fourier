/*This file contains a great many custom rendering functions that
take advantage of freeGlut*/
#ifndef __customGL_h__
#define __customGL_h__

/*Attention Developoers:
	This header file was developed for another project, so there are some namespace
	conflicts with some of the other files. We are in the process of reformatting the
	other files to make room for these names before we #include theme anywhere.*/


#include "geometry.h"
#include "GL/glut.h"
#include "fcolor.h"
#include "waves.h"
#include "camera.h"
//#include "players.h"

#include <string>

enum discreteHue {red, orange, yellow, lime, green, teal, cyan, indigo, blue, purple, magenta, violet};


void setcolor(fcolor& col);

void drawText(point& location, string text);

void glVertexPoint(point dot);
void glVertexSegment(segment& seg);
void glVertexTriangle(triangle& tri);
void glVertexFermatPoint(triangle& tri);

void drawPoint(point dot, float size, bool label);

//Like glTranslatef, but it takes a single argument: a point
void glTranslatePoint(point& offset);

void drawSegment(segment &seg, float &thickness, bool &endpoints, bool &labels);

void definecamera();

void setcolor(metastat col, float opacity);

void rendertext(point location, string text);

void drawaxes();

void drawTriangle(triangle &tri, bool filled, bool vertices, float pointsize, bool labels,
	bool fermatPoint);

//bounds.p1 is bottom-left, bounds.p2 is top-right.
void drawXYgrid(segment bounds, unsigned int levels);

//The following sections added after Feb 16 to clean up the main file

void drawPoint(point &dot);

void drawsegment(segment &seg);

/*Always at (0,0) in the GL matrix, drawn along the x-axis, left bound is like the phase angle.
Resolution represents the number of points used to paint a single crest/trough. Appears clamped by bounds for aesthetic,
but only if passed TRUE for the clamping of that bound.*/


metastat colorfromID(int colorID);


metastat randomhue();
void drawWave(sinusoid &wave, float leftbound, float rightbound, int resolution, bool clampLeft, bool clampRight);



//Like drawWave but for a Fourier series //Seems done to me. Haven't worked on it in a while
void drawSeries(vector<sinusoid> &series, float leftbound, float rightbound, int resolution, bool clampLeft, bool clampRight);

void drawshape(shape &obj);

void drawGraphic(graphic& obj);

void drawCursor(cursor& curse);

//Clears the screen
void ClearScreen();

//Takes a refrence to a wall and then draws it
void drawwall(wall& drawingWall);

/* //This function should theoretically be devoid of purpose.
void rendertext(float x, float y, float z, void *font, string text) { 
	glRasterPos3f(x, y, z);
	for (char c : text)
		glutBitmapCharacter(font, c);
}
*/
void drawray(ray &drawingRay);

void drawCombatant(combatant& fighter);


//////////DEBUG/DESIGN ORIENTED STUFF/////////

void drawArtGUI();

/////////////////////WARNING: MOSTLY BORING STUFF AFTER THIS LINE/////////////////////////

void changeSize(int width, int height);

#endif