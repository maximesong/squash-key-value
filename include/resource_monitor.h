#ifndef _RESOURCE_MONITOR_H_
#define _RESOURCE_MONITOR_H_

#include <time.h>

#include <iostream>

using namespace std;

class ResourceMonitor {
public:
	/**
	 * @return memory size in 'K'
	 */
	static long getMemoryUsage();
};

class Timer {
public:	
	void startTimer() {
		start_time = clock();
	}

	void stopTimer() {
		end_time = clock();
	}

	double getTime() {
		return (double (end_time - start_time)) / CLOCKS_PER_SEC ;
	}
private:
	clock_t start_time;
	clock_t end_time;
};


#endif /* _RESOURCE_MONITOR_H_ */
