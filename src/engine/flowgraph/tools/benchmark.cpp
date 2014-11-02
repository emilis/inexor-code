/**
* Treetest implementation
*/
#include "benchmark.h"

/**
* Interval structure constructor
*/
SInterval::SInterval() 
{
	begin = 0;
	end = 0;
	duration = 0;
}

/**
* Calculate duration
*/
void SInterval::calcduration(void) 
{
	// duration = "now" - "past"
	duration = end - begin;
}

/**
* Calculate average time
*/
void STimerNode::calc_average(void) 
{
	/**
	* Calculate average
	* Some errors with capacity could occur (?)!
	*/
	unsigned long long tmp = 0;

	// Sum up all the entries
	for(unsigned int i=0; i<durations.size(); i++)
	{
		// add duration to 
		tmp += durations[i].duration;
	}

	// Calculate average
	tmp /= durations.size();

	// copy value
	average = tmp;
	// tmp will be deleted from heap memory automaticly...
}

/* ---------------------------------------------------------------------------- */

/**
* Timer node
*/
STimerNode::STimerNode() 
{
	// allocate memory for deque
	durations.resize(100);
	// clean node name
	strcpy_s(name, 256, "");
	// set no parent yet
	parentnode = nullptr;
	// timer is not ticking yet
	ticking = false;
	// free memory
	subnodes.clear();
	// no sums applied
	sums_added = 0;
	// reset average
	average = 0;
}

/* ---------------------------------------------------------------------------- */

/**
* Constructor
*/
CBenchmarking::CBenchmarking() 
{
	// Create root timer node
	root = new STimerNode;
	// copy name
	strcpy_s(root->name, 256, "root");
	// root does not have a parent!
	root->parentnode = nullptr;
	// register root
	timeregister["root"] = root;

	// set time to 0 
	time = 0;

	/**
	* We have a solution for this problem, but only for Microsoft Windows
	*/
	#ifdef WIN32
	// We need this
	QueryPerformanceFrequency((LARGE_INTEGER*) &frequency);
	#endif
}


/**
* Destructor
*/
CBenchmarking::~CBenchmarking() 
{
	// delete it all from root
	delete_node(root);
}

/* ---------------------------------------------------------------------------- */

/**
* Begin measuring
*/
int CBenchmarking::begin(char* name, char* group) 
{
	if(name == nullptr || !strlen(name)) 
	{
		printf("tree error: name is invalid!\n");
		return -1;
	}
	if(group == nullptr || !strlen(group)) 
	{	
		printf("tree error: group is invalid!\n");
		return -2;
	}
	if(0 == strcmp(group, name)) 
	{
		printf("tree error: group equals name!\n");
		return -3;
	}
	if(root == nullptr) 
	{
		printf("tree error: root not set!\n");
		return -4;
	}

	// in case this node does not exist yet
	if(timeregister.count(name) == 0) 
	{
		// only if group exists
		if(timeregister.count(group) > 0) 
		{
			// create new sub node
			STimerNode* newnode = new STimerNode;
			// copy name
			strcpy_s(newnode->name, 256, name);
			// copy parent node
			newnode->parentnode = timeregister[group];
			// time is ticking now!
			newnode->ticking = true;
			// add new node
			timeregister[group]->subnodes.push_back(newnode);

			// register node
			timeregister[name] = newnode;
		}
		else
		{
			// parent group not set?
			printf("tree error: parent group does not exist!\n");
			return -5;
		}
	}
	else 
	{
		if(timeregister[name]->ticking == false) 
		{
			printf("error: time is not ticking!\n");
			return -6;
		}
	}

	// BUG? - is this time already ticking
	/*if(timeregister[name]->ticking == true) 
	{
		//std::cout << "ERROR: time is already ticking!" << std::endl;
	}*/

	// begin time measuring
	QueryPerformanceCounter((LARGE_INTEGER*)&time);

	// time interval instance
	SInterval time_interv;
	time_interv.begin = time;

	// insert one at the end
	// copy, no pointers
	timeregister[name]->durations.push_back(time_interv);
	// remove 1 from the front
	timeregister[name]->durations.pop_front();

	// function successful
	return 1;
}

/**
* End measuring
*/
int CBenchmarking::end(char* name)
{
	if(timeregister.count(name) == 0) 
	{
		printf("this entry does not exist!\n");
		return -1;
	}
	else 
	{
		if(timeregister[name]->ticking == false) 
		{
			printf("error: timer is not ticking!\n");
			return -2;
		}

		// query time
		// Windows specific!
		QueryPerformanceCounter((LARGE_INTEGER*)&time);

		// a bit tricky
		// set the end time of this performance counter's deque
		timeregister[name]->durations[timeregister[name]->durations.size() -1].end = time;
		// Calculate duration
		timeregister[name]->durations[timeregister[name]->durations.size() -1].calcduration();
	}

	return 1;
}

/**
* delete all nodes
*/
void CBenchmarking::deleteall(void) 
{
	delete_node(root);
}

/**
* delete a node
*/
void CBenchmarking::delete_node(STimerNode* node) 
{
	// recursively call sub nodes
	for(size_t i=0; i<node->subnodes.size(); i++) 
	{
		// delete sub nodes as well
		delete_node(node->subnodes[i]);
	}
	// free memory
	delete node;
	node = nullptr;
		
	/* no memory should be left due to 
	the consistency of this tree structure
	and its childs/leafes
	*/
}

/**
* Get access to a process' duration
*/
unsigned long long CBenchmarking::average_duration(char* name)
{
	// Check if this node exists
	if(timeregister.count(name) < 1) 
	{
		printf("error: index %s not found!\n", name);
		return 0;
	}

	// return average duration
	return timeregister[name]->average;
}

/**
* Get access to the last duration of a process
*/
unsigned long long CBenchmarking::last_duration(char* name) 
{
	// check if this node exists
	if(timeregister.count(name) < 1) 
	{
		printf("error: index %s not found!\n", name);
		return 0;
	}

	/**
	* Return duration of last process
	*/
	return timeregister[name]->durations.back().duration;
}

/**
* Dump the whole tree
*/
void CBenchmarking::dumpall(void)
{
	dumptreenode(root, 0);
}

/**
* dump tree node
* this debug function is used to print a visual text map of all nodes in the console
* for debugging purpose only..
*/
void CBenchmarking::dumptreenode(STimerNode* node, unsigned int depth)
{
	char test[1024];
	strcpy_s(test, 1024,"");
	// format - tabulators
	for(unsigned int i=0; i<depth; i++) sprintf_s(test, 1024, "%s-", test);
	sprintf_s(test, 1024, "%s%s [%f]", test, node->name, node->average);

	depth++;
	for(unsigned int i=0; i<node->subnodes.size(); i++) 
	{
		// recursively dump child nodes
		dumptreenode(node->subnodes[i], depth);
	}
}

/**
* Resolve nodes recursively
*/
void CBenchmarking::recursiveresolve(STimerNode* node)
{
	// It should stop at "root" I hope...
	//if(node->parentnode == nullptr) return;
	
	/**
	* Add latest delay value to parent delay sum
	*/
	node->parentnode->durations.back().duration += node->durations.back().duration;
	// increase sums of parent node
	node->parentnode->sums_added++;

	/**
	* Only if the number of sub nodes matches the amount of added summands
	* we will continue
	*/
	if(node->subnodes.size() >= node->sums_added)
	{
		// reset sum indicator
		node->sums_added = 0;
		// recursively resolve parent node
		recursiveresolve(node->parentnode);
	}
}

/**
* Compile all times
* sum up the binary tree backwards!
*/
void CBenchmarking::compile(void)
{
	// First step: find leaves!
	for(std::map<char*, STimerNode*>::iterator it = timeregister.begin();  it != timeregister.end();  it++)
	{
		// This node does not have sub-nodes -> it is a leaf!
		if(it->second->subnodes.size() == 0 && nullptr != it->second->parentnode)
		{
			/**
			* Sum up the duration to the parent node
			* This is important because the time which is needed in a "sub thing"
			* also influences the time which is needed to do a "parent thing"...
			*/
			it->second->parentnode->durations.back().duration += it->second->durations.back().duration;

			/**
			* Debug
			*/
			/*char test[1024];
			sprintf_s(test, 1024, "adding %ul units from node %s to parent node %s (now: %ul)",
				it->second->durations.back().duration, it->second->name, 
				it->second->parentnode->name, it->second->parentnode->durations.back().duration);
			*/

			// increase value of added summands
			it->second->sums_added++;
			// recursively call sub node
			recursiveresolve(it->second);
		}
	}
}


/**
* return a pointer to the root node
*/
STimerNode* CBenchmarking::getroot(void)
{
	return root;
}

/**
* Calculate average
*/
void CBenchmarking::calculate_average(void) 
{
	/**
	* Calculate average of each index
	*/
	for(std::map<char*, STimerNode*>::iterator it = timeregister.begin();  it != timeregister.end();  it++) 
	{
		// calculate average!
		it->second->calc_average();
	}
}

/**
* ...
*/
void CBenchmarking::pushback_and_clear_node_sums(void)
{
	/**
	* Find nodes, not leaves
	*/
	for(std::map<char*, STimerNode*>::iterator it = timeregister.begin();  it != timeregister.end();  it++) 
	{
		// find nodes
		if(0 != it->second->subnodes.size())
		{
			// Create temporal structure instance 
			SInterval tmp;
			tmp.begin = 0;
			tmp.end = 0;
			tmp.duration = 0;

			// append index to the end
			it->second->durations.push_back(tmp);
			// remove index form beginning
			it->second->durations.pop_front();
			// reset sum 
			it->second->sums_added = 0;
		}
	}
}