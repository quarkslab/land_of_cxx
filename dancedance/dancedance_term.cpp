#include "dancedance_term.hpp"

#include <iostream>
#include <unistd.h>

// gets current state, save it
// and sets the state to raw mode
TermiOS::TermiOS() {
    if(!tcgetattr(STDIN_FILENO, &_orig))
        cfmakeraw(&_raw);
}

// restore saved state
TermiOS::~TermiOS() {
    tcsetattr(STDIN_FILENO, TCSANOW, &_orig);
}
