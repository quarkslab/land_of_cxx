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
