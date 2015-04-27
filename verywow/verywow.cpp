/* .~= verywow =~.
 *
 * A program that reads a sentence from standard input and answers by very-wowing you,
 * that is take the longest word from your sentence and tell it back to you
 * prefixed by the words: Wow, so
 *
 * example:
 * >>> printf 'You are very smart! | ./verywow
 * Wow, so smart
 *
 * To complete this exercise, you can use:
 *
 * - std::cin
 * - std::getline
 * - std::string::find_first_of
 * - std::string::substr
 * - std::max_element
 */

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

/* Split a string into chunks and fill a vector with them.
 * The chunks are delimited according to the characters in
 * the `delimiters' array.
 *
 * usage:
 *
 * >>> std::string input = "By Jove";
 * >>> auto output = split(input);
 * >>> std::cout << output[0] << '-' << output[1] << std::endl;
 * By-Jove
 */

std::vector<std::string> split(std::string const& line) {
  static const char delimiters[] = " \t,.!?;\n";
  std::vector<std::string> words;
  // FIXME: split `line' according to `delimiters' instead
  words.push_back(line);
  return words;
}

int main() {
  std::string line;
  // TODO: read line from std::cin
  auto words = split(line);

  // TODO: verify ``words' is not empty

  // FIXME: find the largest string instead
  std::string const& largest_word = words.back();

  std::cout << "Wow, so " << largest_word << std::endl;

  return 0;
}
