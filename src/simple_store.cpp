#include "simple_store.h"
#include <sys/time.h>
#include <iostream>

using namespace std;

int SimpleStore::get(const char* key, char *value) {
	if (store.count(key)) {
		const char *source = store[key].value;
		int size = strlen(source);
		memcpy(value, source, size);		
		return size;
	} else {
		return -1;
	}
}


int SimpleStore::put(const char* key, const char *value, int len) {
    info new_info;
    new_info.compressed = true;
    new_info.temp = 100;
    new_info.last_atime =  SimpleStore::getTime();
    cout<<"time:";
    cout<<new_info.last_atime<<endl;
    new_info.value = value;
	store[key] = new_info;
	return 0;
}

unsigned long long SimpleStore::getTime(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

