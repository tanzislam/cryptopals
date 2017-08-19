# Cryptopals

<a href="https://travis-ci.org/tanzislam/cryptopals">
  <img alt="Build Status"
       src="https://travis-ci.org/tanzislam/cryptopals.svg?branch=master"/>
</a> <a href="https://scan.coverity.com/projects/tanzislam-cryptopals">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/10143/badge.svg"/>
</a> <a href="https://www.codacy.com/app/tanzislam/cryptopals">
  <img src=
   "https://api.codacy.com/project/badge/Grade/0eefb6418259474191b5611351e02dee"
  />
</a> <a href="https://sonarcloud.io/dashboard?id=tanzislam_cryptopals">
  <img alt="SonarCloud"
       src="https://sonarcloud.io/api/badges/gate?key=tanzislam_cryptopals"/>
</a>

These are my solutions to the interesting challenges at [the cryptopals crypto
challenges](https://cryptopals.com). All solutions are coded in C++14 and Boost.

## Requirements

To build these solutions you will need:
 - [GNU Compiler Collection (GCC)](https://gcc.gnu.org/). I used v6.2.0 in
   [MinGW-w64](https://mingw-w64.org/doku.php) (build `x86_64-posix-seh-rev1`)
   on Windows. On macOS Sierra I used the default [Clang](http://clang.llvm.org)
   v8.0.0 (build `clang-800.0.42.1`) which is compatible with GCC.

 - [GNU Make](https://www.gnu.org/software/make/). I used v4.1 in MinGW-w64 in
   Windows 7 (invoked as `mingw32-make`). On macOS Sierra I installed v4.2.1
   using [Homebrew](http://brew.sh) (invoked as `gmake` to distinguish from the
   system default v3.8.1).

 - [Boost C++ Libraries](http://www.boost.org/). I used v1.62.0.
   - You will need to build the libraries as described for your platform in the
     Getting Started Guide. Only specific libraries are needed (as indicated by
     the `BOOST_LIBS` line in each solution's `GNUmakefile`), so you can speed
     up the build by appending the `--with-<library_name>` option below.
   - On Windows: `b2 --layout=system toolset=gcc variant=release`.
   - On macOS and Linux: `./b2 --layout=system variant=release`.

 - [Google Test](https://github.com/google/googletest/). I used the "master"
   branch at commit a2b8a8e07.
   - You will need to build Google Test as described in the Generic Build
     Instructions. To do this on Windows you will need a UNIX-like set of
     utilities (see below).
   - On Windows / MinGW-w64 I had to specify a Makefile override:
     `cd googletest/make; mingw32-make AR=gcc-ar`.

 - [Hunspell](https://hunspell.github.io/). I used the "master" branch at commit
   1fc14b0c7.
   - On Windows 7 / MinGW-w64 I had to first install `libtool` (by running
     `pacman -S libtool` in an admin-mode MSYS2 window), and then I could run
     `autoreconf -vfi && ./configure && mingw32-make` in a MinGW-w64 Win64
      Shell.
   - On macOS Sierra / Homebrew I had to first obtain the GNU Autotools
     packages: `automake`, `libtool`, and `gettext`. I also had to export the
     `LDFLAGS` and `CPPFLAGS` environment variables to what `brew` specified
     when installing `gettext`. After that I could run the standard steps.
   - You will also need an English dictionary for Hunspell (specifically the
     `.dic` and `.aff` files. Get one from [SCOWL](http://wordlist.aspell.net/).

 - [Crypto++](http://www.cryptopp.com/). I used the "master" branch at commit
   84b602cc7.
   - You will need to Build Crypto++ as a static library using the included
     `GNUmakefile`. To do this on Windows you will need a UNIX-like set of
     utilites (see below).

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
inner directory has a `GNUmakefile` that expects these variables:
 - `BOOST_DIR` -- the top-level directory of your Boost installation
 - `GTEST_DIR` -- the top-level directory of your Google Test installation
 - `HUNSPELL_DIR` -- the top-level directory of your Hunspell repository clone
 - `CRYPTOPP_DIR` -- the top-level directory of your Crypto++ repository clone

To build any solution, ensure that the above are set in the environment, then
`cd` to that directory and run `make` (or `gmake`, `mingw32-make`, etc.). This
will produce the executable, along with other intermediary build products. The
executable name will begin with `test_`, so should be easy to locate.

Alternatively, you can use the top-level `GNUmakefile` by running `make` (or its
equivalent for your platform) in the repository root directory. This will
produce the combined test program called `test`.

**NOTES:**
 - None of the above variables may have spaces in them. So, on Windows you can't
   use `C:\Program Files\...`, but `C:\Users\UsernameWithoutSpace\Documents\etc`
   is good.
 - The above variables do not have to be **environment variables**, but can
   instead be specified in the GNU Make command line as [overrides](
   https://www.gnu.org/software/make/manual/make.html#Overriding).
 - The above variables may be either relative paths or absolute paths.

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
