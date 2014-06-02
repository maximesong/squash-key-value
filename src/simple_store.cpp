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
		delete e.first;
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
	int key_len = strlen(key) + 1;
	char *key_copy = new char[key_len];
	memcpy(key_copy, key, key_len);
	if (store.count(key_copy)) {
		store[key_copy]->setData(value, size);
	} else {
		store[key_copy] = new SimpleDataBlock(value, size);
	}
	return 0;
}

int SimpleStore::size() {
	return store.size();
}
