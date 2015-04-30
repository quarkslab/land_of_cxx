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
 * Actually, the program already works, but it smells.
 * Smells like in ``I have a lot of if and switches in my code and I should be polymorphic''.
 *
 * So to complete this exercise, you should perform an in-depth transformation of the code so that
 * it no longer resembles C code... Feel free to use:
 *
 * - an array of Tiles instead of an array of bool to represent the screen
 * - a singleton pattern to guarantee the uniqueness of the entrance
 * - a lightweight factory to generate tiles
 */
#include <array>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <limits>
#include <iterator>

struct Board {
  static constexpr size_t height = 10;
  static constexpr size_t width = 10;

  // TODO: this smells, looks like we have our children class candidates there
  static constexpr char OUT = 'O';
  static constexpr char IN = 'I';
  static constexpr char WALL = '#';

  static constexpr std::pair<size_t, size_t> invalid{-1, -1};

  Board(std::string const& rc) : in_(invalid), out_(invalid) {
    std::ifstream ifs(rc.c_str());
    if(not ifs)
      throw std::runtime_error("invalid resource: " + rc);
    else {
      for(size_t i = 0; i < height; ++i) {
        for(size_t j = 0; j < width; ++j) {
          char point;
          ifs.read(&point, 1);
          // TODO: this smells, add a lightweight factory
          switch(point) {
            case ' ':
              board_[i][j] = false;
              break;
            case WALL:
              board_[i][j] = true;
              break;
            case IN:
              in_ = std::make_pair(i,j);
              board_[i][j] = false;
              break;
            case OUT:
              out_ = std::make_pair(i,j);
              board_[i][j] = false;
              break;
            default: {
              std::ostringstream loc;
              loc << i << ", " << j;
              throw std::runtime_error("invalid marker at " + loc.str());
            }
          }
        }
        char eol;
        ifs.read(&eol, 1);
        if(eol != '\n')
          std::runtime_error("invalid grid size: greater than expected");
      }
      if(in_ == invalid or out_ == invalid)
        std::runtime_error("invalid grid: out or in missing");

    }
  }

  bool check(std::string const& commands) {
    // TODO: this smells, replace curr_pos by an iterator over the board_
    std::pair<size_t, size_t> curr_pos = in_;
    for(char command : commands) {
      switch(command) {
        case 'h':
          slideLeft(curr_pos);
          break;
        case 'j':
          slideDown(curr_pos);
          break;
        case 'k':
          slideUp(curr_pos);
          break;
        case 'l':
          slideRight(curr_pos);
          break;
        default:
          throw std::runtime_error("invalid command");
      }
    }
    return curr_pos == out_;
  }

  std::string str() const {
    constexpr size_t out_width = (width + 2 ), // +2 for surrounding walls
                     out_height = (height + 2);
  // TODO: this smells, looks like we have an additional child class candidate here
    constexpr char BORDER = '@';
    std::string out;
    out.reserve((out_width + 1 ) * out_height); // +1 for line markers
    size_t i, j;
    // TODO: this smells, use a common str() member function to delegate the printing
    auto push_wall_line = [](std::string& out) {
      std::fill_n(std::back_inserter(out), out_width, BORDER);
      out.push_back('\n');
    };
    push_wall_line(out);
    for(i = 0; i < height; ++i) {
      out.push_back(BORDER);
      for(j = 0; j < width; ++j) {
        auto curr = std::make_pair(i,j);
        if(is_wall(curr))
          out.push_back(WALL);
        else if(curr == out_) {
          out.push_back(OUT);
        }
        else if(curr == in_) {
          out.push_back(IN);
        }
        else
          out.push_back(' ');
      }
      out.push_back(BORDER);
      out.push_back('\n');
    }
    push_wall_line(out);
    return out;
  }


  private:

  // TODO: this smells, use a common blocks() member function to delegate the ``is_wall'' property query
  bool is_wall(std::pair<size_t, size_t> const& pos) const { return board_[pos.first][pos.second]; }

  void slideLeft(std::pair<size_t, size_t>& curr_pos) const;
  void slideRight(std::pair<size_t, size_t>& curr_pos) const;
  void slideUp(std::pair<size_t, size_t>& curr_pos) const;
  void slideDown(std::pair<size_t, size_t>& curr_pos) const;

  // TODO: this smells, use an array of tiles instead
  bool board_[height][width];
  std::pair<size_t, size_t> in_, out_;

};

constexpr std::pair<size_t, size_t> Board::invalid;

void Board::slideLeft(std::pair<size_t, size_t>& curr_pos) const {
// TODO: this smells, use the block() member function instead
  while(curr_pos.second != 0 and not is_wall(curr_pos))
    curr_pos.second -= 1;
  if(is_wall(curr_pos))
    curr_pos.second += 1;
}
void Board::slideRight(std::pair<size_t, size_t>& curr_pos) const {
// TODO: this smells, use the block() member function instead
  while(curr_pos.second != Board::width and not is_wall(curr_pos))
    curr_pos.second += 1;
  if(is_wall(curr_pos))
    curr_pos.second -= 1;
}

void Board::slideDown(std::pair<size_t, size_t>& curr_pos) const {
// TODO: this smells, use the block() member function instead
  while(curr_pos.first != Board::height and not is_wall(curr_pos))
    curr_pos.first += 1;
  if(is_wall(curr_pos))
    curr_pos.first -= 1;
}

void Board::slideUp(std::pair<size_t, size_t>& curr_pos) const {
// TODO: this smells, use the block() member function instead
  while(curr_pos.first != 0 and not is_wall(curr_pos))
    curr_pos.first -= 1;
  if(is_wall(curr_pos))
    curr_pos.first += 1;
}

int main(int argc, char* argv[]) {
  std::string map = argc == 1 ? "default.txt" : argv[1];
  Board s{map};
  std::cout << s.str() << std::endl;
  std::fill_n(std::ostream_iterator<char>(std::cout), Board::width, '=');
  std::cout << std::endl << "How do I go from " << Board::IN << " to " << Board::OUT << "? [hjkl]" << std::endl;

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
