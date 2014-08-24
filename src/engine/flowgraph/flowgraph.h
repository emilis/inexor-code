/**
* --------------------------------------------------------------------------------------------------
* Sauerbraten Flowgraph Engine
* Made by Hanni for "Sauerfork"
* https://github.com/sauerbraten-fork
* --------------------------------------------------------------------------------------------------
* Flowgraphs (also called flowcharts) are visual scripting enviroments
* which are easy and safe to use (they can also be used by rather unexperienced users)
* Whereas two-dimensional flowgraphs are very common, 3D flowgraphs are the exception.
* This projects realises a 3D flowgraph visual scripting engine for Cube2 Sauerbraten/Sauerfork
* I try to keep my code as clear as Hanacks code.
* --------------------------------------------------------------------------------------------------
*/


/**
* Include guard: protect this file from being included twice
*/
#ifndef ENGINE_FLOWGRAPH_H
#define ENGINE_FLOWGRAPH_H


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
* I will make use of the hungarian notation
* Which means that the name of an instance contains a 
* prefix which indicates the type.
* http://en.wikipedia.org/wiki/Hungarian_notation
*
* examples:
*    struct SExample;
*    unsigned int uiExample;
*    int g_iExample; // global
*    char cString[128];
*/


/*
* Nodes can be ANYTHING
* The start of a node script can be one of these 3 different types
*
*   1. EVENTS/TRIGGERS
*   An event is NOT called frequently. Events are called automaticly
*   if a certain chain of circumstances is fullfilled.
*   Events/Triggers can interact with map data (geometry, curves, areas) to
*   trigger their code. Sauerbraten has a very rude and basic implementation of
*   triggers, but its very limited.
*
*   2. TIMERS
*   Timers are functions that will be called with a certain frequency (ms).
*   They can run as many code-blocks as we want.
*
*   3. PROGRAM START (MAIN)
*   The main block is the code block which will be called when the program (map) starts
*   Please note that there is not ONE program start, you can place as many code blocks in 
*   the map as you want. They will all run simultaneously.
*
*/
struct SNode;


/*
* A built-in structure for the node's position
*/
struct SPosition 
{
	// standard coordinates
	float x;
	float y;
	float z;

	// position constructor
	SPosition() {
		// set all coordinates to 0
		x = y = z = 0;
	}
};

class CCondition;

/**
* A basic node implementation
*/
class CNode {
	
	public:

	/** 
	* Constructor
	*/
	CNode() {
		// construct here...
	}
	/** 
	* Destructor
	*/
	~CNode() {
		// destruct here...
	}


	/**
	* The node's position
	*/
	SPosition m_Position;

	/*
	* Nodes can be commented
	* Comments will be displayed below the note
	*/
	std::string m_NodeComment;

	/*
	* Is this node active or not?
	* Timers can be active or inactive e.g.
	*/
	bool m_bActive;

	/**
	* The parents of this node.
	* pointers
	*/
	std::vector<CNode* > m_pNodeParents;

	/**
	* The childs of this node.
	* pointers
	*/
	std::vector<CNode* > m_pChilds;

	/**
	* Virtual function which will be called once this node is active
	* Nodes do not have parameters: anything that is needed in the node
	* can be changed before node_input is called
	*/
	virtual void node_input(void) = 0;
	// virtual void node_output(void) = 0;

};


#endif /* #ifdef ENGINE_FLOWGRAPH_H */