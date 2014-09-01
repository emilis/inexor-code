/**
* Include engine header files
*/
#include "engine.h"

/**
* Include torus knot implementation
*/
#include "torusknot.h"


/**
* Constructor
*/
CTorusKnot::CTorusKnot() {
}


/**
* Destructor
*/
CTorusKnot::~CTorusKnot() {
	// nothing to destruct yet
}


/**
* Generate torus knot
* http://www.opengl.org/discussion_boards/showthread.php/140225-torus-knot-problem
*
*/
#define TORUSKNOT_PLOT_DISTANCE 100
void CTorusKnot::GenerateTorusKnot(float p, float q, float distance, float height, vec pos, float precision) 
{
	// output vector
	vec output;

	// Problems with pi?

	// Gnerate torus
	for(float phi=0.0f; phi<360.0f; phi+= 1.0f)
	{
		// Gnerate radius constant
		float r = cos(q * phi) + distance;

		// Gnerate output coordinates
		output.x = r * cos(p * phi) + pos.x;
		output.y = r * sin(p * phi) + pos.y;
		output.z = - sin(q * phi) + pos.z;

		// store computed point
		m_ComputedPoints.push_back( output );
	}
}


/**
* Render torus knot
*/
void CTorusKnot::RenderTorusKnot(void) 
{
}