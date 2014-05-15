#include <iostream>
#include <algorithm>
#include <random>
#include <array>
#include <memory>
#include <unordered_map>
#include <cassert>
#include <thread>
#include <atomic>
#include <mutex>

std::mutex cout_mutex;

namespace {
    std::random_device rd;
    std::mt19937 coin(rd());
    std::uniform_int_distribution<> flip(0, 1);
}

enum Stat : char {
    HP = 'h',
    STR = 's',
    AGI = 'a'
};

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

// pick a random element between ``begin'' and ``end''
char const* random_pick(char const * const *begin, char const * const *end) {
    return begin[std::uniform_int_distribution<>{0, int(end - begin - 1)}(coin)];
}
// A warrior has a name and some HP and is capable of attacking another warrior
class Warrior {

    std::string const _name;
    long _max_hp;
    std::atomic<long> _hp;
    size_t _str;
    size_t _agi;

    public:

        static long constexpr START_HP = 20;
        static size_t constexpr START_AGI = 5;
        static size_t constexpr START_STR = 5;

        Warrior(std::string const& name, long hp=START_HP, size_t str = START_HP, size_t agi = START_AGI):
            _name(name),
            _max_hp(hp),
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
            // deals ceil(str/START_STR) damages, cannot go below 0
            other._hp -= long((_str + START_STR - 1) / START_STR);
        }

        // recover some HP, based on maximum hit points
        // one cannot grow beyond his original HP!
        virtual void recover() {
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

        // test if this warrior is still alive
        explicit operator bool() const {
            return _hp > 0;
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

    static constexpr char const* names[] = {"Anarion", "Vorondil", "Mardil"};

    Knight(std::string const& name) : Warrior(name, START_HP + 10)
    {
    }
    Knight() : Knight(random_pick(std::begin(names), std::end(names)))
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
    Elf() : Elf(random_pick(std::begin(names), std::end(names)))
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
    Orc() : Orc(random_pick(std::begin(names), std::end(names)))
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

template<class T> struct race_trait;
template<>
struct race_trait<Knight> {
    static constexpr char value[] = "knight";
};
constexpr char race_trait<Knight>::value[];
template<>
struct race_trait<Orc> {
    static constexpr char value[] = "orc";
};
constexpr char race_trait<Orc>::value[];
template<>
struct race_trait<Elf> {
    static constexpr char value[] = "elf";
};
constexpr char race_trait<Elf>::value[];

void start_fight(Warrior& self,  Warrior& other) {
    auto constexpr round_duration = 1;
    size_t const nb_strikes = (self.agi() + Warrior::START_AGI - 1) / Warrior::START_AGI;
    std::chrono::duration<double, std::ratio<1>> charging_duration{double(round_duration)/ nb_strikes};
    while(self and other) {
    for(size_t i = 0; i < nb_strikes; ++i)
        if(self and other) {
            std::this_thread::sleep_for(charging_duration);
            auto hp = other.hp();
            self.attack(other);
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << self.name() << " deals " << hp - other.hp() << " damages" << std::endl;
            }
        }
    }
}

// make the two Warriors ``self'' and ``other'' fight until one of the die
void fight(Warrior& self, Warrior& other)
{
    std::thread foe(start_fight, std::ref(other), std::ref(self));
    start_fight(std::ref(self), std::ref(other));
    foe.join();
}


template <typename... Races>
struct RaceSelector {};

template<class Race>
Warrior* race_selector(std::string const& name, char c) {
    assert(c == race_trait<Race>::value[0] and "c was a valid race");
    return new Race(name);
}

template<class Race, class ORace, class... Races>
Warrior* race_selector(std::string const& name, char c)
{
    if(c == race_trait<Race>::value[0])
        return new Race(name);
    else
        return race_selector<ORace, Races...>(name, c);
}

template < class... Races, template <class...> class RaceSelector>
Warrior* pick_race(RaceSelector<Races...>, std::string const& name, std::istream& is, std::ostream& os)
{
    static_assert(sizeof...(Races)>=1, "at least one race");

    static const std::unordered_map<char, char const*> race_names{{race_trait<Races>::value[0], race_trait<Races>::value}...};

    auto format = [](std::string const& s) { return s[0] + ('=' + s);};

    std::string buffer;
    do {
        auto iter = race_names.begin();
        os << "Pick a race [" << format(iter->second);
        std::accumulate(++iter, race_names.end(), std::ref(os),
                        [&](std::ostream& os, std::pair<char, char const*> const& s) -> std::ostream& { return os << ", " << format(s.second) ; });
        os << "]:";
        std::getline(is, buffer);
        trim(buffer);
    } while(buffer.size() != 1 and race_names.find(buffer[0]) == race_names.end());

    os << race_names.find(buffer[0])->second << " `" << name << "' joins the battle!" << std::endl;
    return race_selector<Races...>(name, buffer[0]);
}

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
    Warrior* other = pick_random_race(races);


    Warrior *me = pick_race(races, "me", std::cin, std::cout);
    // boost the stat eight times
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
            me->recover();
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
