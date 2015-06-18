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
 *
 * To complete this exercise, you can use:
 *
 * - std::fstream::good()
 * - std::istream& operator>>(std::istream&, uint32_t)
 * - std::istream::read(char*, size_t)
 * - std::vector<T>::data()
 * - std::vector<T>::size()
 *
 */

#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

using greyscale_image = std::vector<std::vector<unsigned char>>;

greyscale_image read_pgm(std::string const& filename) {
  std::ifstream ifs(filename);
  // FIXME: check if the file opens correctly
  if(true) {
    char magic[2];
    ifs >> magic;
    if(magic[0] == 'P' and magic[1] == '5') {
      uint32_t height = 0, width = 0;
      // TODO: reads height and width from the header
      greyscale_image buffer(height, std::vector<unsigned char>(width));
      // TODO: read the bytes too
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

/* dump the 2D vector `data' to the standard output in ASCII form, using either
 * a space or a sharp depending on the grayscale
 */
void ascii_dump(greyscale_image const& data) {
  //FIXME: iterate over `data' twice and print the relevant characters
  for(auto const& line: data) {
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[]) {
  std::string filename = argc > 1 ? argv[1] : "default.pgm";
  auto img = read_pgm(filename);
  ascii_dump(img);
  return 0;
}
