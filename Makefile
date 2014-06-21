CXX=clang++
CC=clang

LIB_INCLUDE_DIR=lib/include
INCLUDE_DIR=include

INCLUDE_FLAGS=-I $(LIB_INCLUDE_DIR) -I $(INCLUDE_DIR)

CPP_FLAGS=-std=c++11

PROGRAM=squash

MAIN=main

SERVER_PROGRAM=server

CLIENT_PROGRAM=client

objects=spooky.o lz4.o json.o simple_store.o simple_data_block.o \
	complex_store.o resource_monitor.o

all: $(PROGRAM) $(SERVER_PROGRAM) $(CLIENT_PROGRAM)

$(SERVER_PROGRAM):  src/squash-server.cpp $(objects) 
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) $^ -o $@

$(CLIENT_PROGRAM): src/squash-client.cpp $(objects)
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) $^ -o $@

$(PROGRAM): src/squash.cpp $(objects)
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) $^ -o $@

spooky.o: lib/SpookyV2.cpp
	$(CXX) $(INCLUDE_FLAGS) -c $< -o $@

lz4.o: lib/lz4.c
	$(CC) $(INCLUDE_FLAGS) -c $< -o $@

json.o: lib/json11.cpp
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) -c $^ -o $@

complex_store.o: src/complex_store.cpp
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) -c $< -o $@

simple_store.o: src/simple_store.cpp
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) -c $< -o $@

simple_data_block.o: src/simple_data_block.cpp
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) -c $< -o $@

resource_monitor.o: src/resource_monitor.cpp
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) -c $< -o $@

squash-server.o: src/squash-server.cpp
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) -c $< -o $@

clean:
	rm -f $(PROGRAM) client server *.o *.a *~ src/*~ include/*~
