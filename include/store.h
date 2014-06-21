#ifndef STORE_H
#define STORE_H

#include <string>
#include <string.h>

#include "constants.h"

using namespace std;

class Store {
public:
	char buffer[MAX_BUFFER_SIZE];

	/**
	 * @return the size of the value, or -1 if the value it not available
	 */
	virtual int get(const char *key, int key_size, char *dest) = 0;

	virtual int get_str(const char *key, char *dest) {
		return get(key, strlen(key) + 1, dest);
	}

	virtual string get_str(const char *key) {
		int len = get_str(key, buffer);
		if (len != -1) {
			return string{buffer};
		} else {
			return string{""};
		}
	}
	
	virtual string get_str(string key) {
		return get_str(key.c_str());
	}

	/**
	 * @return 0 if put is success, or -1 otherwise
	 */
	virtual int put(const char *key, int key_size, const char *value, int value_size) = 0;

	virtual int put_str(const char* key, const char *value) {
		return put_str(key, value, strlen(value) + 1);
	};

	virtual int put_str(const string &key, const string &value) {
		return put_str(key.c_str(), value.c_str());
	}

	virtual int put_str(const char *key, const char *value, int value_size) {
		return put(key, strlen(key) + 1, value, value_size);
	}

};

#endif
