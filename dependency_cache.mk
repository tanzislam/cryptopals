# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

DEPDIR := deps
DEPEXT := dep
$(shell env mkdir -p $(DEPDIR))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.T$(DEPEXT)

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.T$(DEPEXT) $(DEPDIR)/$*.$(DEPEXT) && touch $@

%.o : %.c
%.o : %.c $(DEPDIR)/%.$(DEPEXT)
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cc
%.o : %.cc $(DEPDIR)/%.$(DEPEXT)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cxx
%.o : %.cxx $(DEPDIR)/%.$(DEPEXT)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.$(DEPEXT)
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.$(DEPEXT) : ;
.PRECIOUS : $(DEPDIR)/%.$(DEPEXT)

-include $(patsubst %,$(DEPDIR)/%.$(DEPEXT),$(basename $(SRCS)))

$(if $(filter undefined,$(origin IS_WINDOWS_PLATFORM)), \
     $(error This plugin must be included after IS_WINDOWS_PLATFORM is defined))
ifneq "" "$(IS_WINDOWS_PLATFORM)"
    # Boost specifies Windows system library dependencies using the Visual C++
    # "#pragma comment(lib, ...)" feature, which GCC doesn't implement. So we
    # masquerade as MSVC to extract all such directives and incorporate them. We
    # also specify GCCXML's flag to avoid problematic MSVC-specific code paths.
    LDEPEXT := ldep
    $(DEPDIR)/%.$(LDEPEXT) : %.cpp
	    $(file > $(DEPDIR)/$*.T$(LDEPEXT),$(shell \
            $(info Preprocessing $< for library dependencies) \
            $(CXX) $(CPP_STANDARD) $(CXXFLAGS) \
                -E -w -D_MSC_VER -D__GCCXML__ $< \
            | grep -E "\# *pragma +comment.*lib" \
            | cut -f 2 -d \" \
        ))
	    mv -f $(DEPDIR)/$*.T$(LDEPEXT) $(DEPDIR)/$*.$(LDEPEXT)

    .PHONY : read_windows_libs
    read_windows_libs : $(patsubst %,$(DEPDIR)/%.$(LDEPEXT),$(basename $(SRCS)))
	    $(eval LDLIBS += \
               $(foreach lib,$(sort $(shell cat $^)),-l$(basename $(lib))))

    $(.DEFAULT_GOAL) : | read_windows_libs
endif

clean : clean_deps
.PHONY : clean_deps
clean_deps :
	rm -f $(wildcard $(DEPDIR)/*.$(DEPEXT) $(DEPDIR)/*.$(LDEPEXT))
