#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>

#include "SpookyV2.h"
#include "lz4.h"
#include "complex_store.h"

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
	store.put("hi", "012345");
    store.put("a","123456");
    store.put("b","234567");
    store.put("c","345678");
    store.put("d","456789");

	char value[100];
    //sleep(5);
	int size = store.get("hi", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
	
    //cout << store.get("h1", value) << endl;
    
    sleep(2);    
	size = store.get("a", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
    sleep(2); 
	size = store.get("b", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
    sleep(3);    
	size = store.get("a", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
    
	size = store.get("a", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
	size = store.get("a", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
    sleep(3);    
	size = store.get("b", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
	}
	size = store.get("hi", value);
	if (size > 0) {
		value[size] = '\0';
		cout << value << endl;
    }
}

void write_pid() {
	fstream fout(PID_FILENAME, ios_base::out | ios_base::trunc);
	fout << getpid() << endl;
}

void remove_pid() {
	remove(PID_FILENAME);
}

int main() {
	write_pid();

	hash_example();
	compress_example();
	store_example();

	remove_pid();
	return 0;
}