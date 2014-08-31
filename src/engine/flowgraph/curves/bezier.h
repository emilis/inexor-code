/**
* A bezier curve (named after french mathematician Pierre Etienne Bezier) is a parametric curve
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
*	Pierre etienne BE0IER   (September 1, 1910 - November 25, 1999), French mathematician and engineer at RENAULT
*   Paul de CASTELJAU   (November 19, 1930), French mathematician and physicist  and engineer ar Citroen
*	Sergei Natanovich BERNSTEIN   (March 5, 1880 - October 26, 1968), Russian mathematician.
*   Charles HERMITE   (December 24, 1822 - January 14, 1901), French mathematician
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
	// the parameter weight of this point
	float weight;
};


/**
* A simple class interface 
* for bezier curve progressing
*/
class CBezierCurve 
{
	public:

	// constructor: class initialisation
	CBezierCurve();
	// destructor
	~CBezierCurve();
	
	// A vector of parameter points which will be computed to a curve
	std::vector<SPoint> m_ParameterPoints;

	// A vector for storing the computed points
	std::vector<vec> m_ComputedPoints;


	/**
	* Method for adding parameter points
	* Parameters: 
	*	float x		the x position of the point which will be added
	*	float y		the y position of the point which will be added
	*	float z		the z position of the point which will be added
	*	weight		the mathematic importance coefficient of this parameter
	*/
	void AddParamPoint(float x, float y, float z, float weight = 1.0f);

	/**
	* Overlaoded Sauerbraten vector method
	*	vec point	a sauerbraten vector which specifies the position
	*	weight		the mathematic importance coefficient of this parameter
	*/
	void AddParamPoint(vec point, float weight = 1.0f);

	/**
	* Randomly generate example curve
	*	unsigned int maxparameterpoints		the amount of parameter points which will be created 
	*	bool autocalculate					should the engine automaticly generate the curve?
	*
	* TODO:
	*	-define a bounding box in which the curve will be generated
	*	-add more parameter specifications
	*/
	void GenerateRandomCurve(unsigned int maxparameterpoints, bool autocalculate = true);



	/**
	* Clear both input and output points
	* no parameters specified
	*/
	void ClearAllPoints(void);

	/**
	* Clear only parameter points
	* computed points will remain in m_ComputedPoints
	*/ 
	void ClearParameterPoints(void);

	/**
	* Clear only computed points
	* parameter points will remain in m_ParameterPoints
	*/
	void ClearComputedPoints(void);



	/**
	* Set Limits
	*	unsigned int limit		the maximum amount of parameter points that can be passed to the function
	*
	*	"Because of the limitations of factorial calculations, the code could only calculates 
	*	curves up to 32 points. More complicated structures are generally represented by a 
	*	combination of these curves"
	*
	*		-Tolga Birdal, codeproject.org
	*/
	void SetParamPointLimit(unsigned int limit);

	/**
	* Set Curve precision
	*	float precision		the final precision of the computed curve
	*	Increasing this value increases precision, because the computation steps
	*	will be generated with 10.0f / precision
	*/
	void SetPrecision(float precision);



	/**
	* Calculate curve with BERNSTEIN polynoms
	*/
	void CalculateCurve_BernsteinPolynom(void);

	/**
	* Calculate curve via recursive DE CASTELJAU algorithm
	*/
	void CalculateCurve_DeCasteljauRecursive(void);


	/** ----------------------------- REAL TIME CALCULATIONS ----------------------------- **/
	/** THESE CALCULATIONS MAY BE SLOWER IF FREQUENTLY USED. USE CACHING INSTEAD!**/

	/**
	* Get a point from the parameter points.
	* This point will be rendered in realtime
	* This could be very slow if you repeat.
	*/
	vec CalculatePointFromFloat(float curveposition);


	/** ----------------------------- CAHCED CALCULATIONS ----------------------------- **/

	/**
	* Get the index of the point which is represented by the curve
	* if parameter curveposition is passed to it.
	*/
	unsigned int GetPointIndexFromFloat(float curveposition);

	/**
	* Get a curve point at parameter interpolation_value
	*/
	vec GetCurvePos(float curveposition);

	/**
	* Get a point with a certain number from the generated curve
	*/
	vec GetComputedPointIndexed(unsigned int index);

	/**
	* Get a parameter point that is point number index of this curve
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
