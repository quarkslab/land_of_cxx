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

    Elf() : Elf(random_pick(names, names + sizeof(names)/sizeof(*names)))
    {
    }

Note that the default constructor of ``Elf`` uses the other constructor of
``Elf``. This is a new feature from C++11: *delegate constructors*! Elvish_names is initialized as a static member with constant size::

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
