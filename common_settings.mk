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
    else ifeq "" "$(CRYPTOPP_DIR)"
        $(error The CRYPTOPP_DIR variable must be set to where you downloaded \
                or cloned Crypto++)
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
           -isystem $(HUNSPELL_DIR)/src/hunspell \
           -isystem $(CRYPTOPP_DIR)
LIB_DIRS = $(if $(BOOST_LIBS),$(BOOST_DIR)/stage/lib) \
           $(HUNSPELL_DIR)/src/hunspell/.libs \
           $(CRYPTOPP_DIR)
LDFLAGS = -pthread -Wl,--gc-sections $(foreach d,$(LIB_DIRS),-L$(d) -Wl,-R,$(d))
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
        $(info Preprocessing $(s) for compilation dependencies)$(eval $(shell \
            $(CXX) -MM $(CPP_STANDARD) $(CXXFLAGS) $(s) | tr -d "\\\\" \
        )) \
    )
    $(SRCS:.cpp=.o) : $(lastword $(MAKEFILE_LIST))

    IS_WINDOWS_PLATFORM := \
        $(filter CYGWIN_% MSYS_% MINGW% windows%,$(strip \
                $(info Detecting platform)$(shell uname -s)))

    # Boost specifies Windows system library dependencies using the Visual C++
    # "#pragma comment(lib, ...)" feature, which GCC doesn't implement. So we
    # masquerade as MSVC to extract all such directives and incorporate them. We
    # also specify GCCXML's flag to avoid problematic MSVC-specific code paths.
    WINDOWS_LIBS := $(if $(IS_WINDOWS_PLATFORM), \
        $(foreach s,$(SRCS_FULL_PATHS),$(strip \
            $(info Preprocessing $(s) for library dependencies)$(shell \
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

    define post_link_actions_for_windows =
        # GCC creates executables with .exe file extension on Windows platforms.
        # GNU Make's implicit rules, however, expect an extension-less program
        # name. So each time we run Make on Windows, it can't find the final
        # target and so re-runs the link recipe unnecessarily. To fix this we
        # produce a hardlink of the .exe file as the extension-less file.
        #
        # NOTE: MSYS utilities can auto-check for <file>.exe when provided
        #       <file>, but not vice-versa. So we first rename the .exe to .sav,
        #       to prevent it from being considered at deletion or hardlink-time
        #       existence-check.
        EXECUTABLE_NAME := $(.DEFAULT_GOAL)
        .PHONY : create_hardlink_without_exe_extension
        create_hardlink_without_exe_extension : $(EXECUTABLE_NAME)
	        @mv -- $(EXECUTABLE_NAME).exe $(EXECUTABLE_NAME).sav
	        @rm -f -- $(EXECUTABLE_NAME)
	        @mv -- $(EXECUTABLE_NAME).sav $(EXECUTABLE_NAME)
	        @ln -f $(EXECUTABLE_NAME) $(EXECUTABLE_NAME).exe || true

        # Windows's PE loader does not support a DLL search path embedded inside
        # the EXE file, without running code like AddDllDirectory() or
        # SetDefaultDllDirectories() beforehand. See
        # https://msdn.microsoft.com/en-us/library/ms682586(VS.85).aspx for
        # details.
        #
        # We work around this limitation by creating a hardlink of any DLLs from
        # the library search path into the directory of the EXE file.
        .PHONY : dll_hardlinks_to_simulate_rpath
        dll_hardlinks_to_simulate_rpath : create_hardlink_without_exe_extension
	        @ln -f $(wildcard $(foreach d,$(LIB_DIRS),$(d)/*.dll)) .
	        @echo *.dll >> .gitignore

        .DEFAULT_GOAL = dll_hardlinks_to_simulate_rpath
    endef
    $(if $(IS_WINDOWS_PLATFORM), \
        $(eval $(value post_link_actions_for_windows)) \
    )

    # https://sourceforge.net/p/mingw-w64/wiki2/gnu%20printf/
    CPPFLAGS += $(if $(IS_WINDOWS_PLATFORM),-D__USE_MINGW_ANSI_STDIO=1)
endif

# Standard cleanup target
.PHONY : clean
clean :
	rm -f -- $(wildcard $(foreach s,$(SRCS),$(s:.cpp=.d) $(s:.cpp=.o)) \
                        $(.DEFAULT_GOAL) $(.DEFAULT_GOAL).exe *.dll \
                        .gitignore)
