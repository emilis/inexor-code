/**
* Things that can be a condition:
*  
* 1) Value compare (X == 4)
* 2) Function call successful/failed (IsPlayerInArea)
* 3) ? more?
* 
*/

/**
* Include engine and flowgraph header file
*/
#include "engine.h"
#include "flowgraph/flowgraph.h"


/**
* Enumeration for condition times
*/
enum FLOWGRAPH_CONDITION_TYPE 
{
	/**
	* A condition can be either a value comparison
	* or the success of a function call
	*/
	CONDITION_TYPE_VALUECMP = 0,
	CONDITION_TYPE_FUNCTION_CALL /* = 1*/
};

/**
* Condition
*/
class CCondition : public CNode {
	
	public:

	/**
	* Constructor
	*/
	CCondition() {
	}
	/**
	* Destructor
	*/
	~CCondition() {
		// nothing to destruct (yet)
	}

	/**
	* What kind of condition is this? 
	*/
	FLOWGRAPH_CONDITION_TYPE m_CondType;

};