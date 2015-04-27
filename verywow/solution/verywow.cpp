// improved wow quoter

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

std::vector<std::string> split(std::string const& line) {
  static const char delimiters[] = " \t,.!?;\n";

  std::vector<std::string> splitted;
  size_t curr_pos = 0;
  do {
    size_t end_pos = line.find_first_of(delimiters, curr_pos);
    size_t len;
    size_t next_pos;
    if(end_pos == std::string::npos) {
      len = std::string::npos;
      next_pos = std::string::npos;
    }
    else {
      len = (end_pos - curr_pos);
      next_pos = end_pos + 1;
    }

    splitted.emplace_back(line.substr(curr_pos, len));
    curr_pos = next_pos;
  } while(curr_pos != std::string::npos);
  return splitted;
}

int main() {
  std::string line;
  std::getline(std::cin, line);
  auto splitted = split(line);
  assert(splitted.size() != 0 && "at least a single element");

  auto cmp = [](std::string const& lhs, std::string const& rhs) {
    return lhs.size() < rhs.size();
  };
  auto largest_word = *std::max_element(splitted.begin(), splitted.end(), cmp);

  std::cout << "Wow, so " << largest_word << std::endl;

  return 0;
}
