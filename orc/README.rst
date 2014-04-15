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
