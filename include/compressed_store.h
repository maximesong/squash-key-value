#include "store.h"

#include <map>
#include <functional>
#include "compressed_data_block.h"

class CompressedStore : public Store {
public:
	CompressedStore();

	virtual ~CompressedStore();

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
	map<SimpleDataBlock*, CompressedDataBlock*,
	    std::function<bool(const SimpleDataBlock*, const SimpleDataBlock*)>> store;
};
