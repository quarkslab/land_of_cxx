#include <iostream>  // for std::cout
#include <random>    // for std::random_device and the likes
#include <string>    // for std::string
#include <fstream>   // for std::ifstream
#include <algorithm> // for std::count and std::find
#include <stdexcept> // for std::runtime_error

// An exception specialized fro Waldo Errors
struct InvalidWaldo : std::runtime_error {
    InvalidWaldo(char const what[] = "") : std::runtime_error(what)
    {
    }
};

// Handle loading and display of Waldo picture
class Waldo {
    std::string picture_;  //< the picture itself

    public:

    // constructor
    Waldo(std::string const& path);

    // common accessors
    std::string const& str() const { return picture_; }

    static const char waldo = 'w';

    protected:
    // helper functions
    void hide_waldo();
    void check_validity() const;

    private:
    void at_least_a_line() const;
    void no_waldo() const;
    void at_least_a_symbol() const;
};

const char Waldo::waldo;

// create a Waldo instance from a file description
// the description is assumed to be an asciiart image
// with at least one line and always the same number of columns
Waldo::Waldo(std::string const & path)
{
    std::ifstream ifs(path.c_str());
    picture_ = std::string{std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{}};
    check_validity();
    hide_waldo();
}

// check a few stuff on the picture and raise if needed
void Waldo::check_validity() const {
    at_least_a_line();
    no_waldo();
    at_least_a_symbol();
}

void Waldo::at_least_a_line() const {
    if(std::count(picture_.begin(), picture_.end(), '\n') == 0)
        throw InvalidWaldo("not even a single line");
}
void Waldo::no_waldo() const {
    if(std::find(picture_.begin(), picture_.end(), waldo) != picture_.end())
        throw InvalidWaldo("Waldo symbol already in input file");
}
void Waldo::at_least_a_symbol() const {
    if(std::all_of(picture_.begin(), picture_.end(), std::isblank))
        throw InvalidWaldo("input file full of blank");
}

// modify the string `from' to hide a Waldo in a non-space location
void Waldo::hide_waldo() {

    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<size_t> uniform_dist(0, picture_.size() - 1);

    // try as long as we can until we find a non-space location
    size_t pos;
    do {
        pos = uniform_dist(rnd);
    } while(std::isblank(picture_[pos]));
    picture_[pos] = waldo;
}

// the fun starts here
int main() {
    Waldo waldo{"waldo.txt"};
    std::cout << waldo.str();
    return 0;
}
