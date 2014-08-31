#ifndef RAY_H_
#define RAY_H_

#include "shot.h"

namespace tux {

struct ray;
struct scatter;

struct ray {
	ray(std::vector<float> from, std::vector<float> dir, int range, int spread);

	std::vector<float> from, dir, range;
};

struct scatter {
	scatter(std::vector<float> from, std::vector<float> dir, int range, int spread, int amount);

	std::vector<ray*> rays;
};

} /* namespace tux */

#endif /* RAY_H_ */
