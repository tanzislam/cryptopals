# Validate that we have everything we need
ifeq "" "$(SRCS)"
    $(error SRCS must be defined before including this file)
else ifeq "" "$(filter clean print-%,$(MAKECMDGOALS))"
    ifeq "" "$(BOOST_DIR)"
        $(error The BOOST_DIR variable must be set to where you installed Boost)
    else ifeq "" "$(GTEST_DIR)"
        $(error The GTEST_DIR variable must be set to where you downloaded or \
                cloned Google Test and Google Mock)
    else ifeq "" "$(HUNSPELL_DIR)"
        $(error The HUNSPELL_DIR variable must be set to where you downloaded \
                or cloned Hunspell)
    endif
endif

# Standard settings
CXX = g++
CPP_STANDARD = -std=c++14
CPP_QUALITY_CHECKS = -Wc++11-compat -Wc++14-compat -pedantic -Wall -Wextra
CPP_OPTIMIZATIONS = -O3 -ffunction-sections -fdata-sections
CPPFLAGS = $(CPP_STANDARD) $(CPP_QUALITY_CHECKS) $(CPP_OPTIMIZATIONS)
CXXFLAGS = -isystem $(BOOST_DIR) \
           -isystem $(GTEST_DIR)/googletest/include \
           -isystem $(HUNSPELL_DIR)/src/hunspell
LDFLAGS = -pthread -Wl,--gc-sections \
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
    #
    # NOTE: GCC emits backslash-newline as line-continuation, but GNU Make's
    #       $(shell) function replaces the newline with space -- but retains the
    #       backslash. We need to remove the backslash ourselves, to prevent it
    #       from escaping the whitespace after it.
    $(foreach \
        s, \
        $(SRCS_FULL_PATHS), \
        $(eval $(shell \
            $(CXX) -MM $(CPP_STANDARD) $(CXXFLAGS) $(s) | tr -d "\\\\" \
        )) \
    )
    $(SRCS:.cpp=.o) : $(lastword $(MAKEFILE_LIST))

    IS_WINDOWS_PLATFORM := \
        $(filter CYGWIN_% MSYS_% MINGW% windows%,$(strip $(shell uname -s)))

    # Boost specifies Windows system library dependencies using the Visual C++
    # "#pragma comment(lib, ...)" feature, which GCC doesn't implement. So we
    # masquerade as MSVC to extract all such directives and incorporate them. We
    # also specify GCCXML's flag to avoid problematic MSVC-specific code paths.
    WINDOWS_LIBS := $(if $(IS_WINDOWS_PLATFORM), \
        $(foreach s,$(SRCS_FULL_PATHS),$(strip $(shell \
            $(CXX) $(CPP_STANDARD) $(CXXFLAGS) \
                -E -w -D_MSC_VER -D__GCCXML__ $(s) \
            | grep -E "\# *pragma +comment.*lib" \
            | cut -f 2 -d \" \
        ))) \
    )
    LDLIBS += $(foreach lib,$(sort $(WINDOWS_LIBS)),-l$(basename $(lib)))

    # Git doesn't know how to ignore extension-less binaries, so we must teach
    # it. No need to check this in, though -- it will be created each time.
    $(file > .gitignore,.gitignore)
    $(file >> .gitignore,$(.DEFAULT_GOAL))

    # GCC creates executables with .exe file extension on Windows platforms. GNU
    # Make's implicit rules, however, expect an extension-less program name. So
    # each time we run Make on Windows, it can't find the final target and so
    # re-runs the link recipe unnecessarily. To fix this we create an extra rule
    # to produce a hardlink of the .exe file as the extension-less file.
    #
    # NOTE: MSYS utilities can auto-check for <file>.exe when provided <file>,
    #       but not vice-versa. So we first rename the .exe to .sav, to prevent
    #       it from considered for deletion or hardlink-time existence-check.
    define recipe_for_program_hardlink_without_exe_extension =
        EXECUTABLE_NAME := $(.DEFAULT_GOAL)
        .PHONY : create_hardlink_without_exe_extension
        create_hardlink_without_exe_extension : $(EXECUTABLE_NAME)
	        @mv -- $(EXECUTABLE_NAME).exe $(EXECUTABLE_NAME).sav
	        @rm -f -- $(EXECUTABLE_NAME)
	        @mv -- $(EXECUTABLE_NAME).sav $(EXECUTABLE_NAME)
	        @ln -f $(EXECUTABLE_NAME) $(EXECUTABLE_NAME).exe || true

        .DEFAULT_GOAL = create_hardlink_without_exe_extension
    endef
    $(if $(IS_WINDOWS_PLATFORM), \
        $(eval $(value recipe_for_program_hardlink_without_exe_extension)) \
    )
endif

# Standard cleanup target
.PHONY : clean
clean :
	rm -f -- $(wildcard *.d *.o *.exe $(.DEFAULT_GOAL) .gitignore)
