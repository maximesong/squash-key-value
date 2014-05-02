all: squash

squash: squash.cpp
	clang++ $< -o $@
