/**
* New and improved benchmark system
* This is system is capable to create a tree structure
* 
*/

/**
* Include guard protects this file from being included twice
*/
#ifndef ENGINE_BENCHMARKING_H
#define ENGINE_BENCHMARKING_H

/**
* C++ standard libraries header files
*/
#include <map>
#include <deque>
#include <iostream>
#include <vector>
#include <Windows.h>


/**
* time will be measured in microseconds
* 1000 microseconds are 1 milisecond
* 1000 miliseconds are 1 second
* 1 seconds consists of 1 million microseconds
*/


/**
* A structure to store time interval
* a time intervals begins with time A
* and ends with time B. The duration is
* C = B - A (end - begin), all in microseconds (64bit int)
*/
struct SInterval 
{
	/**
	* 64 bit integer for microseconds
	*/
	unsigned long long begin, end, duration;

	/**
	* Constructor sets all members to 0 (microseconds)
	*/
	SInterval();

	/**
	* Calculate how long a process took
	*/
	void calcduration(void);
};


/**
* A structure for timer container nodes
*/
struct STimerNode 
{
	/**
	* Constructor
	*/
	STimerNode();
	
	/**
	* The name of this node
	*/
	char name[256];
	/**
	* is this timer active/ticking ?
	*/
	bool ticking;
	
	/**
	* For summing up the time
	*/
	//bool summandapplied;

	/**
	* How many summands have been added?
	*/
	int summands;

	/**
	* this is the next leaf to sum up!
	*/
	//bool todo;

	/**
	* This deque stores the last durations
	*/
	//std::deque<SInterval> durations;
	
	SInterval dur;

	/**
	* The parent node
	*/
	STimerNode* parentnode;
	/**
	* sub nodes (childs)
	*/
	std::vector<STimerNode*> subnodes;
};


/**
* A class for trees
*/
class CBenchmarking
{
	private:

	/**
	* Root of this tree
	*/
	STimerNode* root;

	/**
	* Node registration
	*/
	std::map<char*, STimerNode*> timeregister;

	/**
	* On Microsoft Windows:
	*/
	#ifdef WIN32
	unsigned long long frequency;
	#endif

	/**
	* A dummy variable for microseconds
	* system. It should be faster to store it
	* as a member variable
	*/
	unsigned long long time;
	
	/**
	* the sum of all the time
	*/
	unsigned long long sum_time;

	/**
	* dump a certain node
	*/
	void delete_node(STimerNode* node);
	
	/**
	* dump tree node
	*/
	void dumptreenode(STimerNode* node, unsigned int depth);

	/**
	* Recursive resolve
	*/
	void recursiveresolve(STimerNode* node);



	public:

	// constructor
	CBenchmarking();

	// destructor
	~CBenchmarking();

	/**
	* Read only for root pointer
	*/
	STimerNode* getroot(void);

	/**
	* Begin time measuring
	*/
	int begin(char* name, char* group = "root");

	/**
	* End measuring
	*/
	int end(char* name);

	/**
	* Get access to a process' duration
	*/
	unsigned long long duration(char* name);

	/**
	* dump all nodes and free memory
	*/
	void deleteall(void);

	/**
	* dump whole tree
	*/
	void dumpall(void);

	/**
	* Calculate all times
	*/
	void compile(void);

};

extern CBenchmarking benchmark;

#endif /* #ifdef ENGINE_BENCHMARKING_H*/