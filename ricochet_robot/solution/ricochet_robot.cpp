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

  static constexpr char OUT = 'O';
  static constexpr char IN = 'I';
  static constexpr char WALL = '#';

  static constexpr std::pair<size_t, size_t> invalid{-1, -1};

  Screen(std::string const& rc) : in_(invalid), out_(invalid) {
    std::ifstream ifs(rc.c_str());
    if(not ifs)
      throw std::runtime_error("invalid resource: " + rc);
    else {
      for(size_t i = 0; i < height; ++i) {
        for(size_t j = 0; j < width; ++j) {
          char point;
          ifs.read(&point, 1);
          switch(point) {
            case ' ':
              screen_[i][j] = false;
              break;
            case WALL:
              screen_[i][j] = true;
              break;
            case IN:
              in_ = std::make_pair(i,j);
              screen_[i][j] = false;
              break;
            case OUT:
              out_ = std::make_pair(i,j);
              screen_[i][j] = false;
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
        if(eol != '\n') // FIXME: not portable
          std::runtime_error("invalid grid size: greater than expected");
      }
      if(in_ == invalid or out_ == invalid)
        std::runtime_error("invalid grid: out or in missing");

    }
    // FIXME: verify we can stop on out_
  }

  bool check(std::string const& commands) {
    std::pair<size_t, size_t> curr_pos = in_;
    auto log_position = [&]() {
      std::clog << "curr pos is: " << curr_pos.first << "," << curr_pos.second << std::endl;
    };

    log_position();
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
      log_position();
    }
    return curr_pos == out_;
  }

  std::string str() const {
    constexpr size_t out_width = (width + 2 ), // +2 for surrounding walls
                     out_height = (height + 2);
    constexpr char BORDER = '@';
    std::string out;
    out.reserve((out_width + 1 ) * out_height); // +1 for line markers
    size_t i, j;
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

  bool is_wall(std::pair<size_t, size_t> const& pos) const { return screen_[pos.first][pos.second]; }

  void slideLeft(std::pair<size_t, size_t>& curr_pos) const;
  void slideRight(std::pair<size_t, size_t>& curr_pos) const;
  void slideUp(std::pair<size_t, size_t>& curr_pos) const;
  void slideDown(std::pair<size_t, size_t>& curr_pos) const;

  bool screen_[height][width];
  std::pair<size_t, size_t> in_, out_;

};

constexpr std::pair<size_t, size_t> Screen::invalid;

void Screen::slideLeft(std::pair<size_t, size_t>& curr_pos) const {
  while(curr_pos.second != 0 and not is_wall(curr_pos))
    curr_pos.second -= 1;
  if(is_wall(curr_pos))
    curr_pos.second += 1;
}
void Screen::slideRight(std::pair<size_t, size_t>& curr_pos) const {
  while(curr_pos.second != Screen::width and not is_wall(curr_pos))
    curr_pos.second += 1;
  if(is_wall(curr_pos))
    curr_pos.second -= 1;
}

void Screen::slideDown(std::pair<size_t, size_t>& curr_pos) const {
  while(curr_pos.first != Screen::height and not is_wall(curr_pos))
    curr_pos.first += 1;
  if(is_wall(curr_pos))
    curr_pos.first -= 1;
}

void Screen::slideUp(std::pair<size_t, size_t>& curr_pos) const {
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
  std::cout << std::endl << "How do I go from " << Screen::IN << " to " << Screen::OUT << "? [hjkl]" << std::endl;

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
