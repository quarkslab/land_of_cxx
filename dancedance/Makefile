TARGET=dancedance
TARGETS=$(TARGET) solution/$(TARGET)
CXXFLAGS=-std=c++11 -g -Wall -Wextra
CPPFLAGS=-I.

all:$(TARGETS)

clean:
	$(RM) *.o solution/*.o $(TARGETS)

check:all
	@printf ddfisfun | solution/$(TARGET)
	printf ddfisfun | ./$(TARGET)

dancedance:dancedance.o dancedance_term.o
	$(CXX) $^ -o $@

solution/dancedance:solution/dancedance.o dancedance_term.o
	$(CXX) $^ -o $@
