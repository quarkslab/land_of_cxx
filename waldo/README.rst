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

Level 1
=======

In the real game, Waldo is a nice guy with glasses and a stripped hat. In our
game, it will be a nice ``w`` (with glasses and a stripped hat?). We will hide
this letter in a random location in the ``waldo`` string, but it would be to
easy to put it in-place of a blank character, so we'll randomly pick a
character, check if it is a blank and iterate again if needed.

To begin with, a random number between ``0`` (included) and ``waldo.size()``
(excluded) is needed. C++ has a complex random number generation system, the
header we need is::

    #include <random>

and the typical idiom in that case is::

    std::random_device rd;
    std::default_random_engine rnd(rd());
    std::uniform_int_distribution<size_t> uniform_dist(0, waldo.size() - 1);

as it make it possible to get a random number within the proper bound by
calling::

    size_t pos = uniform_dist(rnd);

where ``size_t`` is a convenient way to say "unsigned int large enough to index
the whole memory".

To check if the character at this particular location is a blank, one can use::

    std::isblank(waldo[pos])

so to keep picking chars until a non-blank value is found, we can use a loop::

    size_t pos;
    do {
        pos = uniform_dist(rnd);
    } while(std::isblank(from[pos]));

and finish the work by setting the proper position with the proper value::

    from[pos] = 'w';

Instead of writing all this in the ``main`` function, it would be nice to have
everything in a function that changes its argument in-place::

    void hide_waldo(std::string& from) {
    // function body
    }

where ``void`` means we are not producing any value, and ``(std::string& from)``
means the function takes a single argument named ``from`` of type
``std::string`` which is passed by reference (the ``&``) i.e. the function can
modify the argument, and the modification will be seen at the call site. The
call site simply looks like::

    hide_waldo(waldo);

Putting everything together, we now have a nice game! There are a few
limitations though, as we perform no check on user input!

Level 2
=======

There are several situations where our game engine fails: what if the file is
empty? What if it only has blanks? What if it already contains a ``w``? Let's
check this out!

C++ provides a convenient mechanism, called *exceptions* to report error. The
basics are "when you find an error, report it has an exception and someone may
manage it". A set of common exceptions are defined in the ``<stdexcept>``
header, we'll use ``std::runtime_error``, as in::

    if(something_wrong)
        throw std::runtime_error("Something happend on the way to heaven")

To perform the checks, we'll use a few standard algorithm from the
``<algorithm>`` header. It's a nice thing to use them instead of raw loops!
We'll design a new function to perform all the checks::

    void check_validity(std::string const& picture) {
    // body
    }

The function does not return anything, and it does not modify its argument (it
is marked as ``const &`` which means you're working on the argument passed from
the call site, but you cannot modify it). The only thing it will do is
eventually ``throw`` an exception.

For instance if there already is a ``w`` in the file. The ``std::find``
algorithm is perfect for the job: it takes an iterator to the beginning and the
end of the string, the character we are looking for and returns either an
iterator pointing to the found character location, or an iterator pointing to
the end of the string. But what is an iterator? It is a common concept in
computer since that abstracts the transversal of a container. In C++, you
generally get an iterator to the beginning of the container through ``.begin()``
and an iterator to the end of the container through ``end()``. So to find a
``w`` lets write::

    if(std::find(picture.begin(), picture.end(), 'w') != picture.end())
        throw std::runtime_error("Waldo symbol already in input file");

note that the ``std::find`` function is generic. It can be used to find an
integer in a vector of integer, a character in a string or a needle in a needle
stack!

The ``std::count`` algorithm is useful to count the number of occurrences of
something within a range. So to count the number of ``\n`` (they embody new
lines on Linux systems), lets write::

     if(std::count(picture.begin(), picture.end(), '\n') == 0)
             throw std::runtime_error("not even a single line");

Finally we may prevent an infinite loop in our random search of non blank
characters by asserting there is at least one non-blank character. The
``std::all_of`` algorithm checks if all elements between two iterators verify a
given function, so what about::

    if(std::all_of(picture.begin(), picture.end(), std::isblank))
            throw std::runtime_error("input file full of blank");

Putting everything together, we get a pretty nice input checking function! The
game is almost complete, but we'll do some code cleaning before playing.
