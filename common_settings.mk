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
          $(if \
              $(BOOST_LIBS), \
              $(foreach d,$(BOOST_DIR)/stage/lib,-L$(d) -Wl,-R -Wl,$(d)) \
          ) \
          $(foreach \
              d, \
              $(HUNSPELL_DIR)/src/hunspell/.libs, \
              -L$(d) -Wl,-R -Wl,$(d) \
          )
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

SRCS_FULL_PATHS := $(foreach \
    s, \
    $(SRCS), \
    $(wildcard ./$(s) $(foreach v,$(VPATH),$(v)/$(s))) \
)

ifeq "" "$(filter clean print-%,$(MAKECMDGOALS))"
    # Generate compilation dependencies from SRCS. We will still rely on
    # implicit rules for the compilations.
    $(foreach \
        s, \
        $(SRCS_FULL_PATHS), \
        $(eval $(shell \
            $(CXX) -MMD -E $(CPPFLAGS) $(CXXFLAGS) -Wno-deprecated $(s) \
        )) \
    )

    IS_WINDOWS_PLATFORM := \
        $(filter CYGWIN_% MSYS_% MINGW% windows%,$(strip $(shell uname -s)))

    # Boost specifies Windows system library dependencies using the Visual C++'s
    # "#pragma comment(lib, ...)" feature, which GCC doesn't implement. So we
    # masquerade as MSVC to extract all such directives and incorporate them.
    WINDOWS_LIBS := $(if $(IS_WINDOWS_PLATFORM), \
        $(foreach s,$(SRCS_FULL_PATHS),$(strip $(shell \
            $(CXX) -E $(CPPFLAGS) $(CXXFLAGS) -D_MSC_VER $(s) -Wno-deprecated \
            | grep -E "\# *pragma +comment.*lib" \
            | cut -f 2 -d \" \
        ))) \
    )
    LDLIBS += $(foreach lib,$(sort $(WINDOWS_LIBS)),-l$(basename $(lib)))

    # GCC creates executables with .exe file extension on Windows platforms. GNU
    # Make's implicit rules, however, expect an extension-less program name. So
    # each time we run Make on Windows, it can't find the final target and so
    # re-runs the link recipe unnecessarily. Therefore, we create an extra rule
	# to produce a hardlink of the .exe file as the extension-less file.
    define recipe_for_program_hardlink_without_exe_extension
        EXECUTABLE_NAME := $(.DEFAULT_GOAL)
        .PHONY : create_hardlink_without_exe_extension
        create_hardlink_without_exe_extension : $(EXECUTABLE_NAME)
	        ln -f $(EXECUTABLE_NAME).exe $(EXECUTABLE_NAME) || true

        .DEFAULT_GOAL = create_hardlink_without_exe_extension
    endef
    $(if $(IS_WINDOWS_PLATFORM), \
        $(eval $(value recipe_for_program_hardlink_without_exe_extension)) \
    )
endif

# Standard cleanup target
.PHONY : clean
clean :
	rm -f -- $(SRCS_FULL_PATHS:.cpp=.d) $(SRCS_FULL_PATHS:.cpp=.o) \
        $(.DEFAULT_GOAL) $(.DEFAULT_GOAL).exe
