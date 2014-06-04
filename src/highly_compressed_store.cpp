#include "highly_compressed_store.h"

#include <iostream>

using namespace std;

HighlyCompressedStore::HighlyCompressedStore()
	: store([](const SimpleDataBlock *a, const SimpleDataBlock *b) {
			int result = a->compare(b);
			return result < 0;
		}) {
	/* empty */
}

HighlyCompressedStore::~HighlyCompressedStore() {
	cout << "~HighlyCompressedStore called" << endl;
	for (auto e : store) {
		delete e.first;
		delete e.second;
	}
}

int HighlyCompressedStore::get(const char* key, int key_size, char *dest) {
	SimpleDataBlock *key_block = new SimpleDataBlock{key, key_size};
	int r = -1;
	if (store.count(key_block)) {
		r = store[key_block]->getData(dest);
	}
	delete key_block;
	return r;
}


int HighlyCompressedStore::put(const char* key, int key_size, const char *value, int value_size) {
	SimpleDataBlock *key_block = new SimpleDataBlock{key, key_size};
	if (store.count(key_block)) {
		store[key_block]->setData(value, value_size);
		delete key_block;
	} else {
		store[key_block] = new HighlyCompressedDataBlock(value, value_size);
	}
	return 0;
}

int HighlyCompressedStore::size() {
	return store.size();
}
