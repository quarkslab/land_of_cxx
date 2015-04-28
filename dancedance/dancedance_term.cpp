#include "dancedance_term.hpp"

#include <iostream>
#include <unistd.h>

// gets current state, save it
// and sets the state to raw mode
TermiOS::TermiOS() {
    if(tcgetattr(STDIN_FILENO, &_orig))
        std::cerr << "failed to get current mode" << std::endl;
    else {
        cfmakeraw(&_raw);
        if(tcsetattr(STDIN_FILENO, TCSANOW, &_raw))
            std::cerr <<  "failed to use raw mode" << std::endl;
    }
}

// restore saved state
TermiOS::~TermiOS() {
    if(tcsetattr(STDIN_FILENO, TCSANOW, &_orig))
        std::cerr << "failed to restore raw mode" << std::endl;
}
