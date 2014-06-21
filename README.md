squash-key-value
================

# 目录结构

```
|-- include
|   |-- Clock.h
|   |-- complex_store.h
|   |-- compressed_data_block.h
|   |-- compressed_store.h
|   |-- constants.h
|   |-- data_block.h
|   |-- Data.h
|   |-- highly_compressed_data_block.h
|   |-- highly_compressed_store.h
|   |-- protocol.h
|   |-- resource_monitor.h
|   |-- simple_data_block.h
|   |-- simple_store.h
|   |-- store.h
|   `-- WorkSetStore.h
|-- lib
|   |-- include
|   |   |-- json11.hpp
|   |   |-- lz4.h
|   |   |-- lz4hc.h
|   |   `-- SpookyV2.h
|   |-- json11.cpp
|   |-- lz4.c
|   |-- lz4hc.c
|   `-- SpookyV2.cpp
|-- Makefile
|-- README.md
|-- src
|   |-- complex_store.cpp
|   |-- compressed_data_block.cpp
|   |-- compressed_store.cpp
|   |-- highly_compressed_data_block.cpp
|   |-- highly_compressed_store.cpp
|   |-- resource_monitor.cpp
|   |-- simple_data_block.cpp
|   |-- simple_store.cpp
|   |-- squash-client.cpp
|   |-- squash.cpp
|   `-- squash-server.cpp
`-- tools
    |-- download_sites.js
    |-- index.js
    |-- missing_sites.json
    |-- package.json
    |-- sites.json
    `-- top-sites.json
```

- include/, src/ 中包含我们对于squash的实现
- lib/ 中包含我们在squash中使用到的程序库的源代码
- tools/ 包含抓取alexa全球排名前500站点主页的脚本
- README.md 本文档，描述项目有关信息

# 项目运行

- 项目依赖: 安装clang++

- 选择编译的Store类型: 修改src/下squash-server.cpp, squash.cpp中Store的具体实例化类型，可供选择的有SimpleStore, ComplexStore, CompressedStore, HighlyCompressedStore, WorkSetStore

- 编译: make

- 编译产物
    - server: server程序
	- client: client程序
	- squash: 用于单个store独立的测试

- 运行
    - 首先开启一个终端，运行./server
	- 开启另一个终端，运行./client

# server，client交互

squash的server, client间通过socket进行通讯，传送数据的起始字节定义了整个数据的类型和长度。其结构如下所示。

```c++
// quoted from include/protocol.h, this is simplified version 
class Head {
	static const int PUT = 1;   // client request for PUT
	static const int GET = 2;   // client request for GET
	static const int OK = 3;    // server response for success
	static const int MISS = 4;  // server reponse for key miss
	static const int STATS = 5; // client request for server memory status
private:
	uint32_t m_method;
	uint32_t m_key_len;
	uint32_t m_value_len;
```

# Store接口

所以类型的Store都继承基类Store，提供get和put方法。

```c++
// quoted from include/store.h, helper functions removed here for simplicity
class Store {
public:
	/**
	 * @return the size of the value, or -1 if the value it not available
	 */
	virtual int get(const char *key, int key_size, char *dest) = 0;

	/**
	 * @return 0 if put is success, or -1 otherwise
	 */
	virtual int put(const char *key, int key_size, const char *value, int value_size) = 0;
};
```

# SimpleStore

- 使用std::map作为数据的索引结构
- 对数据不进行压缩

# CompressedStore

- 使用std::map作为数据的索引结构
- 对所有value，使用lz4算法进行压缩

# HighlyCompressedStore

- 使用std::map作为数据的索引结构
- 对所有value，使用高压缩率的lz4算法进行压缩
