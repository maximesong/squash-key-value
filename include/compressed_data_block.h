#include "simple_data_block.h"

class CompressedDataBlock : public DataBlock {
public:
	CompressedDataBlock();

	CompressedDataBlock(const char* data, int size);

	virtual ~CompressedDataBlock();

	virtual void setData(const char* data, int size);

	virtual int getData(char *dest) const;

	virtual int getSize() const;

private:
	char * m_data;
	int m_size;
	int m_compressed_size;
};
