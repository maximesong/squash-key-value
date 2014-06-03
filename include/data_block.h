class DataBlock {
public:
	virtual void setData(const char* data, int size) = 0;

	/**
	 * @return size of data, -1 if null.
	 */
	virtual int getData(char *dest) const = 0;

	/**
	 * @return size of data, -1 if null.
	 */
	virtual int getSize() const = 0;

	/**
	 * @return size of data, -1 if null.
	 */
	virtual int readData(char *dest) {
		return getData(dest);
	}
};
