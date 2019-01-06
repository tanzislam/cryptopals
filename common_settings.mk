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
this_plugin := $(lastword $(MAKEFILE_LIST))
.DELETE_ON_ERROR :
CXX = g++
CPP_STANDARD = -std=c++11 -pthread
CPP_QUALITY_CHECKS = -pedantic -pedantic-errors -Wall -Wextra
CPP_OPTIMIZATIONS = -O3 -ffunction-sections -fdata-sections
CPPFLAGS = $(CPP_STANDARD) $(CPP_QUALITY_CHECKS) $(CPP_OPTIMIZATIONS)
CXXFLAGS := -isystem $(BOOST_DIR) \
            -isystem $(GTEST_DIR)/googletest/include \
            -isystem $(HUNSPELL_DIR)/src/hunspell \
            -isystem $(CRYPTOPP_DIR)
LIB_DIRS = $(if $(BOOST_LIBS),$(BOOST_DIR)/stage/lib) \
           $(GTEST_DIR)/googletest/make \
           $(HUNSPELL_DIR)/src/hunspell/.libs \
           $(CRYPTOPP_DIR)
GNU_LD := $(shell ld -v 2>&1 | grep GNU)
LDFLAGS = -pthread -Wl,$(if $(GNU_LD),--gc-sections,-dead_strip) \
          $(foreach d,$(LIB_DIRS),-L$(d))
LDLIBS = -lgtest_main \
         $(foreach lib,$(BOOST_LIBS),-lboost_$(lib)) \
         $(foreach lib,$(LIBS),-l$(lib))

# Adapt VPATH for out-of-source builds, i.e. "make -f .../path/to/GNUmakefile".
# Also allow only source files, not build products, to be directory-searched.
caller_makefile := $(lastword $(filter-out $(this_plugin),$(MAKEFILE_LIST)))
VPATH := $(addprefix $(dir $(caller_makefile)),$(VPATH) .)
vpath %.h $(VPATH)
vpath %.hpp $(VPATH)
vpath %.c $(VPATH)
vpath %.cc $(VPATH)
vpath %.cxx $(VPATH)
vpath %.cpp $(VPATH)

# This is so that a Challenge-specific makefile can "include" the directory of
# another Challenge using VPATH for both dependencies and C++ headers.
#
# Precondition: VPATH must be space-delimited, and not colon- or
#               semicolon-delimited.
CXXFLAGS += $(addprefix -I,$(VPATH))
VPATH :=

# The built-in rule for linking object files uses the $(CC) variable, which
# expands to "cc". However, the "cc" program does not exist in a standard
# MinGW-w64 installation, so using $(CXX) instead as defined above.
CC = $(CXX)

PLATFORM := $(strip $(shell uname -s))
IS_WINDOWS_PLATFORM := $(filter CYGWIN_% MSYS_% MINGW% windows%,$(PLATFORM))
IS_MACOSX_PLATFORM := $(filter Darwin,$(PLATFORM))
include $(dir $(this_plugin))dependency_cache.mk

LDFLAGS += $(if $(IS_MACOSX_PLATFORM),, \
                $(foreach d,$(LIB_DIRS),-Wl,-rpath,$(d)))

ifeq "" "$(filter clean print-%,$(MAKECMDGOALS))"
    $(SRCS:.cpp=.o) : $(this_plugin) $(dir $(this_plugin))dependency_cache.mk

    # Git doesn't know how to ignore extension-less binaries, so we must teach
    # it. No need to check this in, though -- it will be created each time.
    EXECUTABLE_NAME := $(.DEFAULT_GOAL)
    $(file >> .gitignore,.gitignore)
    $(file >> .gitignore,$(EXECUTABLE_NAME))

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
        .PHONY : create_hardlink_without_exe_extension
        create_hardlink_without_exe_extension : $(EXECUTABLE_NAME)
	        mv $(EXECUTABLE_NAME).exe $(EXECUTABLE_NAME).sav
	        rm -f $(EXECUTABLE_NAME)
	        mv $(EXECUTABLE_NAME).sav $(EXECUTABLE_NAME)
	        ln -f $(EXECUTABLE_NAME) $(EXECUTABLE_NAME).exe || true

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
	        rm -f ./*.dll || true
	        ln -f $(wildcard $(foreach d,$(LIB_DIRS),$(d)/*.dll)) .
	        echo *.dll >> .gitignore

        .DEFAULT_GOAL = dll_hardlinks_to_simulate_rpath
    endef
    $(if $(IS_WINDOWS_PLATFORM), \
        $(eval $(value post_link_actions_for_windows)) \
    )

    # https://sourceforge.net/p/mingw-w64/wiki2/gnu%20printf/
    CPPFLAGS += $(if $(IS_WINDOWS_PLATFORM),-D__USE_MINGW_ANSI_STDIO=1)

    space :=
    space +=
    relpath = $(if $(filter-out /%,$(1)),$(1),$(strip \
              $(subst $(space),/,$(patsubst %,..,$(subst /, ,$(CURDIR))))$(1)))

    define post_link_actions_for_darwin =
        # Darwin (macOS) has support for dynamic library (dylib) search path
        # embedded inside the executable, but it takes effect only if the
        # "install name" of the dylib starts with "@rpath". Prior to OSX 10.5 we
        # can use only "@executable_path", though. The "install name" is copied
        # over from the dylib when linking the executable, and typically doesn't
        # contain these tokens. We fix it here.
        #
        # http://jorgen.tjer.no/post/2014/05/20/dt-rpath-ld-and-at-rpath-dyld/
        # https://wincent.com/wiki/@executable_path,_@load_path_and_@rpath
        .PHONY : fix_relative_paths_to_dylibs
        fix_relative_paths_to_dylibs : $(EXECUTABLE_NAME)
	       otool -L $(EXECUTABLE_NAME) \
               | tail -n +2 \
               | cut -d' ' -f1 \
               | grep -F $(foreach lib,$(BOOST_LIBS),-e libboost_$(lib)) \
                         $(foreach lib,$(LIBS),-e lib$(lib)) \
               | xargs -L1 -J% sh -c '\
                   install_name_tool \
                       -change "$$0" \
                       @executable_path/$$(\
                           ls $(foreach d,$(LIB_DIRS),$(call relpath,$(d))/` \
                               basename "$$0"`) 2>/dev/null) \
                       $(EXECUTABLE_NAME) </dev/null' %

        .DEFAULT_GOAL = fix_relative_paths_to_dylibs
    endef
    $(if $(and $(BOOST_LIBS)$(LIBS),$(IS_MACOSX_PLATFORM)), \
        $(eval $(value post_link_actions_for_darwin)) \
    )
endif

# Standard cleanup target
.PHONY : clean
clean :
	rm -f $(wildcard $(foreach s,$(SRCS),$(s:.cpp=.d) $(s:.cpp=.o)) \
                     $(.DEFAULT_GOAL) $(.DEFAULT_GOAL).exe *.dll)
	grep -n "^.gitignore" .gitignore \
        | cut -d: -f1 \
        | head -n 1 \
        | xargs -I "{}" head -n "{}" .gitignore \
        > .gitignore.sav
	mv .gitignore.sav .gitignore
	$(if $(filter .gitignore,$(firstword $(file < .gitignore))),rm .gitignore)
