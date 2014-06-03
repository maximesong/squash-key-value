#include "simple_store.h"

#include <assert.h>

#include <iostream>

using namespace std;

SimpleStore::SimpleStore()
	: store([](const SimpleDataBlock *a, const SimpleDataBlock *b) {
			int result = a->compare(b);
			return result < 0;
		}) {
	/* empty */
}

SimpleStore::~SimpleStore() {
	cout << "~SimpleStore called" << endl;
	for (auto e : store) {
		delete e.first;
		delete e.second;
	}
}

int SimpleStore::get(const char* key, int key_size, char *dest) {
	cout << "GET" << endl;
	SimpleDataBlock *key_block = new SimpleDataBlock{key, key_size};
	int r = -1;
	cout << "Store: " << store.size() << endl;
	if (store.count(key_block)) {
		r = store[key_block]->getData(dest);
	} else {
		cout << "not count: " << store.size() << endl;
	}
	delete key_block;
	return r;
}


int SimpleStore::put(const char* key, int key_size, const char *value, int value_size) {
	SimpleDataBlock *key_block = new SimpleDataBlock{key, key_size};
	if (store.count(key_block)) {
		cout << "Old" << endl;
		store[key_block]->setData(value, value_size);
		delete key_block;
	} else {
		cout << "New" << endl;
		store[key_block] = new SimpleDataBlock(value, value_size);
	}
	return 0;
}

int SimpleStore::size() {
	return store.size();
}
