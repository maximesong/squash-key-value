#include "simple_store.h"

#include <iostream>

using namespace std;

int SimpleStore::get(const char* key, char *value) {
	if (store.count(key)) {
		const char *source = store[key];
		int size = strlen(source);
		memcpy(value, source, size);		
		return size;
	} else {
		return -1;
	}
}


int SimpleStore::put(const char* key, const char *value, int len) {
	store[key] = value;
	return 0;
}

