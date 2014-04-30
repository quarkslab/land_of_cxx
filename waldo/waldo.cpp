#include <iostream>  // for std::cout
#include <string>    // for std::string
#include <fstream>   // for std::ifstream

// the fun starts here
int main() {
    std::ifstream ifs{"waldo.txt"};
    std::string waldo{std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{}};
    std::cout << waldo;
    return 0;
}
