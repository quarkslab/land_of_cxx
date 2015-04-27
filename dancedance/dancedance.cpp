/* .~= dance dance fingers =~.
 *
 * This game prints character on the screen and the player must type them as
 * fast as possible to get its timing.
 *
 * The string printed on the string is displayed character by character, if a
 * character is incorrect, then the game stops. When all characters are
 * correctly typed, the game ends and the time taken to type all the characters
 * is printed out.
 *
 * To complete this exercise, you can use:
 *
 * - #include
 * - const qualifier
 * - range based loops
 * - operator>>(std::istream&, char)
 * - operator<<(std::ostream&, char)
 * - std::chrono::system_clock::now()
 * - std::string::size
 *
 */

// FIXME: get the right declaration of TermiOS from dancedance_term.hpp
struct TermiOS {};

#include <iostream>
#include <string>
#include <chrono>


int main() {
    std::cout << "--<< Dance Dance Fingers >>--" << std::endl;

    // melody to type with your finger
    // FIXME: make sure this string is never modified
    std::string melody = "ddfisfun";

    // game duration
    auto now = std::chrono::system_clock::now();

    // number of notes successfully typed
    size_t count = 0;
    {
        TermiOS tos; // this prevents the terminal from buffering the input. Just believe me, it does!

        // let's echo the notes and check the user types them correctly
        //TODO: iterate over the character in `melody', print one, get the user input, check, rince&repeat
    }

    // FIXME: use the appropriate check
    if(count == 0) {
      // TODO: tell the user is not as good as he could be
      return 1;
    }
    else {
        // FIXME: get the right clock value
        auto end = now;
        std::cout << std::endl << "Winner! (" << count << " letters)"
                  << std::endl << "in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - now).count()  << "ms" << std::endl;
        return 0;
    }
}
