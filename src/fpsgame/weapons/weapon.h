#ifndef WEAPON_H_
#define WEAPON_H_

#include "game.h"
#include <vector>
#include <string>

namespace tux {

struct weapon;


struct weapon {

	// we fire this
	shot *shot;

	std::string hudgun;
	std::string config;

};

extern std::vector<weapon*> weapons;

} /* namespace tux */

#endif /* WEAPON_H_ */
