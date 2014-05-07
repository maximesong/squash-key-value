#include <iostream>
#include <string.h>

#include "SpookyV2.h"
#include "lz4.h"

using namespace std;

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

int main() {
	hash_example();
	compress_example();
	return 0;
}
