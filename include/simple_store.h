#include <map>
#include <functional>
#include <string.h>

#include "store.h"
#include "simple_data_block.h"

using namespace std;

class SimpleStore : public Store {
public:
	SimpleStore(): store([](const char *a, const char *b) {
			return strcmp(a, b) < 0;
		}) {
	}

	~ SimpleStore() {
		for (auto e : store) {
			delete e.second;
		}
	}

	/**
	 * @return the size of the value, or -1 if the value it not available
	 */
	virtual int get(const char* key, char *dest);

	/**
	 * @return 0 if put is success, or -1 if not
	 */
	virtual int put(const char* key, const char *value, int size);

private:
	map<const char*, SimpleDataBlock*,
	    std::function<bool(const char*, const char*)>> store;
};
