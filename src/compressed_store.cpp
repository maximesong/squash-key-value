#include "compressed_store.h"

#include <iostream>

using namespace std;

CompressedStore::CompressedStore()
	: store([](const SimpleDataBlock *a, const SimpleDataBlock *b) {
			int result = a->compare(b);
			return result < 0;
		}) {
	/* empty */
}

CompressedStore::~CompressedStore() {
	cout << "~CompressedStore called" << endl;
	for (auto e : store) {
		delete e.first;
		delete e.second;
	}
}

int CompressedStore::get(const char* key, int key_size, char *dest) {
	SimpleDataBlock *key_block = new SimpleDataBlock{key, key_size};
	int r = -1;
	cout << "get" << endl;
	if (store.count(key_block)) {
		cout << "get here" << endl;
		r = store[key_block]->getData(dest);
		cout << "get there" << endl;
	}
	delete key_block;
	return r;
}


int CompressedStore::put(const char* key, int key_size, const char *value, int value_size) {
	SimpleDataBlock *key_block = new SimpleDataBlock{key, key_size};
	if (store.count(key_block)) {
		store[key_block]->setData(value, value_size);
		delete key_block;
	} else {
		cout << "put here" << endl;
		store[key_block] = new CompressedDataBlock(value, value_size);
		cout << "put there" << endl;
	}
	return 0;
}

int CompressedStore::size() {
	return store.size();
}
