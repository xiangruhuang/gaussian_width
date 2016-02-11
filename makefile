
CXX ?= g++
CC ?= gcc
CFLAGS = -w -fopenmp -O3 -std=c++11

all: compute

.PHONY: compute

compute:
	$(CXX) $(CFLAGS) -o compute main.cpp
