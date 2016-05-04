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

std::default_random_engine RandomEngine{std::random_device()()};

struct Dice {
  virtual void reroll() = 0;
  virtual unsigned value() const = 0;
  virtual std::string name() const = 0;
};

//FIXME: there's a lot of redundancy between D6, D20 and coin
class D6 : public Dice {
  unsigned state_;

  public:
  D6() { reroll(); }
  unsigned value() const override { return state_; }
  void reroll() override {
    state_ = std::uniform_int_distribution<int>(1,6)(RandomEngine);
  }
  std::string name() const override { return "d6"; }
};

class D20 : public Dice {
  unsigned state_;

  public:
  D20() { reroll(); }
  unsigned value() const override { return state_; }
  void reroll() override {
    state_ = std::uniform_int_distribution<int>(1,20)(RandomEngine);
  }
  std::string name() const override { return "d20"; }
};

class Coin : public Dice {
  //FIXME: choose the underlying type (here, a uint8_t) based on the maximum number of states
  unsigned state_;

  public:
  Coin() { reroll(); }
  unsigned value() const override { return state_; }
  void reroll() override {
    state_ = std::uniform_int_distribution<int>(1,2)(RandomEngine);
  }
  std::string name() const override { return "coin"; }
};

//FIXME: make this a LittlePocky (dices are magic)
// by using a tuple to store a **predefined** set of dices
class MyBigPocky {
  std::vector<std::unique_ptr<Dice>> pocket_;

  public:
  template<class... Dices>
  MyBigPocky(Dices... dices) {
    std::initializer_list<bool> __attribute__((unused)) _ {(pocket_.emplace_back(new Dices(dices)), true)...};
  }

  auto begin() -> decltype(pocket_.begin()) { return pocket_.begin(); }
  auto begin() const  -> decltype(pocket_.begin()) { return pocket_.begin(); }
  auto end()  -> decltype(pocket_.end()) { return pocket_.end(); }
  auto end() const -> decltype(pocket_.end()) { return pocket_.end(); }
};

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
  template<class T, class Op>
  void apply(T& , Op&& , std::integral_constant<size_t, 0>) {
  }
  template<class T, class Op, size_t N>
  void apply(T& values, Op&& op, std::integral_constant<size_t, N>) {
    op(std::get<N-1>(values));
    apply(values, std::forward<Op>(op), std::integral_constant<size_t, N - 1>{});
  }
}

template<class Op, class... Types>
void apply(std::tuple<Types...>& values, Op&& op) {
  details::apply(values, std::forward<Op>(op), std::integral_constant<size_t, sizeof...(Types)>{});
}

//FIXME: check concepts for Pocket, and handle MyLittlePocky case
template<class Pocket>
void reroll(Pocket& P) {
  for(auto & dice : P)
    dice->reroll();
}

struct Dumper {
  template<class D>
    void operator()(D &&v) {
      std::cout << '\t' << v.name() << ": " << v.value() << std::endl;
    }
};

int main() {
  MyBigPocky MBP{D6(), D20(), D6(), Coin()};
  std::cout << "Rock'n'roll!\n";
  for(auto const &dice_ptr : MBP)
    std::cout << '\t' << dice_ptr->name() << ": " << dice_ptr->value() << std::endl;

  std::cout << "rerolling!\n";
  reroll(MBP);

  for(auto const &dice_ptr : MBP)
    std::cout << '\t' << dice_ptr->name() << ": " << dice_ptr->value() << std::endl;

  MyLittlePocky<D6, D20, D6, Coin> MLP;
  std::cout << "Dices are magic!\n";
  apply(MLP.pocket(), Dumper{});

  std::cout << "weight: " << sizeof(MLP) << "\n";
  return 0;
}
