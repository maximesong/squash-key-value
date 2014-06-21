#ifndef SIMPLE_STORE_H
#define SIMPLE_STORE_H

#include <map>
#include <functional>

#include "store.h"
#include "simple_data_block.h"

using namespace std;

class SimpleStore : public Store {
public:
	SimpleStore();
	
	virtual ~SimpleStore();

	/**
	 * @return the size of the value, or -1 if the value it not available
	 */
	virtual int get(const char* key, int key_size, char *dest);

	/**
	 * @return 0 if put is success, or -1 if not
	 */
	virtual int put(const char* key, int key_size, const char *value, int value_size);

	virtual int size();
private:
	map<SimpleDataBlock*, SimpleDataBlock*,
	    std::function<bool(const SimpleDataBlock*, const SimpleDataBlock*)>> store;
};

#endif
