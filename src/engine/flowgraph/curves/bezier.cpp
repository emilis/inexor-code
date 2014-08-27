/**
* Include bezier curve header file
* and Sauerbraten engine file
*/
#include "engine.h"
#include "bezier.h"


/**
* Constructor
*/
CBezierCurve::CBezierCurve() {
	// curve not computed in the beginning
	m_bComputed = false;
	// set default precision
	m_fCalcPrecision = 30.0f; // 1.0f up to 20.0f ?
	// set parameter point limit
	m_uiParamLimit = 7;
}

/**
* Destructor
*/
CBezierCurve::~CBezierCurve() {
	// nothing to destruct (yet)
}

/**
* Try to add parameter point to curve
*/
void CBezierCurve::AddParamPoint(vec point) 
{
	// check if parameter point limit was reached or not
	if(m_ParameterPoints.size() < m_uiParamLimit) {
		// push back (= add) parameter point
		m_ParameterPoints.push_back( point);
	}
}

void CBezierCurve::AddParamPoint(float x, float y, float z) 
{
	// check if parameter point limit was reached or not
	if(m_ParameterPoints.size() < m_uiParamLimit) 
	{
		// push back (= add) parameter point
		m_ParameterPoints.push_back( vec(x,y,z));
	}
}

/*
* Reset both input and output vector of this curve
*
*/
void CBezierCurve::ClearPoints(void) 
{
	// clear input buffer
	m_ParameterPoints.clear();
	// clear output buffer
	m_ComputedPoints.clear();
}

// binomial coefficient for bernstein polynom
unsigned int CBezierCurve::binomialCoef(unsigned int n, const unsigned int k)
{
	unsigned int r = 1;
	if(k > n) return 0;
	for(unsigned int d = 1; d <= k; d++) {
		r *= n--;
		r /= d;
	}
	return r;
}

/**
* Generate random curve
*/
void CBezierCurve::GenerateRandomCurve(unsigned int maxparameterpoints) 
{
	// Initialise random number generator
	srand((unsigned)time(NULL) + SDL_GetTicks() );

	// Generate at most 100 points with random positions
	for(unsigned int i=0; i<maxparameterpoints;  i++) 
	{
		vec point;
		
		// Generate positions
		point.x = rand() % 512 - rand() % 512 + 512;
		point.y = 550 + rand() % 512;
		point.z = rand() % 512 + rand() % 512 + 512;

		// Debug messages
		//#define BEZ_CURVE_TEST
		#ifdef BEZ_CURVE_TEST
			conoutf(CON_DEBUG, "%f %f %f", point.x, point.y, point.z);
		#endif

		// Add point
		m_ParameterPoints.push_back(point);
	}
}


/**
* Get point number # from computed curve
*/
vec CBezierCurve::GetComputedPointIndexed(unsigned int index) 
{
	// return null vector if setup is invalid
	if(index > m_ComputedPoints.size() || m_bComputed == false) return vec(0.0f, 0.0f, 0.0f);
	// return point
	return vec( m_ComputedPoints[index].x,m_ComputedPoints[index].y ,m_ComputedPoints[index].z );
}
/**
* Get parameter point number # from computed curve
*/
vec CBezierCurve::GetParameterPointIndexed(unsigned int index) 
{
	// return null vector if setup is invalid
	if(index > m_ParameterPoints.size() || m_bComputed == false) return vec(0.0f, 0.0f, 0.0f);
	// return point
	return vec( m_ParameterPoints[index].x,m_ParameterPoints[index].y ,m_ParameterPoints[index].z );
}


/**
* Calculate (Method 1)
*/
void CBezierCurve::CalculateCurve_BernsteinPolynom(void) 
{
	// Calculate precision
	float fStep = 1.0f / m_fCalcPrecision;

	// debug messages
	//#define CURVE_RENDERER_DEBUG_MODE
	#ifdef CURVE_RENDERER_DEBUG_MODE
		conoutf(CON_DEBUG, "Precision: %f", fStep);
	#endif
	
	// go along our curve in fPos steps
	for( float fPos=0.0f;  fPos <= 1.0f;  fPos+=fStep) 
	{
		// add computed point
		m_ComputedPoints.push_back(  calculateposition(fPos) );
	}

	// curve computed!
	m_bComputed = true;
}



/**
* Calculate exact point
*/
vec CBezierCurve::calculateposition(float position)
{
	// computed point
	vec finished_point;
	finished_point.x = 0.0f;
	finished_point.y = 0.0f;
	finished_point.z = 0.0f;

	// the number of parameter points given
	int uiElements = m_ParameterPoints.size() -1;

	// apply fPos to our bernstein polynom					
	for(int i=0; i<=uiElements; i++) 
	{
		// compute bezier coordinates using bernstein polynoms
		finished_point.x += binomialCoef( uiElements, i) * pow( position, i) * pow(  (1-position),  (uiElements-i) ) * m_ParameterPoints[ i ].x;
		finished_point.y += binomialCoef( uiElements, i) * pow( position, i) * pow(  (1-position),  (uiElements-i) ) * m_ParameterPoints[ i ].y;
		finished_point.z += binomialCoef( uiElements, i) * pow( position, i) * pow(  (1-position),  (uiElements-i) ) * m_ParameterPoints[ i ].z;
	}

	// return finished point
	return finished_point;
}


/**
* Render current point position
*/
vec CBezierCurve::CalculatePointFromFloat(float curveposition) 
{
	// return calculated position
	return calculateposition(curveposition);
}


/**
* Calculate (Method 2)
*/
void CBezierCurve::CalculateCurve_DeCasteljauRecursive(void) {
	// curve computed!
	m_bComputed = true;
}

// set parameter point limit
void CBezierCurve::SetParamPointLimit(unsigned int limit) {
	// set limit
	m_uiParamLimit = limit;
}

// set final curve points' precision
// We should not compute it in real time...
void CBezierCurve::SetPrecision(float precision) {
	// set precision
	m_fCalcPrecision = precision;
}