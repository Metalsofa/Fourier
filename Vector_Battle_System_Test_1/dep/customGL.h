
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

void setcolor(metastat col, float opacity) { //DP: May want to pass by reference
	float R = float(col.som) / 255.0f;
	float G = float(col.emo) / 255.0f;
	float B = float(col.cog) / 255.0f;
	glColor4f(R, G, B, opacity);
}

void rendertext(point location, string text) {  //DP: IF glRasterPos2f doesn't edit the inputs, you might want to pass location in by reference and not create dot;
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
	glLineWidth(0.0f);
	//Draw Grey Backdrop
	glBegin(GL_QUADS);
	glColor3f(.1f, .1f, .1f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(currentbattle.BoardWidth(), 0.0f, 0.0f);
	glVertex3f(currentbattle.BoardWidth(), currentbattle.BoardHeight(), 0); glVertex3f(0, currentbattle.BoardHeight(), 0);
	glEnd();

	//Gridlines
	glBegin(GL_LINES);
	float tickiterator = 0.0f;
	glColor3f(0.4f, 0.4f, 0.4f);
	while (tickiterator < currentbattle.BoardWidth() - 1)
	{
		tickiterator++;
		glVertex3f(tickiterator, currentbattle.BoardHeight(), 0.0f); glVertex3f(tickiterator, 0.0f, 0.0f); //X along y
		glVertex3f(tickiterator, 0.0f, 0.0f); glVertex3f(tickiterator, 0.0f, float(BoardDepth)); //X along Z
	}
	tickiterator = 0.0f;
	while (tickiterator < currentbattle.BoardHeight() - 1) {
		tickiterator++;
		glVertex3f(currentbattle.BoardWidth(), tickiterator, 0.0f); glVertex3f(0.0f, tickiterator, 0.0f); //Y along x
		glVertex3f(0.0f, tickiterator, float(BoardDepth)); glVertex3f(0.0f, tickiterator, 0.0f); //Y along z
	}
	glEnd();

	//White Border
	glBegin(GL_LINE_STRIP);
	//glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0); glVertex3f(currentbattle.BoardWidth(), 0, 0);
	glVertex3f(currentbattle.BoardWidth(), currentbattle.BoardHeight(), 0); glVertex3f(0, currentbattle.BoardHeight(), 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glColor3f(1, 1, 1);
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

void drawshape(shape &obj) {
	setcolor(obj.color, obj.opacity);
	glLineWidth(obj.line_thickness);
	glBegin(obj.mode);
	for (int i = 0; i < obj.vertices.size(); i++) {
		glVertex2f(obj.vertices[i].x, obj.vertices[i].y);
	}
	glEnd();
}

void drawcursor(cursor& curse) {
	glLineWidth(1.0f);
	//Draw the mouse-locator lines
	if (show_ticks) {
		float ticksize = 0.2f;
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex2f(curse.Position.x, 0); glVertex2f(curse.Position.x, ticksize);
		glVertex2f(curse.Position.x, currentbattle.BoardHeight()); glVertex2f(curse.Position.x, currentbattle.BoardHeight() - ticksize);
		glVertex2f(0, curse.Position.y); glVertex2f(ticksize, curse.Position.y);
		glVertex2f(currentbattle.BoardWidth(), curse.Position.y); glVertex2f(currentbattle.BoardWidth() - ticksize, curse.Position.y);
		glEnd();
	}
	glColor3f(curse.Red(), curse.Green(), curse.Blue()); //OH THERE IT IS I FOUND THE CURSOR COLOR LINE FINALLY		//DP: YAY
	if (DESIGN_FUNCTION == BD_MAKE_SHAPES)
		setcolor(inverse(art.pieces[Gindex].color), 1.0f);
	glPushMatrix();
	glTranslatef(curse.Position.x, curse.Position.y, 0); //Translate #1
	glRotatef(-90 + curse.Tilt * 180 / PI, 0, 1, 0); //Rotate #1
	glRotatef(curse.Yaw * 180 / PI, 0, 0, 1); //Rotate #2
	glRotatef(curse.Rot * 180 / PI, 1, 0, 0); //Rotate #3
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(curse.Size * cos(curse.Spread / 2), curse.Size * sin(curse.Spread / 2));
	glVertex2f(0, 0);
	glVertex2f(curse.Size * cos(curse.Spread / 2), curse.Size * -sin(curse.Spread / 2));
	///These lines make it look kind of like a cone, so I commented them out.
	//glVertex3f(0, 0, 0);
	//glVertex3f(curse.Size * cos(curse.Spread / 2), 0, curse.Size * -sin(curse.Spread / 2));
	//glVertex3f(0, 0, 0);
	//glVertex3f(curse.Size * cos(curse.Spread / 2), 0, curse.Size * sin(curse.Spread / 2));
	glVertex2f(0.0f, 0.0f); glVertex2f(0.8f * curse.Size / curse.Spread, 0.0f);
	glLoadIdentity();
	glEnd();
	glPopMatrix();
	//definecamera(); //For some reason, calling this here causes some weird things to happen
}

//Clears the screen
void ClearScreen() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

//Takes a refrence to a wall and then draws it
void drawwall(wall& drawing_wall) { //DP: Pass by reference?
	glLineWidth(drawing_wall.material.thickness);
	glBegin(GL_LINES);
	glColor4f(
		drawing_wall.material.getRed(),
		drawing_wall.material.getGreen(),
		drawing_wall.material.getBlue(),
		drawing_wall.material.getAlpha()
	);
	glVertex2f(drawing_wall.getbody().p1.x, drawing_wall.getbody().p1.y);
	glVertex2f(drawing_wall.getbody().p2.x, drawing_wall.getbody().p2.y);
	glEnd();
	glLineWidth(1.0f);
}

/* //This function should theoretically be devoid of purpose.
void rendertext(float x, float y, float z, void *font, string text) { //DP: It might be slightly faster to use glutBitmapString https://stackoverflow.com/questions/544079/how-do-i-use-glutbitmapstring-in-c-to-draw-text-to-the-screen
	glRasterPos3f(x, y, z);
	for (char c : text)
		glutBitmapCharacter(font, c);
}
*/
void drawray(ray &drawing_ray) {
	glColor3f(drawing_ray.getRed(), drawing_ray.getGreen(), drawing_ray.getBlue());
	glLineWidth(drawing_ray.getthickness());
	glBegin(GL_LINE_STRIP);
	for (point dot : drawing_ray.getbits()) {
		glVertex2f(dot.x, dot.y);
	}
	glEnd();

	//Draw the joints (mainly for debug purposes)
	bool showjoints = false;
	if (showjoints) {
		unsigned int i = 1;
		for (point dot : drawing_ray.getbits()) { //DP: if you are already using an index, it might be better to do what I put on line 420
			drawpoint(dot);
			glColor3f(drawing_ray.getRed(), drawing_ray.getGreen(), drawing_ray.getBlue());
			rendertext(dot, to_string(i));
			i++;
		}
		/*for (unsigned int i = 1; i <= drawing_ray.getbits(); i++) {
			drawpoint(drawing_ray.getbits()[i-1]);
			glColor3f(drawing_ray.getRed(), drawing_ray.getGreen(), drawing_ray.getBlue());
			rendertext(drawing_ray.getbits()[i - 1], to_string(i));
		}*/
	}

	//Draw Arrowhead
	if (!drawing_ray.terminating && false) {
		glTranslatef(drawing_ray.getbits()[0].x, drawing_ray.getbits()[0].y, 0.0f);

		glRotatef(atan2f(drawing_ray.movevector().y, drawing_ray.movevector().x) * 180 / PI, 0, 0, 1);
		glBegin(GL_POLYGON);
		float arrow_width = 0.1f;
		float arrow_height = 0.2f;
		glVertex2f(0.0f, 0.0f);
		glVertex2f(-arrow_height, arrow_width / 2.0f);
		glVertex2f(-arrow_height * 0.5f, 0.0f);
		glVertex2f(-arrow_height, -arrow_width / 2.0f);
		glEnd();
	}
	//Reset transformations
	glLoadIdentity();
	definecamera();

	//Draw tail sphere. This is really just for fun.
	bool draw_tail_sphere = false;
	if (draw_tail_sphere) {
		float excesslength = drawing_ray.length() - drawing_ray.length_nominal();
		float unborn = -excesslength;
		if (unborn > 0) {
			float desired_area = unborn * drawing_ray.getthickness() * 0.001f;
			glTranslatef(drawing_ray.getbits().back().x, drawing_ray.getbits().back().y, 0);
			point unitback = unitvector(difference(drawing_ray.getbits()[drawing_ray.getbits().size() - 2],
				drawing_ray.getbits().back()));
			glRotatef(unborn * 20, unitback.x, unitback.y, 0);
			glutSolidSphere(sqrt(desired_area / PI), 10, 10);
			glLoadIdentity();
			definecamera();
		}
	}
}

void draw_combatant(combatant &fighter) { //DP: This is the coolest function I've ever read
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(fighter.position.x, fighter.position.y, 0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i += 2) { //Doing only half the work with i += 2
		float theta = PI * i / 90;
		glVertex2f(0.5f*cosf(theta), 0.5f*sinf(theta));
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i += 10) {
		float theta = PI * i / 90;
		glVertex2f(0.01f*cosf(theta) + fighter.direction.x, 0.01f*sinf(theta) + fighter.direction.y);
	}
	glEnd();
	glTranslatef(-1 * fighter.position.x, -1 * fighter.position.y, 0);

	//glPopMatrix();
}


//////////DEBUG/DESIGN ORIENTED STUFF/////////

void draw_art_GUI() { //Idea I just had: Every player metastat caps at 255; WHITE is the optimum in any area		DP: Cool Idea
	rendertext(point(0.0f, currentbattle.map.height + 0.5f), "ART MODE: " + current_graphic_name);
	for (unsigned int i = 0; i < art.pieces.size(); i++) {
		drawshape(art.pieces[i]);
		if (show_dots && (i == Gindex)) {
			setcolor(inverse(art.pieces[i].color), 1.0f);
			glLineWidth(2.0f);
			glPointSize(5.0f);
			glBegin(GL_LINE_STRIP);
			for (point dot : art.pieces[i].vertices) {
				glVertex2f(dot.x, dot.y);
			}
			glEnd();
			glBegin(GL_POINTS);
			for (point dot : art.pieces[i].vertices) {
				glVertex2f(dot.x, dot.y);
			}
			glEnd();
		}
		if (show_layers) {
			point lay_disp_pos(0.05f, 5.0f);
			glTranslatef(lay_disp_pos.x + i * 0.1f, lay_disp_pos.y - i * 0.1f, 0.0f);
			if (i == Gindex)
				glColor3f(1, 0.1, 0);
			else
				glColor3f(1, 1, 1);
			glBegin(GL_LINE_LOOP);
			glVertex2f(0.0f, 0.0f);  glVertex2f(0.4f, 0.0f);
			glVertex2f(0.4f, -0.4f); glVertex2f(0.0f, -0.4f);
			glEnd();
			glTranslatef(-lay_disp_pos.x - i * 0.1f, -lay_disp_pos.y + i * 0.1f, 0.0f);
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