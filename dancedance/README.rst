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

In text based *interactive* games, one often need to read data from the
standard input. This is accomplished in C++ using the counterpart of
``std::cout``, namely ``std::cin``.

To read a data from standard input, one uses the ``>>`` operator, which is
overloaded for many types; For instance, to read a ``char`` from standard
input, use::

    char input;
    std::cin >> input;

Note that most modern terminal only flush to standard input when a new line is
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
        std::endl << "Well done!" << std::endl;
        return 1;
    }

The return value of a program is always ``0`` in case of success, another value
(here ``1``) in case of error.

It's now time to use a C-style trick, more about this in level 2!
