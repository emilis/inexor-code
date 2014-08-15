#include "ray.h"

namespace tux {

ray::ray(std::vector<float> from, std::vector<float> dir, int range, int spread) {
	this->from = from;
	this->range = range;
}

}
