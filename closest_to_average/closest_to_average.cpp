/* .~= closest_to_average =~.
 *
 * This program is not very funny, and certainly not a game. It involves extreeeeemly complex mathematics: the user calls it with a number of floating point arguments and the program prints out the first value closest to the average of the sequence, and its index. Wow, so smart, very difficult!
 *
 * example:
 *
 * >>> ./closest_to_average 1 2 3 4 # average is 2.5
 * 2 at index 1
 *
 * To complete this exercise, you can use:
 *
 * - std::transform(Iterator, Iterator, Function)
 * - std::accumulate(Iterator, Iterator, Seed)
 * - std::vector<T>::empty()
 * - std::stod(char const*)
 * - std::abs(double)
 * - std::min_element(Iterator, Iterator, Comparator)
 * - std::distance(iterator, Iterator)
 */

#include <iostream>
#include <vector>


/* this function computes the average of the `data' vector and returns it
 */
double average(std::vector<double> const& data) {
  // FIXME: the average, not the first element!
  return data.front();
}

int main(int argc, char const* argv[]) {
  // TODO: fill `data' with the floating point values in `argv'
  std::vector<double> data(argc - 1);

  // FIXME: use a simpler test
  if(data.size() == 0) {
    std::cerr << "no input!" << std::endl;
    return 1;
  }
  else {
    double av = average(data);
    // FIXME: find the closest element to `av', not the first one
    auto where = data.begin();
    // FIXME: find the correct index
    std::cout << *where << " at index " << av << std::endl;
    return 0;
  }
}
