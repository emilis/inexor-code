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
	// loop through computed points and render them	
	for(unsigned int i=0; i<m_pCurve->m_ComputedPoints.size(); i++) {
		// render as default particle
		particle_text( vec(m_pCurve->m_ComputedPoints[i].x,
							m_pCurve->m_ComputedPoints[i].y,
							m_pCurve->m_ComputedPoints[i].z) 
							, "HELLOWORLD", PART_TEXT, 1, 0xFF00B2, 10.0f);
	}
}