this_makefile_dir := $(dir $(lastword $(MAKEFILE_LIST)))
mkfiles := $(wildcard $(this_makefile_dir)*/*/GNUmakefile)
VPATH := $(patsubst $(this_makefile_dir)%,%,$(sort $(dir $(mkfiles))))

# '%' is a metacharacter in Windows batch files (which GNU Make uses when
# launching processes under cmd.exe), so we escape it and later correct it.
SRCS := $(sort $(foreach \
    mkfile, \
    $(mkfiles), \
    $(shell \
        echo 'print-%%:;@echo $$*=$$($$*)' \
        | tr -d "'" \
        | tr -s "%" \
        | $(MAKE) -C $(dir $(mkfile)) -f - -f $(notdir $(mkfile)) print-SRCS \
        | grep SRCS= \
        | cut -d= -f2- \
    ) \
))

BOOST_LIBS = system
LIBS = cryptopp hunspell-1.6

# The implicit rule for linking is used only if the target name is the same as
# that of any constituent .o file. We abide by this here and later create a hard
# link to our final desired name.
TEMP_TARGET := $(notdir $(basename $(firstword $(filter test_%.cpp,$(SRCS)))))
$(TEMP_TARGET) : $(SRCS:.cpp=.o)
include $(this_makefile_dir)common_settings.mk

# The "clean" recipe in common_settings.mk relies on the eventual value of
# .DEFAULT_GOAL, so we avoid overwriting it if the user ran the "clean" target.
ifeq "" "$(filter clean,$(MAKECMDGOALS))"
test : $(.DEFAULT_GOAL)
	$(file >> .gitignore,$@)
	ln -f $(TEMP_TARGET) $@
.DEFAULT_GOAL = test
else
.PHONY clean : clean-test
clean-test :
	rm -f test
endif
