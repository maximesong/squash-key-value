#ifndef SIMPLE_DATA_BLOCK_H
#define SIMPLE_DATA_BLOCK_H

#include "data_block.h"

class SimpleDataBlock : public DataBlock {
public:
	SimpleDataBlock();

	SimpleDataBlock(const char* data, int size);

	virtual ~SimpleDataBlock();

	virtual void setData(const char* data, int size);

	virtual int getData(char *dest) const;

	virtual const char *getData() const;

	virtual int getSize() const;

	virtual int compare(const SimpleDataBlock *block) const;

private:
	char * m_data;
	int m_size;
};

#endif
