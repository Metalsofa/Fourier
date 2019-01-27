/* This class, player, is not the one used in battle. It keeps track of metastats outside of battle.
It is possible that a join purpose will be be served by it, but for now, this is an outside-of-battle
thing, to be called by the function that initiates battle */
//Looks like this file will just be a general thing for my game.
#define players
#include <vector>
#include <fstream>
#include "crypt.h"
#include <sstream>
#include <map>

using namespace std;



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
	int sum() {return (som + emo + cog);}
	
	//Returns the mean of all stat components
	int mean() { return (sum() / 3);}
	
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
	void savetofile(string filename) { //DP: Seems to be repetitive with cryptogram, just make 1 function. Pass by ref
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

	void set_origin(point org) {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] = difference(vertices[i], org);
		}
	}
	//Returns the GL drawing mode associated with this shape
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
	}
	//Returns the diagonal segment making up the bounds of this shape
};

class graphic {
public:
	vector<shape> pieces; //This alone gives the graphic form.
	int delay = 0; //Optional member, only useful if this graphic is in an animation.

	//Set the origin for this graphic
	void set_origin(point org) {
		for (int i = 0; i < pieces.size(); i++) {
			pieces[i].set_origin(org);
		}
	}

	//Default constructor
	graphic() {
		pieces.emplace_back(shape());
	}

	//Initialize this graphic from a file
	graphic(string filename) {
		vector<string> contents = unencrypted_contents(filename, "The Doors of Perception");
		if (contents.size() > 0) {
			for (string line : contents) {
				pieces.emplace_back(shape(line));
			}
		}
	}

	//Save this graphic to a file
	void savetofile(string filename) {
		vector<string> lines;
		for (shape piece : pieces) {
			lines.emplace_back(piece.cryptogram());
		}
		encrypt_and_overwrite(lines, filename, "The Doors of Perception");
	}
};

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
};

class spritesheet {
public:
	map<string, animation> animations;
	map<string, graphic> graphics;
};

//This version of player is for overworld logic and battle initiation
class player {
private:
	//Player metastats cap out at 255
	metastat maxHP; //maximum HP
	metastat maxFP; //maximum FP
	metastat HP; //present HP
	metastat FP; //present FP
	metastat vitality; //like constitution
	metastat sensitivity; //Inverse of DEF
	metastat agility; //DEX
	metastat power; //ATK
	metastat resilience; //Benefit from healing
	metastat stability; //Reduces rate of life loss

	//Abilities also need to somehow appear
public:
	int Hitpoints(int comp) { return HP.component(comp); }
	int MaxHitpoints(int comp) { return maxHP.component(comp); }
	int HitpointsFraction(int comp) { return Hitpoints(comp) / MaxHitpoints(comp); }
	int Functionpoints(int comp) { return FP.component(comp); }
	int MaxFunctionpoints(int comp) { return maxFP.component(comp); }
	int FunctionpointsFraction(int comp) { return Functionpoints(comp) / MaxFunctionpoints(comp); }
	int Vitality(int comp) { return vitality.component(comp); }
	int Sensitivity(int comp) { return sensitivity.component(comp); }
	int Agility(int comp) { return agility.component(comp); }
	int Power(int comp) { return power.component(comp); }
	int Resilience(int comp) { return resilience.component(comp); }
	int Stability(int comp) { return stability.component(comp); }
};

//all players AND enemies are one of these
class combatant {
public:
	player statblock;
	point position;
	//Sprites
	
	//Each segment represents a hitbox; p1 is lower left, p2 is upper right.
	vector<segment> hitboxes;
};

#define cl_white metastat(255, 255, 255) /*White*/
#define cl_black metastat(0, 0, 0) /*Black*/
#define cl_red metastat(255,0,0) /*Red*/
#define domain_red "action"
#define cl_orange metastat(255,127,0) /*Orange*/
#define domain_orange "artistry"
#define cl_yellow metastat(255,255,0) /*Yellow*/
#define domain_yellow "expression"
#define cl_lime metastat(127,255,0) /*Lime*/
#define domain_lime "evocation"
#define cl_green metastat(0,255,0) /*Green*/
#define domain_green "emotion"
#define cl_teal metastat(0,255,127) /*Teal*/
#define domain_teal "introspection"
#define cl_cyan metastat(0,255,255) /*Cyan*/
#define domain_cyan "belief"
#define cl_indigo metastat(0,127,255) /*Indigo*/
#define domain_indigo "heuristic"
#define cl_blue metastat(0,0,255) /*Blue*/
#define domain_blue "cognition"
#define cl_purple metastat(127,0,255) /*Purple*/
#define domain_purple "synthesis"
#define cl_magenta metastat(255,0,255) /*Magenta*/
#define domain_magenta "application"
#define cl_violet metastat(255,0,127) /*Violet*/
#define domain_violet "operation"