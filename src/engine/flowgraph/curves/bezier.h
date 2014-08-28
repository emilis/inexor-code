/**
* A b�zier curve (named after french mathematician Pierre �tienne B�zier) is a parametric curve
* whose only purpose is to look nice, soft and elegant.
* Sometimes it is not easy to create elegant and flexible objects with mathematics.
* Bezier curves are FUNDAMENTAL curves and very essential in computer graphics and image processing
* because it is used for interpolation, approximation...
*
* Computation
* You pass a list of parameter points and the engine will calculate the final curve from it.
* Unfortunately, curve computation needs a lot of resources. 
* You can solve the problem either with RECUSION of BERNSTEIN POLYNOMS
* 
* Method 1: Recusion [De-Casteljau-Algorithm]
* possibly needs a lot of heap memory for every sub-call (not sure about this by the way)
* anyway it is MUCH SLOWER (but MORE FLEXIBLE) than the BERNSTEIN solution
* 
* Method 2: Bernstein Polynoms [named after russian mathematician Sergei Natanovich Bernstein]
* In short, Bernstein polynoms replace recursion with a simple sum of interpolations
* This engine will support both solutions.
* Problems which need to be solved:
*
*  1) Should the curve be pre-computed or computed in realtime?
*  2) Which solution is faster? Bernstein or De-Casteljau?
*  3) Is it possible to derivate the curve to gain angle and rotation?
*  4) Can we calculate the length of a curve?
*  5) Can we calculate the required precision of a curve?
*/

/**
* In honor of:
*
*	Pierre �tienne B�ZIER   (September 1, 1910 � November 25, 1999), French mathematician and engineer at RENAULT
*   Paul de CASTELJAU   (November 19, 1930), French mathematician and physicist  and engineer ar Citroen
*	Sergei Natanovich BERNSTEIN   (March 5, 1880 � October 26, 1968), Russian mathematician.
*   Charles HERMITE   (December 24, 1822 � January 14, 1901), French mathematician
*/


/**
* Include guards protects the file from being included twice
*/
#ifndef ENGINE_BEZIER_CURVE_H
#define ENGINE_BEZIER_CURVE_H


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
* We need an own structure for weight as well
*/
struct SPoint 
{
	// the point's position
	vec pos;
	// the weight of this point
	float weight;
};


/**
* A class for bezier curves
*/
class CBezierCurve 
{
	public:

	// constructor
	CBezierCurve();
	// destructor
	~CBezierCurve();

	/**
	* A vector of parameter points which will be computed to a curve
	*/
	std::vector<SPoint> m_ParameterPoints;
	std::vector<vec> m_ComputedPoints;

	/**
	* TODO: Store a map of "current points"
	*/
	//std::map<char*, vec> m_NamedPoints;

	vec m_CurrentPoint;

	/**
	* Adding parameter points
	* As soon as we add points, the curve is NOT computed (again)
	*/
	// In order to scale the curve to the middle correctly, use Sauerbraten's map scale
	#define SAUERBRATEN_STANDARD_MAP_SIZE 512.0f
	// Add point methods
	void AddParamPoint(float x, float y, float z, float weight = SAUERBRATEN_STANDARD_MAP_SIZE);
	void AddParamPoint(vec point);

	/**
	* Add random curve
	*/
	void GenerateRandomCurve(unsigned int maxparameterpoints, bool autocalculate = true);

	/**
	* Clear parameter input and output buffer
	*
	*/
	void ClearAllPoints(void);
	void ClearParameterPoints(void);
	void ClearComputedPoints(void);


	/**
	* Set Limits
	* "Because of the limitations of factorial calculations, the code could only calculates 
	*  curves up to 32 points. More complicated structures are generally represented by a 
	* combination of these curves"
	*   -Tolga Birdal, codeproject.org
	*/
	void SetParamPointLimit(unsigned int limit);

	/**
	* Set Curve precision
	* Increasing this value increases precision
	* calculation example:
	* prec = 10.0f / precision;
	*/
	void SetPrecision(float precision);

	/**
	* Calculate curve
	*/
	void CalculateCurve_BernsteinPolynom(void);
	void CalculateCurve_DeCasteljauRecursive(void);


	/**
	* Get a point
	*
	*	! ## ## IMPORTANT WARNING ## ## !
	*
	* THIS POINT WILL BE COMPUTED IN REAL TIME! 
	* IT IS REALLY SLOW. SEE MORE DETAILS IN bezier.cpp
	*
	*/
	vec CalculatePointFromFloat(float curveposition);

	
	/**
	* Get a point from the cache according to the progress (float)
	*/
	unsigned int GetPointIndexFromFloat(float curveposition);


	/**
	* Get finished curve data
	*/
	vec GetCurvePos(float interpolation_value);

	/**
	* Get point number # from computed curve
	*/
	vec GetComputedPointIndexed(unsigned int index);

	/**
	* Get Parameter point number # from this curve
	*/
	vec GetParameterPointIndexed(unsigned int index);

	/**
	* Get access to m_bComputed [Read only!]
	*/
	bool IsCurveComputed(void);

	/**
	* We will add some get/set functions so we cann keep this private
	*/
	protected:

	// bernstein position
	float bernsteinposition(float val, int i, float position, int elementcount, float weight);
	
	// binomial coefficient for bernstein polynom
	unsigned int binomialCoef(unsigned int n, const unsigned int k);

	// calculate point
	vec calculateposition(float position);

	/**
	* Maybe we should add a limit for parameter points
	*/
	unsigned int m_uiParamLimit;

	/**
	* Precision
	*/
	float m_fCalcPrecision;

	/**
	* Is the curve computed or not?
	*/
	bool m_bComputed;

};


/**
* Make curve global
*/
extern CBezierCurve dynamic_curve;

#endif /* #ifndef ENGINE_BEZIER_CURVE_H */
