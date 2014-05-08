#include "simple_store.h"

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
    new_info.value = value;
	store[key] = new_info;
	return 0;
}

