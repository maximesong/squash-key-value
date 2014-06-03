#include "simple_store.h"

#include <assert.h>

#include <iostream>

using namespace std;

SimpleStore::SimpleStore()
	: store([](const SimpleDataBlock *a, const SimpleDataBlock *b) {
			int a_size = a->getSize();
			int b_size = b->getSize();
			int min_size = a_size < b_size ? a_size : b_size;
			assert(a_size >= 0);
			assert(b_size >= 0);
//			char *a_str = new char[a_size];
//			char *b_str = new char[b_size];
//			a->getData(a_str);
//			b->getData(b_str);
			int cmp = memcmp(a->getData(), b->getData(), min_size);
//			delete [] a_str;
//			delete [] b_str;
			if (cmp != 0) {
				return cmp;
			} else {
				return a_size - b_size;
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
	SimpleDataBlock *key_block = new SimpleDataBlock{key, key_size};
	int r = -1;
	if (store.count(key_block)) {
		r = store[key_block]->getData(dest);
	}
	delete key_block;
	return r;
}


int SimpleStore::put(const char* key, int key_size, const char *value, int value_size) {
	SimpleDataBlock *key_block = new SimpleDataBlock{key, key_size};
	if (store.count(key_block)) {
		store[key_block]->setData(value, value_size);
		delete key_block;
	} else {
		store[key_block] = new SimpleDataBlock(value, value_size);
	}
	return 0;
}

int SimpleStore::size() {
	return store.size();
}
