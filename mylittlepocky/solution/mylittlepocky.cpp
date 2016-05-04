/* MyLittlePocky
 *   -- dices are magic
 *
 *
 *                (( _______
 *      _______     /\O    O\
 *     /O     /\   /  \      \
 *    /   O  /O \ / O  \O____O\ ))
 * ((/_____O/    \\    /O     /
 *   \O    O\    / \  /   O  /
 *    \O    O\ O/   \/_____O/
 *     \O____O\/ )) mrf      ))
 *   ((
 */

#include <random>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cstdint>

std::default_random_engine RandomEngine{std::random_device()()};

namespace details {
template <size_t N> struct DiceName {
  std::string operator()() const { return "d" + std::to_string(N); }
};
template <> struct DiceName<2> {
  std::string operator()() const { return "coin"; }
};
constexpr size_t log2(size_t n) {
  return n <= 1 ? 1 : (1 + log2(n >> 1));
}

constexpr size_t mlog(size_t n) {
  return (log2(n) + 7) / 8;
}

}

template<size_t N> struct small_container;

template<> struct small_container<1> {
  using type = std::uint8_t;
};

template<> struct small_container<2> {
  using type = std::uint16_t;
};

template<> struct small_container<4> {
  using type = std::uint32_t;
};

template<size_t N>
class D {
  public:
  typename small_container<details::mlog(N)>::type state_;

  public:
  D() { reroll(); }
  unsigned value() const { return state_; }
  void reroll() {
    state_ = std::uniform_int_distribution<int>(1,N)(RandomEngine);
  }
  std::string name() const {
    return details::DiceName<N>()();
  }
};

using D6 = D<6>;
using D20 = D<20>;
using Coin = D<2>;

template<class... Dices>
class MyLittlePocky {
  std::tuple<Dices...> pocket_;

  public:

  MyLittlePocky() = default;
  std::tuple<Dices...> & pocket() { return pocket_; }
  std::tuple<Dices...> const & pocket() const { return pocket_; }
  // no begin / end

};

namespace details {
  template<class Op, class T>
  void map(Op&& , T& , std::integral_constant<size_t, 0>) {
  }
  template<class Op, class T, size_t N>
  void map(Op&& op, T& values, std::integral_constant<size_t, N>) {
    op(std::get<N-1>(values));
    map(std::forward<Op>(op), values, std::integral_constant<size_t, N - 1>{});
  }
}

template<class Op, class... Types>
void map(Op&& op, std::tuple<Types...>& values) {
  details::map(std::forward<Op>(op), values, std::integral_constant<size_t, sizeof...(Types)>{});
}

struct Dumper {
  template<class D>
    void operator()(D const &v) const {
      std::cout << '\t' << v.name() << ": " << v.value() << std::endl;
    }
};

struct Reroller {
  template<class D>
    void operator()(D &v) const {
      v.reroll();
    }
};

int main() {

  MyLittlePocky<D6, D20, D6, Coin> MLP;
  std::cout << "Dices are magic!\n";
  map(Dumper{}, MLP.pocket());
  map(Reroller{}, MLP.pocket());
  map(Dumper{}, MLP.pocket());

  std::cout << "weight: " << sizeof(MLP) << "\n";
  return 0;
}
