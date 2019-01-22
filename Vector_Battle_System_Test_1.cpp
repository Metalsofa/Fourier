#include<Windows.h> 
#include<gl/glut.h>
#include<cmath>
#include"battle.h"
#include<stdlib.h>
#include<string>
#include<map>

using namespace std;

//Global Constants
///const float PI = 3.14159265358978f;

//Global Variables for detecting mode
bool OVERWORLD_MODE = false;
bool BATTLE_MODE = true;
bool BATLEFIELD_DESIGN_MODE = true;

//Global Variables for if in OVERWORLD_MODE


//Global Variables for if in BATTLE_MODE
battle_preset testpreset(10, 6); //Width, Height
battlestate currentbattle(testpreset); //This holds the information about the current battle scene
const int BoardDepth = 0; //Not sure about this. Probably will never change from 0.
float timer = 0; //We'll see if this ends up being necessary
float increment = 0.01f; //Incremental time in seconds
float gamma = 1.0f; //Time dilation, from the viewer's refrence frame

//Global Variables for BATTLEFIELD_DESIGN_MODE
int DESIGN_FUNCTION = BD_MAKE_SHAPES;
metastat CHOSEEN_COLOR = cl_cyan;
int SELECTED_MATERIAL = MATERIALS_BASIC_REFLECTIVE;

//Global Variables for the console
bool show_console = false;
vector<string> console_history;
string user_input = "";
int console_scroll = 0;

//Global settings (mostly debug)
bool show_corners = false; ///Debug: Draw the corners formed by wall intersections?
bool show_ticks = false; ///Finally found those annoying cursor ticks and made a setting for showing them.
bool enable_persprot = false; ///fun: enable the user to rotate their perspective with the middle mouse button

//Global Variables for BD_MAKE_SHAPES
bool show_layers = true; ///Art: Show the intuitive shape to explain what layer is being edited?
bool show_dots = true; ///Art: Show the dots in the currently edited shape
graphic art;
int Gindex = 0;
int dragdot = -1;


//Global Camera Position Data
float LookPointX = float(currentbattle.BoardWidth()) / 2.0f;
float LookPointY = float(currentbattle.BoardHeight()) / 2.0f;
float LookPointZ = BoardDepth / 2.0f;
const float PerspectiveRiseMax = PI / 2.0f - 0.0005f;
float PerspectiveOrbit = 0.0f;
float PerspectiveRise = PerspectiveRiseMax;
float PerspectiveDist = 7.5f;

//Global Custom Cursor Properties and Position
float CursorX = currentbattle.BoardWidth() / 2.0f;
float CursorY = currentbattle.BoardHeight() / 2.0f;
point CursorPosition(currentbattle.BoardWidth() / 2.0f, currentbattle.BoardHeight() / 2.0f);
const float CursorSensitivity = 0.3f;
float CursorSnap = 0.0f;
const float CursorTilt = PI / 4.0f;
float CursorRot = 0.0f;
const float CursorYaw = 0.0f; //0.2;
const float CursorSpreadDefault = PI / 8.0f; // PI / 12;
float CursorSpread = CursorSpreadDefault;
const float CursorSize = 0.4f;
float CursorRotSpeed = 0.3f; //0.05;
float CursorRed = 1.0f;
float CursorGreen = 1.0f;
float CursorBlue = 1.0f;

//Global Key Detections
const char keys[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' ',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
'W', 'X', 'Y', 'Z', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_',
'+', '=', '{', '}', '[', ']', '\\', '|', ';', ':', '"', '\'', ',', '<', '.', '>', '?', '\/', '`', '~' };
map<char, bool> normal_keysdown;

///Special and other keys
bool up_down = false;
bool left_down = false;
bool down_down = false;
bool right_down = false;
bool esc_down = false;
bool enter_down = false;
bool space_down = false;
bool backspace_down = false;
///For Numerals in Particular

//Global MouseButton Detections
bool leftclicking = false;
bool rightclicking = false;
bool middleclicking = false;
segment clickdragtrail(0.0f, 0.0f, 0.0f, 0.0f);
segment rightclicktrail(0.0f, 0.0f, 0.0f, 0.0f);

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
	int hueID = rand() % 12 + 1;
	return colorfromID(hueID);
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

void output_console(string output) {
	console_history.insert(console_history.begin(), output);
}

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

void rendertext(point location, string text) {
	point dot = location;
	
	// set position to text    
	glRasterPos2f(dot.x, dot.y);

	for (unsigned int i = 0; i < text.size(); i++)
	{
		// draw each character    
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
	//glTranslatef(-dot.x, -dot.y, 0.0f);
}

/*Always at (0,0) in the matrix, drawn along the x-axis, left bound is like the phase angle.
Resolution represents the number of points used to paint a single crest/trough. Appears clamped by bounds for aesthetic,
but only if passed TRUE for the clamping of that bound.*/
void draw_wave(sinusoid &wave, float leftbound, float rightbound, int resolution, bool clamp_left, bool clamp_right) {
	if (leftbound > rightbound) {
		float temp = rightbound;
		rightbound = leftbound;
		leftbound = temp;
	}
	float cycles = wave.frequency * (rightbound - leftbound);
	int nodes = floor(2 * cycles * (1 + resolution));
	float spacing = ( (1 / wave.frequency) / 2 ) / (resolution + 1);
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

//Like draw_wave but for a Fourier series ///Incomplete!!!!! ///May not do saw waves properly
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

void feedkeyboardinput(string &field) {
	for (char c : keys) {
		if (normal_keysdown[c]) {
			normal_keysdown[c] = false;
			field += c;
		}
	}
	if (backspace_down) {
		backspace_down = false;
		if (field.size() > 0)
			field.pop_back();
	}
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
	while (tickiterator < currentbattle.BoardWidth()-1)
	{
		tickiterator++;
		glVertex3f(tickiterator, currentbattle.BoardHeight(), 0.0f); glVertex3f(tickiterator, 0.0f, 0.0f); //X along y
		glVertex3f(tickiterator, 0.0f, 0.0f); glVertex3f(tickiterator, 0.0f, float(BoardDepth)); //X along Z
	}
	tickiterator = 0.0f;
	while (tickiterator < currentbattle.BoardHeight()-1) {
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

void setcolor(metastat col, float opacity) {
	float R = float(col.som) / 255.0f;
	float G = float(col.emo) / 255.0f;
	float B = float(col.cog) / 255.0f;
	glColor4f(R, G, B, opacity);
}

void drawshape(shape &obj) {
	setcolor(obj.color, obj.opacity);
	glLineWidth(obj.line_thickness);
	glBegin(obj.mode);
	for (point dot : obj.vertices) {
		glVertex2f(dot.x, dot.y);
	}
	glEnd();
}

void drawcursor() {
	glLineWidth(1.0f);
	//Draw the mouse-locator lines
	if (show_ticks) {
		float ticksize = 0.2f;
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		glVertex2f(CursorPosition.x, 0); glVertex2f(CursorPosition.x, ticksize);
		glVertex2f(CursorPosition.x, currentbattle.BoardHeight()); glVertex2f(CursorPosition.x, currentbattle.BoardHeight() - ticksize);
		glVertex2f(0, CursorPosition.y); glVertex2f(ticksize, CursorPosition.y);
		glVertex2f(currentbattle.BoardWidth(), CursorPosition.y); glVertex2f(currentbattle.BoardWidth() - ticksize, CursorPosition.y);
		glColor3f(CursorRed, CursorGreen, CursorBlue); //OH THERE IT IS I FOUND THE CURSOR COLOR LINE FINALLY
		glEnd();
	}
	if (DESIGN_FUNCTION == BD_MAKE_SHAPES)
		setcolor(inverse(art.pieces[Gindex].color), 1.0f);
	glPushMatrix();
	glTranslatef(CursorPosition.x, CursorPosition.y, 0);
	glRotatef(-90 + CursorTilt * 180 / PI, 0, 1, 0);
	glRotatef(CursorYaw * 180 / PI, 0, 0, 1);
	glRotatef(CursorRot * 180 / PI, 1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(CursorSize * cos(CursorSpread / 2), CursorSize * sin(CursorSpread / 2));
	glVertex2f(0, 0);
	glVertex2f(CursorSize * cos(CursorSpread / 2), CursorSize * -sin(CursorSpread / 2));
	///These lines make it look kind of like a cone
	//glVertex3f(0, 0, 0);
	//glVertex3f(CursorSize * cos(CursorSpread / 2), 0, CursorSize * -sin(CursorSpread / 2));
	//glVertex3f(0, 0, 0);
	//glVertex3f(CursorSize * cos(CursorSpread / 2), 0, CursorSize * sin(CursorSpread / 2));
	glVertex2f(0.0f, 0.0f); glVertex2f(0.8f * CursorSize / CursorSpread, 0.0f);
	glPopMatrix();
	glLoadIdentity();
	definecamera();
	glEnd();
}

void ClearScreen() {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}

void drawwall(wall drawing_wall) {
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

void rendertext(float x, float y, float z, void *font, string text) {
	glRasterPos3f(x, y, z);
	for (char c : text)
		glutBitmapCharacter(font, c);
}

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
		for (point dot : drawing_ray.getbits()) {
			drawpoint(dot);
			glColor3f(drawing_ray.getRed(), drawing_ray.getGreen(), drawing_ray.getBlue());
			rendertext(dot, to_string(i));
			i++;
		}
	}

	//Draw Arrowhead
	if (!drawing_ray.terminating) {
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

void drawfighter(combatant &fighter) {

}

void draw_art_GUI() { //Idea I just had: Every player metastat caps at 255; WHITE is the optimum in any area
	string headerstring = "ART MODE: ";
	rendertext(point(0.0f, currentbattle.map.height + 0.5f), headerstring);
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

void handle_input(string &input) {
	stringstream cons(input);
	string arg;
	cons >> arg;
	if (arg == "exit") {
		exit(0);
	}
	else if (arg == "dfunc") {
		cons >> arg;
		if (arg == "walls") {
			DESIGN_FUNCTION = BD_CREATE_WALLS;
			output_console("Set design function to BD_CREATE_WALLS");
		}
		else if (arg == "rays") {
			DESIGN_FUNCTION = BD_MAKE_RAYS;
			output_console("Set design function to BD_MAKE_RAYS");
		}
		else if (arg == "art") {
			DESIGN_FUNCTION = BD_MAKE_SHAPES;
			output_console("Set design function to BD_MAKE_SHAPES");
		}
		else
			output_console("Invalid Design Function: " + arg);
	}
	else if (arg == "save") {
		cons >> arg;
		if (arg == "battle") { //Save the currently designed battle to a preset file

		}
		else if (arg == "graphic") {
			cons >> arg;
			art.savetofile(arg);
			output_console("Wrote graphic to file " + arg);
		}
		else if (arg == "shape") { //Save the currently designed shape to a shape file
			cons >> arg;
			art.pieces[Gindex].savetofile(arg);
			output_console("Wrote shape #" + to_string(Gindex) + " to file " + arg);
		}
	}
	else if (arg == "load" || arg == "open") {
		cons >> arg;
		if (arg == "battle") { //Replace the current battle with a preset file

		}
		else if (arg == "graphic") {//Load a grpahic file and replace the current one with it
			cons >> arg;
			art = graphic(arg);
			output_console("Opened graphic " + arg);
		}
		else if (arg == "shape") { //Load a shape file into art.pieces
			cons >> arg;
			art.pieces.push_back(shape(arg, 0));
			output_console("Opened shape" + arg + " at end of Graphic shape vector");
		}
	}
	else if (arg == "canvas") {
		output_console("    " + to_string(art.pieces.size()) + " shapes");
		output_console("    Currently editing Shape #" + to_string(Gindex));
		for (unsigned int i = 0; i < art.pieces.size(); i++) {
			output_console("Shape #" + to_string(i));
			output_console("    Verticies: " + to_string(art.pieces[i].vertices.size()));
			output_console("    Color:     " + to_string(art.pieces[i].color.som) + " " + to_string(art.pieces[i].color.emo) + " " 
				+ to_string(art.pieces[i].color.cog));
			output_console("    Opacity:   " + to_string(art.pieces[i].opacity));
			output_console("    Rendering mode: " + art.pieces[i].getGLMODE());
			output_console("    Line Thickness: " + to_string(art.pieces[i].line_thickness));
		}
	}
	else if (arg == "shape") {
		cons >> arg;
		if (arg == "color") {
			cons >> art.pieces[Gindex].color.som;
			cons >> art.pieces[Gindex].color.emo;
			cons >> art.pieces[Gindex].color.cog;
			output_console("Set Shape #" + to_string(Gindex) + "'s color to " + to_string(art.pieces[Gindex].color.som) +
				" " + to_string(art.pieces[Gindex].color.emo) + " " + to_string(art.pieces[Gindex].color.cog));
		}
		else if (arg == "line_thickness") {
			cons >> art.pieces[Gindex].line_thickness;
			output_console("Set Shape #" + to_string(Gindex) + "'s line_thickness to "
				+ to_string(art.pieces[Gindex].line_thickness));
		}
		else if (arg == "opacity") {
			cons >> art.pieces[Gindex].opacity;
			output_console("Set Shape #" + to_string(Gindex) + "'s opacity to "
				+ to_string(art.pieces[Gindex].opacity));
		}
		else if (arg == "mode") {
			cons >> art.pieces[Gindex].mode;
			output_console("Set Shape #" + to_string(Gindex) + "'s drawing mode to "
				+ art.pieces[Gindex].getGLMODE());
		}
		else if (arg == "clear") {
			art.pieces[Gindex].vertices.clear();
			output_console("Cleared vertecies of Shape #" + to_string(Gindex));
		}
		else if (arg == "origin") {
			point O; cons >> O.x; cons >> O.y;
			art.pieces[Gindex].set_origin(O);
			output_console("Set origin to " + O.label());
		}
		else
			output_console("Invalid shape property: " + arg);
	}
	else if (arg == "graphic") {
		if (arg == "origin") {
			
		}
	}
	else if (arg == "layer") {
		cons >> Gindex;
		Gindex %= art.pieces.size();
		output_console("Now editing Shape #" + to_string(Gindex));
	}
	else if (arg == "perspective") {
		cons >> arg;
		if (arg == "orbit") {
			cons >> PerspectiveOrbit;
		}
		else if (arg == "rise") {
			cons >> PerspectiveRise;
			if (PerspectiveRise > PerspectiveRiseMax)
				PerspectiveRise = PerspectiveRiseMax;
		}
		else if (arg == "dist") {
			cons >> PerspectiveDist;
		}
	}
	else if (arg == "new") {
		cons >> arg;
		if (arg == "shape") {
			int loc; cons >> loc; loc %= art.pieces.size() + 1;
			if (loc <= Gindex)
				Gindex++;
			art.pieces.emplace(art.pieces.begin() + loc, shape());
			output_console("Added new Shape at position " + to_string(loc));
		}
		else {
			output_console("Invalid object type: " + arg);
		}
	}
	else if (arg == "toggle") {
		cons >> arg;
		if (arg == "show_corners") {
			show_corners = !show_corners;
			string set = show_corners ? "TRUE" : "FALSE";
			output_console("Set show_corners to " + set);
		}
		else if (arg == "show_layers") {
			show_layers = !show_layers;
			string set = show_layers ? "TRUE" : "FALSE";
			output_console("Set show_corners to " + set);
		}
		else if (arg == "enable_persprot") {
			enable_persprot = !enable_persprot;
			string set = enable_persprot ? "TRUE" : "FALSE";
			output_console("Set enable_persprot to " + set);
		}
		else if (arg == "show_dots") {
			show_dots = !show_dots;
			string set = show_dots ? "TRUE" : "FALSE";
			output_console("Set show_dots to " + set);
		}		
		else {
			output_console("Invalid setting: " + arg);
		}
	}
	else if (arg == "help") {
		output_console("	Here is a list of supported commands (may be outdated) (*unfinished):");
		output_console("dfunc <walls,rays,art> ................... Set the current design function");
		output_console("perspective <orbit,rise,dist> ............ Modify the camera perspective.");
		output_console("*toggle <setting> ......................... Toggle the sepecified bool setting");
		output_console("*save <battle,shape> <filename> ......... Save one of the elements of the current workshop");
		output_console("*load <battle,shape> <filename> ......... Load element from a file into the workshop");
		output_console("canvas ................................... View the information about the current canvas state");
		output_console("shape <property> <value> ............... Modify one of the properties of the current shape");
		output_console("shape clear ............................ Delete all vertices of the current shape");
		output_console("*new <shape,> ........................... Create a new object of the specified type");
		output_console("help ..................................... View a list of available console commands");
		output_console("exit ..................................... Exit the program without saving");
	}
	else {
		output_console("Invalid command: " + arg);
	}
}

void draw_console() {
	float consolespacing = 0.2f;
	int i = console_scroll;
	while (i < console_history.size()) {
		string line = console_history[i];
		//if (line[0] == '>')
		//	glColor3f(0.1f, 0.4f, 0.1f); //User background
		//else
		//	glColor3f(0.1f, 0.1f, 0.4f); //Computer background
		//rendertext(point(0.03f, float(i - console_scroll + 1) * consolespacing - 0.03f), line);
		if (line[0] == '>')
			glColor3f(0.8f, 0.8f, 0.8f); //User foreground
		else
			glColor3f(1.0f, 1.0f, 1.0f); //Computer foreground
		rendertext(point(0.0f, float(i - console_scroll + 1) * consolespacing), line);
		i++;
	}
	//glColor3f(0.0f, 0.5f, 0.0f); //Input background
	//rendertext(point(0.53f, -0.03f), ">" + user_input);
	glColor3f(0.8f, 0.8f, 0.8f); //Input foreground
	rendertext(point(0.5f, 0.0f), ">" + user_input);
	feedkeyboardinput(user_input);
	if (enter_down) { //Submit the commandline input if enter is hit
		enter_down = false;
		console_history.insert(console_history.begin(), ">" + user_input);
		handle_input(user_input);
		user_input = "";
	}

}

void renderScene(void) { //The custom function that tells openGL what to do when it refreshes
	Sleep(int(increment * 1000.0f * gamma));
	timer += increment;

	// Clear Color and Depth Buffers
	ClearScreen();

	// Reset transformations
	glLoadIdentity();

	// Set the camera to look at the appropriate spot (usually center of the board)
	definecamera();

	//Axis drawing code
	drawaxes();

	//Debug: Show timer
	rendertext(point(0.0f, 0.0f), to_string(timer));

	//Draw Battlefield Geometry
		//Draw Walls
	int walliterator = 0;
	while (walliterator < currentbattle.map.wallcount()) {
		drawwall(currentbattle.map.getwall(walliterator));
		walliterator++;
	}

	if (show_corners) {
		for (point corner : currentbattle.map.intersections()) {
			drawpoint(corner);
			glColor3f(1, 1, 1);
			/*drawsegment(reflective_bisector(point(3, 3),
				currentbattle.getmap().getwall(0).getbody(),
				currentbattle.getmap().getwall(1).getbody()));*/
		}
	}
		//Debug: object counts
	const bool show_objectcounts = false;
	if (show_objectcounts) {
		rendertext(point(0.0f, 4.0f), to_string(currentbattle.map.wallcount()) + " walls.");
		rendertext(point(0.0f, 5.0f), to_string(currentbattle.raycount()) + " rays.");
	}

	//Draw Combatants
	for (int i = 0; i < currentbattle.fighters.size(); i++) {
		drawfighter(currentbattle.fighters[i]);
	}

	//Draw Spells

	//Draw Rays
	int i = 0;
	while (i < currentbattle.raycount()) {
		drawray(currentbattle.rays[i]);
		i++;
	}

	//Controls for BATTLEFIELD_DESIGN_MODE
	if (normal_keysdown['h'] && !show_console) {
		if (normal_keysdown['1'])
			DESIGN_FUNCTION = BD_CREATE_WALLS;
		if (normal_keysdown['2'])
			DESIGN_FUNCTION = BD_MAKE_RAYS;
	}
	if (esc_down) {
		show_console = !show_console;
		esc_down = false;
	}
	if (BATLEFIELD_DESIGN_MODE) {
		vector<sinusoid> test_series = tri_series(floor(CursorPosition.y * 10)+1, 1, 0.25);
		glTranslatef(0.0f, 2.0f, 0.0f);
		draw_series(test_series, 2*timer, 2*timer + 8, 30, false, false);
		glTranslatef(0.0f, -2.0f, 0.0f);
		switch (DESIGN_FUNCTION) {
		case BD_CREATE_WALLS:
			if (clickdragtrail.length() != 0) {
				if (!leftclicking) {
					wall new_wall(clickdragtrail, SELECTED_MATERIAL, true);
					clickdragtrail.define(0, 0, 0, 0);
					currentbattle.construct_wall(new_wall);
				}
				else {
					wall new_wall(clickdragtrail, SELECTED_MATERIAL, true);
					drawwall(new_wall);
				}
			}
			if (rightclicktrail.length() != 0) {
				if (!rightclicking) {
					segment eraser = rightclicktrail;
					rightclicktrail.define(0, 0, 0, 0);
					int wallID = 0;
					for (wall checker : currentbattle.map.walls){
						if (isintersect(eraser, checker.body))
							currentbattle.destroy_wall(wallID);
						else
							wallID++;
					}
				}
				else {
					wall new_wall(rightclicktrail, MATERIALS_ERASER, true);
					drawwall(new_wall);
				}
			}
			break;
		case BD_ERASE_WALLS:
			break;
		case BD_MAKE_RAYS:
			if (clickdragtrail.length() != 0) {
				if (!leftclicking) {
					ray new_ray(randomhue(),clickdragtrail.p1,clickdragtrail.p2,clickdragtrail.length(), 
						2.0f, 2);
					clickdragtrail.define(0, 0, 0, 0);
					currentbattle.spawn_ray(new_ray);
				}
				else {
					wall new_wall(clickdragtrail, SELECTED_MATERIAL, true);
					drawwall(new_wall);
				}
			}
			break;
		case BD_MAKE_SHAPES:
			if (rightclicking) {
				for (int i = 0; i < art.pieces[Gindex].vertices.size(); i++) {
					if (difference(CursorPosition, art.pieces[Gindex].vertices[i]).magnitude() < 0.05) {
						art.pieces[Gindex].vertices.erase(art.pieces[Gindex].vertices.begin() + i);
						i--;
					}
				}
			}
			if (normal_keysdown['z']) {
				for (int i = 0; i < art.pieces[Gindex].vertices.size(); i++) {
					if (difference(CursorPosition, art.pieces[Gindex].vertices[i]).magnitude() < 0.05) {
						dragdot = i;
					}
				}
				if (dragdot != -1)
					art.pieces[Gindex].vertices[dragdot] = CursorPosition;
			}
			else
				dragdot = -1;
			if (clickdragtrail.length() != 0 || clickdragtrail.p1.x != 0.0f || clickdragtrail.p1.y != 0.0f) {
				if (!leftclicking && !normal_keysdown['z']) {
					art.pieces[Gindex].vertices.emplace_back(clickdragtrail.p2);
					clickdragtrail.define(0, 0, 0, 0);
					draw_art_GUI();
				}
				else {
					if (dragdot != -1)
						dragdot = -1;
					art.pieces[Gindex].vertices.emplace_back(clickdragtrail.p2);
					draw_art_GUI();
					art.pieces[Gindex].vertices.pop_back();
				}
			}
			else {
				draw_art_GUI();
			}
			break;

		}
	}

	//Draw console
	if (show_console)
		draw_console();

	//Draw Cursor
	drawcursor();
	CursorRot += (CursorRotSpeed / CursorSpread) * increment;

	glutSwapBuffers();

	//Process Iterative Behaviour
	if (down_down)
		PerspectiveRise -= 0.1f;
	if (up_down)
		PerspectiveRise += 0.1f;
	currentbattle.iterate(increment);
}

void ProcessNormalKeys(unsigned char key, int x, int y) {
	if (key == 27)
		esc_down = true;
	//if (key == 32)
		//space_down = true;
	if (key == 13)
		enter_down = true;
	if (key == 8)
		backspace_down = true;
	normal_keysdown[key] = true;
}

void ReleaseNormalKeys(unsigned char key, int x, int y) {
	if (key == 27)
		esc_down = false;
	//if (key == 32)
		//space_down = false;
	if (key == 13)
		enter_down = false;
	if (key == 8)
		backspace_down = false;
	normal_keysdown[key] = false;
}

void ReleaseSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		up_down = false;
		break;
	case GLUT_KEY_LEFT:
		left_down = false;
		break;
	case GLUT_KEY_DOWN:
		down_down = false;
		break;
	case GLUT_KEY_RIGHT:
		right_down = false;
		break;
	}
}

void ProcessSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		up_down = true;
		break;
	case GLUT_KEY_DOWN:
		down_down = true;
		break;
	case GLUT_KEY_RIGHT:
		right_down = true;
		break;
	case GLUT_KEY_LEFT:
		left_down = true;
		break;
	}
}

void MouseClick(int button, int state, int x, int y) { //Note that this is good for button press OR release
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftclicking = true;
			clickdragtrail.p1 = CursorPosition;
			clickdragtrail.p2 = CursorPosition;
		}
		else
		{
			leftclicking = false;
		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			rightclicking = true;
			rightclicktrail.p1 = CursorPosition;
		}
		else
			rightclicking = false;
	}
	if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN)
			middleclicking = true;
		else
			middleclicking = false;
	}
	if (button == 3 && state == GLUT_DOWN) { //4 means scrolling down
		if (show_console) {
			console_scroll++;
			if (console_scroll > console_history.size() - 1)
				console_scroll = console_history.size() - 1;
		}
	}
	if (button == 4 && state == GLUT_DOWN) { //3 means scrolling up
		if (show_console) {
			console_scroll--;
			if (console_scroll < 0)
				console_scroll = 0;
		}
	}
}

void ActiveMouseMove(int x, int y) {

}

void PassiveMouseMove(int x, int y) {
	int mousebase = 300;
	CursorSnap = 0;
	if (normal_keysdown['1'])
		CursorSnap = 1.0f;
	else if (normal_keysdown['2'])
		CursorSnap = 1.0f / 2.0f;
	else if (normal_keysdown['3'])
		CursorSnap = 1.0f / 4.0f;
	else if (normal_keysdown['4'])
		CursorSnap = 1.0f / 8.0f;
	float deltax = (x - mousebase) * CursorSensitivity / 30;
	float deltay = (mousebase - y) * CursorSensitivity / 30;
	if (CursorSnap != 0) {
		deltax *= 2;
		deltay *= 2;
	}
	if (CursorSnap != 0) {
		CursorPosition.x = roundf(CursorPosition.x / CursorSnap) * CursorSnap;
		CursorPosition.y = roundf(CursorPosition.y / CursorSnap) * CursorSnap;
	}
	if (!(middleclicking && enable_persprot)) {
		CursorPosition.x += deltax * cos(PerspectiveOrbit) - deltay * sin(PerspectiveOrbit);
		CursorPosition.y += deltay * cos(PerspectiveOrbit) + deltax * sin(PerspectiveOrbit);
		if (CursorPosition.x < 0)
			CursorPosition.x = 0;
		if (CursorPosition.x > currentbattle.BoardWidth())
			CursorPosition.x = currentbattle.BoardWidth();
		if (CursorPosition.y < 0)
			CursorPosition.y = 0;
		if (CursorPosition.y > currentbattle.BoardHeight())
			CursorPosition.y = currentbattle.BoardHeight();
	}
	if (middleclicking && enable_persprot) {
		PerspectiveOrbit -= deltax;
		PerspectiveRise -= deltay;
	}
	if (leftclicking) {
		clickdragtrail.p2 = CursorPosition;
	}
	if (rightclicking) {
		rightclicktrail.p2 = CursorPosition;
	}
	glutWarpPointer(mousebase, mousebase);
}

//main function
int main(int argc, char **argv) {
	//For development purposes
	///Tumbleweeds

	//Define the keyboard map
	for (char c : keys) {
		normal_keysdown.insert(pair<char, bool>(c, false));
	}

	output_console("Welcome to the Vector_Battle_System_Test_1.cpp Console");
	output_console("Type help for a list of commands.");

	//Initialize GLUT
	glutInit(&argc, argv);

	//Choose some settings for our Window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(900, 500);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//Create the Window
	glutCreateWindow("GLUTwindow");
	glutFullScreen();

	//Some settings
	glutIgnoreKeyRepeat(1);
	glutSetCursor(GLUT_CURSOR_NONE); //Hide the cursor

	//register callbacks
	glutDisplayFunc(renderScene); //Callback for when we refresh
	glutReshapeFunc(changeSize); //Callback for when window is resized
	glutIdleFunc(renderScene); //Callback for when the programs is idle (this will happen continuousCameraLookingY)
	glutKeyboardFunc(ProcessNormalKeys); //Callback pressing a "normal" key
	glutSpecialFunc(ProcessSpecialKeys); //Callback for a "special" key
	glutKeyboardUpFunc(ReleaseNormalKeys); //Callback for releasing "normal" keys
	glutSpecialUpFunc(ReleaseSpecialKeys); //Callback for releasing special keys
	glutMouseFunc(MouseClick); //Callback for mouse clicks
	glutMotionFunc(PassiveMouseMove); //Callback for mouse movement with button down
	glutPassiveMotionFunc(PassiveMouseMove); //Callback for mouse movement with no button down

	//enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}