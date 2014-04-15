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
Warrior has a name, say a ``std::string``, and it was given by his mum, and
will never change, so it is ``const``. It also has a few hit points, which
leaves us with::

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
will use a top-level constant, ``START_HP`` declared as a constant using the
new keyword ``constexpr``. In that case, its use is similar to ``const``, but
more on this later::

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
``Warrior me();`` looking like a function declaration.


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
            Warrior *first= &self, *second = &other;
            if(flip(coin))
                std::swap(first, *second);
            first->attack(second);
            if(*second)
                second->attack(*first);
        }
    }

Note how ``std::swap`` is used to permute the Warriors depending on the coin
flip. The implementation of ``Warrior`` would have prevented to use
``std::swap`` on the references as the copy constructor is deleted. Try it!

Now we should win exactly half of the games... Not very entertaining. Try next
level to make the game engine more complex!
