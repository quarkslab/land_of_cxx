#include <iostream>
#include <algorithm>
#include <random>
#include <array>

namespace {
    std::random_device rd;
    std::mt19937 coin(rd());
    std::uniform_int_distribution<> flip(0, 1);
}

// pick a random element between ``begin'' and ``end''
char const* random_pick(char const * const *begin, char const * const *end) {
    return begin[std::uniform_int_distribution<>{0, int(end - begin - 1)}(coin)];
}
// A warrior has a name and some HP and is capable of attacking another warrior
class Warrior {

    std::string const _name;
    long _hp;
    size_t _str;
    size_t _agi;

    public:

        static long constexpr START_HP = 20;
        static size_t constexpr START_AGI = 5;
        static size_t constexpr START_STR = 5;

        Warrior(std::string const& name, long hp=START_HP, size_t str = START_HP, size_t agi = START_AGI):
            _name(name),
            _hp(hp),
            _str(str),
            _agi(agi)
        {
        }

        virtual ~Warrior() {}

        Warrior(Warrior const&) = delete;

        // this warrior attacks another warrior and deals some damage
        virtual void attack(Warrior& other) const {
            static_assert(START_STR>0, "not dividing by zero");
            other._hp = std::max(0L, other._hp - long((_str + START_STR - 1) / START_STR));
        }

        std::string const& name() const {
            return _name;
        }
        size_t hp() const { return _hp; }
        size_t str() const { return _str; }
        size_t agi() const { return _agi; }

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
void fight(Warrior& self, Warrior& other)
{
    while(self and other) {
        using std::swap; // allows argument Dependent Lookup
        Warrior *first = &self, *second = &other;
        if(other.agi() > self.agi())
            swap(first, second);
        else if(other.agi() == self.agi() and flip(coin))
            swap(first, second);
        auto strikes = 1 + (first->agi() - second->agi()) / Warrior::START_AGI ;
        std::cout << first->name() << " strikes " << strikes << " times" << std::endl;
        while(strikes--)
            first->attack(*second);
        if(*second)
            second->attack(*first);
        std::cout << "after this round, you have:" << self.hp() << " HP left and " << other.name() << " has:" << other.hp() << " HP left" << std::endl;
    }
}


template <typename... Races>
struct RaceSelector {};

// pick a random race and create a Warrior from it, named as ``name''
template < class... Races, template <class...> class RaceSelector,
           class... Args>
Warrior* pick_random_race(RaceSelector<Races...>, Args const&... args)
{
    std::array<Warrior *, sizeof...(Races)> challengers{{new Races(args...)...}};
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

    RaceSelector<Orc, Elf, Knight> races;
    Warrior*me = pick_random_race(races, "me"),
           *other = pick_random_race(races);

    fight(*me, *other);

    Warrior* winner = *me ? me : other;
    std::cout << R"(
                 /\
                /  |
  *            /  /________________________________________________
 (O)77777777777)  7                                                `~~--__
8OO>>>>>>>>>>>>] <===========================================>          __-
 (O)LLLLLLLLLLL)  L________________________________________________.--~~
  *            \  \
                \  |
                 \/
)";
    std::cout << R"(The winner is \o/ )" << winner->name() << R"( \o/)" << std::endl;

    delete me;
    delete other;

    return 0;
}
