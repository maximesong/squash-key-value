#include <iostream>
#include "SpookyV2.h"

int main() {
	uint64 hash = SpookyHash::Hash64("Haha", 4, 0);
	return 0;
}
