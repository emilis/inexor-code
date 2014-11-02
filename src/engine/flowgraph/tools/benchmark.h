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
	// new: average value
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
	* This deque stores the last durations
	*/
	std::deque<SInterval> durations;
	
	/**
	* The parent node
	*/
	STimerNode* parentnode;

	/**
	* sub nodes (childs)
	*/
	std::vector<STimerNode*> subnodes;

	/**
	* How many sums have been applied to this node?
	* the parent node will be called if sums_added == this_node->childs.size();
	* which means that all childs have added their sum to the parent.
	*/
	unsigned int sums_added;

	/**
	* Average time
	*/
	unsigned long long average;

	/**
	* Calculate average time
	*/
	void calc_average(void);
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
	unsigned long long average_duration(char* name);
	unsigned long long last_duration(char* name);
	
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

	/**
	* Calculate average
	*/
	void calculate_average(void);

	/**
	* Push back node sums
	*/
	void pushback_and_clear_node_sums(void);
};

extern CBenchmarking bms;

#endif /* #ifdef ENGINE_BENCHMARKING_H*/