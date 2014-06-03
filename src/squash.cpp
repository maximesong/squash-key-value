#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>

#include <assert.h>

#include "SpookyV2.h"
#include "lz4.h"
#include "complex_store.h"
#include "simple_store.h"
#include "resource_monitor.h"

using namespace std;

const char *PID_FILENAME="squash.pid";

void hash_example() {
	cout << "##### Start Hash Example" << endl;

	uint64 hash1 = SpookyHash::Hash64("Haha", 4, 0);
	uint64 hash2 = SpookyHash::Hash64("Hah", 3, 0);

	uint32 hash3 = SpookyHash::Hash32("Haha", 4, 0);
	uint32 hash4 = SpookyHash::Hash32("Hah", 3, 0);
	
	cout << "hash1: " << hash1 << endl;
	cout << "hash2: " << hash2 << endl;
	cout << "hash3: " << hash3 << endl;
	cout << "hash4: " << hash4 << endl;

	cout << "##### End Hash Example" << endl;
}

void compress_example() {
	cout << "##### Start Compress Example" << endl;

	char compressed[1024];
	char decompressed[1024];
	char source[] = "Please compress this string for me. compress this ...";

	int source_size = strlen(source);
	int compressed_size = LZ4_compress(source, compressed, source_size);
	int decompressed_size = LZ4_decompress_safe(compressed, decompressed,
						    compressed_size, 1024);

	cout << "source text: " << source << endl;
	cout << "source size: " << source_size << endl;
	cout << "compressed size: " << compressed_size << endl;
	cout << "decompressed size: " << decompressed_size << endl;
	decompressed[decompressed_size] = '\0';
	cout << "decompressed text: " << decompressed << endl;

	cout << "##### End Compress Example" << endl;
}

void store_example() {
	ComplexStore store;
	store.put_str("hi", "01234vczvsfqwervcxzfadsfqwegdxczbxcvdsdfqergfdbcxbfdsafewqr");
    store.put_str("a","123456xxxxxxxxxxxxxx");
    store.put_str("b","234567xxxxxxxxxxxxx");
    store.put_str("c","345678yyyyyyyyyyyyyyyy");
    store.put_str("d","456789zzzzzzzzzzzz");

	char value[100];
    //sleep(5);
	int size = store.get_str("hi", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
	

    //cout << store.get_str("h1", value) << endl;
    
    sleep(2);    
	size = store.get_str("a", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
    sleep(2); 
	size = store.get_str("b", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
    sleep(3);    
	size = store.get_str("a", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
    
	size = store.get_str("a", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
	size = store.get_str("a", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
    sleep(3);    
	size = store.get_str("b", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
	size = store.get_str("hi", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
    }
}

void simple_store_example() {
	SimpleStore store;
}

void write_pid() {
	fstream fout(PID_FILENAME, ios_base::out | ios_base::trunc);
	fout << getpid() << endl;
}

void remove_pid() {
	remove(PID_FILENAME);
}

void memory_example() {
	int i = 0;
	cout << ResourceMonitor::getMemoryUsage() << "k" << endl;
	while (i < 500000) {
		char *b = new char[1024]; // 1k
		++i;
	}
	cout << ResourceMonitor::getMemoryUsage() << "k" << endl;
}

void gen_random(char *s, const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = '\0';
}

void test_simple_store(int key_size = 1024, int value_size = MAX_VALUE_SIZE, int count = 500) {
	SimpleStore store;
	for (int i = 0; i != count; ++i) {
		cout << value_size << endl;
		char *key = new char[key_size + 1];
		char *value = new char[value_size + 1];
		char *back_value = new char[value_size + 1];
		gen_random(key, key_size);
		gen_random(value, value_size);
		store.put_str(key, value);
		store.get_str(key, back_value);
		assert(strcmp(value, back_value) == 0);
	}
}

void test_simple_store0() {
	cout << MAX_VALUE_SIZE << endl;
	char *key = new char[1024 + 1];
	char *value = new char[MAX_VALUE_SIZE + 1];
	gen_random(key, 1024);
	gen_random(value, MAX_VALUE_SIZE);
	SimpleStore store;
	store.put_str("Helo", "Hi");
	string r = store.get_str("Helo");
	cout << r << endl;
}

int main() {
//	write_pid();
//	memory_example();
//	test_simple_store();
	test_simple_store();
	return 0;
}
