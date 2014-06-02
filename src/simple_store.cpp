#include "simple_store.h"

#include <iostream>

using namespace std;

SimpleStore::SimpleStore()
	: store([](const SimpleDataBlock *a, const SimpleDataBlock *b) {
			int a_size = a->getSize();
			int b_size = b->getSize();
			int min_size =  < a_size < b_size ? a_size : b_size;
			int cmp = memcmp(a->getData(), b->getData());
			if (cmp != 0) {
				return cmp;
			} else {
				return a_size < b_size;
			}
			
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
	SimpleDataBlock key_block{key, key_size};
	if (store.count(key_block)) {
		return store[key_block]->getData(dest);
	} else {
		return -1;
	}
}


int SimpleStore::put(const char* key, int key_size, const char *value, int value_size) {
	SimpleDataBlock key_block{key, key_size};
	if (store.count(key_block)) {
		store[key_block]->setData(value, value_size);
	} else {
		store[key_block] = new SimpleDataBlock(value, value_size);
	}
	return 0;
}

int SimpleStore::size() {
	return store.size();
}
