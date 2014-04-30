=====
Waldo
=====

In WALDO, you are playing a text based version of the famous *Where's Waldo?*
books from Martin Handford. The goal is to find the ``w`` character in an ascii
art image!

To begin with, the image in ``waldo.txt``, which suits perfectly the theme of
the game, is used. A minimal ``Makefile`` has been setup, as well as an even
more minimal version of the ``waldo`` program. Run::

    $> make

to build the program, then::

    $> ./waldo

to run it, and finally::

    $> git checkout waldo/level0

to start the tutorial!


Level 0
=======

The first step is to read the content of the ``waldo.txt`` file and to display
it on the screen. To do so, C++ provides an *input file stream* abstraction. You
can create such a file using::

    std::ifstream ifs{"waldo.txt"};

to do so you must make the ``std::ifstream`` type, from the standard library (as
hinted by the ``std::``) available to the compiler, by including the proper
header::

    #include <fstream>

It will just open the file for reading (``i`` stands for input). The easiest way
to manipulate the file is to slurps everything in a string. The standard library
defines a string type ``std::string`` through another header::

    #include <string>

One can build a string from anything that contains characters, which is the case
of our file. The following line involves a bit of dark magic::

    std::string waldo{std::istreambuf_iterator<char>{ifs},
                      std::istreambuf_iterator<char>{}

so lets explain it. We are creating a ``std::string`` variable named ``waldo``.
We are using two arguments (between curly braces) to initialize it. One denotes
the first character to read, the other denotes the end. To stream ``char`` out
of the file, we use a special wrapper, ``std::istreambuf_iterator<char>``
initialized by our file object for the beginning, and nothing for the end.

Finally we can stream the resulting string to the standard output::

    std::cout << waldo

and be wowed by the result!

In next level, we will hide a small ``w`` in the resulting image!
