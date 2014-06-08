#include "dancedance_term.hpp"

#include <iostream>
#include <string>
#include <chrono>


int main() {
    std::cout << "--<< Dance Dance Fingers >>--" << std::endl;

    // melody to type with your finger
    const std::string melody = "ddfisfun";

    // game duration
    auto now = std::chrono::system_clock::now();

    // number of notes successfully typed
    // set to 0 in case of error
    size_t count = 0;
    {
        TermiOS tos;
        // let's echo the notes and check the user types them correctly
        for(char note : melody) {
            std::cout << note;
            char in;
            std::cin >> in;
            if(in != note) {
                count = 0;
                break;
            }
            else {
                ++count;
            }
        }
    }

    // error handling
    if(count == 0) {
        std::cerr << std::endl << "Looser!" << std::endl;
        return 1;
    }
    else {
        auto end = std::chrono::system_clock::now();
        std::cout << std::endl << "Winner! (" << count << " letters)"
                  << std::endl << "in " << (end - now).count() / 1000000. << "s" << std::endl;
        return 0;
    }
}
