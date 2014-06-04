#include "simple_data_block.h"

class HighlyCompressedDataBlock : public DataBlock {
public:
	HighlyCompressedDataBlock();

	HighlyCompressedDataBlock(const char* data, int size);

	virtual ~HighlyCompressedDataBlock();

	virtual void setData(const char* data, int size);

	virtual int getData(char *dest) const;

	virtual int getSize() const;

private:
	char * m_data;
	int m_size;
	int m_compressed_size;
};
