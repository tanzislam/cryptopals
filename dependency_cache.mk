# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

DEPDIR := deps
DEPEXT := dep
$(shell env mkdir -p $(DEPDIR))
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.T$(DEPEXT)

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.T$(DEPEXT) $(DEPDIR)/$*.$(DEPEXT)

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

clean : clean_deps
.PHONY : clean_deps
clean_deps :
	rm -f $(DEPDIR)/*.$(DEPEXT)
