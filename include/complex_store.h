#include <functional>
#include <list>
#include <string.h>
#include <map>
#include "store.h"
#include "simple_data_block.h"

using namespace std;

class ComplexStore : public Store {
public:
    ComplexStore();
	/**
	 * @return the size of the value, or -1 if the value it not available
	 */
	virtual int get(const char* key, int key_size, char *value);


	// virtual int put(const char* key, const char *value) {
	// 	return put(key, strlen(key) + 1, value, strlen(value)+1);
	// }

	/**
	 * @return 0 if put is success, or -1 if not
	 */
	virtual int put(const char* key, int key_size, const char *value, int value_size);

    unsigned long long getTime(void);

    double calcTemp(double temp,int time);

    void compressing(void);

private:
    typedef struct {
        bool compressed;
        double temp;
        unsigned long long last_atime;
        int compressed_size;
        char *value;
    } info;

    map<SimpleDataBlock*, info,std::function<bool(const SimpleDataBlock*, const SimpleDataBlock*)>> store;

    unsigned long long last_compressed_time = 0;
    list<SimpleDataBlock*> hot_list;
};
