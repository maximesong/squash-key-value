#include "simple_store.h"

#include <iostream>

using namespace std;

int SimpleStore::get(const char* key, char *dest) {
	if (store.count(key)) {
		return store[key]->getData(dest);
	} else {
		return -1;
	}
}


int SimpleStore::put(const char* key, const char *value, int size) {
	if (store.count(key)) {
		store[key]->setData(value, size);
	} else {
		store[key] = new SimpleDataBlock(value, size);
	}
	return 0;
}
