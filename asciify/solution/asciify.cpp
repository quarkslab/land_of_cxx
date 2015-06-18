/* .~= asciify =~.
 *
 * A program that reads a PGM file and turns into a vaguely resembling ASCII
 * art image.
 *
 * The PGM format is rather simple: it starts with an ASCII header in the form
 * of the P5 magic number followed by three integers describing the image: its
 * height, its width and its color depth. The image is then stored in array of
 * height x width bytes in binary form.
 *
 *  P5
 *  HEIGHT WIDTH
 *  COLORDEPTH
 *  BYTES...
 *
 * It's a greyscale representation, the idea is to use a space to encode any
 * byte lower than 128, and a sharp to encode any byte greater or equal to 128,
 * and print this out.
 */

#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

std::vector<std::vector<unsigned char>> read_pgm(std::string const& filename) {
  std::ifstream ifs(filename);
  if(ifs) {
    char magic[2];
    ifs >> magic;
    if(magic[0] == 'P' and magic[1] == '5') {
      uint32_t height, width, skip;
      ifs >> height >> width >> skip;
      std::vector<std::vector<unsigned char>> buffer(height, std::vector<unsigned char>(width));
      for(auto& line: buffer)
        ifs.read(reinterpret_cast<char*>(line.data()), line.size());
      return buffer;
    }
    else {
      throw std::runtime_error("not a PGM file: " + filename);
    }
  }
  else {
    throw std::runtime_error("failed to open file: " + filename);
  }
}

void ascii_dump(std::vector<std::vector<unsigned char>>const& data) {
  for(auto const& line: data) {
    for(auto value: line) {
      if(value < 64)
        std::cout << ' ';
      else if (value < 128)
        std::cout << '.';
      else if (value < 192)
        std::cout << '-';
      else
        std::cout << '#';
    }
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[]) {
  std::string filename = argc > 1 ? argv[1] : "default.pgm";
  auto img = read_pgm(filename);
  ascii_dump(img);
  return 0;
}
