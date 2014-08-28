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
	// abort if curve is not computed yet
	if(m_pCurve->IsCurveComputed() == false) return;

	glPushMatrix();
	// begin render process
	glBegin(GL_LINES);
	
	// Set color to red
	glColor3f(1.0f, 0.0f, 0.0f);

	// Set line thickness
	glLineWidth(15.0f);

	// loop through computed points and render them	
	for(unsigned int i=0; i<m_pCurve->m_ComputedPoints.size()   /*!!!*/ -1 ; i++) 
	{
		vec* cur = &m_pCurve->m_ComputedPoints[i];
		vec* next = &m_pCurve->m_ComputedPoints[i  +1];
				
		// set vertices
		glVertex3f(cur->x, cur->y, cur->z);
		glVertex3f(next->x, next->y, next->z);
		
	}
	// end render process
	glEnd();
	glPopMatrix();
}


/**
* Render big point at the current position
*/
void CCurveRenderer::RenderPoint(vec point) 
{
	glPushMatrix();
	glBegin(GL_POINT);
	
	// set big point
	glPointSize(100.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(point.x, point.y, point.z);

	glEnd();
	glPopMatrix();
}

/**
* Render parameter points
*/
void CCurveRenderer::RenderParameterPoints(void) 
{
	// loop through computed points and render them	
	for(unsigned int i=0; i<m_pCurve->m_ParameterPoints.size(); i++) 
	{
		// render text
		particle_text( m_pCurve->GetParameterPointIndexed(i), "H", PART_TEXT, 1, 0x00FF19, 10.0f);
	}
}