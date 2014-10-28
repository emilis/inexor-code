/**
* Include bezier curve header file
* and Sauerbraten engine file
*/
#include "engine.h"
#include "bezier.h"

/**
* The more points you add as parameter points, the tinier
* the computed values will become! float has a limited precision
* (although its IEEE). Dont use more than [BEZIER_CURVE_PARAMETER_POINT_LIMIT] points!
* The actual limit should be somewhere around 32
* but it would be too slow and to inaccurate
*
* If you use 50 for example, some point values
* will approximate 0!
*/
#define BEZIER_CURVE_PARAMETER_POINT_LIMIT 20

/**
* Define bezier curve's default precision
* 100.0f should fit very well
*/
#define BEZIER_CURVE_DEFAULT_PRECISION 100.0f


/**
* Constructor
*/
CBezierCurve::CBezierCurve()
{
	// Curve os not computed in the beginning
	m_bComputed = false;
	// Set default precision
	m_fCalcPrecision = BEZIER_CURVE_DEFAULT_PRECISION; 
	// set parameter point limit
	m_uiParamLimit = BEZIER_CURVE_PARAMETER_POINT_LIMIT;
}


/**
* Destructor
*/
CBezierCurve::~CBezierCurve() 
{
}





/* ---------------------------------------------- ADD POINT METHODS ----------------------------------------------------- */
/**
* Try to add parameter point to curve
*/
void CBezierCurve::AddParamPoint(vec point, float weight) 
{
	// check if parameter point limit was reached or not
	if(m_ParameterPoints.size() < m_uiParamLimit) 
	{
		SPoint p;
		p.pos = point;
		// apply weight
		p.weight = weight;
		// push back (= add) parameter point
		m_ParameterPoints.push_back( p );
	}
}


/**
* Add parameter (overloaded)
*/
void CBezierCurve::AddParamPoint(float x, float y, float z, float weight) 
{
	// check if parameter point limit was reached or not
	if(m_ParameterPoints.size() < m_uiParamLimit) 
	{
		SPoint p;
		p.pos = vec(x,y,z);
		// apply weight
		p.weight = weight;
		// push back (= add) parameter point
		m_ParameterPoints.push_back( p);
	}
}
/* ---------------------------------------------- ADD POINT METHODS ----------------------------------------------------- */









/* ---------------------------------------------- CLEAR FUNCTIONS ----------------------------------------------------- */
/**
* Reset both input and output vector of this curve
*/
void CBezierCurve::ClearAllPoints(void) 
{
	// clear parameter points
	m_ParameterPoints.clear();
	// clear computed points
	m_ComputedPoints.clear();
}


/**
* Clear only computed points vector
*/
void CBezierCurve::ClearComputedPoints(void) 
{
	// clear computed points
	m_ComputedPoints.clear();
}


/**
* Clear parameter points vector
*/
void CBezierCurve::ClearParameterPoints(void) 
{
	// clear parameter points
	m_ParameterPoints.clear();
}
/* ---------------------------------------------- CLEAR FUNCTIONS ----------------------------------------------------- */










/* ---------------------------------------------- RANDOM CURVE GENERATION ----------------------------------------------------- */
/**
* Generate random curve
*/
void CBezierCurve::GenerateRandomCurve(unsigned int maxparameterpoints, bool autocalculate) 
{
	// break if too many parameter were passed
	if(maxparameterpoints > m_uiParamLimit) return;

	// Initialise random number generator
	srand((unsigned)time(NULL) + maxparameterpoints * SDL_GetTicks() );

	// Generate at most 100 points with random positions
	for(unsigned int i=0; i<maxparameterpoints;  i++) 
	{
		SPoint p;
		// Generate positions
		p.pos.x = rand() % 1024 - rand() % 1024 + 1024; 
		p.pos.y = rand() % 1024 - rand() % 1024 + 1024;
		p.pos.z = rand() % 1024 - rand() % 1024 + 1024;

		// Generate random weight
		p.weight = 1.0f;

		// Add point
		m_ParameterPoints.push_back( p );
	}
	// Calculate curve
	if(autocalculate) CalculateCurve_BernsteinPolynom();
}
/* ---------------------------------------------- RANDOM CURVE GENERATION ----------------------------------------------------- */











/* ---------------------------------------------- GET FUNCTIONS ----------------------------------------------------- */
/**
* Get point number # from computed curve
*/
vec CBezierCurve::GetComputedPointIndexed(unsigned int index) 
{
	// return null vector if setup is invalid
	if(index >= m_ComputedPoints.size() || m_bComputed == false) return vec(0.0f, 0.0f, 0.0f);
	// return point
	return vec( m_ComputedPoints[index].x,m_ComputedPoints[index].y ,m_ComputedPoints[index].z );
}

/**
* Get parameter point number # from computed curve
*/
vec CBezierCurve::GetParameterPointIndexed(unsigned int index) 
{
	// return null vector if setup is invalid
	if(index >= m_ParameterPoints.size() || m_bComputed == false) return vec(0.0f, 0.0f, 0.0f);
	// return point
	return vec( m_ParameterPoints[index].pos.x,m_ParameterPoints[index].pos.y ,m_ParameterPoints[index].pos.z );
}

/**
* This technique is much faste to use
* You can acquire an index according to the float value's progress of the curve
* If position is 0, you get the first index of the computed points vector
* If position is 1, you get the last index of the computed points vector
*/
unsigned int CBezierCurve::GetPointIndexFromFloat(float position) 
{
	// return null vector if setup is invalid
	if(m_bComputed == false) return 0;

	/**
	* Try to find the current position
	* cut off the max value via modulo 
	* in order not to access arrays out of memory 
	*/
	return ceil( (float)position*m_ComputedPoints.size() );
}

/**
* Render current point position
*/
vec CBezierCurve::CalculatePointFromFloat(float curveposition) 
{
	if(m_bComputed == false) return vec(0.0f,0.0f,0.0f);
	// return calculated position
	return calculateposition(curveposition);
}
/* ---------------------------------------------- GET FUNCTIONS ----------------------------------------------------- */










/* ---------------------------------------------- BERNSTEIN ALGORITHM ----------------------------------------------------- */
/**
* binomial coefficient (for bernstein polynom)
*/
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
* Calculate exact point using bernstein polynoms
*/
/**
*	IMPORTANT NOTE
*	THIS FUNCTION REQUIRES A LOT OF CPU TIME!
*	TRY TO USE THE CACHING TECHNIQUE IN ORDER NOT
*	COMPUTE THIS CURVE IN REAL TIME. THAT WOULD BE REALLY SLOW!
*/
vec CBezierCurve::calculateposition(float position)
{
	// computed point
	vec finished_point;

	// reset point's coordinates
	finished_point.x = 0.0f;
	finished_point.y = 0.0f;
	finished_point.z = 0.0f;

	// the number of parameter points given
	int uiElements = m_ParameterPoints.size() -1;

	// apply fPos to our bernstein polynom					
	for(int i=0; i<=uiElements; i++) 
	{
		// compute bezier coordinates using bernstein polynoms
		finished_point.x += bernsteinposition(m_ParameterPoints[i].pos.x, i, position, uiElements, m_ParameterPoints[i].weight);
		finished_point.y += bernsteinposition(m_ParameterPoints[i].pos.y, i, position, uiElements, m_ParameterPoints[i].weight);
		finished_point.z += bernsteinposition(m_ParameterPoints[i].pos.z, i, position, uiElements, m_ParameterPoints[i].weight);
	}

	// return finished point
	return finished_point;
}

/**
* Bernstein polynom
*/
float CBezierCurve::bernsteinposition(float value, int currentelement, float position, int elementcount, float weight)
{
	// avoid crash
	if(position >= 1.0f) position= 0.999f;
	/**
	* Return bernstein polynom
	*/
	return binomialCoef(elementcount, currentelement) // bonomial term
				* pow(position, currentelement) // polynomial term 
				* pow((1-position), (elementcount-currentelement)) // polynomial term
				* value  // actual value
				* weight; // weight
}

/**
* Calculate (Method 1)
*/
void CBezierCurve::CalculateCurve_BernsteinPolynom(void) 
{
	// Calculate precision
	float fStep = 1.0f / m_fCalcPrecision;

	// go along our curve in fPos steps
	for( float fPos=0.0f;  fPos <= 1.0f;  fPos+=fStep) 
	{
		// add computed point
		m_ComputedPoints.push_back( calculateposition(fPos) );
	}

	// curve computed!
	m_bComputed = true;
}
/* ---------------------------------------------- BERNSTEIN ALGORITHM ----------------------------------------------------- */












/* ---------------------------------------------- COMPUTATION WITH DE CASTELJAU ALGORITHM ----------------------------------------------------- */
/*
* deCasteljau's algorithm is recursive!
*/
vec CBezierCurve::decasteljau(int index, int nextindex, float t0)
{
	// If index is the first index, stop computing
	if(0 == index) return m_ParameterPoints[index].pos;

	/**
	* No overloaded operators for 
	* Sauerbratens vec class??
	* ... seriously?
	*/
	vec output;
	output = decasteljau(index - 1, nextindex, t0).mul(1 - t0);
	output.add( decasteljau(index - 1, nextindex + 1, t0).mul(t0) );
	//return (1 - t0) * decasteljau(index - 1, nextindex, t0) + t0 * decasteljau(index - 1, nextindex + 1, t0);
	return output;
}

/**
* Calculate (Method 2)
*/
void CBezierCurve::CalculateCurve_DeCasteljauRecursive(void) 
{
	// DeCasteljau berechnen lassen
	for(unsigned int i=0; i<1.0f / m_fCalcPrecision; i++) 
	{
		// Recursively compute points
		vec p = decasteljau(0, m_ParameterPoints.size(), i/m_fCalcPrecision);
		// Die vorberechneten Punkte aufnehmen
		m_ComputedPoints.push_back( p );
	}

	// curve computed!
	m_bComputed = true;
}
/* ---------------------------------------------- COMPUTATION WITH DE CASTELJAU ALGORITHM ----------------------------------------------------- */








/* ---------------------------------------------- LIMIT METHODS ----------------------------------------------------- */
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
/* ---------------------------------------------- LIMIT METHODS ----------------------------------------------------- */




/**
* Is Curve computed? [Read only!]
*/
bool CBezierCurve::IsCurveComputed(void) {
	return m_bComputed;
}


/**
* Make curve global
*/
CBezierCurve dynamic_curve, target_curve;
