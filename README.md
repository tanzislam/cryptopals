# Cryptopals

<a href="https://scan.coverity.com/projects/tanzislam-cryptopals">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/10143/badge.svg"/>
</a>

These are my solutions to the interesting challenges at [the cryptopals crypto
challenges](https://cryptopals.com). All solutions are coded in C++14 and Boost.

## Requirements

To run these solutions, you will need:
 - [GNU Compiler Collection (GCC)](https://gcc.gnu.org/). I used v6.2.0 in
   [MinGW-W64](https://mingw-w64.org/doku.php) (build `x86_64-posix-seh-rev1`).

 - [GNU Make](https://www.gnu.org/software/make/). I used v4.1 in MinGW-W64
   (invoked as `mingw32-make`).

 - [Boost C++ Libraries](http://www.boost.org/). I used v1.62.0.
  - You will need to build the libraries as described for your platform in the
    Getting Started Guide. Specify `--layout=system toolset=gcc variant=release`
    as command-line options to the `b2` or `bjam` commands, and use the default
    location for the `stage` target.

 - [Google Test](https://github.com/google/googletest/). I used the "master"
   branch at commit ec44c6c.
  - You will need to build Google Test as described in the Generic Build
    Instructions. To do this on Windows you will need a UNIX-like set of
    utilities (see below).

 - [Hunspell](https://hunspell.github.io/). I used the "master" branch at commit
   1474200bb.
  - The included documentation doesn't mention a way to build Hunspell with the
    GCC in MinGW-W64, but I managed to do so by running `./configure` followed
    by `mingw32-make` in an MSYS2 window.
  - You will also need an English dictionary for Hunspell (specifically the
    `.dic` and `.aff` files. Get one from [SCOWL](http://wordlist.aspell.net/).

UNIX-like utilities on Windows are provided by any of the following:
 - [MSYS2](http://msys2.github.io/), or originally [MSYS](http://www.mingw.org/)
 - [Cygwin](https://www.cygwin.com/)
 - Interix / [Windows Services for Unix](
   https://technet.microsoft.com/en-gb/library/bb463212.aspx) / [Subsystem for
   Unix-based Applications (SUA)](
   https://msdn.microsoft.com/en-us/library/cc772343(v=ws.11).aspx) --
   available as an optional Windows feature up to Windows 8 / Server 2012
 - [GnuWin](http://gnuwin32.sourceforge.net/)

You may need to install additional packages beyond the ones bundled with the
default installation. Check the respective documentation for guidance on how to
locate and install packages.

Although my development environment is in Windows (primarily [Notepad++](
https://notepad-plus-plus.org/) enhanced with some humble [NppExec](
https://sourceforge.net/projects/npp-plugins/files/NppExec/) scripts), I tried
to make no assumptions about the environment and instead stuck by whatever is
guaranteed by the language standard and library documentation. The solutions
should build in a UNIX environment just fine. Check out the [Wiki](
https://github.com/tanzislam/cryptopals/wiki) for guides to using additional
development tools.

## Building

The solutions are organized in two directory levels: the first level for the Set
and the second level for the Challenge (as in the Cryptopals website). Each
inner directory has a `GNUmakefile` that expects these environment variables:
 - `BOOST_DIR` -- the top-level directory of your Boost installation
 - `GTEST_DIR` -- the top-level directory of your Google Test installation
 - `HUNSPELL_DIR` -- the top-level directory of your Hunspell repository clone

To build any solution, ensure that the above environment variables are set, then
`cd` to that directory and run `make` (or `gmake`, `mingw32-make`, etc.). This
will produce the executable, along with other intermediary build products. The
executable name will begin with `test_`, so should be easy to locate.

Some solutions will require the location of a Hunspell dictionary to be provided
in the `HUNSPELL_AFFIX_PATH` and `HUNSPELL_DICT_PATH` environment variables.

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
 - custom stream buffers as input/output filters

### Why not Boost.Iostreams?

I could have started out with the [Boost.Iostreams](
http://www.boost.org/doc/libs/release/libs/iostreams/) framework for all this,
instead of trying to make ends meet with the Standard IOStreams library. While
the Boost framework is impressively designed for ease of use, my intention here
was to primarily explore the C++ Standard Library thoroughly and identify its
shortcomings, followed by a gradual feature-comparison with the Boost libraries.
