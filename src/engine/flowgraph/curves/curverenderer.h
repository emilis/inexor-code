/**
* Include bezier curve engine
*/
#include "bezier.h"

/**
* Include guard: protect this file from being included twice
*/
#ifndef ENGINE_BEZIER_CURVE_RENDERER_H
#define ENGINE_BEZIER_CURVE_RENDERER_H


/**
* A class for bezier curve renderers
*/
class CCurveRenderer {

	public:
	
	// constructor
	CCurveRenderer();
	// destructor
	~CCurveRenderer();

	/**
	* Render
	*/
	void RenderCurve(void);

	/**
	* The curve to render
	*/
	void SetCurve(CBezierCurve *curve);

	protected:

	/**
	* A pointer to a bezier curve
	*/
	CBezierCurve* m_pCurve;
};


#endif /* #ifndef ENGINE_BEZIER_CURVE_RENDERER_H */