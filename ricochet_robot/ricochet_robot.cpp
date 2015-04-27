/* .~= ricochet robot =~.
 *
 * Ricochet robot is a funny board game where you control robots and you plan
 * their moves on the board, given that once they start in a direction, they
 * only stop when they bump in a wall, which then allow you to choose another
 * direction. The goal is to go from one point to another while respecting this
 * displacement rule.
 *
 * Let's use the vi key bindings to describe a displacement:
 *
 *  h: left
 *  j: down
 *  k: up
 *  l: right
 *
 * But you already know that, right?
 *
 * then given this map:
 *
 *  @@@@@@@@@@@@
 *  @        O @
 *  @          @
 *  @          @
 *  @#         @
 *  @         #@
 *  @          @
 *  @          @
 *  @          @
 *  @          @
 *  @I         @
 *  @@@@@@@@@@@@
 *
 * If you want to go from `I'n to `O'ut, A possible way is to go up, right and up again, thus klk.
 * A longer but valid path would be lkhklk.
 *
 * The goal of this program is to read a map from a file, print it then prompt the user for a path.
 * If the path is valid, it's a win, otherwise, it's a fail.
 *
 * To complete this exercise, you may want to use:
 *
 * - member variables
 * - initialization lists
 * - references
 * - std::fill_n(Iterator, size_t, Value)
 * - std::clog
 * - lambda expressions
 */

#include <array>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <limits>
#include <iterator>

struct Screen {
  static constexpr size_t height = 10;
  static constexpr size_t width = 10;

  // TODO: declare constant values for the 'I' 'O' and '#' special symbols

  static constexpr std::pair<size_t, size_t> invalid{-1, -1};

  // FIXME: give the `invalid' value to in_ and out_ upon construction
  Screen(std::string const& rc) {
    std::ifstream ifs(rc.c_str());
    if(not ifs)
      throw std::runtime_error("invalid resource: " + rc);
    else {
      // FIXME: set the content of `screen_' from `rc        }
      // TODO: check user input! Is the grid layout correct?
    }
  }

  bool check(std::string const& commands) {
    std::pair<size_t, size_t> curr_pos = in_;
    // FIXME: process each command using the slide* methods
    // TODO: log on the standard logging stream the position after each step
    return curr_pos == out_;
  }

  std::string str() const {
    constexpr size_t out_width = (width + 2 ), // +2 for surrounding walls
                     out_height = (height + 2);

    constexpr char WALL = '@';
    std::string out;
    out.reserve((out_width + 1 ) * out_height); // +1 for line markers

    // FIXME: the walls are not printed out!

    size_t i, j;
    for(i = 0; i < height; ++i) {
      for(j = 0; j < width; ++j) {
        auto curr = std::make_pair(i,j);
        if(is_wall(curr))
          out.push_back('#');
        else if(curr == out_) {
          out.push_back('O');
        }
        else if(curr == in_) {
          out.push_back('I');
        }
        else
          out.push_back(' ');
      }
      out.push_back('\n');
    }
    return out;
  }


  private:

  // return true if there's a wall at the given position
  bool is_wall(std::pair<size_t, size_t> const& pos) const {
    // FIXME: walls everywhere!
    return true;
  }

  // FIXME: the signature is incorrect, curr_pos will never be updated!
  void slideLeft(std::pair<size_t, size_t> curr_pos) const;
  void slideRight(std::pair<size_t, size_t> curr_pos) const;
  void slideUp(std::pair<size_t, size_t> curr_pos) const;
  void slideDown(std::pair<size_t, size_t> curr_pos) const;

  // The screen is just a grid, with implicit walls surrounding the borders.
  // A point set to true means there's a wall
  bool screen_[height][width];

  // These state variables represent the position of the entrance and the exit
  std::pair<size_t, size_t> in_, out_;

};

constexpr std::pair<size_t, size_t> Screen::invalid;

void Screen::slideLeft(std::pair<size_t, size_t> curr_pos) const {
  while(curr_pos.second != 0 and not is_wall(curr_pos))
    curr_pos.second -= 1;
  if(is_wall(curr_pos))
    curr_pos.second += 1;
}
void Screen::slideRight(std::pair<size_t, size_t> curr_pos) const {
  while(curr_pos.second != Screen::width and not is_wall(curr_pos))
    curr_pos.second += 1;
  if(is_wall(curr_pos))
    curr_pos.second -= 1;
}

void Screen::slideDown(std::pair<size_t, size_t> curr_pos) const {
  while(curr_pos.first != Screen::height and not is_wall(curr_pos))
    curr_pos.first += 1;
  if(is_wall(curr_pos))
    curr_pos.first -= 1;
}

void Screen::slideUp(std::pair<size_t, size_t> curr_pos) const {
  while(curr_pos.first != 0 and not is_wall(curr_pos))
    curr_pos.first -= 1;
  if(is_wall(curr_pos))
    curr_pos.first += 1;
}

int main(int argc, char* argv[]) {
  std::string map = argc == 1 ? "default.txt" : argv[1];
  Screen s{map};
  std::cout << s.str() << std::endl;
  std::fill_n(std::ostream_iterator<char>(std::cout), Screen::width, '=');
  std::cout << std::endl << "How do I go from " << 'I' << " to " << 'O' << "? [hjkl]" << std::endl;

  std::string commands;
  std::cin >> commands;
  if(s.check(commands)) {
    std::cout << "gotcha!" << std::endl;
    return 0;
  }
  else {
    std::cerr << "go buy some glasses, you looser!" << std::endl;
    return 0;
  }
}
