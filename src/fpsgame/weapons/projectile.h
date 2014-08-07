#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "shot.h"

namespace tux {

// basic prjectile strucure
struct projectile;

// different types of projectiles
struct projectile_static;
struct projectile_steered;
struct projectile_phys;



struct projectile {

	std::vector<float> dir;
	std::vector<float> pos;

	projectile();

	virtual void update();
};


extern std::list<projectile*> projectiles;

} /* namespace tux */

#endif /* WEAPON_H_ */
