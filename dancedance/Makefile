TARGET=dancedance
CXXFLAGS=-std=c++11 -g -Wall -Wextra
CXX=clang++
all:$(TARGET)

dancedance:dancedance.o dancedance_term.o
	$(CXX) $^ -o $@
