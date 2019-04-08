//Include global variables
#include "globals.h"

#include "console.h"

using namespace std;
using namespace fgr;

//Put into the string any characters being pressed on the keyboard
void feedkeyboardinput(string &field) {
	for (map<char, bool>::iterator itr = normalKeysdown.begin(); itr != normalKeysdown.end(); itr++) {
		if (itr->second) {
			itr->second = false;
			field += itr->first;
		}
	}
	if (backspacePress) {
		backspacePress = false;
		if (field.size() > 0)
			field.pop_back();
	}
}

//Send a message to the console
void outputConsole(string output) {
	//console_history.insert(console_history.begin(), output); //Trying something else less intensive
	cout << output << endl;
}

//Process an input string
void handleInput(string &input) {
	stringstream cons(input);
	string arg;
	cons >> arg;
	if (arg == "exit") {
		exit(0);
	} else if (arg == "dfunc") {
		cons >> arg;
		if (arg == "walls") {
			DESIGN_FUNCTION = BD_CREATE_WALLS;
			outputConsole("Set design function to BD_CREATE_WALLS");
		} else if (arg == "rays") {
			DESIGN_FUNCTION = BD_MAKE_RAYS;
			outputConsole("Set design function to BD_MAKE_RAYS");
		} else if (arg == "art") {
			DESIGN_FUNCTION = BD_MAKE_SHAPES;
			outputConsole("Set design function to BD_MAKE_SHAPES");
		} else
			outputConsole("Invalid Design Function: " + arg);
	} else if (arg == "save") {
		cons >> arg;
		if (arg == "battle") { //Save the currently designed battle to a preset file
			///PROTOTYPE
		} else if (arg == "graphic") {
			if (cons >> arg) {
				art.savetofile(arg + ".fgr");
				outputConsole("Wrote graphic to file " + arg + ".fgr");
			} else {
				art.savetofile(currentGraphicName + ".fgr");
				outputConsole("Graphic saved");
			}
		} else if (arg == "shape") { //Save the currently designed shape to a shape file
			cons >> arg;
			art[editingLayer].savetofile(arg);
			outputConsole("Wrote shape #" + to_string(editingLayer) + " to file " + arg + ".fgr");
		}
	} else if (arg == "load" || arg == "open") {
		cons >> arg;
		if (arg == "battle") { //Replace the current battle with a preset file

		} else if (arg == "graphic") {//Load a grpahic file and replace the current one with it
			cons >> arg;
			art = graphic(arg + ".fgr");
			outputConsole("Opened graphic " + arg);
			currentGraphicName = arg;
		} else if (arg == "shape") { //Load a shape file into art
			cons >> arg;
			art.push_back(shape(arg + ".fgr", 0));
			outputConsole("Opened shape" + arg + " at end of Graphic shape vector");
		}
	} else if (arg == "canvas") {
		outputConsole("    " + to_string(art.size()) + " shapes");
		outputConsole("    Currently editing Shape #" + to_string(editingLayer));
		for (unsigned int i = 0; i < art.size(); i++) {
			outputConsole("Shape #" + to_string(i));
			outputConsole("    Verticies: " + to_string(art[i].size()));
			outputConsole("    Color:     " + to_string(art[i].color.getLevel('r')) + " " + to_string(art[i].color.getLevel('g')) + " "
				+ to_string(art[i].color.getLevel('b')));
			outputConsole("    Opacity:   " + to_string(art[i].opacity));
			outputConsole("    Rendering mode: " + string(art[i].glModeString()));
			outputConsole("    Line Thickness: " + to_string(art[i].lineThickness));
		}
	} else if (arg == "shape") {
		cons >> arg;
		if (arg == "color") {
			/*cons >> art[editingLayer].color.getLevel('r');
			cons >> art[editingLayer].color.getLevel('g');
			cons >> art[editingLayer].color.getLevel('b');*/
			outputConsole("Set Shape #" + to_string(editingLayer) + "'s color to " + to_string(art[editingLayer].color.getLevel('r')) +
				" " + to_string(art[editingLayer].color.getLevel('g')) + " " + to_string(art[editingLayer].color.getLevel('b')));
		} else if (arg == "lineThickness") {
			cons >> art[editingLayer].lineThickness;
			outputConsole("Set Shape #" + to_string(editingLayer) + "'s lineThickness to "
				+ to_string(art[editingLayer].lineThickness));
		} else if (arg == "opacity") {
			cons >> art[editingLayer].opacity;
			outputConsole("Set Shape #" + to_string(editingLayer) + "'s opacity to "
				+ to_string(art[editingLayer].opacity));
		} else if (arg == "mode") {
			cons >> art[editingLayer].mode;
			outputConsole("Set Shape #" + to_string(editingLayer) + "'s drawing mode to "
				+ art[editingLayer].glModeString());
		} else if (arg == "clear") {
			art[editingLayer].clear();
			outputConsole("Cleared vertecies of Shape #" + to_string(editingLayer));
		} else if (arg == "origin") {
			float Ox; cons >> Ox;
			float Oy; cons >> Oy;
			point O(Ox, Oy);
			art[editingLayer].setOrigin(O);
			outputConsole("Set origin to " + O.label());
		} else
			outputConsole("Invalid shape property: " + arg);
	} else if (arg == "graphic") {
		cons >> arg;
		if (arg == "standardize") {
			art.standardize();
		}
		if (arg == "origin") {
			float Ox; cons >> Ox;
			float Oy; cons >> Oy;
			point O(Ox, Oy);
			art.setOrigin(O);
		}
		if (arg == "expand") {
			//Rescale to take up most of the height of the canvas
			art.fitWithin(segment(currentbattle.boardWidth() * 0.1f, currentbattle.boardHeight() * 0.1f,
				currentbattle.boardWidth() * 0.9f, currentbattle.boardHeight() * 0.9f));
		}
		if (arg == "clear") {
			editingLayer = 0;
			art = graphic();
			outputConsole("Cleared shapes of graphic");
		}
	} else if (arg == "layer") {
		cons >> editingLayer;
		editingLayer %= art.size();
		outputConsole("Now editing Shape #" + to_string(editingLayer));
	} else if (arg == "perspective") {
		cons >> arg;
		if (arg == "orbit") {
			cons >> PerspectiveOrbit;
		} else if (arg == "rise") {
			cons >> PerspectiveRise;
			if (PerspectiveRise > PerspectiveRiseMax)
				PerspectiveRise = PerspectiveRiseMax;
		} else if (arg == "dist") {
			cons >> PerspectiveDist;
		}
	} else if (arg == "new") {
		cons >> arg;
		if (arg == "shape") {
			int loc; cons >> loc; loc %= art.size() + 1;
			if (loc <= editingLayer)
				editingLayer++;
			art.emplace(art.begin() + loc, shape());
			outputConsole("Added new Shape at position " + to_string(loc));
		} else {
			outputConsole("Invalid object type: " + arg);
		}
	} else if (arg == "toggle") {
		cons >> arg;
		if (arg == "showCorners") {
			showCorners = !showCorners;
			string set = showCorners ? "TRUE" : "FALSE";
			outputConsole("Set showCorners to " + set);
		} else if (arg == "showLayers") {
			showLayers = !showLayers;
			string set = showLayers ? "TRUE" : "FALSE";
			outputConsole("Set showCorners to " + set);
		} else if (arg == "enablePersprot") {
			enablePersprot = !enablePersprot;
			string set = enablePersprot ? "TRUE" : "FALSE";
			outputConsole("Set enablePersprot to " + set);
		} else if (arg == "showDots") {
			showDots = !showDots;
			string set = showDots ? "TRUE" : "FALSE";
			outputConsole("Set showDots to " + set);
		} else {
			outputConsole("Invalid setting: " + arg);
		}
	} else if (arg == "help") {
		outputConsole("	Here is a list of supported commands (may be outdated) (*unfinished):");
		outputConsole("dfunc <walls,rays,art> ................... Set the current design function");
		outputConsole("perspective <orbit,rise,dist> ............ Modify the camera perspective.");
		outputConsole("*toggle <setting> ......................... Toggle the sepecified bool setting");
		outputConsole("*save <battle,shape> <filename> ......... Save one of the elements of the current workshop");
		outputConsole("*load <battle,shape> <filename> ......... Load element from a file into the workshop");
		outputConsole("canvas ................................... View the information about the current canvas state");
		outputConsole("shape <property> <value> ............... Modify one of the properties of the current shape");
		outputConsole("shape clear ............................ Delete all vertices of the current shape");
		outputConsole("*new <shape,> ........................... Create a new object of the specified type");
		outputConsole("help ..................................... View a list of available console commands");
		outputConsole("exit ..................................... Exit the program without saving");
	} else {
		outputConsole("Invalid command: " + arg); //DP: Should probably add "type help for a list of commands"
	}
}

//Pause the game and activate the exterior console for user input.
void exteriorConsole() {
	string inp;
	while (true) {
		if (inp == "q" || inp == "b" || inp == "back") {
			break;
		}
		getline(cin, inp);
		handleInput(inp);
	}
}

void drawConsole() {
	/*
	float consolespacing = 0.2f;
	int i = consoleScroll;
	while (i < console_history.size()) {
		string line = console_history[i];
		//if (line[0] == '>')
		//	glColor3f(0.1f, 0.4f, 0.1f); //User background
		//else
		//	glColor3f(0.1f, 0.1f, 0.4f); //Computer background
		//rendertext(point(0.03f, float(i - consoleScroll + 1) * consolespacing - 0.03f), line);
		if (line[0] == '>')
			glColor3f(0.8f, 0.8f, 0.8f); //User foreground
		else
			glColor3f(1.0f, 1.0f, 1.0f); //Computer foreground
		rendertext(point(0.0f, float(i - consoleScroll + 1) * consolespacing), line);
		i++;
	}
	//glColor3f(0.0f, 0.5f, 0.0f); //Input background
	//rendertext(point(0.53f, -0.03f), ">" + userInput);
	glColor3f(0.8f, 0.8f, 0.8f); //Input foreground
	rendertext(point(0.5f, 0.0f), ">" + userInput);
	feedkeyboardinput(userInput);
	if (enterPress) { //Submit the commandline input if enter is hit
		enterPress = false;
		//console_history.insert(console_history.begin(), ">" + userInput);
		handleInput(userInput);
		userInput = "";
	}
	*/
}