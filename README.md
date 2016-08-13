# Cryptopals

These are my solutions to the interesting challenges at [the cryptopals crypto
challenges](https://cryptopals.com). All solutions are coded in C++11 and Boost.

## Requirements

To run these solutions, you will need:
 - GNU Compiler Collection (GCC). I used v5.3.0 in MinGW-W64 (`posix-seh-rev0`).
 - Boost C++ Libraries. I used v1.61.0.
 - Google Test. I used the "master" branch at commit ec44c6c.
 - GNU Make. I used v4.1 in MinGW-W64 (`mingw32-make`).

Although my development environment is in Windows, I tried to make no
assumptions about the environment, instead stuck by whatever is guaranteed by
the language standard and library documentation. The solutions should build in a
UNIX environment just fine.

## Building

The solutions are organized in two directory levels: the first level for the Set
and the second level for the Challenge (as in the Cryptopals website). Each
inner directory has a `GNUmakefile` that expects these environment variables:
 - `BOOST_DIR` -- the top-level directory of your Boost installation
 - `GTEST_DIR` -- the top-level directory of your Google Test installation

To build any solution, ensure that the above environment variables are set, then
`cd` to that directory and run `make` (or `gmake`, `mingw32-make`, etc.). This
will produce the executable, along with other intermediary build products. The
executable name will begin with `test_`, so should be easy to locate.

## Deciphering the Code

The `main` translation unit of each solution is full of test cases written for
the Google Test framework. The `main()` inside Google Test is used, so you will
not find a `main()` function in here.

Unlike most solutions, these solutions aim to be scalable to very large inputs
with constant memory requirements. They are therefore designed to work with C++
IOStreams for input and output, instead of in-memory `std::string` objects. They
never read in _all_ the contents of the input streams into memory, but do read
in small chunks at a time for code-conversion, frequency analysis, etc. They
also work on the assumption that the input and output streams are slow devices,
and therefore try to minimize the number of times they are read or written to
(ideally only one pass).

The test drivers (i.e. the source files with names beginning with `test_`) are
_not_ considered part of the solutions, so you will find them constructing
in-memory streams containing the input and output data.

Because of the above design constraints, these solutions have become somewhat of
an adventure in IOStreams programming, using some of its more advanced details
and techniques:
 - custom manipulators with arguments
 - checking and setting `std::ios_base::io_state` flags
 - custom string buffers
