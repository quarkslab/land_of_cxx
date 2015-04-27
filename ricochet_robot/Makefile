TARGET=ricochet_robot
TARGETS=$(TARGET) solution/$(TARGET)
CXXFLAGS=-std=c++11 -g -Wall -Wextra

all:$(TARGETS)

clean:
	$(RM) $(TARGETS)

check:all
	@printf "klk\n" | solution/$(TARGET) 2>&1 | diff -c session.txt -
	printf "klk\n" | ./$(TARGET) 2>&1 | diff -c session.txt -
