//Spells implementation file
#include "globals.h"
#include "spells.h"

//Check if a spell of a given color can pass through a given material
int permitted(const metastat& spellColor, const metastat& permittivity)  { //DP: Pass by ref
	//These 3 lines of code return 0 (for 'kill') if any component of the ray is not permitted.
	if (permittivity.som == 0 && spellColor.som) return 0;
	if (permittivity.emo == 0 && spellColor.emo) return 0;
	if (permittivity.cog == 0 && spellColor.cog) return 0;
	//These 3 lines of cour return 1 (for 'bounce') if any component of the ray is to be bounced.
	if (permittivity.som == 1 && spellColor.som) return 1;
	if (permittivity.emo == 1 && spellColor.emo) return 1;
	if (permittivity.cog == 1 && spellColor.cog) return 1;
	//If no return has happened by now, the ray is permitted by this permittivity.
	return 2;
}
