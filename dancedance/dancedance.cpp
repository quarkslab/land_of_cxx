#include "dancedance_term.hpp"

#include <iostream>


int main() {
    std::cout << "--<< Dance Dance Fingers >>--" << std::endl;

    std::cout << "Strike the 'h' key:";

    char input;
    {
        TermiOS tos;
        std::cin >> input;
    }

    if(input == 'h') {
        std::cout << std::endl << "Well done!" << std::endl;
        return 0;
    }
    else {
        std::cerr << std::endl << "No no no!" << std::endl;
        return 1;
    }
}
