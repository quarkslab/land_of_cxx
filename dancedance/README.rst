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
