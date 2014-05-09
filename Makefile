CXX=clang++
CC=clang

LIB_INCLUDE_DIR=lib/include
INCLUDE_DIR=include

INCLUDE_FLAGS=-I $(LIB_INCLUDE_DIR) -I $(INCLUDE_DIR)

CPP_FLAGS=-std=c++11

PROGRAM=squash

objects=spooky.o lz4.o simple_store.o simple_data_block.o

all: $(PROGRAM)

$(PROGRAM): src/squash.cpp $(objects)
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) $^ -o $@

spooky.o: lib/SpookyV2.cpp
	$(CXX) $(INCLUDE_FLAGS) -c $< -o $@

lz4.o: lib/lz4.c
	$(CC) $(INCLUDE_FLAGS) -c $< -o $@

simple_store.o: src/simple_store.cpp
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) -c $< -o $@

simple_data_block.o: src/simple_data_block.cpp
	$(CXX) $(INCLUDE_FLAGS) $(CPP_FLAGS) -c $< -o $@

clean:
	rm -f $(PROGRAM) *.o
