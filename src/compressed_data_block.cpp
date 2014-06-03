#include "compressed_data_block.h"

#include <assert.h>
#include <string.h>

#include "lz4.h"

CompressedDataBlock::CompressedDataBlock() {
	m_data = 0;
	m_size = -1;
	m_compressed_size = -1;
}

CompressedDataBlock::CompressedDataBlock(const char* data, int size)
	: CompressedDataBlock() {
	setData(data, size);
}

CompressedDataBlock::~CompressedDataBlock() {
	if (m_data != 0)
		delete [] m_data;
}

void CompressedDataBlock::setData(const char* data, int size) {
	assert(size >= 0);


	char *compressed_buffer = new char[LZ4_compressBound(size)];
	m_compressed_size = LZ4_compress(data, compressed_buffer, size);
	assert(m_compressed_size != 0);
	if (m_data != 0)
		delete [] m_data;
	m_data = new char[m_compressed_size];
	memcpy(m_data, compressed_buffer, m_compressed_size);
	delete [] compressed_buffer;
	m_size = size;
}

int CompressedDataBlock::getData(char *dest) const {
	assert(m_data != 0);
	int uncompreesed_size = LZ4_decompress_safe(m_data, dest, 
						    m_compressed_size, m_size);
	assert(uncompreesed_size == m_size);
	return m_size;
}

int CompressedDataBlock::getSize() const {
	return m_size;
};
