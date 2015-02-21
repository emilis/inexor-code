/// @file BezierCurve.cpp
/// @author Johannes "Hanni" Schneider
/// @date 18.02.2015
/// @brief Basic implementation of Bezier curves

/// http://pomax.github.io/bezierinfo/
/// http://en.wikipedia.org/wiki/B%C3%A9zier_curve
/// http://mathworld.wolfram.com/BezierCurve.html
/// http://theagsc.com/community/tutorials/so-whats-the-big-deal-with-horizontal-vertical-bezier-handles-anyway#comment-1351842776
/// http://learn.scannerlicker.net/2014/04/16/bezier-curves-and-type-design-a-tutorial/
/// https://geom.ivd.kit.edu/downloads/pubs/pub-boehm-prautzsch_2002_preview.pdf
/// https://www.clear.rice.edu/comp360/lectures/BezSubd.pdf


/// Include engine header file
#include "BezierCurve.h"

/// Default parameter point limit
/// More parameter points will require more memory!
/// Please keep in mind that 4 points are more than enough
/// Every complex curve can be splitted into curve parts!
#define BEZIER_CURVE_DEFAULT_PARAMETER_POINT_LIMIT 4

/// Default precision with which cached points will be precomputed
/// Higher numbers will lead to increased precision, because one step is 1/PRECISION
#define BEZIER_CURVE_DEFAULT_PRECISION 50.0f // HIGH!!


/// constructor
CBezierCurve::CBezierCurve()
{	
	m_bComputed = false;
	SetCachedCurvePrecision(BEZIER_CURVE_DEFAULT_PRECISION);
	SetParamPointLimit(BEZIER_CURVE_DEFAULT_PARAMETER_POINT_LIMIT);
}


/// destructor
CBezierCurve::~CBezierCurve()
{
	// nothing to destroy...
	// (he could not do this anyway!)
}



/// set parameter point (input) limit
void CBezierCurve::SetParamPointLimit(unsigned int input_point_limit)
{
	m_uiParamLimit = input_point_limit;
}

/// set the precision of the computed (cached) curve data
void CBezierCurve::SetCachedCurvePrecision(float precision)
{
	m_fCalcPrecision = precision;
}



/// add a parameter point vector
/// standard value for weight coefficient is 1.0f
void CBezierCurve::AddParamPoint(vec point, float weight)
{
	// check if parameter point limit was reached or not
	if(m_ParameterPoints.size() < m_uiParamLimit) 
	{
		SPointStructure p;

		// I want to thank eihrul here for not overloading the damn =+/-*() operators...
		p.x = point.x;
		p.y = point.y;
		p.z = point.z;

		// apply weight
		p.weight = weight;

		// push back (= add) parameter point
		m_ParameterPoints.push_back( p );
	}
}

/// add a parameter point (by passing his coordinates)
/// standard value for weight coefficient is 1.0f
void CBezierCurve::AddParamPoint(float x, float y, float z, float weight)
{
	// check if parameter point limit was reached or not
	if(m_ParameterPoints.size() < m_uiParamLimit) 
	{
		SPointStructure p;
		
		// copy point coordinates and weight
		p.x = x;
		p.y = y;
		p.z = z;
		p.weight = weight;

		// push back (= add) parameter point
		m_ParameterPoints.push_back(p);
	}
}



/// reset all data
void CBezierCurve::ClearAllPoints(void)
{
	m_ParameterPoints.clear();
	m_ComputedPoints.clear();
}

/// clear all cached points
void CBezierCurve::ClearComputedPoints(void)
{
	m_ComputedPoints.clear();
}

/// clear all parameter points
void CBezierCurve::ClearParameterPoints(void)
{
	m_ParameterPoints.clear();
}



/// is the curve already computed?
bool CBezierCurve::IsCurveComputed(void)
{
	return m_bComputed;
}

/// precompute curve to cache
/// @param algo the algorithm which will be used to precompute the curve
/// @see m_ComputedPoints
void CBezierCurve::PreComputeCache(BEZIER_ALGORITHM algorithm)
{
	switch(algorithm)
	{
		case ALGORITHM_BERNSTEIN_POLYNOM:
			CalculateCurveCacheWithBernsteinPolynoms();
			break;
		
		case ALGORITHM_DECASTELJAU:
			CalculateCurveCacheWithDeCasteljau();
			break;
	}
}

/// get a point from the cached curve
/// @return returns the point which is approximately nearest this position
vec CBezierCurve::GetPointFromCache(float curvepos)
{
	/// abort if curve has not been precomputed yet
	if(!IsCurveComputed()) return vec(0.0f,0.0f,0.0f);

	/// @bug maybe this could be improved!
	/// @warning not tested on many platforms (yet)!
	/// @warning Hanack is probably going to kill me for using old C-type casts (instead of killing me for using static_cast<>)
	return m_ComputedPoints[ ceil((float)curvepos*m_ComputedPoints.size()) ];
}




/// "One interesting property of Bézier curves is that an nth order curve can always be perfectly represented by an (n+1)th order curve, by giving the higher order curve specific control points."
/// "[...] and one of the more amusing observations about Bézier curves is that their derivatives are, in fact, also Bézier curves"
/// "[...] or, in plain text: the derivative of an nth degree Bézier curve is an (n-1)th degree Bézier curve" 
/// http://pomax.github.io/bezierinfo/
/// https://github.com/Pomax
/// -Mike "Pomax" Kamerams


/// REALTIME calculation

/// @warning Realtime calculation is more flexible but relies on more memory/time!
/// @param curveposition the interpolated position on the curve from 0.0f to 1.0f (beginning to end)
/// @param algorithm 
vec CBezierCurve::CalcRealtimePos(float curveposition, BEZIER_ALGORITHM algorithm)
{
	vec output;
	switch(algorithm)
	{
		case ALGORITHM_BERNSTEIN_POLYNOM:
			output = calculate_bernstein_coordinates(curveposition);
			break;
		
		case ALGORITHM_DECASTELJAU:
			output = calculate_de_casteljau_coordinate(0, m_ParameterPoints.size(), curveposition/m_fCalcPrecision);
			break;
	}
	return output;
}




// DE CASTELJAU ALGORITHM

void CBezierCurve::CalculateCurveCacheWithDeCasteljau(void)
{	
	if(m_ParameterPoints.size() < 2) return;

	SPreComputedPointStructure out;
	for(float curvepos=0.0f; curvepos<=1.0f; curvepos+= 1.0f / m_fCalcPrecision)
	{
		/// where to begin
		int end_number = 0;
		int begin_number = m_ParameterPoints.size()-1;
		
		/// calculate point
		vec p = calculate_de_casteljau_coordinate(begin_number, end_number, curvepos);

		/// copy coordiantes
		out.x = p.x;
		out.y = p.y;
		out.z = p.z;

		/// Add point to computed points list
		m_ComputedPoints.push_back(out);
	}
}

vec CBezierCurve::calculate_de_casteljau_coordinate(int index, int nextindex, float curvepos)
{
	// If index is the first index, stop computing
	if(0 == index) return m_ParameterPoints[nextindex]; // return the first parameter
		
	// Return position
	return calculate_de_casteljau_coordinate(index -1, nextindex, curvepos) *(1 - curvepos) + calculate_de_casteljau_coordinate(index-1, nextindex+1, curvepos) * curvepos;
}


/// BERNSTEIN

void CBezierCurve::CalculateCurveCacheWithBernsteinPolynoms(void) 
{
	if(m_ParameterPoints.size() < 2) return;

	/// calculate step value using curve precision coefficient
	float fStep = 1.0f / m_fCalcPrecision;

	// go along our curve in fPos steps
	for(float fPos=0.0f;  fPos <= 1.0f;  fPos+=fStep) 
	{
		/// compute a new point on the curve
		SPreComputedPointStructure ComputedPoint =  calculate_bernstein_coordinates(fPos);
		/// add the cached point to the curve
		m_ComputedPoints.push_back(ComputedPoint);
	}
	/// curve computed!
	m_bComputed = true;
}

/// [proteced] Helper function to generate coordinates
SPreComputedPointStructure CBezierCurve::calculate_bernstein_coordinates(float t)
{
	SPreComputedPointStructure p;
	int n = m_ParameterPoints.size() - 1;

	/// calculate coordinates			
	/// @warning if i is UNsigned, pow() function is ambiguous (cast?)
	for(int i=0; i<m_ParameterPoints.size(); i++) 
	{
		SPointStructure cur = m_ParameterPoints[i];
		// relative weight coefficient of each point (standard value = 1.0f)
		//float fWeight = m_ParameterPoints[i].weight;

		// compute bezier coordinates using bernstein polynoms
		p.x += /*fWeight */ BinomialCoefficient(n, i) * pow(t, i) * pow(1-t, n-i) * cur.x;
		p.y += /*fWeight */ BinomialCoefficient(n, i) * pow(t, i) * pow(1-t, n-i) * cur.y;
		p.z += /*fWeight */ BinomialCoefficient(n, i) * pow(t, i) * pow(1-t, n-i) * cur.z;
	}

	/// Calculate derivatives of bezier curves according to
	/// https://www.rose-hulman.edu/~finn/CCLI/Notes/day13.pdf

	/// Comment on simplification: 
	/// An easier way would be to take the vector to the next point on the curve and calculate the difference.
	/// This would lead to a direction vector with satisfying precision. But this would implicate that the 
	/// precision of the derivation depends on the curve precision!
	/// With this technique, we can have precise derivations even if we have a precision of only 10.0f units!
	for(int i=0; i<n; i++)
	{
		SPointStructure cur = m_ParameterPoints[i];
		SPointStructure next = m_ParameterPoints[i +1];

		// difference in weights
		//float fDeltaWeight = next.weight - cur.weight;

		/// calculate coordinates
		p.tangent.x += /*(fDeltaWeight) */ BinomialCoefficient(n -1, i) * pow(t, i) * pow(1-t, n -1 -i) * cur.x;
		p.tangent.y += /*(fDeltaWeight) */ BinomialCoefficient(n -1, i) * pow(t, i) * pow(1-t, n -1 -i) * cur.y;
		p.tangent.z += /*(fDeltaWeight) */ BinomialCoefficient(n -1, i) * pow(t, i) * pow(1-t, n -1 -i) * cur.z;
	}
	
	/// subtract point position from tangent vector
	/// so the tangent vector is relative
	p.tangent = p.tangent.sub(p);

	/// NOTE: There is more than -one- normal vector to another vector!

	/// calculate relative normal vector
	float len = p.tangent.magnitude();
	p.normal = vec(-p.tangent.y / len, p.tangent.x / len, 0);
	
	/// NOTE: Do -NOT- normalize tangent vector before you have copied the normal vector! They will be incorrect!

	/// normalise vector: divide by length
	/// so vector length is 1 (all have the same length)
	p.normal.normalize();
	p.tangent.normalize();

	// NOTE: use vector.rescale(floatval); to change the length according to your wish!
	//p.tangent.rescale(100.0f);
	//p.normal.rescale(100.0f);

	// return finished point for interpolation value t
	return p;
}

