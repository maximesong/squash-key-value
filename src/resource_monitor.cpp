#include "resource_monitor.h"

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

long ResourceMonitor::getMemoryUsage() {
	ifstream stat_stream("/proc/self/statm",ios_base::in);

	unsigned long vsize, resident, share, text, lib, data, dt;
	
	stat_stream >> vsize >> resident >> share >> text >> lib >> data >> dt;

        // don't care about the rest
	stat_stream.close();

	double vm_usage_kb = vsize * sysconf(_SC_PAGESIZE) / 1024;

	return vm_usage_kb;
}
