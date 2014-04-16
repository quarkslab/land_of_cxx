#include <iostream>
#include <algorithm>
#include <random>
#include <array>

namespace {
    std::random_device rd;
    std::mt19937 coin(rd());
    std::uniform_int_distribution<> flip(0, 1);
}

long constexpr START_HP = 20;
size_t constexpr START_AGI = 5;
size_t constexpr START_STR = 5;

const std::array<std::string, 3> elvish_names{{"Aegnor", "Beleg", "Curufin"}};
const std::array<std::string, 3> orcish_names{{"Azog", "Bolg", "Grishnakh"}};
const std::array<std::string, 3> human_names {{"Anarion", "Vorondil", "Mardil"}};

class Warrior {
    std::string const _name;
    long _hp;
    size_t _str;
    size_t _agi;

    public:

        Warrior(std::string const& name, long hp=START_HP, size_t str = START_HP, size_t agi = START_AGI):
            _name(name),
            _hp(hp),
            _str(str),
            _agi(agi)
        {
        }

        virtual ~Warrior() {}

        Warrior(Warrior const&) = delete;

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

        explicit operator bool() const {
            return _hp;
        }
};

class Knight : public Warrior {
    public:

    Knight(std::string const& name) : Warrior(name, START_HP + 10)
    {
    }
    Knight() : Knight(human_names[std::uniform_int_distribution<>{0, human_names.size() - 1}(coin)])
    {
    }
    virtual ~Knight() {}
};

class Elf : public Warrior {
    public:

    Elf(std::string const& name) : Warrior(name, START_HP - 5)
    {
    }
    Elf() : Elf(elvish_names[std::uniform_int_distribution<>{0, elvish_names.size() - 1}(coin)])
    {
    }
    virtual ~Elf() {}

    void attack(Warrior& other) const {
        Warrior::attack(other);
        if(flip(coin))
            attack(other);
    }
};

class Orc : public Warrior {
    public:
    Orc(std::string const& name) : Warrior(name)
    {
    }
    Orc() : Orc(orcish_names[std::uniform_int_distribution<>{0, orcish_names.size() - 1}(coin)])
    {
    }

    virtual ~Orc() {}

    void attack(Warrior& other) const {
        Warrior::attack(other);
        if(flip(coin))
            Warrior::attack(other);
    }
};

void fight(Warrior& self, Warrior& other)
{
    while(self and other) {
        Warrior *first = &self, *second = &other;
        if(other.agi() > self.agi())
            std::swap(first, second);
        else if(other.agi() == self.agi() and flip(coin))
            std::swap(first, second);
        auto strikes = 1 + (first->agi() - second->agi()) / START_AGI ;
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
