#include "simple_data_block.h"

#include <string.h>
#include <assert.h>

SimpleDataBlock::SimpleDataBlock() {
	m_data = 0;
	m_size = -1;
}

SimpleDataBlock::SimpleDataBlock(const char* data, int size) {
	setData(data, size);
}

SimpleDataBlock::~SimpleDataBlock() {
	if (m_data != 0)
		delete [] m_data;
}

void SimpleDataBlock::setData(const char* data, int size) {
	assert(size > 0);

	if (m_data != 0)
		delete [] m_data;
	m_data = new char[size];
	memcpy(m_data , data, size);
	m_size = size;
}

int SimpleDataBlock::getData(char *dest) {
	if (m_data != 0)
		memcpy(dest, m_data, m_size);
	return m_size;
}

int SimpleDataBlock::getSize() {
	return m_size;
};
