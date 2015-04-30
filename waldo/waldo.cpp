/* .~= Waldo =~.
 *
 * Where is Waldo is a famous book series where you try to find a small guy
 * wearing striped clothes named Waldo in a large image full of details. The
 * goal of this game will be to print an (ASCII, of course) image loaded from a
 * file, with a slight change: one of the character of the input image will be
 * changed to a `W', like Waldo. Then the player can use her eagle-eye skill to
 * find it!
 *
 * For instance the following ASCII image
 *
 *  o(^_-)O <3 \(^_^)/
 *
 * can be turned into
 *
 *  o(^_-)O <W \(^_^)/
 *
 * The updated character can be any non black character!
 *
 * To solve this exercise, you can use:
 *
 * - std::runtime_error
 * - std::find
 * - std::all_of
 * - std::random_device
 * - std::default_random_engine
 * - std::uniform_int_distribution
 * - std::is_blank
 */

#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <algorithm>

// An exception specialized for Waldo Errors
// TODO: make it inherit from a standard exception
struct InvalidWaldo {
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
};

const char Waldo::waldo;

// create a Waldo instance from a file description
// the description is assumed to be an asciiart image
// with at least one line and always the same number of columns
Waldo::Waldo(std::string const & path)
{
    std::ifstream ifs(path.c_str());
    // TODO: convert the file to a string
    picture_ = "I am waldo";
    check_validity();
    hide_waldo();
}

// check a few stuff on the picture and raise if needed
void Waldo::check_validity() const {
    // TODO: move each check to a member function
    if(std::count(picture_.begin(), picture_.end(), '\n') == 0)
        // TODO: add extra information to the exception
        throw InvalidWaldo();
    // TODO: check whether a waldo character already exists in the picture
    // TODO: check the picture is not blank
}

// modify the string `from' to hide a Waldo in a non-space location
void Waldo::hide_waldo() {

    // FIXME: choose a random position

    // try as long as we can until we find a non-space location
    size_t pos = -1;;
    do {
        pos += 1;
    // FIXME: check for any kind of blank character
    } while(picture_[pos] == ' ');
    picture_[pos] = waldo;
}

// the fun starts here
int main() {
    Waldo waldo{"waldo.txt"};
    std::cout << waldo.str();
    return 0;
}
