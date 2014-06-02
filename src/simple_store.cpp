#include "simple_store.h"

#include <iostream>

using namespace std;

SimpleStore::SimpleStore()
	: store([](const char *a, const char *b) {
			cout << "cmp" << endl << a << endl << b << endl
			     << strcmp(a, b) << endl;
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
	// cout << "SimpleStore:: put size: " << size << endl;
	// cout << "SimpleStore:: key: " << key << endl;
	// cout << "SimpleStore:: key cout : " << store.count("abc") << endl;
	// cout << "SimpleStore:: put end" << endl;
	int key_len = strlen(key) + 1;
	char *key_copy = new char[key_len];
	memcpy(key_copy, key, key_len);
	if (store.count(key_copy)) {
		cout << "SimpleStore:: store size: " << store.size() << endl;
		store[key_copy]->setData(value, size);
	} else {
		cout << "SimpleStore:: new store size: " << store.size() << endl;

		store[key_copy] = new SimpleDataBlock(value, size);
	}
	return 0;
}

int SimpleStore::size() {
	return store.size();
}
