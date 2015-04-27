TARGET=asciify
TARGETS=$(TARGET) solution/$(TARGET)
CXXFLAGS=-std=c++11 -g -Wall -Wextra

all:$(TARGETS)

clean:
	$(RM) *.o $(TARGETS)

check:all
	@solution/$(TARGET) lena-small.pgm | cmp lena-small.txt -
	./$(TARGET) lena-small.pgm | cmp lena-small.txt -
