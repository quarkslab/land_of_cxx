#include <iostream>
#include <algorithm>
#include <random>
#include <array>
#include <memory>

namespace {
    std::random_device rd;
    std::mt19937 coin(rd());
    std::uniform_int_distribution<> flip(0, 1);
}

long constexpr START_HP = 20;
size_t constexpr START_AGI = 5;
size_t constexpr START_STR = 5;

enum Stat : char {
    HP = 'h',
    STR = 's',
    AGI = 'a'
};

const std::array<std::string, 3> elvish_names{{"Aegnor", "Beleg", "Curufin"}};
const std::array<std::string, 3> orcish_names{{"Azog", "Bolg", "Grishnakh"}};
const std::array<std::string, 3> human_names {{"Anarion", "Vorondil", "Mardil"}};

std::string& trim(std::string & s) {
    auto isnotspace =  [](std::string::const_reference c) -> bool { return not std::isspace(c); };
    std::string::iterator first_non_space = std::find_if(s.begin(), s.end(), isnotspace);
    std::string::reverse_iterator last_non_space = std::find_if(s.rbegin(), s.rend(), isnotspace);
    s.erase(last_non_space.base(), s.end());
    s.erase(s.begin(), first_non_space);
    return s;
}

template<size_t N>
class StatChooser : public std::array<char, N> {
    public:
    StatChooser(std::istream& is, std::ostream& os) {
        std::string buffer;
        do {
            os << "Select <" << N << "> stats buf [sha]:";
            buffer.empty();
            std::getline(is, buffer);
            trim(buffer);
        }
        while(buffer.length() != N and buffer.find_first_not_of("sha") == std::string::npos);
        std::copy(buffer.begin(), buffer.end(), this->begin());
    }
};

class Warrior {
    std::string const _name;
    long _max_hp;
    long _hp;
    size_t _str;
    size_t _agi;

    public:

        Warrior(std::string const& name, long hp=START_HP, size_t str = START_STR, size_t agi = START_AGI):
            _name(name),
            _max_hp(hp),
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
        virtual void regen() {
            auto recovery_rate = _max_hp / 10;
            _hp = std::min(_max_hp, _hp + recovery_rate * 2);
        }

        std::string const& name() const {
            return _name;
        }
        size_t max_hp() const { return _max_hp; }
        size_t hp() const { return _hp; }
        size_t str() const { return _str; }
        size_t agi() const { return _agi; }

        explicit operator bool() const {
            return _hp;
        }

        template<size_t N>
        void buf(StatChooser<N> && sc) {
            for(auto const& c: sc)
            {
                switch(c) {
                    case Stat::HP: _hp+=2; _max_hp += 2 ; break;
                    case Stat::STR: _str+=1; break;
                    case Stat::AGI: _agi+=1; break;
                }
            }
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
    virtual ~Knight() override {}
};

class Elf : public Warrior {
    public:

    Elf(std::string const& name) : Warrior(name, START_HP - 5)
    {
    }
    Elf() : Elf(elvish_names[std::uniform_int_distribution<>{0, elvish_names.size() - 1}(coin)])
    {
    }
    virtual ~Elf() override {}

    void attack(Warrior& other) const override  {
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

    virtual ~Orc() override {}

    void attack(Warrior& other) const override {
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
    Warrior* other = pick_random_race(races);


    Warrior *me = pick_random_race(races, "me");
    me->buf(StatChooser<8>(std::cin, std::cout));

    for(size_t round = 1; *me; ++round)
    {
        std::vector<std::shared_ptr<Warrior>> foes{(round  + 9) / 10, nullptr};
        std::generate(foes.begin(), foes.end(),
                      [&races]() { return std::shared_ptr<Warrior>(pick_random_race(races)); });
        std::cout << "***** round " << round << " *****" << std::endl;
        for(auto foe: foes) {
            std::cout << ">>> " << me->name() << " VS " << foe->name() << "<<<" << std::endl;
            fight(*me, *foe);
            Warrior* winner = *me ? me : other;
            std::cout << "The winner is ``" << winner->name() << "''" << std::endl;
        }
        if(*me) {
            std::cout << "You survived one more round! It's time to harvest the fruits of your efforts" << std::endl
                      << "status: HP=" << me->hp() << '/' << me->max_hp() << " STR=" << me->str() << " AGI=" << me->agi() <<std::endl;
            me->buf(StatChooser<1>{std::cin, std::cout});
            me->regen();
        }
    }

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
    std::cout << R"(,'`~.~^~._ you were killed by )" << winner->name() << R"( _.~^~`',)" << std::endl;

    delete me;

    return 0;
}
