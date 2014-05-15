===
ORC
===

In ORC, you are a great hero trying to slash as many foes as possible before
meeting a glorious end. Let's use as many C++11 features to write this
old-school, text-based game!

This directory has been populated with a minimalistic C++ source and the
associated makefile. Feel free to compile::

    $> make

And run::

    $> ./orc


You can now checkout branch ``orc/level0`` to start the tutorial!

Level 0
-------

Let's roll! To begin with, the banner of the game looks very minimalistic. We
would like to use some ASCII art to make it look prettier::

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


To do so, we can use a new feature from C++11, *raw string literals*::

    R"(unquoted\ stuff)"
    R"delim(!%
    $)delim"

We can for instance put the banner in a ``static const char []``, or in an
unnamed namespace, and feed it to ``std::cout``.

Easy enough? Move on to next level!

Level 1
-------

In ORC, you are a brave warrior. So let's design a simple ``Warrior`` class. A
Warrior has a name, say a ``std::string``, given by his mum. It will never
change, so it is defined ``const``. It also has a few hit points, which leaves
us with::

    class Warrior {
        std::string const _name;
        long _hp;

        public:

            Warrior(std::string const& name, long hp=20):
                _name(name),
                _hp(hp)
            {
            }
    };

By default, the compiler generates a copy constructor for our class. There is
no cloning in ORC, so let's prevent the automatic generation using a new
feature of C++11::

    Warrior(Warrior const&) = delete;

Instead of ``delete`` we could have used ``default`` to make the implicit
generation explicit.

It's not very nice to have hard-coded constants like this ``20``. Instead we
will use a class constant, ``START_HP`` declared as a constant using the new
keyword ``constexpr``. In that case, its looks similar to ``const``, but more
on this later::

   long constexpr START_HP = 20;

In the ``main`` function, we can create two instances of ``Warrior``, give them
different name and have them fight together. To do so we need an ``attack``
method in warrior that removes a hit point to the target::

    void attack(Warrior& other) const {
        other._hp = std::max(0L, other._hp - 1);
    }

We also need a way to check if the warrior is alive, for instance by
overloading the ``(bool)`` operator::

    explicit operator bool() const {
        return _hp;
    }

Note that in C++11, it is possible to add the ``explicit`` to disable implicit
conversion to boolean, which we use here.

And finally a top-level function to handle the fight::

    void fight(Warrior& self, Warrior& other) {
        while(self and other) {
            self.attack(other);
            if(other) // don't attack if you're dead
                other.attack(self);
        }
    }

In the end, the winner is the warrior with at least one hit point, so our main
could look like (providing you've implemented the great ``Warrior::name()``
accessor)::

    Warrior me{"me"}, other{"other"};
    fight(me, other);
    Warrior& winner = me ? me : other;
    std::cout << R"(The winner is \o/ )" << winner.name() << R"( \o/)" << std::endl;

Note that we used braces to initialize our objects. It's a new feature from
C++11 called *uniform initialization* and it prevents strange stuffs like
``Warrior me();`` not looking like a function declaration, but parsed as a
function declaration (aka the "most vexing parse").

The game is not very fun as of now... let's step up one level!


Level 2
-------

Let's start to add some randomness to the game. C++11 comes with a new
``<random>`` library, that defines several random number generators and random
distributions. To use it you first need to instantiate a random device, then a
random number generator and finally a distribution. We'll use coin flips in the
game, so::

    namespace {
        std::random_device rd;
        std::mt19937 coin(rd());
        std::uniform_int_distribution<> flip(0, 1);
    }

makes it possible to ``flip(coin)`` to get a random number in [0, 1].

Let's use this randomness to choose which warrior attacks and which one retaliates::

    void fight(Warrior& self, Warrior& other)
    {
        while(self and other) {
            Warrior *first{&self}, *second{&other};
            if(flip(coin)) {
                using std::swap;
                swap(first, second);
            }
            first->attack(second);
            if(*second)
                second->attack(*first);
        }
    }

Note how ``std::swap`` is used to permute the Warriors depending on the coin
flip. The ``using`` constructs enables *Argument Dependent Lookup*, see
http://en.wikipedia.org/wiki/Argument-dependent_name_lookup. The implementation
of ``Warrior`` would have prevented to use ``std::swap`` on the references as
the copy constructor is deleted. Try it!

Now we should win exactly half of the games... Not very entertaining. Try next
level to make the game engine more complex!

Level 3
-------

There are several kind of warriors in ORC. Brave Knights, pretty Elves and, of
course, terrible ORCs. It's common to represent this using a simple class
hierarchy. A ``Knight`` is just a ``Warrior`` with an HP boost::

    class Knight : public Warrior {
        public:

        Knight(std::string const& name) : Warrior(name, START_HP + 10)
        {
        }
        virtual ~Knight() override {}
    };

An elf is much less tough, but it can chain several attacks in a row::

    class Elf : public Warrior {
        public:

        Elf(std::string const& name) : Warrior(name, START_HP - 5)
        {
        }
        virtual ~Elf() override {}

        void attack(Warrior& other) const override {
            Warrior::attack(other);
            if(flip(coin))
                attack(other);
        }
    };

And an ``Orc`` is a real warrior with a single-or double slash::

    class Orc : public Warrior {
        public:
        Orc(std::string const& name) : Warrior(name)
        {
        }
        virtual ~Orc() override {}

        void attack(Warrior& other) const override {
            Warrior::attack(other);
            if(flip(coin))
                Warrior::attack(other);
        }
    };

If you forget to make ``attack`` or the destructor virtual, the ``override``
qualifier will make the compiler grunt! Thank you C++11!

The name of the warrior somehow lacks the flavor brought by *Aegnor* the elf or
*Gorbag* the orc. Let's implement a default constructor for each race that
randomly picks a flavorful name::

    Elf() : Elf(random_pick(std::begin(names), std::end(names)))
    {
    }

Note that the default constructor of ``Elf`` uses the other constructor of
``Elf``. This is a new feature from C++11: *delegate constructors*!
Elvish_names is initialized as a static member with constant size::

    static constexpr char const* names[]{{"Aegnor", "Beleg", "Curufin"}};

Now that we have a more cosmopolitan world with a lot of funky names, instead
of a ``Warrior``, the player can get a random race::

    Warrior* pick_random_race(std::string const& name) [[yeah]] {
        std::array<Warrior *, 3> challengers{{ new Knight(name),
                                              new Elf(name),
                                              new Orc(name)
                                           }};
        std::random_shuffle(challengers.begin(), challengers.end());
        std::for_each(challengers.begin() + 1, challengers.end(),
                      [](Warrior * warrior) { delete warrior; });
        return challengers[0];
    }

That's not very efficient, but it works. Note that ``array`` and
*initialization lists* are used even if an ``std::vector`` might more suitable
there, and that we are also using a lambda function! More on this later though.

Also note the ``[[yeah]]`` attribute, which uses the new attribute mechanism.
Here the attribute is not recognized by the compiler and would result in a
warning.

In the ``main``, we can now write::

    Warrior* me = pick_random_race("me"),
           * other = new Orc();

Don't forget to add the ``delete`` calls in the end ;-) C++11 provides a way,
through ``unique_ptr`` and ``shared_ptr`` to avoid the use of ``new`` and
``delete`` in many cases, but let's go at our own speed.

Have you noticed that ``pick_random_race`` is tied to a specific Warrior's
constructor. It would be cumbersome to rewrite this function for each possible
constructor. More on this in the next level!

Level 4
-------

We left previous level in a dangerous state: we were just about to replicate
the code of ``pick_random_race`` to allow multiple constructor signature.
Fortunately C++11 introduces *variadic templates* to elegantly solve the issue.
The Syntax is the following::

    template<class... Args>
    Warrior* pick_random_race(Args const&... args) {
        std::array<Warrior *, 3> challengers{{new Knight(args...),
                                              new Elf(args...),
                                              new Orc(args...)
                                             }};
        std::random_shuffle(challengers.begin(), challengers.end());
        std::for_each(challengers.begin() + 1, challengers.end(),
                      [](Warrior * warrior) { delete warrior; });
        return challengers[0];
    }

It's not as generic as it could be but the rough idea is there. Called with no
argument, this function will call the default constructor, called with one
argument, it will forward it to the other constructor. The new call site is::

    Warrior*me = pick_random_race("me"),
           *other = pick_random_race();

But wait. Some stuff are still hard coded in our function: the race choice.
Let's use template parameters for templates (-::

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

The idea is to fill ``RaceSelector`` with several types that will populate the
``Races`` variadic template, while ``Args`` is still used for argument types.
At call site we can pick the races we want to play with at a single point::

    RaceSelector<Orc, Elf, Knight> races;
    Warrior*me = pick_random_race(races, "me"),
           *other = pick_random_race(races);

The ``races`` variable is only used for template argument type inference. Thats
why it is bound to no formal arguments in ``pick_random_race``.

Woah, that was a tough level! Lets try something a bit less mind-breaking ;-)

Level 5
-------

The game engine of ORC is still a bit rudimentary... In real games heroes have
more stats than just HP. Let's add the *STR* and *AGI* stats to pump our
warrior a bit! To do so, we will extend the ``Warrior`` class with a few more
member variables, accessors and also extend its constructor::

    size_t _str;
    size_t _agi;

    Warrior(std::string const& name, size_t hp = START_HP, size_t str = START_STR, size_t agi= START_AGI) :
        _name(name),
        _hp(hp),
        _str(str),
        _agi(agi)
    {
    }

    size_t hp() const { return _hp; }
    size_t str() const { return _str; }
    size_t agi() const { return _agi; }

We introduced a few more constants::

    static size_t constexpr START_AGI = 5;
    static size_t constexpr START_STR = 5;

AGIlity is used to compute who strikes first, and how many time you strike::

    void fight(Warrior& self, Warrior& other)
    {
        while(self and other) {
            using std::swap;
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
            std::cout << "after this round, " << self.name() << " has:" << self.hp() << " HP left and " << other.name() << " has:" << other.hp() << " HP left" << std::endl;
        }
    }

Some simple logging has been added to make the fight more entertaining. Also
notice that we did not specify the type of the ``strikes`` variable. Instead we
used the ``auto`` keyword that gets a new meaning in C++11: it performs local
type inference!

STRenght is used to compute the amount of damage dealt per blow::

    virtual void attack(Warrior& other) const {
        static_assert(START_STR>0, "not dividing by zero");
        other._hp = std::max(0, other._hp - long(_str + START_STR - 1) / START_STR);
    }

Notice this ``static_assert``? It verifies some properties on a compile-time
constant, just as an ``assert`` would for runtime expressions.

By the way, the game looks better with more asciiart, as in::

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

Near the end of the main. Thanks to
http://www.retrojunkie.com/asciiart/weapons/swords.htm for the nice arts!

And after this rest, see you next level!

Level 6
-------

Currently, the player of ORC never makes a choice, and that's not the
definition of a game. Let's had some leveling to the game! The idea is to pump
the stats of your warrior and have him defeat multiple foes. The more foes you
defeat, the stronger you get... That's how addictive games work ;-)

First step is to make it possible to choose which stat to pump. To do so we
introduce an enumeration::

    enum Stat : char {
        HP = 'h',
        STR = 's',
        AGI = 'a'
    };

That's kinda strange to have an enumeration inherit from a native type, but
that's a new feature of C++11, that lets you force the underlying type used to
represent the enumeration.

Now we want to prompt the user for several stats he/she wants to pump, using a
``char`` as stat code::

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

That's not the most generic piece of code, but there are several things to note
about it. First it uses an integral template parameter ``N``. Second it
inherits from ``std::array<char, N>``, which automatically grants him
``begin()`` and ``end()`` members. Then it uses the ``trim`` function that
operates in-place and removes spaces::

    std::string& trim(std::string & s) {
        auto isnotspace =  [](std::string::const_reference c) -> bool { return not std::isspace(c); };
        std::string::iterator first_non_space = std::find_if(s.begin(), s.end(), isnotspace);
        std::string::reverse_iterator last_non_space = std::find_if(s.rbegin(), s.rend(), isnotspace);
        s.erase(last_non_space.base(), s.end());
        s.erase(s.begin(), first_non_space);
        return s;
    }

This functions uses C++11 lambda function. A lambda function has four parts:

- a capture list in ``[]`` to capture variable from the environment;

- an argument list that resembles a classical argument list;

- an optional return type specification in the form ``-> type``. Local type
  inference usually does it for you

- a statement ``{ expr; }``

We used the ``auto`` keyword to avoid specifying the function type, but C++11
introduces a parametric type to store function-like objects::

    std::function<bool(std::string::const_reference)> isnotspace =  [](std::string::const_reference c) { return not std::isspace(c); };

Note that's a generic object that will store the lambda, it's not the lambda type!

To use our ``StatChooser``, we need an extra method in the ``Warrior`` class::

    template<size_t N>
    void buf(StatChooser<N> && sc) {
        for(auto const& c: sc)
        {
            switch(c) {
                case Stat::HP: _hp+=2; break;
                case Stat::STR: _str+=1; break;
                case Stat::AGI: _agi+=1; break;
            }
        }
    }

the ``buf`` method has nothing unusual, except the ``&&`` in the argument list.
It means ``StatChooser<N>`` is an r-value, a temporary object. Basically, you
are not allowed to write this in your ``main`` function call::

    StatChooser<8> sc(std::cin, std::cout);
    me->buf(sc);

you have to write::

    me->buf(StatChooser<8>(std::cin, std::cout));

You would not do this in a real code. But that's a tutorial, right? It also
uses a *range-based for loop*, that automates the ``for(auto iter = v.begin(),
end = v.end(); iter != end; ++iter)`` idiom, based on the availability of the
``begin()`` and ``end()`` method.

Update your main with the ``StatChooser``, and watch your buffed warrior win
all his fights... Until next level!

Level 7
-------

Now that your warrior can boost his stat, it's time to meet stronger opponents!
Each time you win a duel, you get some stat boost, you partially recover your HP
and a new challenger comes. After a few round, you'll have to fight several
foes in a row and so until you die... First step is to update the ``main`` function::

    Warrior *me = pick_random_race(races, "me");
    me->buf(StatChooser<8>(std::cin, std::cout));

    for(size_t round = 1; *me; ++round)
    {
        std::vector<Warrior*> foes{(round  + 9) / 10, nullptr};
        std::generate(foes.begin(), foes.end(), [&races]() { return pick_random_race(races); });
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

We are using quite a few C++11 features here. First, notice the ``nullptr``
identifier is an elegant replacement to ``NULL`` or ``0``. It is a new
*keyword* that has its own type ``std::nullptr_t`` that is implicitly
convertible and comparable to any pointer, but not to integral typs except
``bool``. Then the lambda used for the generator uses a capture list with a
capturing mode specifier. The ``=`` means we are capturing the ``races``
variable by reference. We could have omitted the ``&`` to capture the variable
by copy, or even remove the variable name to say we are capturing any variable
by reference. A *ranged-based for loop* is used with the ``auto`` qualifier.
There is no extra qualifier around the ``auto`` so we get a copy (of a
``Warrior`` pointer).

Using your eagle-eye powers, you may have noticed that we have started leaking
memory, as the memory allocated by ``pick_random_race`` is never freed. To make
memory management, C++11 provides several wrappers in the ``<memory>`` header.
Let's use some ``std::shared_ptr<T>``::

    std::vector<std::shared_ptr<Warrior>> foes{(round + 9) / 10, nullptr};
    std::generate(foes.begin(), foes.end(),
                  [&races]() { return std::shared_ptr<Warrior>(pick_random_race(races)); });

Creates a bunch of shared pointer pointing on nothing, then our lambda function
will set a proper value to the shared pointer, which will be automatically
freed when all references to that pointer are lost. That will certainly happens
at the end of the outermost loop body, when the holding vector will be deleted.

To help our warrior to survive several battles, we allow him to have a rest
between each round, using the ``Warrior::recover()`` method. This method is
defined as::

    virtual void recover() {
        auto recovery_rate = _max_hp / 10;
        _hp = std::min(_max_hp, _hp + recovery_rate * 2);
    }

with ``_max_hp`` properly initialized upon construction::

    long _max_hp;
    long _hp;

    Warrior(std::string const& name, size_t hp = START_HP, size_t str = START_STR, size_t agi= START_AGI) :
        _name(name),
        _max_hp(hp),
        _hp(hp),

And updated when we ``buf`` the warrior::

    case Stat::HP: _hp+=2; _max_hp += 2 ; break;

The game looks pretty decent now, you can start to play and see how many rounds
you can survive before going to next level.

Level 8
-------

The race we are fighting with in ORC is chosen at random. That's not very
satisfying: I don't want to be a ``Knight``. I wanna be an ``Orc``! So let's
start some more meta-programming, and define a way to pick a race. To do so, we
will associate a key to each race. This could be done by adding a static field
to each race, but we'll use type trait to do so. A trait begins as a templated
``struct`` declaration::

    template<class T> struct race_trait;

Then we specialize this template for each race::

    template<>
    struct race_trait<Knight> {
        static constexpr char value[] = "knight";
    };
    constexpr char race_trait<Knight>::value[];

And so fourth for the other races. Note that each trait can be evaluated at compile time thanks to the ``constexpr`` keyword.

Now let's write the ``pick_race`` function. It resembles the ``pick_random_race`` function::

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
            os << "] :";
            std::getline(is, buffer);
            trim(buffer);
        } while(buffer.size() != 1 and race_names.find(buffer[0]) == race_names.end());

        os << race_names.find(buffer[0])->second << " `" << name << "' joins the battle!" << std::endl;
        return race_selector<Races...>(name, buffer[0]);
    }

What a tough one! There are a lot of things we have already seen: the
``static_assert``, the lambdas, the variadic templates... But there are some
new stuff too. First let's welcome ``std::unordered_map<Key, value>``, the
C++11 hash table from ``<unordered_map>``. It got initialized through an
initializer list that itself contains initializer list of pairs, and this list
is automagically created through the variadic parameter expansion of the
``race_trait``! Woah, that's some C++11 magic!

Then we get a regular lambda function, but we're used to them now.

The do-loops prompt is generated from the content of ``race_names``. Note that
an iterator over an ``unordered_map`` yields an ``std::pair<Key, Value>``, and
that we are carefully capturing the ouptut stream ``os`` in the lambda -- it's
not copyable anyway. The careful programmer also noted the ``std::ref(...)``
call, that forced ``std::accumulate`` to take a reference to ``os``, while it
would have taken a copy (and fail miserably) otherwise.

In the end we get a ``char`` that represents the chosen race, we know it's
valid because it's in ``race_names``, but we must circle through each ``Races``
to create our Warrior. That can be done using a recursive variadic template
function (-::

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

This function recursively calls itself until it finds a ``Race`` with a
``race_trait`` matching selection character ``c``. It basically does compile
time recursion over the template argument ``Races`` until they are exhausted,
and a runtime check to pick the right ``Warrior`` child class. The ``assert``
from ``cassert`` is a bit old-school, it could have been a ``throw``.

We can now update our ``main`` function with::

   Warrior *me = pick_race(races, "me", std::cin, std::cout);

And enjoy our favorite race before next level, where all opponents with rush at
you at once!
