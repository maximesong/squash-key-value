#ifndef _RESOURCE_MONITOR_H_
#define _RESOURCE_MONITOR_H_

#include <sys/time.h>

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
		start_time = getCurrentTime();
	}

	void stopTimer() {
		end_time = getCurrentTime();
	}

	double getTime() {
		return (end_time - start_time) / 1000;
	}
private:
	double getCurrentTime(){
		struct timeval tp;
		gettimeofday(&tp, NULL);
		return (double) (tp.tv_sec * 1000 + tp.tv_usec / 1000);
	}
	double start_time;
	double end_time;
};


#endif /* _RESOURCE_MONITOR_H_ */
