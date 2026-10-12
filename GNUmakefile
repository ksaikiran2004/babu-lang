CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2

.PHONY: all test clean

all: babu

babu: src/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: babu
	bash tests/run_tests.sh ./babu

clean:
	rm -f babu