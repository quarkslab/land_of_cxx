#include <array>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <limits>
#include <iterator>
#include <algorithm>

/* Base class for all tiles
 *
 * Implements the default behavior of a tile, i.e. an empty tile
 */
struct Tile {
  virtual char str() const = 0;
  virtual bool blocks() const { return false; }
  virtual bool wins() const { return false;}
};

/* Regular empty tile
 */
struct Empty : Tile {
  static constexpr char value = ' ';
  virtual char str() const override { return value; }
};

/* A blocking tile
 */
struct Wall : Tile {
  static constexpr char value = '#';
  virtual char str() const override { return value; }
  virtual bool blocks() const override { return true; }
};

/* Borders are blocking tiles with a different rendering
 */
struct Border : Wall {
  static constexpr char value = '@';
  virtual char str() const override { return value; }
};

/* Exit tile are non blocking (you must stop on them)
 * they make you win!
 */
struct Exit : Empty {
  static constexpr char value = 'O';
  virtual char str() const override { return value; }
  virtual bool wins() const override { return true;}
};

/* An entrance tile is very special: there can be only one, a property enforced
 * by its singleton design
 */
struct Entrance : Empty {

  static constexpr char value = 'I';
  virtual char str() const override { return value; }
  static Entrance &get() { return singleton_; }

  private:
    static Entrance singleton_;
    Entrance() = default;
};
Entrance Entrance::singleton_;

/* lightweight factory of tile: all tiles have no state anyway
 */
Tile *make_tile(char code) {
  static Empty empty;
  static Wall wall;
  static Exit exit;
  static Border border;
  switch (code) {
  case Empty::value:
    return &empty;
  case Wall::value:
    return &wall;
  case Entrance::value:
    return &Entrance::get();
  case Exit::value:
    return &exit;
  case Border::value:
    return &border;
  default:
    throw std::domain_error("invalid tile specification");
  }
}

struct Board {
  static constexpr size_t height = 10;
  static constexpr size_t width = 10;

  Tile** entrance_ = nullptr;

  Board(std::string const& rc) {
    std::ifstream ifs(rc.c_str());
    if(not ifs)
      throw std::runtime_error("invalid resource: " + rc);
    else {
      // FIXME: add more input checks
      auto board_iter = begin();
      board_iter = std::fill_n(board_iter, width + 2, make_tile(Border::value));
      for(size_t i = 0; i < height; ++i) {
        *board_iter++ = make_tile(Border::value);
        for(size_t j = 0; j < width; ++j) {
          char point;
          ifs.read(&point, 1);
          *board_iter++ = make_tile(point);
        }
        *board_iter++ = make_tile(Border::value);
        char eol;
        ifs.read(&eol, 1);
        if(eol != '\n') // FIXME: not portable
          throw std::runtime_error("invalid grid size: greater than expected");
      }
      board_iter = std::fill_n(board_iter, width + 2, make_tile(Border::value));
      assert(board_iter == end());
      auto entrance_iter = std::find(begin(), end(), &Entrance::get());
      if(entrance_iter == end())
          throw std::runtime_error("invalid grid no entrance");
      else
        entrance_ = entrance_iter;
    }
  }

  bool run(std::string const& commands) {
    auto curr_pos = entrance_;

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
    return (*curr_pos)->wins();
  }

  std::string str() const {
    std::string out;
    auto out_iter = std::back_inserter(out);
    for ( Tile* const (&line)[width +2] : board_) {
      *std::transform(std::begin(line), std::end(line), out_iter,
                      [](Tile *const tile) { return tile->str(); })++ = '\n';
    }
    return out;
  }


  private:

  using iterator = Tile * *;
  using const_iterator = Tile * const*;

  iterator begin() { return &board_[0][0]; }
  iterator end() { return begin() + sizeof(board_)/sizeof(Tile*); }
  const_iterator begin() const { return &board_[0][0]; }
  const_iterator end() const { return begin() + sizeof(board_)/sizeof(Tile*); }

  void slideLeft(iterator& curr_pos) const;
  void slideRight(iterator& curr_pos) const;
  void slideUp(iterator& curr_pos) const;
  void slideDown(iterator& curr_pos) const;

  Tile* board_[height + 2 ][width + 2];


};

void Board::slideLeft(iterator& curr_pos) const {
  while(not (*curr_pos)->blocks())
    curr_pos -= 1;
  curr_pos += 1;
}
void Board::slideRight(iterator& curr_pos) const {
  while(not (*curr_pos)->blocks())
    curr_pos += 1;
  curr_pos -= 1;
}

void Board::slideDown(iterator& curr_pos) const {
  while(not (*curr_pos)->blocks())
    curr_pos += width + 2;
  curr_pos -= width + 2;
}

void Board::slideUp(iterator& curr_pos) const {
  while(not (*curr_pos)->blocks())
    curr_pos -= width + 2;
  curr_pos += width + 2;
}

int main(int argc, char* argv[]) {
  std::string map = argc == 1 ? "default.txt" : argv[1];
  Board s{map};
  std::cout << s.str() << std::endl;
  std::fill_n(std::ostream_iterator<char>(std::cout), Board::width, '=');
  std::cout << std::endl << "How do I go from " << Entrance::value << " to " << Exit::value << "? [hjkl]" << std::endl;

  std::string commands;
  std::cin >> commands;
  if(s.run(commands)) {
    std::cout << "gotcha!" << std::endl;
    return 0;
  }
  else {
    std::cerr << "go buy some glasses, you looser!" << std::endl;
    return 0;
  }
}
