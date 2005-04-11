# Oliver Kullmann, 6.3.2002 (Swansea)

# Usage:

# make all
# = make unoptimsed + make optimised

# make unoptimised
# creates all dependencies files, object files, header files and program files;

# make optimised
# creates the same (names of object and program files modified), but using optimisation options;

# make file.d, file.o, file.hpp, program 
# creates the respective file;

# make check
# = make test + make testop

# make test
# recompiles the test program if necessary, and runs the test if necessary.

# make testop
# recompiles the test program with optimisation if necessary, and runs the test if necessary.

# ---------------------------------------------------

SHELL = /bin/sh
.SUFFIXES :

ifdef BIBLIOTHEK
  Bibliothek := -I$(BIBLIOTHEK)
else
  Bibliothek :=
endif
ifdef LOKI
  Loki := -I$(LOKI)
else
  Loki :=
endif
ifdef BOOST
  Boost := -I$(BOOST)
else
  Boost :=
endif
ifdef XERCES_H
  Xerces_h := -I$(XERCES_H)
else
  Xerces_h :=
endif
ifdef XERCES_SO
  Xerces_so := -I$(XERCES_SO)
else
  Xerces_so :=
endif

srcdir := $(wildcard .)

source_libraries := -I$(srcdir) $(Bibliothek) $(Xerces_h) $(Loki) $(Boost)
link_libraries := -lboost_date_time-gcc $(Xerces_so)

Root := $(wildcard ../../..)
prefix := $(Root)

exec_prefix := $(prefix)
bindir := $(exec_prefix)/bin
libdir := $(exec_prefix)/lib

includedir := $(prefix)/include

aux_dir := $(prefix)/aux

Directories := $(bindir) $(libdir) $(includedir) $(aux_dir)

# -----------------------------------------------------------------------------------

General_options :=
Optimisation_options := -O3

programs := TestBibliothek NumberTheory_Application_gcd Algebra_Applications_ModulareExponentiation TimeHandling_Applications_DaysDifference TimeHandling_Applications_DifferenceList
programs := $(addprefix $(bindir)/, $(programs))

test_program := TestBibliothek
test_program := $(addprefix $(bindir)/, $(test_program))

# ---------------------------------------------------

name_addition := $(strip $(Optimisation_options))
All_options := $(General_options) $(Optimisation_options)

programs_optimised := $(programs:=$(name_addition))

header_files := $(wildcard $(srcdir)/*.hpp)
header_files := $(notdir $(header_files))
header_files :=  $(addprefix $(includedir)/, $(header_files))

compilation_units := $(wildcard $(srcdir)/*.cpp)
compilation_units := $(notdir $(compilation_units))

dependency_files := $(compilation_units:.cpp=.d)
dependency_files := $(addprefix $(aux_dir)/, $(dependency_files))

object_files := $(compilation_units:.cpp=.o)
object_files := $(addprefix $(libdir)/, $(object_files))
object_files_optimised := $(compilation_units:.cpp=$(name_addition).o)
object_files_optimised := $(addprefix $(libdir)/, $(object_files_optimised))

# ----------------------------------------------------------------

.PHONY : all unoptimised optimised check allandcheck

all : unoptimised optimised

allandcheck : all check

unoptimised : $(Directories) $(header_files) $(object_files) $(programs)

optimised : $(Directories) $(header_files) $(object_files_optimised) $(programs_optimised)

check : test testop

test : $(Directories) $(test_program)
	./$(test_program)
	touch test
testop : $(Directories) $(test_program)$(name_addition)
	./$(test_program)$(name_addition)
	touch testop

# ---------------------------------------------------------------

include $(dependency_files)

$(Directories) : % :
	mkdir $@

$(header_files) : $(includedir)/%.hpp : $(srcdir)/%.hpp
	cp $< $@

$(dependency_files) : $(aux_dir)/%.d : $(srcdir)/%.cpp
	g++ -MM $(source_libraries) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@.$$$$$$; \
	sed 's,\($*\)\.o \(.*\)\.d [ :]*,\1.o \1$(name_addition).o \2.d : ,g' < $@.$$$$$$ > $@; \
	rm -f $@.$$$$ $@.$$$$$$
#%.d : %.cpp
#	g++ -MM $(source_libraries) $< > $@.$$$$; \
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@.$$$$$$; \
#	sed 's,\($*\)\.o \(.*\)\.d [ :]*,\1.o \1$(name_addition).o \2.d : ,g' < $@.$$$$$$ > $@; \
#	rm -f $@.$$$$ $@.$$$$$$

$(object_files_optimised) : $(libdir)/%$(name_addition).o : $(srcdir)/%.cpp
	$(CXX) -c -o $@ $(Optimisation_options) $(source_libraries) $<
#%$(name_addition).o : %.cpp
#	$(CXX) -c -o $@ $(Optimisation_options) $(source_libraries) $<
$(object_files) : $(libdir)/%.o : $(srcdir)/%.cpp
	$(CXX) -c -o $@ $(General_options) $(source_libraries) $<
#%.o : %.cpp
#	$(CXX) -c -o $@ $(General_options) $(source_libraries) $<

$(programs) : $(bindir)/% : $(libdir)/%.o
	$(CXX) -o $@ $(General_options) $< $(link_libraries)
$(programs_optimised) : $(bindir)/% : $(libdir)/%.o
	$(CXX) -o $@ $(Optimisation_options) $< $(link_libraries)
#%$(name_addition) : %$(name_addition).o
#	$(CXX) -o $@ $(Optimisation_options) $< $(link_libraries)
#% : %.o
#	$(CXX) -o $@ $(General_options) $< $(link_libraries)


# --------------------------------

.PHONY : clean cleanobj cleandep cleanhead cleanall

cleanobj :
	rm -f $(object_files) $(object_files_optimised)

cleandep :
	rm -f $(dependency_files)

cleanhead :
	rm -f $(header_files)

clean : cleanobj cleandep cleanhead
	rm -f test testop

cleanall : clean
	rm -f $(programs) $(programs_optimised)
