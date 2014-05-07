CC=clang++

LIB_INCLUDE_DIR=lib/include

INCLUDE_FLAG=-I $(LIB_INCLUDE_DIR)

PROGRAM=squash

all: $(PROGRAM)

$(PROGRAM): src/squash.cpp spooky.o lz4.o
	clang++ $(INCLUDE_FLAG) $^ -o $@

spooky.o: lib/SpookyV2.cpp
	clang++ $(INCLUDE_FLAG) -c $< -o $@

lz4.o: lib/lz4.c
	clang $(INCLUDE_FLAG) -c $< -o $@

clean:
	rm -f $(PROGRAM) *.o
