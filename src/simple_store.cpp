#include "simple_store.h"

#include <iostream>

using namespace std;

SimpleStore::SimpleStore()
	: store([](const char *a, const char *b) {
			return strcmp(a, b) < 0; }) {
	/* empty */
}

SimpleStore::~SimpleStore() {
	for (auto e : store) {
		delete e.second;
	}
}

int SimpleStore::get(const char* key, char *dest) {
	if (store.count(key)) {
		return store[key]->getData(dest);
	} else {
		return -1;
	}
}


int SimpleStore::put(const char* key, const char *value, int size) {
	cout << "SimpleStore:: put size: " << size << endl;
	if (store.count(key)) {
		store[key]->setData(value, size);
	} else {
		store[key] = new SimpleDataBlock(value, size);
	}
	return 0;
}
