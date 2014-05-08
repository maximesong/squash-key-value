#include <map>
#include <functional>

#include "store.h"

using namespace std;

class SimpleStore : public Store {
public:
	SimpleStore(): store([](const char* a, const char* b) {
			return strcmp(a, b) == -1;
		}) {
	}
	/**
	 * @return the size of the value, or -1 if the value it not available
	 */
	virtual int get(const char* key, char *value);


	virtual int put(const char* key, const char *value) {
		return put(key, value, strlen(value));
	}

	/**
	 * @return 0 if put is success, or -1 if not
	 */
	virtual int put(const char* key, const char *value, int len);

private:
    typedef struct {
        bool compressed;
        int temp;
        const char *value;
    } info;

	map<const char*, info,
	    std::function<bool(const char*, const char*)>> store;
};
