#include <iostream>
#include <algorithm>
#include <random>
#include <array>

namespace {
    std::random_device rd;
    std::mt19937 coin(rd());
    std::uniform_int_distribution<> flip(0, 1);
}

char const* random_pick(char const * const *begin, char const * const *end) {
    return begin[std::uniform_int_distribution<>{0, int(end - begin - 1)}(coin)];
}
// A warrior has a name and some HP and is capable of attacking another warrior
class Warrior {

    std::string const _name;
    long _hp;

    public:

        static long constexpr START_HP = 20;

        Warrior(std::string const& name, long hp=START_HP):
            _name(name),
            _hp(hp)
        {
        }

        virtual ~Warrior() {}

        Warrior(Warrior const&) = delete;

        // this warrior attacks another warrior and deals some damage
        virtual void attack(Warrior& other) const {
            other._hp = std::max(0L, other._hp - 1);
        }

        std::string const& name() const {
            return _name;
        }

        // test if this warrior is still alive
        explicit operator bool() const {
            return _hp;
        }
};

class Knight : public Warrior {
    public:

    static constexpr char const* names[] = {"Anarion", "Vorondil", "Mardil"};

    Knight(std::string const& name) : Warrior(name, START_HP + 10)
    {
    }
    Knight() : Knight(random_pick(names, names +sizeof(names)/(sizeof*names)))
    {
    }
    virtual ~Knight() override {}

};
constexpr char const* Knight::names[];

class Elf : public Warrior {
    public:

    static constexpr char const* names[] = {"Aegnor", "Beleg", "Curufin"};

    Elf(std::string const& name) : Warrior(name, START_HP - 5)
    {
    }
    Elf() : Elf(random_pick(names, names + sizeof(names)/sizeof(*names)))
    {
    }
    virtual ~Elf() override {}

    void attack(Warrior& other) const override  {
        Warrior::attack(other);
        if(flip(coin))
            attack(other);
    }
};
constexpr char const* Elf::names[];

class Orc : public Warrior {
    public:

    static constexpr char const* names[] = {"Azog", "Bolg", "Grishnakh"};

    Orc(std::string const& name) : Warrior(name)
    {
    }
    Orc() : Orc(random_pick(names, names + sizeof(names)/sizeof(*names)))
    {
    }

    virtual ~Orc() override {}

    void attack(Warrior& other) const override {
        Warrior::attack(other);
        if(flip(coin))
            Warrior::attack(other);
    }
};
constexpr char const* Orc::names[];

// make the two Warriors ``self'' and ``other'' fight until one of the die
void fight(Warrior& self, Warrior& other) {
    while(self and other) {
        Warrior *first{&self}, *second{&other};
        if(flip(coin)) {
            using std::swap; // allows argument Dependent Lookup
            swap(first, second);
        }
        first->attack(*second);
        if(*second)
            second->attack(*first);
    }
}

Warrior* pick_random_race(std::string const& name) {
    std::array<Warrior *, 3> challengers{{ new Knight(name),
                                          new Elf(name),
                                          new Orc(name)
                                       }};
    std::random_shuffle(challengers.begin(), challengers.end());
    std::for_each(challengers.begin() + 1, challengers.end(),
                  [](Warrior * warrior) { delete warrior; });
    return challengers[0];
}

static const char banner [] = R"(
  _______________________-------------------                       `\
 /:--__                                                              |
||< > |                                   ___________________________/
| \__/_________________-------------------                         |
|                                                                  |
 |                               ORC                                |
 |                                                                  |
  |                                              ____________________|_
  |  ___________________-------------------------                      `\
  |/`--_                                                                 |
  ||[ ]||                                            ___________________/
   \===/___________________--------------------------
)";

int main(int argc, char * argv[]) {
    std::cout << banner << std::endl;

    Warrior*me = pick_random_race("me"),
           *other = new Orc();

    fight(*me, *other);

    Warrior* winner = *me ? me : other;
    std::cout << R"(The winner is \o/ )" << winner->name() << R"( \o/)" << std::endl;

    delete me;
    delete other;

    return 0;
}
