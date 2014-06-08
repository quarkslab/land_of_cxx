#ifndef DANCEDANCE_TERM_HPP
#define DANCEDANCE_TERM_HPP

#include <termios.h>
/* This class modifies the terminal states upon construction
 * and restores it to its previous value upon deletion.
 */
class TermiOS {

    // original and modified states
    termios _orig, _raw;

    public:
    TermiOS();
    ~TermiOS();
};

#endif
