#include <iostream>
#include "SpookyV2.h"

using namespace std;

int main() {
	uint64 hash = SpookyHash::Hash64("Haha", 4, 0);
	uint64 hash2 = SpookyHash::Hash64("Hah", 4, 0);

	cout << hash << endl;
	cout << hash2 << endl;
	return 0;
}
