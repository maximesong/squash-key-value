#include "highly_compressed_data_block.h"

#include <assert.h>
#include <string.h>

#include "lz4.h"
#include "lz4hc.h"

HighlyCompressedDataBlock::HighlyCompressedDataBlock() {
	m_data = 0;
	m_size = -1;
	m_compressed_size = -1;
}

HighlyCompressedDataBlock::HighlyCompressedDataBlock(const char* data, int size)
	: HighlyCompressedDataBlock() {
	setData(data, size);
}

HighlyCompressedDataBlock::~HighlyCompressedDataBlock() {
	if (m_data != 0)
		delete [] m_data;
}

void HighlyCompressedDataBlock::setData(const char* data, int size) {
	assert(size >= 0);


	char *compressed_buffer = new char[LZ4_compressBound(size)];
	m_compressed_size = LZ4_compressHC(data, compressed_buffer, size);
	assert(m_compressed_size != 0);
	if (m_data != 0)
		delete [] m_data;
	m_data = new char[m_compressed_size];
	memcpy(m_data, compressed_buffer, m_compressed_size);
	delete [] compressed_buffer;
	m_size = size;
}

int HighlyCompressedDataBlock::getData(char *dest) const {
	assert(m_data != 0);
	int uncompreesed_size = LZ4_decompress_safe(m_data, dest, 
						    m_compressed_size, m_size);
	assert(uncompreesed_size == m_size);
	return m_size;
}

int HighlyCompressedDataBlock::getSize() const {
	return m_size;
};
