#include <iostream>  // for std::cout
#include <random>    // for std::random_device and the likes
#include <string>    // for std::string
#include <fstream>   // for std::ifstream
#include <algorithm> // for std::count and std::find
#include <stdexcept> // for std::runtime_error

// check a few stuff on the picture and raise if needed
void check_validity(std::string const& picture) {
    if(std::count(picture.begin(), picture.end(), '\n') == 0)
        throw std::runtime_error("not even a single line");
    if(std::find(picture.begin(), picture.end(), 'w') != picture.end())
        throw std::runtime_error("Waldo symbol already in input file");
    if(std::all_of(picture.begin(), picture.end(), std::isblank))
        throw std::runtime_error("input file full of blank");
}

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
    check_validity(waldo);
    hide_waldo(waldo);
    std::cout << waldo;
    return 0;
}
