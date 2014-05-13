class Store {
public:
	/**
	 * @return the size of the value, or -1 if the value it not available
	 */
	virtual int get(const char* key, char *dest) = 0;

	/**
	 * @return 0 if put is success, or -1 otherwise
	 */
	virtual int put_str(const char* key, const char *value) {
		return put(key, value, strlen(value) + 1);
	};

	/**
	 * @return 0 if put is success, or -1 otherwise
	 */
	virtual int put(const char* key, const char *value, int size) = 0;
};
