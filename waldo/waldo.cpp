#include <iostream>  // for std::cout
#include <random>    // for std::random_device and the likes
#include <string>    // for std::string
#include <fstream>   // for std::ifstream
#include <algorithm> // for std::count and std::find
#include <stdexcept> // for std::runtime_error

// Handle loading and display of Waldo picture
class Waldo {
    std::string _picture;  //< the picture itself

    public:

    // constructor
    Waldo(std::string const& path);

    // common accessors
    std::string const& str() const { return _picture; }

    protected:
    // helper functions
    void hide_waldo();
    void check_validity() const;
};

// create a Waldo instance from a file description
// the description is assumed to be an asciiart image
// with at least one line and always the same number of columns
Waldo::Waldo(std::string const & path)
{
    std::ifstream ifs(path.c_str());
    _picture = std::string{std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{}};
    check_validity();
    hide_waldo();
}

// check a few stuff on the picture and raise if needed
void Waldo::check_validity() const {
    if(std::count(_picture.begin(), _picture.end(), '\n') == 0)
        throw std::runtime_error("not even a single line");
    if(std::find(_picture.begin(), _picture.end(), 'w') != _picture.end())
        throw std::runtime_error("Waldo symbol already in input file");
    if(std::all_of(_picture.begin(), _picture.end(), std::isblank))
        throw std::runtime_error("input file full of blank");
}

// modify the string `from' to hide a Waldo in a non-space location
void Waldo::hide_waldo() {

    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<size_t> uniform_dist(0, _picture.size() - 1);

    // try as long as we can until we find a non-space location
    size_t pos;
    do {
        pos = uniform_dist(rnd);
    } while(std::isblank(_picture[pos]));
    _picture[pos] = 'w';
}

// the fun starts here
int main() {
    Waldo waldo{"waldo.txt"};
    std::cout << waldo.str();
    return 0;
}
