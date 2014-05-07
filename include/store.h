#include <string.h>

class Store {
public:
	/**
	 * @return the size of the value, or -1 if the value it not available
	 */
	virtual int get(const char* key, char *value) = 0;

	/**
	 * @return 0 if put is success, or -1 if not
	 */
	virtual int put(const char* key, const char *value) {
		return put(key, value, strlen(value));
	}

	/**
	 * @return 0 if put is success, or -1 if not
	 */
	virtual int put(const char* key, const char *value, int len) = 0;
};
