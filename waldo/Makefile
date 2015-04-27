TARGET=waldo
TARGETS=$(TARGET) solution/$(TARGET)
CXXFLAGS=-Wall -std=c++11 -Werror

all:$(TARGETS)

clean:
	$(RM) $(TARGETS)

check:all
	@! { solution/$(TARGET) | diff waldo.txt - 2>&1 1> /dev/null; }
	! { solution/$(TARGET) | diff waldo.txt - ; }


