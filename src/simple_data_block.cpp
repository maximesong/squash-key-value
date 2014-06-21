#include "simple_data_block.h"

#include <assert.h>
#include <string.h>

SimpleDataBlock::SimpleDataBlock() {
	m_data = 0;
	m_size = -1;
}

SimpleDataBlock::SimpleDataBlock(const char* data, int size)
	: SimpleDataBlock() {
	setData(data, size);
}

SimpleDataBlock::~SimpleDataBlock() {
	if (m_data != 0)
		delete [] m_data;
}

void SimpleDataBlock::setData(const char* data, int size) {
	assert(size >= 0);

	if (m_data != 0)
		delete [] m_data;
	m_data = new char[size];
	memcpy(m_data, data, size);
	m_size = size;
}

int SimpleDataBlock::getData(char *dest) const {
	assert(m_data != 0);
	memcpy(dest, m_data, m_size);
	return m_size;
}

const char *SimpleDataBlock::getData() const {
	return m_data;
}

int SimpleDataBlock::getSize() const {
	return m_size;
};

int SimpleDataBlock::compare(const SimpleDataBlock *b) const {
	int a_size = getSize();
	int b_size = b->getSize();
	int min_size = a_size < b_size ? a_size : b_size;
	assert(a_size >= 0);
	assert(b_size >= 0);
	int cmp = memcmp(getData(), b->getData(), min_size);
	if (cmp != 0) {
		return cmp;
	} else {
		return a_size - b_size;
	}
}
