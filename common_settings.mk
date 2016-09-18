# Validate that we have everything we need
ifeq "" "$(SRCS)"
    $(error SRCS must be defined before including this file)
else ifeq "" "$(filter clean print-%,$(MAKECMDGOALS))"
    ifeq "" "$(BOOST_DIR)"
        $(error The BOOST_DIR variable must be set to where you installed Boost)
    else ifeq "" "$(GTEST_DIR)"
        $(error The GTEST_DIR variable must be set to where you downloaded or\
                cloned Google Test and Google Mock)
    else ifeq "" "$(HUNSPELL_DIR)"
        $(error The HUNSPELL_DIR variable must be set to where you downloaded \
                or cloned Hunspell)
    endif
endif

# Standard settings
CXX = g++
CPPFLAGS = -std=c++11 -Wc++11-compat -Wc++14-compat -pedantic -Wall -Wextra -O3
CXXFLAGS = -isystem $(BOOST_DIR) \
           -isystem $(GTEST_DIR)/googletest/include \
           -isystem $(HUNSPELL_DIR)/src/hunspell
LDFLAGS = -pthread \
          $(if $(BOOST_LIBS),-L$(BOOST_DIR)/stage/lib) \
          -L$(HUNSPELL_DIR)/src/hunspell/.libs
LDLIBS = $(GTEST_DIR)/googletest/make/gtest_main.a \
         $(foreach lib,$(BOOST_LIBS),-lboost_$(lib)) \
         $(foreach lib,$(LIBS),-l$(lib))

# This is so that a Challenge-specific makefile can "include" the directory of
# another Challenge using VPATH for both dependencies and C++ headers.
#
# Precondition: VPATH must be space-delimited, and not colon- or
#               semicolon-delimited.
CXXFLAGS += $(foreach v,$(VPATH),-I$(v))

# The built-in rule for linking object files uses the $(CC) variable, which
# expands to "cc". However, the "cc" program does not exist in a standard
# MinGW-w64 installation, so using $(CXX) instead as defined above.
CC = $(CXX)

# Generate compilation dependencies from SRCS. We will still rely on implicit
# rules for the compilations.
ifneq "" "$(filter-out clean print-%,$(MAKECMDGOALS))"
    $(foreach s, \
              $(wildcard $(SRCS)), \
              $(eval $(shell $(CXX) -MMD -E $(CPPFLAGS) $(CXXFLAGS) $(s))))
endif

# Standard cleanup target
.PHONY : clean
clean :
	rm -f -- $(wildcard *.d *.o *.exe $(.DEFAULT_GOAL))
