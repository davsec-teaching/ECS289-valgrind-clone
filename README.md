# Valgrind clone

Instruments each `malloc` and `free` call in a C program and prints the number of heap objects currently allocated.

Build:

    $ cd llvm-pass-skeleton
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Run:

    $ ./run.sh
