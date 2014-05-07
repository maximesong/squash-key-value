CC=clang++

all: squash

squash: squash.cpp spooky.o
	clang++ -I lib/include $^ -o $@

spooky.o: lib/SpookyV2.cpp
	clang++ -c -I lib/include $< -o $@
