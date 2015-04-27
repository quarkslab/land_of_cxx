TARGET=closest_to_average
TARGETS=$(TARGET) solution/$(TARGET)
CXXFLAGS=-std=c++11 -g -Wall -Wextra

all:$(TARGETS)

clean:
	$(RM) $(TARGETS)

check:all
	@test "`solution/$(TARGET) 1 2 3`" = "2 at index 1"
	test "`./$(TARGET) 1 2 3`" = "2 at index 1"
