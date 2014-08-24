/**
* Trigger are nodes that call other nodes as soon as
* a chain of conditions is fullfilled.
* Please note that triggers can call as many nodes as you like
*/

/**
* Include engine and flowgraph header file
*/
#include "engine.h"
#include "flowgraph/flowgraph.h"


/**
* A class for event triggers
* Triggers are called IMMEDIATELY
* If you want a delayed trigger, use a timer!
*/
class CTrigger : public CNode {

	/**
	* Constructor
	*/ 
	CTrigger() {
		// ...
	}
	/**
	* Destructor
	*/
	~CTrigger() {
		// ...
	}

	/**
	* Trigger nodes have no "node_input", 
	* because they are checked!
	*/

	/**
	* Vector of Conditions that must be fullfilled
	*/
	std::vector<CCondition*> m_ConditionSet;


	/**
	* Check if chain of conditions is fullfilled
	* and then run the code
	*/
	void Check(void) 
	{
		/**
		* if <chain of conditions> is fullfilled
		*/
		if(check_chain_of_conditions_passed()) 
		{
			// run code/nodes here
			run_sub_nodes_code();
		}
	}

	/**
	* Nodes that will be called in case conditions are fullfilled!
	*/
	std::vector<CNode*> m_NodesToCall;

	private:
	

	/**
	* Check the chain of conditions.
	* We start with a vector of conditions which
	* will be related with 
	*/
	bool check_chain_of_conditions_passed(void) 
	{
		/**
		* Go through all conditions
		*/
		for(int i=0; i<m_ConditionSet.size(); i++) 
		{
			
		}
		// check here...
		return true;
	}

	/**
	* Private function call
	*/
	void run_sub_nodes_code() 
	{	
		/**
		* Nodes should be added in the order you 
		* want them to run
		*/
		for(int i=0; i<m_NodesToCall.size(); i++) 
		{
			// call sub nodes
			m_NodesToCall[i]->node_input();
		}
	}

};