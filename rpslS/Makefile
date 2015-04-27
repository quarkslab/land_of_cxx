TARGET=rpslS
TARGETS=$(TARGET) solution/$(TARGET)
CXXFLAGS=-std=c++11 -Wall -g -Wextra

all:$(TARGETS)

clean:
	$(RM) $(TARGETS)

check:all
	@printf "r\nr\nr\nr\nr\nr\nr\nr\n" | solution/$(TARGET) 1
	printf "r\nr\nr\nr\nr\nr\nr\nr\n" | ./$(TARGET) 1

