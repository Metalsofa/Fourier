/*This header file is home to a lot of console, design, and debug functionality.
In theory, nothing that the player will need during gameplay should be in here.*/
#pragma once
#ifndef __console_h
#define __console_h

#include "gl/glut.h"
#include "battle.h"
#include "customGL.h"
#include "players.h"
#include "controls.h"


void feedkeyboardinput(string &field) {
	for (char c : keys) { //Not sure how to eliminate this for loop
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

void output_console(string output) {
	//console_history.insert(console_history.begin(), output); //Trying something else less intensive
	cout << output << endl;
}

//Process an input string
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
			if (cons >> arg) {
				art.savetofile(arg + ".fgr");
				output_console("Wrote graphic to file " + arg + ".fgr");
			}
			else {
				art.savetofile(current_graphic_name + ".fgr");
				output_console("Graphic saved");
			}
		}
		else if (arg == "shape") { //Save the currently designed shape to a shape file
			cons >> arg;
			art.pieces[Gindex].savetofile(arg);
			output_console("Wrote shape #" + to_string(Gindex) + " to file " + arg + ".fgr");
		}
	}
	else if (arg == "load" || arg == "open") {
		cons >> arg;
		if (arg == "battle") { //Replace the current battle with a preset file

		}
		else if (arg == "graphic") {//Load a grpahic file and replace the current one with it
			cons >> arg;
			art = graphic(arg + ".fgr");
			output_console("Opened graphic " + arg);
			current_graphic_name = arg;
		}
		else if (arg == "shape") { //Load a shape file into art.pieces
			cons >> arg;
			art.pieces.push_back(shape(arg + ".fgr", 0));
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
		cons >> arg;
		if (arg == "standardize") {
			art.standardize();
		}
		if (arg == "origin") {
			point O; cons >> O.x; cons >> O.y;
			art.set_origin(O);
		}
		if (arg == "expand") {
			//Rescale to take up most of the height of the canvas
			art.fit_within(segment(currentbattle.BoardWidth() * 0.1, currentbattle.BoardHeight() * 0.1,
				currentbattle.BoardWidth() * 0.9, currentbattle.BoardHeight() * 0.9));
		}
		if (arg == "clear") {
			Gindex = 0;
			art = graphic();
			output_console("Cleared shapes of graphic");
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
		output_console("Invalid command: " + arg); //DP: Should probably add "type help for a list of commands"
	}
}

//Pause the game and activate the exterior console for user input.
void exterior_console() {
	string inp;
	while (true) {
		if (inp == "q" || inp == "b" || inp == "back") {
			break;
		}
		getline(cin, inp);
		handle_input(inp);
	}
}

void draw_console() {
	/*
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
		//console_history.insert(console_history.begin(), ">" + user_input);
		handle_input(user_input);
		user_input = "";
	}
	*/

}


#endif