## Constraints #########################################################################################

# Binaries directory.
BINDIR = ${HOMEDIR}/bin

# Shared objects included to main fradia application.
BASELIB = $(HOMEDIR)/Base/Base.so
COMMONLIB = $(HOMEDIR)/Common/Common.so

# Variable defining whether CUDA is installed, thus should be used, or not.
USE_CUDA = 0

# Install command.
INSTALL=-@ln -f

# Default command.
Default: all

## Compiler options ####################################################################################

# Compiler.
CXX = gcc -pthread

# Compiler flags.
CXXFLAGS = -openmp -g -O0 -msse2 `xml2-config --cflags` `pkg-config --cflags opencv gsl` `fltk-config --cxxflags --use-forms` -fpic -g #-Wall

# Linker flags.
LDFLAGS = -g -O0 `xml2-config --libs` `pkg-config --libs opencv gsl` `fltk-config --ldflags --use-forms` -rdynamic -lboost_thread-mt
# Add additional flags for ICC compiler.            
ifeq ($(CXX), icpc)
LDFLAGS += -lguide -lpthread
endif


# Include flags.
CPPFLAGS=-I$(HOMEDIR)/Base -I$(HOMEDIR)/Common -I$(HOMEDIR)/Core 


## Compilation scripts #################################################################################
# Directory with dependences.
DEPDIR = .deps
df = $(DEPDIR)/$(*F)
# Makedepend command.
#MAKEDEPEND = mkdir -p .deps; gcc -M $(CXXFLAGS) -o $(DEPDIR)/$*.d $<
MAKEDEPEND = mkdir -p .deps; gcc -M `xml2-config --cflags` `pkg-config --cflags opencv gsl` `fltk-config --cxxflags --use-forms` ${CPPFLAGS} -o $(DEPDIR)/$*.d $<

# Overwritten command for *.cpp compilation - also creates dependency files (*.P) in adequate directory.
%.o : %.cpp
	@$(MAKEDEPEND); \
	cp $(df).d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	rm -f $(df).d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

# Overwritten command for *.cc compilation - also creates dependency files (*.P) in adequate directory.
%.o : %.cc
	@$(MAKEDEPEND); \
	cp $(df).d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	rm -f $(df).d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

# Wildcards used during dependency include.
SOURCES_CC = ${wildcard *.cc}
SOURCES_CPP = ${wildcard *.cpp}
DEPENDS = ${patsubst %.cpp,$(DEPDIR)/%.P,${SOURCES_CPP}} ${patsubst %.cc,$(DEPDIR)/%.P,${SOURCES_CC}} 

# Include created dependency files.
-include $(DEPENDS)
