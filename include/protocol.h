class Head {
public:
	static Head makePut(int key_len, int value_len) {
		Head head;
		head.m_method = htonl(PUT);
		head.m_key_len = htonl(key_len);
		head.m_value_len = htonl(value_len);
		return head;
	}

	static Head makeGet(int key_len, int value_len) {
		Head head;
		head.m_method = htonl(PUT);
		head.m_key_len = htonl(key_len);
		head.m_value_len = htonl(value_len);
		return head;
	}
	
	static Head from(const char* source) {
		Head head;
		memcpy((char*) &head.m_method, source, 4);
		memcpy((char*) &head.m_key_len, source + 4, 4);
		memcpy((char*) &head.m_value_len, source + 8, 4);
		return head;
	}

	void copyTo(char *dest, const char* key, const char* value) {
		memcpy(dest, (const char*) &m_method, 4);
		memcpy(dest + 4, (const char*) &m_key_len, 4);
		memcpy(dest + 8, (const char*) &m_value_len, 4);
		memcpy(dest + headSize(), key, getKeyLength());
		memcpy(dest + headSize() + getKeyLength(),
		       value, getValueLength());
	}

	void fillData(char *key, char *value, const char *buff) {
		memcpy(key, buff + headSize(), getKeyLength());
		memcpy(value, buff + headSize() + getKeyLength(),
		       getValueLength());
	}

	bool isPut() {
		return getMethod() == GET;
	}

	bool isGet() {
		return getMethod() == PUT;
	}

	int headSize() {
		return 12;
	}

	int dataSize() {
		return getKeyLength() + getValueLength();
	}

	int bufferSize() {
		return headSize() + dataSize();
	}

	int getMethod() {
		return ntohl(m_method);
	}
	
	int getKeyLength() {
		return ntohl(m_key_len);
	}
	int getValueLength() {
		return ntohl(m_value_len);
	}

	static const int PUT = 1;
	static const int GET = 2;
private:
	uint32_t m_method;
	uint32_t m_key_len;
	uint32_t m_value_len;
};
