/**
* Benchmark implementation
*
*
*/

/**
* Include benchmark library
*/
#include "benchmark.h"

/**
* Include SDL library
* for SDL_GetTicks
*/
#include "SDL_timer.h"

/**
* SDL_GetTicks is equivalent to timeGetTime() on Windows x86,x64
* ---
*/



/**
* constructor
*/
CBenchmark::CBenchmark()
{
}


/**
* destructor
*/
CBenchmark::~CBenchmark() {
}


/**
* Start ticking process
*/
void CBenchmark::StartTick(char* name) 
{
	// start time measurement
	m_Time[name].start = SDL_GetTicks();
	// timer is ticking
	m_Time[name].ticking = true;
}

/**
* Stop ticking process
*/
void CBenchmark::StopTick(char* name) 
{
	m_Time[name].end = SDL_GetTicks();
	// timer is NO LONGER ticking
	m_Time[name].ticking = false;
}

/**
* Reset timer
*/
void CBenchmark::ResetTick(char* name) 
{
	// reset both indices
	m_Time[name].start = m_Time[name].end = 0;
	// not ticking!
	m_Time[name].ticking = false;
}

/**
* Get time which passed
*/
unsigned long CBenchmark::GetTime(char* name) 
{
	// is this timer ticking?
	if(!m_Time[name].ticking) return 0;
	/**
	* Calculate time which passed and return it!
	*/
	return m_Time[name].GetTime();
}