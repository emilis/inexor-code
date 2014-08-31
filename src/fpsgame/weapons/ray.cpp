#include "ray.h"

namespace tux {

ray::ray(std::vector<float> from, std::vector<float> dir, int range, int spread) {
	this->from = from;
	this->range = range;
}

scatter::scatter(std::vector<float> from, std::vector<float> dir, int range, int spread, int amount) {
	this->rays = new std::vector[amount];
	for(int i = 0; i < amount; i++)
		this->rays[i] = new ray(from, dir, range, spread);
}

} /* namespace tux */
