// find the number closest to the average in a sequence
#include <iostream>
#include <vector>
#include <algorithm>


double average(std::vector<double> const& data) {
  return std::accumulate(data.begin(), data.end(), 0.) / data.size();
}

int main(int argc, char const* argv[]) {
  std::vector<double> data;
  auto conv = [](char const *arg) { return std::stof(arg); };
  std::transform(argv + 1, argv + argc, std::back_inserter(data), conv);

  if(data.empty()) {
    std::cerr << "no input!" << std::endl;
    return 1;
  }
  else {
    double av = average(data);
    auto cmp = [=](double self, double other) {
      return std::abs(self - av) < std::abs(other - av);
    };
    auto where = std::min_element(data.begin(), data.end(), cmp);
    std::cout << *where << " at index " << std::distance(data.begin(), where) << std::endl;
    return 0;
  }
}
