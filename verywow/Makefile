TARGET=verywow
TARGETS=$(TARGET) solution/$(TARGET)
CXXFLAGS=-std=c++11 -g -Wall -Wextra
all:$(TARGETS)

clean:
	$(RM) *.o $(TARGETS)

check:all
	@test "`printf 'You are very smart!' | solution/$(TARGET)`" = 'Wow, so smart'
	test "`printf 'You are very smart!' | ./$(TARGET)`" = 'Wow, so smart'
