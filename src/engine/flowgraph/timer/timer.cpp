/**
* Timer - Basic implementation of a Script timer
*/

/**
* Include this, mainly for SDL_GetTicks();
*/
#include "engine.h"

/** 
* Include flowgraph class
*/
#include "flowgraph/flowgraph.h"


/** 
* Timers are called after a certain time passed
* these timers will be stored in another vector in
* our timer management class
*
* The timer management class will run in an extra
* thread 
*/
class CNodeTimer : public CNode 
{
	public:

	/**
	* Constructor
	*/
	CNodeTimer() 
	{
		// Activated by default
		m_bActive = true;
		// Interval = 1000 ms (1 second)
		m_ulInterval = 1000;
		// No call limit
		m_ulCallLimit = -1;
		/**
		* Not called yet
		*/
		m_ulFirstTimeCalled = 0;
	}

	/**
	* Overloaded constructor
	*/
	CNodeTimer(bool autostart, unsigned long interval, unsigned long call_limit) 
	{
		// automaticly start this timer or not?
		m_bActive = autostart;
		// copy interval
		m_ulInterval = interval;
		// call limit
		m_ulCallLimit = call_limit;
		// not called yet
		m_ulFirstTimeCalled = 0;
	}

	/**
	* Destructor
	*/
	~CNodeTimer() {
		// no destructor actions (yet)
	}
	
	/**
	* Is this timer active or disabled?
	*/
	bool m_bActive;

	/**
	* Timer interval
	*/
	unsigned long m_ulInterval;

	/**
	* How often will this timer we called
	*/
	unsigned long m_ulCallLimit;

	/**
	* A vector of sub-Nodes which we will call
	*/
	std::vector<CNode* > m_TimerNodesToCall;

	/**
	* A vector of nodes that can call this timer (needed ?)
	*/
	//std::vector<CNode*> m_NodesThatCanCallMe;

	/**
	* When was this timer called for the first time?
	* When was this timer called the last time?
	* When will it be called the next time?
	*/
	unsigned long m_ulFirstTimeCalled;
	unsigned long m_ulLastTimeCalled;
	unsigned long m_ulNextCallTime;

	/**
	* How often has this timer been called?
	*/
	unsigned long m_ulCalled;

	
	/**
	* Start the timer
	*/
	void StartTimer(int call_limit = -1, bool call_directly_at_start = true) 
	{
		// timer is now active
		m_bActive = true;

		// do we call this timer now at startup
		if(call_directly_at_start == true) {
			// store first call time
			m_ulFirstTimeCalled = SDL_GetTicks();
			// call methods
			CallTimer();
		}
	}
	

	/**
	* Set timer's interval
	*/
	void SetInterval(unsigned long ulCallInterval) 
	{
		// set interval
		m_ulInterval = ulCallInterval;
	}	
	

	/**
	* Call this timer (custom)
	*/
	void CallTimer(void) 
	{		
		// First, lets do some timer specific stuff
		// when was this timer called?
		m_ulLastTimeCalled = SDL_GetTicks();
		// increment the amount of timer calls
		m_ulCalled ++;

		/**
		* Stop timer if call limit has exceeded
		*/
		if(m_ulCalled = m_ulCallLimit) StopTimer();

		// Now run the timer's code blocks
		node_input();
	}

	/**
	* node_input is a virtual function which will have to be implemented for every node type
	* for this type of node (TIMER) we will just call the child nodes
	*/
	void node_input(void) 
	{
		// run node's child nodes
		for(int i=0; i<m_pChilds.size(); i++) 
		{
			/**
			* Call child function which will be implemented
			* for every specific node
			*/
			m_pChilds[i]->node_input();
		} 
	}

	/**
	* Stop timer
	*/
	void StopTimer(void) 
	{
		// timer is now disabled
		m_bActive = false;
	}

};