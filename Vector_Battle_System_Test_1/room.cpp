//Implementation file for certain overworld classes

#include "room.h"

prop::prop() : fgr::spritesheet() {

}

void prop::interact() {
	interact_func(interactionCount, *this);
	interactionCount++;
}

