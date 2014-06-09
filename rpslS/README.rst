================================
Rock-paper-scissors-lizard-Spock
================================

Quoting Wikipedia:

Rock-paper-scissors-lizard-Spock is an expansion of the classic selection
method game rock-paper-scissors. It operates on the same basic principle, but
includes two additional weapons: the lizard (formed by the hand as a
sock-puppet-like mouth) and Spock (formed by the Star Trek Vulcan salute).

The rules of rock-paper-scissors-lizard-Spock are:

* Scissors cut paper

* Paper covers rock

* Rock crushes lizard

* Lizard poisons Spock

* Spock smashes scissors

* Scissors decapitate lizard

* Lizard eats paper

* Paper disproves Spock

* Spock vaporizes rock

* Rock crushes scissors

The original rules remain the same.

Get ready to develop this famous game and jump to ``rpsls/level0``!

Level 0
=======

In rpslS, the user provides the number of win non-tie rounds he wants to play.
To do so, one relies on the ``main`` signature::

    int main(int argc, char* argv[]) {
    ...
    }

The ``int`` return value is used to state wether an error occured. ``0`` means
everything is OK, a non-zero value means an error occured. The ``argc``
parameter is the number of arguments. It's always at least 1, as the first
argument is the name of the program itself. ``argv`` is a null-terminated array
of strings.

In rpslS, a typical game begins with::

    ./rpslS 10

In that situation, ``argv[0]`` contains ``./rpslS`` and ``argv[1]`` contains
``10``. So we first need to check if an argument is given, and exit otherwise::

    int main(int argc, char* argv[]) {
        if(argc != 2) {
            return usage(argv[0]);
        }
        return 0;
    }

where usage is a helper function implemented as::

    static
    int usage(char const *progname) {
        std::cerr << "usage: " << progname << " <positive_number>" << std::endl;
        return 1;
    }

The ``static`` qualifer means the function is local to the compilation unit. It
cannot be used outside of it.

Let's jump to ``rpslS/level1`` to start the real game coding!

Level 1
=======

It is necessary to convert the string from the CLI to an integer. The function
to do so is ``std::stoi`` from ``<string>``::

    int nb_round = std::stoi(argv[1]);

Let's just verify the user did not provide a non-positive value::

    if(nb_round <= 0) {
        return usage(argv[0]);
    }

See how useful it was to put the usage printing into a function? Code reuse!

We need to extra integers to count the score of the player and the socre of the
AI. It is possible to declare multiple variables in a row, using::

    size_t your_score = 0,
           ai_score = 0;

``size_t`` means a (plateform-dependant) unsigned integer large enough to count
all bytes in memory. This should be enough :-)

The main loop of the game might look like this::

    while(nb_round) {
        /* do some stuff */
        --nb_round;
    }

And ends as::

   std::cout << "Your score: " << your_score << std::endl
             << "AI score:   " << ai_score << std::endl;

Note the use of the prefix ``--`` operator, which means decrease the value of
``nb_round`` by one and return the new value.

We'll start to model the game in next level. Step forward to ``rpslS/level2``!

Level 2
=======

There are 5 types of weapons in rpslS. An enumeration of stuff can be encoded
using an enumeration::

    enum class Weapon {
        ROCK,
        PAPER,
        SCISSOR,
        LIZARD,
        SPOCK
    };

An enumeration just defines a few identifiers, associated to integer values.
The default is to start at 0 and then to increase the values by one. Said
otherwise, ``ROCK == 0``, ``PAPER == 1`` etc.

Note the ``class`` qualifier, which means that the conversion from or to
integer is not automatic.

A similar enumeration can be used for a weapon clash::

    enum class Status {
        LOOSE,
        DRAW,
        WIN
    };

The core of the game is the status matrix. Using two ``Weapons`` (and thus
integers thanks to the conversion to ints), it makes it easy to access the
clash status. To do so, one can use a nested static array structure. A
fixed-length array type is declared in ``<array>``. It is a parameteric type
stated as ``std::array<Type, NumberOfElements>``. In our case, we get::

    std::array< std::array< Status, 5>, 5> Matrix {
        {
            /* Rock */      { Status::DRAW, Status::LOOSE, Status::WIN, Status::WIN, Status::LOOSE },
            /* Paper */     { Status::WIN, Status::DRAW, Status::LOOSE, Status::LOOSE, Status::WIN },
            /* Scissors */  { Status::LOOSE, Status::WIN, Status::DRAW, Status::WIN, Status::LOOSE },
            /* Lizard */    { Status::LOOSE, Status::WIN, Status::LOOSE, Status::DRAW, Status::WIN },
            /* Spock */     { Status::WIN, Status::LOOSE, Status::WIN, Status::LOOSE, Status::DRAW }
        }
    };

Note the use of bracket for the initializations, instead of the equal sign. That's *uniform initialization*!

We now need to type in our choice of weapon. We want something like this in the main loop::

    Weapon your_weapon;
    std::cout << "(r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? ";
    std::cin >> your_weapon;

Eventually, one should add ``std::cin.ignore(1)`` to drop the newline character.

Trying to compile the above code results in an error because the compiler
doesn't know how to read a ``Weapon`` from a stream. Let's tell him! This is
done by *overloading* the ``>>`` operator::

    std::istream& operator>>(std::istream& is, Weapon& w) {
        char c;
        is >> c;
        switch(c) {
            case 'r': w = Weapon::ROCK; break;
            case 'p': w = Weapon::PAPER; break;
            case 's': w = Weapon::SCISSOR; break;
            case 'l': w = Weapon::LIZARD; break;
            case 'S': w = Weapon::SPOCK; break;
        };
        return is;
    }

Note that ``w`` is passed by *reference*, i.e. modifying ``w`` in the function
modifies the call site. A ``switch`` is used to traverse the possible
conversions. the ``break`` are mandatory to prevent the ``switch`` to go to the
next ``case``.

The main loop can now be written as::

    Weapon your_weapon;
    std::cout << "(r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? ";
    std::cin >> your_weapon;
    std::cin.ignore(1);

    Weapon ai_weapon = Weapon::ROCK; // always the same!
    Status status = Matrix[static_cast<int>(your_weapon)][static_cast<int>(ai_weapon)];

    switch(status) {
        case Status::WIN:
            ++your_score;
            --nb_round;
            break;
        case Status::LOOSE:
            ++ai_score;
            --nb_round;
            break;
        case Status::DRAW:
            ;
    }

What happens in this peice of code? Using your weapon and AI's, it is possible
to index ``Matrix``. ``static_cast<int>`` is used to perform the conversion
between the ``Weapon`` type and the ``int`` type. As stated before, using an
``enum class`` makes the use of a cast mandatory.

The ``switch`` just updates the score depending on the status. It's now time to
create a smarter AI... in next level!

Level 3
=======

An easy way to get a not-too-dumb AI is to make an AI that always chooses its
weapon at random. To do so we'll use the ``<random>`` header, ad we'll define a
very simple class::

    class AI {
        std::default_random_engine _rengine;
        std::uniform_int_distribution<int> _uniform_dist;

        public:

        AI() :
            _rengine(std::random_device()()),
            _uniform_dist(0,4)
        {
        }
    };

This small ``class`` defines two ``private`` (the default) fields used for
random number generation, and a ``public`` constructor. A constructor is a
special *member function* used to initialize the object. Whenever a variable is
created, its constructor is called. Here we are just defining the *default
constructor*, the constructor that takes no arguments.
``std::uniform_int_distribution<int>`` is a random number adaptor, that takes
the source of randomness ``std::default_random_engine`` and returns a number
between ``0`` and ``4`` included each time it is called.

To call  for a weapon, let's define a ``weapon`` member function::

    Weapon weapon() {
        return static_cast<Weapon>(_uniform_dist(_rengine));
    }

Using the ``int`` to ``Weapon`` explicit conversion, we just draw a random
integer and convert it to the relevant ``Weapon``.

If we instantiate the AI, using::

    AI ai;

We can now use it in the core loop::

    Weapon ai_weapon = ai.weapon();

Which makes the game much more interesting! There are still a few other thing
to clean up, but that's for the next level!
