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
	* Only on Microsoft Windows
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
	// new
	timeregister[name]->dur.begin = time;

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
unsigned long long CBenchmarking::duration(char* name)
{
	if(timeregister.count(name) < 1) 
	{
		printf("error: index %s not found!\n", name);
		return 0;
	}
	// return duration of the last process
	//return timeregister[name]->durations[timeregister[name]->durations.size() - 1].duration;
	
	return timeregister[name]->dur.duration;
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
*/
void CBenchmarking::dumptreenode(STimerNode* node, unsigned int depth)
{
	char test[1024];
	strcpy_s(test, 1024,"");
	// format - tabulators
	for(unsigned int i=0; i<depth; i++) sprintf_s(test, 1024, "%s-", test);
	sprintf_s(test, 1024, "%s%s", test, node->name);
	
	// output
	//conoutf(CON_DEBUG, test);

	depth++;
	for(unsigned int i=0; i<node->subnodes.size(); i++) 
	{
		dumptreenode(node->subnodes[i], depth);
	}
}


/**
* Resolve nodes recursively
*/
void CBenchmarking::recursiveresolve(STimerNode* node)
{
	if(node->parentnode == nullptr) return;

	// increase sums of parent node
	node->parentnode->summands++;
	
	/**
	* Add latest delay to parent
	*/
	node->parentnode->dur.duration += node->dur.duration;

	// recursively resolve parent node
	recursiveresolve(node->parentnode);
}


/**
* Compile all times
*/
void CBenchmarking::compile(void)
{
	/**
	* First step: find leaves!
	*/
	for(std::map<char*, STimerNode*>::iterator it = timeregister.begin();  it != timeregister.end();  it++)
	{
		/**
		* This node does not have sub-nodes
		* it is a leaf!
		*/
		if(it->second->subnodes.size() == 0)
		{
			/**
			* This is the last index of the leave's duration buffer
			* and NOT of its parent!!
			*/
			/*
			int lastindexofleaf = it->second->durations.size() - 1;
			// remove one at the front of the parent's buffer
			it->second->parentnode->durations.pop_front();
			// push back one in the parent's buffer
			it->second->parentnode->durations.push_back(it->second->durations[lastindex]);
			*/

			// New way
			it->second->parentnode->dur.duration = it->second->dur.duration;

			// increase value of added summands
			it->second->summands++;
			
			// recursively call this
			recursiveresolve(it->second);
		}
	}

}

/**
* return root
*/
STimerNode* CBenchmarking::getroot(void)
{
	return root;
}
