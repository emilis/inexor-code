/**
* Benchmark system-
* This system can be used to measure the amount of time which is required
* to run certain operations
*
* Just create an instance of this class and use StopBenchmark("NAME");
* use StopBenchmark("NAME"); to stop process ticking
* use GetTime("NAME");
*/

/**
* Include guards protects the file from being included twice
*/
#ifndef ENGINE_BENCHMARK_H
#define ENGINE_BENCHMARK_H

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
* Simple class implementation for benchmarking
*/
class CBenchmark 
{	
	private: // classes are automaticly private but...

	struct timestruct 
	{
		// begin and end in miliseconds
		unsigned long start;
		unsigned long end;

		// is this benchmark timer ticking or not?
		bool ticking;

		// Calculate time
		unsigned long GetTime() {
			return end - start;
		}
	};

	/**
	* A standard map (hashset) to access time
	*/
	std::map<char*, timestruct> m_Time;

	public:

	// constructor
	CBenchmark();
	// destructor
	~CBenchmark();

	/**
	* Start a benchmark ticker
	*/
	void StartTick(char* name);
	
	/**
	* Start a benchmark ticker
	*/
	void StopTick(char* name);

	/**
	* Stop benchmark ticker
	*/
	void ResetTick(char* name);
	
	/**
	* Get the time which passed from end to start
	*/
	unsigned long GetTime(char* name);
};


extern CBenchmark benchmarking;

#endif /* #ifdef ENGINE_BENCHMARK_H */