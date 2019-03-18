#include "spells.h"
#include "materials.h"
#include "battle.h"

wall::wall(segment definingsegment, Material wallmaterial, bool isfixed) {
	body = definingsegment;
	materialtype definingMatarial(wallmaterial);
	material = definingMatarial;
	fixed = isfixed;
}

