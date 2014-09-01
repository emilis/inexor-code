/**
* Torus knots are functions which are plotted in 
* 3D space in a radial coordinate system
* Its possible that these functions could create
* nice effects for cameras, particles or physics
*/


/**
* Include guards protects the file from being included twice
*/
#ifndef ENGINE_TORUS_KNOTS_H
#define ENGINE_TORUS_KNOTS_H

/**
* Include the most common standard libraries
*/
#include <string>
#include <map>
#include <vector>
#include <list>
#include <deque>
#include <iterator>

/**
* Basic torus knot implementation
*/
class CTorusKnot 
{
	public:

	// constructor
	CTorusKnot();
	// destructor
	~CTorusKnot();

	/**
	* A standard vector which contains the computed points
	*/
	std::vector<vec> m_ComputedPoints;

	// Generate knot
	void GenerateTorusKnot(float p, float q, float distance, float height, vec pos, float precision = 10.0f);

	// Render knot
	void RenderTorusKnot(void);
};

// Declaration
extern CTorusKnot torus_knoten;


#endif /* #ifdef ENGINE_TORUS_KNOTS_H */