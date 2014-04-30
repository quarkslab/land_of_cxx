#include <iostream>  // for std::cout
#include <random>    // for std::random_device and the likes
#include <string>    // for std::string
#include <fstream>   // for std::ifstream

// modify the string `from' to hide a Waldo in a non-space location
void hide_waldo(std::string& from) {

    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<size_t> uniform_dist(0, from.size() - 1);

    // try as long as we can until we find a non-space location
    size_t pos;
    do {
        pos = uniform_dist(rnd);
    } while(std::isblank(from[pos]));
    from[pos] = 'w';
}

// the fun starts here
int main() {
    std::ifstream ifs{"waldo.txt"};
    std::string waldo{std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{}};
    hide_waldo(waldo);
    std::cout << waldo;
    return 0;
}
