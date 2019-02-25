/*The purpose of this header file is to implement methods associated with
user-friendly fgr manipulation*/
#pragma once
#ifndef __graphics_h
#define __graphics_h

#include "geometry.h"
#include "crypt.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>



//Here we define the class metastat, which holds three values, in RGB order.

class metastat {
public:
	int som;
	int emo;
	int cog;
	metastat(int somatic_value, int emotional_value, int cognitive_value) {
		som = somatic_value;
		emo = emotional_value;
		cog = cognitive_value;
	}

	metastat() {
		som = 0;
		emo = 0;
		cog = 0;
	}

	void define(int new_som, int new_emo, int new_cog) {
		som = new_som;
		emo = new_emo;
		cog = new_cog;
	}

	//Returns the sum of all stat components
	int sum() { return (som + emo + cog); }

	//Returns the mean of all stat components
	int mean() { return (sum() / 3); }

	//1: Somatic 2: Emotional 3: Cognitive 4: Mean else: sum
	int component(int comp) {
		if (comp == 1)
			return som;
		if (comp == 2)
			return emo;
		if (comp == 3)
			return cog;
		if (comp == 4)
			return mean();
		else //DP: sum()? mean()*3?
			return (som + emo + cog);
	}
};

//255 - each stat's value for 
metastat inverse(metastat &base) {
	metastat inv;
	inv.som = 255 - base.som;
	inv.emo = 255 - base.emo;
	inv.cog = 255 - base.cog;
	return inv;
	//DP: concise ver:
	//return metastat(255-base.som, 255-base.emo, 255-base.cog);
}


class shape {
public:
	vector<point> vertices;
	metastat color;
	float opacity;
	int mode;
	float line_thickness;

	//Returns the shape's encrypted string
	string cryptogram() {
		string translation;
		translation = to_string(vertices.size()); ///The first argument is an integer counting the number of verticies
		for (int i = 0; i < vertices.size(); i++) {
			translation += " ";
			translation += to_string(vertices[i].x);
			translation += " ";
			translation += to_string(vertices[i].y);
			//DP:
			//translation += " "
			//	+ to_string(vertices[i].x)
			//	+ " "
			//	+ to_string(vertices[i].y);
		}
		translation += " ";
		translation += to_string(color.som) + " ";
		translation += to_string(color.emo) + " ";
		translation += to_string(color.cog) + " ";
		translation += to_string(opacity) + " ";
		translation += to_string(mode) + " ";
		translation += to_string(line_thickness) + " ";
		return translation;
		//DP: return translation + " "
			//+ to_string(color.som) + " "
			//+ to_string(color.emo) + " "
			//+ to_string(color.cog) + " "
			//+ to_string(opacity) + " "
			//+ to_string(mode) + " "
			//+ to_string(line_thickness) + " ";
	}

	//Write this shape's contents to a text file
	void savetofile(string& filename) { //DP: Seems to be repetitive with cryptogram, just make 1 function. Pass by ref
		vector<string> translation(2, "");
		translation[0] = to_string(vertices.size()); ///The first argument is an integer counting the number of verticies
		for (int i = 0; i < vertices.size(); i++) {
			translation[0] += " ";
			translation[0] += to_string(vertices[i].x);
			translation[0] += " ";
			translation[0] += to_string(vertices[i].y);
		}
		translation[0] += " ";
		translation[0] += to_string(color.som) + " ";
		translation[0] += to_string(color.emo) + " ";
		translation[0] += to_string(color.cog) + " ";
		translation[0] += to_string(opacity) + " ";
		translation[0] += to_string(mode) + " ";
		translation[0] += to_string(line_thickness) + " ";
		encrypt_and_overwrite(translation, filename, "The Doors of Perception");
	}

	//Default constructor
	shape() {
		color = metastat(60, 60, 60);
		opacity = 1.0f;
		mode = 5;
		line_thickness = 1.0f;
	}

	//Initialize a shape by reading it from a string
	shape(string text) { //DP: Good use of stringstream, pass by ref
		///Container 0: vertices
		stringstream reader(text);
		int vertexcount;
		reader >> vertexcount;
		for (int j = 0; j < vertexcount; j++) { //DP: Might want to declare x and y outside loop
			float X; reader >> X;
			float Y; reader >> Y;
			point vert(X, Y);
			vertices.emplace_back(vert);
		}
		///Extract color
		reader >> color.som; reader >> color.emo; reader >> color.cog;
		///Extract opacity
		reader >> opacity;
		///Extract mode
		reader >> mode;
		///Extract line_thickness
		reader >> line_thickness;
	}

	//Initialize a shape by reading it from a file, telling it where to begin reading
	shape(string filename, int begin) { //DP: Pass string by ref, repetitive with func above
		vector<string> shape_contents = unencrypted_contents(filename, "The Doors of Perception");
		if (shape_contents.size() >= 1) {
			///Container 0: vertices
			stringstream reader(shape_contents[begin]);
			int vertexcount;
			reader >> vertexcount;
			for (int j = 0; j < vertexcount; j++) {//DP: Might want to declare x and y outside loop
				float X; reader >> X;
				float Y; reader >> Y;
				point vert(X, Y);
				vertices.emplace_back(vert);
			}
			///Extract color
			reader >> color.som; reader >> color.emo; reader >> color.cog;
			///Extract opacity
			reader >> opacity;
			///Extract mode
			reader >> mode;
			///Extract line_thickness
			reader >> line_thickness;
		}
	}
	//Translate every vertex relative to the origin
	void set_origin(point org) {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] = difference(vertices[i], org);
		}
	}
	//Scale this shape around the origin
	void rescale(float scaling_factor) {
		for (unsigned long i = 0; i < vertices.size(); i++) {
			vertices[i] *= scaling_factor;
		}
	}
	//Scale this shape strictly in the x-dimension around the origin
	void rescale_x(float scaling_factor) {
		for (unsigned long i = 0; i < vertices.size(); i++) {
			vertices[i].x *= scaling_factor;
		}
	}
	//Scale this shape strictly in the y-dimension around the origin
	void rescale_y(float scaling_factor) {
		for (unsigned long i = 0; i < vertices.size(); i++) {
			vertices[i].y *= scaling_factor;
		}
	}
	//Returns the diagonally-spanning segment for this shape
	const segment bounds() const {
		segment rets = segment(vertices[0], vertices[1]);
		//Iterate through every point and set the bounding segment accordingly
		for (unsigned int i = 0; i < vertices.size(); i++) {
			//Check rightbounds
			if (vertices[i].x > rets.p2.x) {
				rets.p2.x = vertices[i].x;
			}
			//Check topbounds
			if (vertices[i].y > rets.p2.y) {
				rets.p2.y = vertices[i].y;
			}
			//Check leftbounds
			if (vertices[i].x < rets.p1.x) {
				rets.p1.x = vertices[i].x;
			}
			//Check bottombounds
			if (vertices[i].y < rets.p1.y) {
				rets.p1.y = vertices[i].y;
			}
		}
		//Now rets is the bounding segment for this shape!
		return rets;
	}
	//Returns the name of the GL drawing mode associated with this shape
	string getGLMODE() {
		switch (mode % 10) {
		case 0:
			return "GL_POINTS";
		case 1:
			return "GL_LINES";
		case 2:
			return "GL_LINE_LOOP";
		case 3:
			return "GL_LINE_STRIP";
		case 4:
			return "GL_TRIANGLES";
		case 5:
			return "GL_TRIANGLE_STRIP";
		case 6:
			return "GL_TRIANGLE_FAN";
		case 7:
			return "GL_QUADS";
		case 8:
			return "GL_QUAD_STRIP";
		case 9:
			return "GL_POLYGON";
		}
		cerr << "Something happened in players.h/shape/getGLMODE that isn't supposed to..." << endl;
		return "ERR_NOT_GLMODE";
	}
};

class graphic {
public:
	vector<shape> pieces; //This alone gives the graphic form.
	int delay = 0; //Optional member, only useful if this graphic is in an animation.

	//Set the origin for this graphic
	void set_origin(point org) { //DP: Pass by ref?
		for (int i = 0; i < pieces.size(); i++) {
			pieces[i].set_origin(org);
		}
	}

	//Rescale this graphic about the origin 
	void rescale(float scaling_factor) {
		//Iterate through this graphic's component shapes and rescale each one
		for (unsigned long i = 0; i < pieces.size(); i++) {
			pieces[i].rescale(scaling_factor);
		}
	}

	//Rescale this graphic only in the x-dimension about the origin
	void rescale_x(float scaling_factor) {
		//Iterate through this graphic's component shapes and rescale each one
		for (unsigned long i = 0; i < pieces.size(); i++) {
			pieces[i].rescale_x(scaling_factor);
		}
	}

	//Rescale this graphic only in the y-dimension about the origin
	void rescale_y(float scaling_factor) {
		//Iterate through this graphic's component shapes and rescale each one
		for (unsigned long i = 0; i < pieces.size(); i++) {
			pieces[i].rescale_y(scaling_factor);
		}
	}

	//Get this graphic's bounding box
	const segment bounds() {
		//Declare the segment to be returned
		segment rets = pieces[0].bounds();
		//Use each shape's bounding box to find this shape's bounding box!
		for (unsigned int i = 0; i < pieces.size(); i++) {
			//This way we don't have to keep calling 'bounds'
			const segment checker = pieces[i].bounds();
			//Check rightbounds
			if (checker.p2.x > rets.p2.x) {
				rets.p2.x = checker.p2.x;
			}
			//Check topbounds
			if (checker.p2.y > rets.p2.y) {
				rets.p2.y = checker.p2.y;
			}
			//Check leftbounds
			if (checker.p1.x < rets.p1.x) {
				rets.p1.x = checker.p1.x;
			}
			//Check bottombounds
			if (checker.p1.y < rets.p1.y) {
				rets.p1.y = checker.p1.y;
			}
		}
		//Return the augmented segment
		return rets;
	}

	//Squeezes this graphic between the lines y = 0 and y = 1, making it the standard width for one of these graphics
	void standardize() {
		//Make this graphic flush with the x and y axes
		set_origin(bounds().p1);
		//Rescale this graphic to be flush with y = 1
		rescale(1 / (bounds().p2.x));
	}

	//Fit this object to a new bounding box, breaking aspect ratio as neccessary
	void fit_stretch(segment new_bounds) {
		//These calculatoins all work best from the origin point
		standardize();
		//Rescale horizontally
		rescale_x(new_bounds.p2.x);
		//Rescale vertically
		rescale_y(new_bounds.p2.y);
		set_origin(new_bounds.p1 * -1.0f);
	}

	//Fit this object within a new bounding box without breaking the aspect ratio
	void fit_within(segment new_bounds) {
		//These calculatoins all work best from the origin point
		standardize();
		//Rescale to fit with x
		rescale(new_bounds.p2.x - new_bounds.p1.x);
		//If it's too tall,
		if (bounds().height() > new_bounds.height()) {
			//Rescale rationally to correct height
			rescale(new_bounds.height() / bounds().height());
		}
		//Move the object to where it goes
		//First, let the object's origin be the center of its bounding box
		set_origin(bounds().midpoint());
		//Finally, move the graphic to where it belongs
		set_origin(new_bounds.midpoint() * -1.0f);
	}

	//Fit this object to fill a new bounding box without breaking the aspect ratio
	void fit_without(segment new_bounds) {
		//These calculatoins all work best from the origin point
		standardize();
		//Rescale to fit with x
		rescale(new_bounds.p2.x - new_bounds.p1.x);
		//If it isn't tall enough,
		if (bounds().height() < new_bounds.height()) {
			//Rescale rationally to correct height
			rescale(new_bounds.height() / bounds().height());
		}
		//Move the object to where it goes
		//First, let the object's origin be the center of its bounding box
		set_origin(bounds().midpoint());
		//Finally, move the graphic to where it belongs
		set_origin(new_bounds.midpoint() * -1.0f);
	}

	//Default constructor
	graphic() {
		pieces.emplace_back(shape());
	}

	//Initialize this graphic from a file
	graphic(string filename) {  //DP: Pass by ref?
		vector<string> contents = unencrypted_contents(filename, "The Doors of Perception");
		if (contents.size() > 0) {//DP: Don't need the if statement
			for (string line : contents) {
				pieces.emplace_back(shape(line));
			}
		}
	}

	//Save this graphic to a file
	void savetofile(string filename) { //DP: Pass by ref?
		vector<string> lines;
		for (shape piece : pieces) {
			lines.emplace_back(piece.cryptogram());
		}
		encrypt_and_overwrite(lines, filename, "The Doors of Perception");
	}

	//Translate this graphic into a string
	string filestring() {
		vector<string> sublines;
		for (unsigned int i = 0; i < pieces.size(); i++) {
			sublines.push_back(pieces[i].cryptogram());
		}
		string rets = "{\n";
		for (unsigned int i = 0; i < sublines.size(); i++) {
			rets += "\t\t" + sublines[i] + ";\n";
		}
		rets.push_back('\t');
		rets.push_back('}');
		return rets;
	}
};

/*Structures multiple graphics into an animated sprite*/
class animation {
public:
	vector<graphic> frames;
	int framenumber; //Dynamic, meant for cycling through frames
	int frameclock; //Dynamic, meant for counting how long it's been on this frame.
	bool cycle; //Should this animation restart after it ends?

	animation() {
		framenumber = 0;
		frameclock = 0;
		cycle = true;
	}

	graphic& operator[] (int frame) {
		return frames[frame];
	}

	void tick() {
		if (frameclock < frames[framenumber].delay) {
			frameclock++;
		}
		else {
			frameclock = 0;
			if (framenumber < frames.size() - 1)
				framenumber++;
			else if (cycle) {
				framenumber = 0;
			}
		}
	}
	//Translate this animation to an (unencrypted) string
	string filestring() {
		vector<string> graphica;
		for (unsigned int i = 0; i < frames.size(); i++) {
			graphica.push_back(frames[i].filestring());
		}
		string rets = "{\n";
		for (unsigned int i = 0; i < graphica.size(); i++) {
			rets += "\t" + graphica[i] + '\n';
		}
		rets.push_back('}');
	}

	//Translate an unencrypted string into an animation
	animation(string fstring) {

	}
};


//To be included as a member in animated objects
class spritesheet {
public:
	map<string, animation> animations;
	map<string, graphic> graphics;

	//Translate this entire spritesheet into a big (unencrypted) string
	string cryptogram() {
		/*Uh oh I have no Idea how to iterate through maps*/
		
	}
};


#endif