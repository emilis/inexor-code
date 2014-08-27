#include "engine.h"
#include "curverenderer.h"


// yeah its C++11, not everywhere available yet
#define nullptr 0

/**
* Set curve to null in constructor
*/
CCurveRenderer::CCurveRenderer() {
	m_pCurve = nullptr;
}

/**
* Destructor
*/
CCurveRenderer::~CCurveRenderer() {
	// nothing to destruct yet
}

/**
* Store curve pointer
*/
void CCurveRenderer::SetCurve(CBezierCurve *curve) 
{
	if(curve == nullptr) return;
	// store curve pointer
	m_pCurve = curve;
}



/**
* Render curve
*/
void CCurveRenderer::RenderCurve(void) 
{
	Shader* shader = lookupshaderbyname("stdworld");

	glPushMatrix();
	// use standard shader
	shader->set();
	// begin render process
	glBegin(GL_LINES);
	
	// what is blending?
	glEnable(GL_BLEND);

	glColor4f(1.0f, 0.0f, 1.0f, 0.75f);

	// loop through computed points and render them	
	for(unsigned int i=0; i<m_pCurve->m_ComputedPoints.size()   /*!!!*/ -1 ; i++) 
	{
		vec* cur = &m_pCurve->m_ComputedPoints[i];
		vec* next = &m_pCurve->m_ComputedPoints[i  +1];

		// render path using standard particles
		particle_text( vec(m_pCurve->m_ComputedPoints[i].x,
							m_pCurve->m_ComputedPoints[i].y,
							m_pCurve->m_ComputedPoints[i].z) 
							, "H", PART_TEXT, 1, 0xFF00B2, 10.0f);
		
		// set vertices
		glVertex3d(cur->x, cur->y, cur->z);
		glVertex3d(next->x, next->y, next->z);
		
	}
	// end render process
	glEnd();
	glPopMatrix();
}