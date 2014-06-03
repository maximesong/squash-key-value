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

private:
	char * m_data;
	int m_size;
};
