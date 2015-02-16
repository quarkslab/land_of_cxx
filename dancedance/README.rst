===================
Dance Dance Fingers
===================

In DDF, your fingers learn to dance on the keyboard! Try to type as fast as the
letters appear on the screen!

To begin with, a minimal ``Makefile`` has been setup, as well as an even
more minimal version of the ``dancedance`` program. Run::

    $> make

to build the program, then::

    $> ./dancedance

to run it, and finally::

    $> git checkout dancedance/level0

to start the tutorial!

Level 0
=======

Any text-based game needs a banner, some text printed out on the screen to
welcome the gamer. In C++, raw text is represented between double quotes, as
in::

    "this is a string"

This is a simple banner::

    "--<< Dance Dance Fingers >>--"

To print something on the screen, a special output *stream* is used. There are
three default output streams in C++: ``std::cout``, ``std::clog`` and
``std::cerr``. The ``std::`` prefix means they come from the standard library
(they embody the standard namespace, but that's off topic as of now). To *send*
something to an output stream, one uses the *stream operator* ``<<``, as in the
famous::

    std::cout << "hello world";

To get the definition of this stream, one needs to include a *standard header*,
using::

    #include <iostream>

at the top of the program, outside of any function.

To print a newline character, a special construct from the standard library is
used, ``std::endl``. It takes care of system-specific representation of a
newline::

    std::cout << "hello world";
    std::cout << std::endl;

One can chain stream operators, for a more concise code::

    std::cout << "hello world" << std::endl;

Write your own banner like this, and jump to level1!

Level 1
=======

In text based *interactive* games, one often needs to read data from the
standard input. This is accomplished in C++ using the counterpart of
``std::cout``, namely ``std::cin``.

To read a data from standard input, one uses the ``>>`` operator, which is
overloaded for many types; For instance, to read a ``char`` from standard
input, use::

    char input;
    std::cin >> input;

Note that most modern terminals only flush to standard input when a new line is
typed (that's the feature that makes line editing possible).

Now ask the user to strike the ``h`` key, using the standard output stream::

    std::cout << "Strike the 'h' key:";

You can check the ``input`` value using a conditional, as in::

    if(input == 'h') {
        std::cout <<  std::endl << "Well done!" << std::endl;
        return 0;
    }

In case of error, the *error stream* is usually used. It's ``std::cerr`` and
it's an output stream, so its use is similar to ``std::cout``::

    else {
        std::cerr << std::endl << "You failed!" << std::endl;
        return 1;
    }

The return value of a program is always ``0`` in case of success, another value
(here ``1``) in case of error.

It's now time to use a C-style trick to put the terminal in raw mode, more
about this in level 2!


Level 2
=======

It's a pain in the ``***`` that the terminal only flushes once a new line is
filled. For the great DDF game, we need to interact on a char-based stream,
without the terminal even echoing the key we strike.

There is a standard way to communicate with the terminal and change its
behavior, but it uses a C interface. Fortunately, C++ code can use any C code
or header. The header is ``<termios.h>`` and the functions we need are
``tcgetattr(3)``, ``tcsetattr(3)`` and ``cfmakeraw(3)``. The first gets the
state of the terminal, the second changes it and the third creates the
attributes needed to use the *raw mode*.

It's a bit soon to understand how we use these functions, so let's use the code
from ``dancedance_term.cpp``. To do so, we first include the interface from
``dancedance_term.hpp``, using quotes ``#include "..."`` to tell the compiler
we are using a local header, as opposed to system header that use ``#include
<...>``::

    #include "dancedance_term.hpp"

This header defines a new type, ``TermiOS``, that behaves as follows:

    * when a variable of type ``TermiOS`` is created, the terminal is set in
      raw mode;

    * when a variable of type ``TermiOS`` is deleted, the terminal is set back
      to its original mode.

The implementation is in ``dancedance_term.cpp`` that has been added to the Makefile.

In C++, a variable lives from its definition to the end of current block -- a
block is usually associated to a single instruction as in::

    if(1)
        something(); // this instruction is a one-line block

or to a pair of brackets, as in::

    {
        something();
        else();
    }

It is a common idiom to associate begin/end behavior to a variable, using the
*constructor* to define the begin behavior, and the *destructor* to define the
end behavior. It's called *Resource Acquisition Is Initialization*.

In our case, we can use it to set the term in raw mode during char input::

    char input;
    {
        TermiOS tos;
        std::cin >> input;
    }

If you are curious enough, you had a look to ``dancedance_term.hpp``. You may
have noticed two different kinds of comments:

    * one line comments, starting with a ``//`` and ending with the line;

    * multi-line comments, enclosed between ``/*`` and ``*/``.

And if you are really curious concerning how we'll end this game, jump to level 3!

Level 3
=======

In DDF, the user sings a melody with its finger. The melody is a suite of
chars, so a *string* is suitable to store them. The standard library defines
the ``std::string`` type in the ``<string>`` header. It can be initialized from
a string literal, as in::

    std::string melody = "ddfisfun";

If we are sure we will never modify it, it is possible to tell it so to the
compiler, using the ``const`` qualifier::

    const std::string melody = "ddfisfun";

Once the variable is declared, we can *iterate* over each of its element using a *range based for loop*::

    for(char note : melody) {
        ...
    }

which associates the ``note`` variable of type ``char`` to each element of the
``std::string``. The loop *body* is roughly the main as the one before::

    std::cout << note;
    char in;
    std::cin >> in;
    if(in != note) {
        break;
    }

The ``break`` instruction means we end the loop before reaching the end of the
iterated variable. In order to record whether we ended the loop because of a
``break`` or because we reached the end of the *melody*, we'll use a counter
that counts (!) the number of loop trips, and is set to zero when the player
mistypes a note::

    size_t count = 0;
    ...
    if(in != note) {
        count = 0;
        break;
    }
    else {
        ++count;
    }

Where ``size_t`` is a kind of unsigned integers... The error handling is pretty
straight forward::

    // error handling
    if(count == 0) {
        std::cerr << std::endl << "Looser!" << std::endl;
        return 1;
    }

We'll use the ability of ``std::cout`` to seamlessly print integers in the win report::

    else {
        std::cout << std::endl << "Winner! (" << count << " letters)" << std::endl;
        return 0;
    }

Of course, it is important to keep a ``TermiOS`` variable somewhere in the
code!

We're almost done with this game. The last level adds some bells and whistles to
this already great game ;-)

Level 4
=======

To make DDF more challenging, let's add a chronometer so that you can strive to
beat your own speed record! Timing utilities are available in the ``<chrono>``
header. It's a very rich header, but we'll only use the
``std::chrono::system_clock::now()`` function. The chaining of ``::`` is used
because of nested *namespaces* and static member functions: we'll be using the
``now`` member function from the ``system_clock`` class from the ``chrono``
namespace from the ``std`` namespace.

When we don't want to bother with the return type of a function, it's possible
in C++ to use the ``auto`` keyword. It will automatically match the type of the
*right hand side* of the assignment::

    auto now = std::chrono::system_clock::now();

If we add a similar statement at the end of the game::

    auto end = std::chrono::system_clock::now();

We can finally count the elapsed time, in millisecond::

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now).count();

And eventually print it to the standard output. This last part concludes this
DDF game, now, let the fingers dance!
