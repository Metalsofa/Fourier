/*This file contains a great many custom rendering functions that
take advantage of freeGlut*/
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
#include "camera.h"

#include <string>

enum discreteHue {red, orange, yellow, lime, green, teal, cyan, indigo, blue, purple, magenta, violet};


void setcolor(fcolor& col) {
	glColor4f(col.getLevel('r'), col.getLevel('g'), col.getLevel('b'), col.getLevel('a'));
}

void drawText(point& location, string text) {
	point dot = location;
	glRasterPos2f(dot.x, dot.y);
	for (unsigned int i = 0; i < text.size(); i++) { //glutBitmapString() https://stackoverflow.com/questions/544079/how-do-i-use-glutbitmapstring-in-c-to-draw-text-to-the-screen
		//Draw each character    
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void glVertexPoint(point dot) {
	glVertex2f(dot.x, dot.y);
}
void glVertexSegment(segment& seg) {
	glVertexPoint(seg.p1);
	glVertexPoint(seg.p2);
}
void glVertexTriangle(triangle& tri) {
	glVertexPoint(tri.p1);
	glVertexPoint(tri.p2);
	glVertexPoint(tri.p3);
}
void glVertexFermatPoint(triangle& tri) {
	glVertexPoint(tri.fermatpoint(0));
}

void drawPoint(point dot, float size, bool label) {
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertexPoint(dot);
	glEnd();
	if (label)
		drawText(dot, dot.label());
}

//Like glTranslatef, but it takes a single argument: a point
void glTranslatePoint(point& offset) {
	glTranslatef(offset.x, offset.y, 0.0f);
}

void drawSegment(segment &seg, float &thickness, bool &endpoints, bool &labels) {
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glVertexSegment(seg);
	glEnd();
	if (endpoints) {
		drawPoint(seg.p1, thickness * 2.0f, labels);
		drawPoint(seg.p2, thickness * 2.0f, labels);
	}

}

void definecamera() {
	if (PerspectiveRise > PerspectiveRiseMax)
		PerspectiveRise = PerspectiveRiseMax;
	if (PerspectiveRise < 0)
		PerspectiveRise = 0;
	float CameraXpos = cos(PerspectiveOrbit - PI / 2) * cos(PerspectiveRise) * PerspectiveDist + LookPointX;
	float CameraYpos = sin(PerspectiveOrbit - PI / 2) * cos(PerspectiveRise) * PerspectiveDist + LookPointY;
	float CameraZpos = (sin(PerspectiveRise) * PerspectiveDist + LookPointZ) * (1.0f + 0.7f * abs(sin(PerspectiveOrbit)));
	gluLookAt(CameraXpos, CameraYpos, CameraZpos, //Eye Position
		LookPointX, LookPointY, LookPointZ, //Looking at this point
		0.0f, 0.0f, 1.0f); //Vector that defines the "up" direction
}

void setcolor(metastat col, float opacity) {
	float R = float(col.som) / 255.0f;
	float G = float(col.emo) / 255.0f;
	float B = float(col.cog) / 255.0f;
	glColor4f(R, G, B, opacity);
}

void rendertext(point location, string text) {
		// set position to text    
	glRasterPos2f(location.x, location.y);

	for (unsigned int i = 0; i < text.size(); i++)
	{
		// draw each character    
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
	//glTranslatef(-dot.x, -dot.y, 0.0f);
}



void drawaxes() {
	glLineWidth(1.0f);
	//Draw Grey Backdrop
	glBegin(GL_QUADS);
	glColor3f(.1f, .1f, .1f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(currentbattle.boardWidth(), 0.0f, 0.0f);
	glVertex3f(currentbattle.boardWidth(), currentbattle.boardHeight(), 0); glVertex3f(0, currentbattle.boardHeight(), 0);
	glEnd();

	//Gridlines
	glBegin(GL_LINES);
	float tickiterator = 0.0f;
	glColor3f(0.4f, 0.4f, 0.4f);
	while (tickiterator < currentbattle.boardWidth() - 1)
	{
		tickiterator++;
		glVertex3f(tickiterator, currentbattle.boardHeight(), 0.0f); glVertex3f(tickiterator, 0.0f, 0.0f); //X along y
		glVertex3f(tickiterator, 0.0f, 0.0f); glVertex3f(tickiterator, 0.0f, float(BoardDepth)); //X along Z
	}
	tickiterator = 0.0f;
	while (tickiterator < currentbattle.boardHeight() - 1) {
		tickiterator++;
		glVertex3f(currentbattle.boardWidth(), tickiterator, 0.0f); glVertex3f(0.0f, tickiterator, 0.0f); //Y along x
		glVertex3f(0.0f, tickiterator, float(BoardDepth)); glVertex3f(0.0f, tickiterator, 0.0f); //Y along z
	}
	glEnd();

	//White Border
	glBegin(GL_LINE_STRIP);
	//glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0); glVertex3f(currentbattle.boardWidth(), 0, 0);
	glVertex3f(currentbattle.boardWidth(), currentbattle.boardHeight(), 0); glVertex3f(0, currentbattle.boardHeight(), 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glColor3f(1, 1, 1);
}

void drawTriangle(triangle &tri, bool filled, bool vertices, float pointsize, bool labels,
	bool fermatPoint) {
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
		drawPoint(tri.p1, pointsize, labels);
		drawPoint(tri.p2, pointsize, labels);
		drawPoint(tri.p3, pointsize, labels);
	}
	if (fermatPoint) {
		drawPoint(tri.fermatpoint(0),pointsize,labels);
	}
}

//bounds.p1 is bottom-left, bounds.p2 is top-right.
void drawXYgrid(segment bounds, unsigned int levels) {
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
			xfirst = int(floor(bounds.p1.x));
		else
			xfirst = int(ceil(bounds.p1.x));
		if (bounds.p1.y < 0)
			yfirst = int(floor(bounds.p1.y));
		else
			yfirst = int(ceil(bounds.p1.y));
		if (bounds.p2.x > 0)
			xlast = int(floor(bounds.p2.x));
		else
			xlast = int(ceil(bounds.p2.x));
		if (bounds.p2.y > 0)
			ylast = int(floor(bounds.p2.y));
		else
			ylast = int(ceil(bounds.p2.y));
		for (unsigned int i = 1; i < level; i++) {
			bounds.p1.x /= 2; bounds.p1.y /= 2;
			bounds.p2.x /= 2; bounds.p2.y /= 2;
		}
		float lv = float(level);
		for (int i = xfirst; i <= xlast; i++) {
			float it = float(i);
			glVertex2f(it / powf(2, lv - 1), bounds.p1.y); glVertex2f(it / powf(2, lv - 1), bounds.p2.y);
		}
		for (int i = yfirst; i <= ylast; i++) {
			float it = float(i);
			glVertex2f(bounds.p1.x, it / powf(2, lv - 1)); glVertex2f(bounds.p2.x, it / powf(2, lv - 1));
		}
		glEnd();
		level++;
	}
}

//The following sections added after Feb 16 to clean up the main file

void drawPoint(point &dot) {
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
		return clBlack;
	case 1:
		return clRed;
		break;
	case 2:
		return clOrange;
	case 3:
		return clYellow;
	case 4:
		return clLime;
	case 5:
		return clGreen;
	case 6:
		return clTeal;
	case 7:
		return clCyan;
	case 8:
		return clIndigo;
	case 9:
		return clBlue;
	case 10:
		return clPurple;
	case 11:
		return clMagenta;
	case 12:
		return clViolet;
	case 13:
		return clWhite;
	}
	return clBlack;
}


metastat randomhue() {
	int hueID = rand() % 12 + 1;
	return colorfromID(hueID);
}
void drawWave(sinusoid &wave, float leftbound, float rightbound, int resolution, bool clampLeft, bool clampRight) {
	if (leftbound > rightbound) {
		float temp = rightbound;
		rightbound = leftbound;
		leftbound = temp;
	}
	float cycles = wave.frequency * (rightbound - leftbound);
	int nodes = int(floor(2 * cycles * (1 + resolution)));
	float spacing = ((1 / wave.frequency) / 2) / (resolution + 1);
	//float beginningoffset = 1.0f / wave.frequency - fmod(leftbound, 1.0f / wave.frequency);
	float beginningoffset = spacing - fmod(leftbound, spacing);
	if (beginningoffset = spacing)
		beginningoffset = 0;
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.0f, (wave.eval(leftbound) * !clampLeft));
	for (int i = 0; i < nodes; i++) {
		glVertex2f(float(i) * spacing + beginningoffset, wave.eval(leftbound + float(i) * spacing + beginningoffset));
	}
	glVertex2f(rightbound - leftbound, wave.eval(rightbound) * !clampRight);
	glEnd();
}



//Like drawWave but for a Fourier series //Seems done to me. Haven't worked on it in a while
void drawSeries(vector<sinusoid> &series, float leftbound, float rightbound, int resolution, bool clampLeft, bool clampRight) {
	if (leftbound > rightbound) {
		float temp = rightbound;
		rightbound = leftbound;
		leftbound = temp;
	}
	float cycles = series[0].frequency * (rightbound - leftbound);
	int nodes = int(floor(2 * cycles * (1 + resolution)));
	float spacing = ((1 / series[0].frequency) / 2) / (resolution + 1);
	//float beginningoffset = 1.0f / wave.frequency - fmod(leftbound, 1.0f / wave.frequency);
	float beginningoffset = spacing - fmod(leftbound, spacing);
	if (beginningoffset = spacing)
		beginningoffset = 0;
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.0f, evalSeries(series, leftbound) * !clampLeft);
	for (int i = 0; i < nodes; i++) {
		glVertex2f(float(i) * spacing + beginningoffset, evalSeries(series, leftbound + float(i) * spacing + beginningoffset));
	}
	glVertex2f(rightbound - leftbound, evalSeries(series, rightbound) * !clampRight);
	glEnd();
}

void drawshape(shape &obj) {
	setcolor(obj.color, obj.opacity);
	glLineWidth(obj.lineThickness);
	glBegin(obj.mode);
	for (int i = 0; i < obj.vertices.size(); i++) {
		glVertex2f(obj.vertices[i].x, obj.vertices[i].y);
	}
	glEnd();
}

void drawGraphic(graphic& obj) {
	for (shape& s : obj.pieces) { drawshape(s);}
}

void drawCursor(cursor& curse) {
	glLineWidth(1.0f);
	//Draw the mouse-locator lines
	if (showTicks) {
		float ticksize = 0.2f;
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex2f(curse.Position.x, 0); glVertex2f(curse.Position.x, ticksize);
		glVertex2f(curse.Position.x, currentbattle.boardHeight()); glVertex2f(curse.Position.x, currentbattle.boardHeight() - ticksize);
		glVertex2f(0, curse.Position.y); glVertex2f(ticksize, curse.Position.y);
		glVertex2f(currentbattle.boardWidth(), curse.Position.y); glVertex2f(currentbattle.boardWidth() - ticksize, curse.Position.y);
		glEnd();
	}
	glColor3f(curse.Red(), curse.Green(), curse.Blue()); //OH THERE IT IS I FOUND THE CURSOR COLOR LINE FINALLY
	if (DESIGN_FUNCTION == BD_MAKE_SHAPES)
		setcolor(inverse(art.pieces[editingLayer].color), 1.0f);
	glPushMatrix();
	glTranslatef(curse.Position.x, curse.Position.y, 0); //Translate #1
	glRotatef(-90 + curse.Tilt * 180 / PI, 0, 1, 0); //Rotate #1
	glRotatef(curse.Yaw * 180 / PI, 0, 0, 1); //Rotate #2
	glRotatef(curse.Rot * 180 / PI, 1, 0, 0); //Rotate #3
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(curse.Size * cos(curse.spread / 2), curse.Size * sin(curse.spread / 2));
	glVertex2f(0, 0);
	glVertex2f(curse.Size * cos(curse.spread / 2), curse.Size * -sin(curse.spread / 2));
	///These lines make it look kind of like a cone, so I commented them out.
	//glVertex3f(0, 0, 0);
	//glVertex3f(curse.Size * cos(curse.spread / 2), 0, curse.Size * -sin(curse.spread / 2));
	//glVertex3f(0, 0, 0);
	//glVertex3f(curse.Size * cos(curse.spread / 2), 0, curse.Size * sin(curse.spread / 2));
	glVertex2f(0.0f, 0.0f); glVertex2f(0.8f * curse.Size / curse.spread, 0.0f);
	glLoadIdentity();
	glEnd();
	glPopMatrix();
	//definecamera(); //For some reason, calling this here causes some weird things to happen
}

//Clears the screen
void ClearScreen() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

//Takes a refrence to a wall and then draws it
void drawwall(wall& drawingWall) {
	glLineWidth(drawingWall.material.thickness);
	glBegin(GL_LINES);
	glColor4f(
		drawingWall.material.getRed(),
		drawingWall.material.getGreen(),
		drawingWall.material.getBlue(),
		drawingWall.material.getAlpha()
	);
	glVertex2f(drawingWall.getbody().p1.x, drawingWall.getbody().p1.y);
	glVertex2f(drawingWall.getbody().p2.x, drawingWall.getbody().p2.y);
	glEnd();
	glLineWidth(1.0f);
}

/* //This function should theoretically be devoid of purpose.
void rendertext(float x, float y, float z, void *font, string text) { 
	glRasterPos3f(x, y, z);
	for (char c : text)
		glutBitmapCharacter(font, c);
}
*/
void drawray(ray &drawingRay) {
	glColor3f(drawingRay.getRed(), drawingRay.getGreen(), drawingRay.getBlue());
	glLineWidth(drawingRay.getthickness());
	glBegin(GL_LINE_STRIP);
	for (point dot : drawingRay.getbits()) {
		glVertex2f(dot.x, dot.y);
	}
	glEnd();

	//Draw the joints (mainly for debug purposes)
	bool showjoints = false;
	if (showjoints) {
		for (unsigned int i = 1; i <= drawingRay.getbits().size(); i++) {
			drawPoint(drawingRay.getbits()[i-1]);
			glColor3f(drawingRay.getRed(), drawingRay.getGreen(), drawingRay.getBlue());
			rendertext(drawingRay.getbits()[i - 1], to_string(i));
		}
	}

	////Draw Arrowhead
	//if (!drawingRay.terminating ) {
		glTranslatef(drawingRay.getbits()[0].x, drawingRay.getbits()[0].y, 0.0f);
		point unit = unitvector(difference(drawingRay.getbits()[0], drawingRay.getbits()[1]));
		glRotatef(atan2f(unit.y, unit.x) * 180 / PI, 0, 0, 1);
		glBegin(GL_POLYGON);
		float arrowWidth = 0.1f;
		float arrowHeight = 0.2f;
		glVertex2f(0.0f, 0.0f);
		glVertex2f(-arrowHeight, arrowWidth / 2.0f);
		glVertex2f(-arrowHeight * 0.5f, 0.0f);
		glVertex2f(-arrowHeight, -arrowWidth / 2.0f);
		glEnd();
	//}
	//Reset transformations
	glLoadIdentity();
	definecamera();

	//Draw tail sphere. This is really just for fun.
	bool drawTailSphere = false;
	if (drawTailSphere) {
		float excesslength = drawingRay.length() - drawingRay.lengthNominal();
		float unborn = -excesslength;
		if (unborn > 0) {
			float desiredArea = unborn * drawingRay.getthickness() * 0.001f;
			glTranslatef(drawingRay.getbits().back().x, drawingRay.getbits().back().y, 0);
			point unitback = unitvector(difference(drawingRay.getbits()[drawingRay.getbits().size() - 2],
				drawingRay.getbits().back()));
			glRotatef(unborn * 20, unitback.x, unitback.y, 0);
			glutSolidSphere(sqrt(desiredArea / PI), 10, 10);
			glLoadIdentity();
			definecamera();
		}
	}
}

void drawCombatant(combatant& fighter) {
	glPushMatrix();
	glTranslatef(fighter.position.x, fighter.position.y, 0.0f);
	glRotatef(180 * fighter.direction.angle() / PI, 0, 0, 1);
	glScalef(fighter.width, fighter.width, fighter.width);
	glTranslatef(-0.5f, -0.5f, 0.0f);
	drawGraphic(fighter.sprite);
	glPopMatrix();
	//if (fighter.tog) { //DP: Combatant doesn't have a tog var now
		glColor3f(1.0f, 1.0f, 1.0f);
	/*} else {
		glColor3f(.5f, .5f, .5f);
	}*/
	glTranslatef(fighter.position.x, fighter.position.y, 0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i += 10) { //Doing only half the work with i += 2
		float theta = PI * i / 90;
		glVertex2f(0.25f*cosf(theta), 0.25f*sinf(theta));
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i += 50) {
		float theta = PI * i / 90;
		glVertex2f(0.01f*cosf(theta) + fighter.direction.x, 0.01f*sinf(theta) + fighter.direction.y);
	}
	glEnd();
	glTranslatef(-1 * fighter.position.x, -1 * fighter.position.y, 0);

	//glPopMatrix();
}


//////////DEBUG/DESIGN ORIENTED STUFF/////////

void drawArtGUI() { //Idea I just had: Every player metastat caps at 255; WHITE is the optimum in any area		DP: Cool Idea
	rendertext(point(0.0f, currentbattle.map.height + 0.5f), "ART MODE: " + currentGraphicName);
	//For each frame...
	drawGraphic(animart[editingFrame]);
	for (unsigned int f = 0; f < animart.frames.size(); f++) {
		//For each layer...
		for (unsigned int i = 0; i < animart[f].pieces.size(); i++) {
			if (showDots && (i == editingLayer)) {
				setcolor(inverse(animart[f].pieces[i].color), 1.0f);
				glLineWidth(2.0f);
				glPointSize(5.0f);
				glBegin(GL_LINE_STRIP);
				for (point& dot : animart[f].pieces[i].vertices) {
					glVertex2f(dot.x, dot.y);
				}
				glEnd();
				glBegin(GL_POINTS);
				for (point& dot : animart[f].pieces[i].vertices) {
					glVertex2f(dot.x, dot.y);
				}
				glEnd();
			}
			if (showLayers) {
				glPushMatrix();
				glTranslatePoint(layDispPos);
				glTranslatef(f * ladDispSize, -(i * ladDispSize), 0);
				if (i == editingLayer && f == editingFrame)
					glColor3f(1.0f, 0.1f, 0.0f);
				else
					glColor3f(1.0f, 1.0f, 1.0f);
				glBegin(GL_LINE_LOOP);
				glVertex2f(0.0f, 0.0f);  glVertex2f(0.4f, 0.0f);
				glVertex2f(0.4f, -0.4f); glVertex2f(0.0f, -0.4f);
				glEnd();
				glPopMatrix();
			}
		}
	}
}

/////////////////////WARNING: MOSTLY BORING STUFF AFTER THIS LINE/////////////////////////

void changeSize(int width, int height) {
	//To avoid divide by zero:
	if (height == 0)
		height = 1;

	float ratio = 1.0f * width / height;

	//Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	//Reset Matrix
	glLoadIdentity();

	//Set viewport to be the entire window
	glViewport(0, 0, width, height);

	//Set the correct perspective
	gluPerspective(45, ratio, 1, 100);

	//Get back to the modelview
	glMatrixMode(GL_MODELVIEW);

}

#endif