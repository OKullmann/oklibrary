# Oliver Kullmann, 6.3.2002 (Swansea)

# Usage:

# make
# creates all definition files, object files and program files;

# make optimised
# creates the same (names of object and program files modified), but using optimisation options;

# make file.d, file.o, program 
# creates the respective file;

# make test
# recompiles the test program if necessary, and runs the test if necessary.

# make testop
# recompiles the test program with optimisation if necessary, and runs the test if necessary.

# ---------------------------------------------------

SHELL = /bin/sh
.SUFFIXES :

#ifdef BIBLIOTHEK
#  Bibliothek := -I$(BIBLIOTHEK)
#else
#  Bibliothek :=
#endif
# ToDo: needed

ifdef OKLIB_ROOT
  OKlib_root := -I$(OKLIB_ROOT)
else
  OKlib_root :=
endif
# ToDo: not yet

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

boost_filesystem := -lboost_filesystem-gcc
boost_date_time := -lboost_date_time-gcc

srcdir := $(wildcard .)

source_libraries := -I$(srcdir) $(OKlib_root) $(Bibliothek) $(Xerces_h) $(Loki) $(Boost)
link_libraries := $(boost_date_time) $(boost_filesystem)

# We should not fix root in this way. 
Root := $(wildcard ../../..)
prefix := $(Root)
# ToDo: we seem to need it

exec_prefix := $(prefix)
bindir := $(exec_prefix)/bin
libdir := $(exec_prefix)/lib

aux_dir := $(prefix)/aux

Directories := $(bindir) $(libdir) $(aux_dir)

General_options :=
Optimisation_options := -O3

programs := Backup
programs := $(addprefix $(bindir)/, $(programs))

#test_program := TestBibliothek
#test_program := $(addprefix $(bindir)/, $(test_program))
# ToDo: test needed

# ---------------------------------------------------

name_addition := $(strip $(Optimisation_options))
All_options = $(General_options) $(Optimisation_options)

programs_optimised := $(programs:=$(name_addition))

compilation_units := $(wildcard $(srcdir)/*.cpp)
compilation_units := $(notdir $(compilation_units))

dependency_files := $(compilation_units:.cpp=.d)
dependency_files := $(addprefix $(aux_dir)/, $(dependency_files))

object_files := $(compilation_units:.cpp=.o)
object_files := $(addprefix $(libdir)/, $(object_files))
object_files_optimised := $(compilation_units:.cpp=$(name_addition).o)
object_files_optimised := $(addprefix $(libdir)/, $(object_files_optimised))

.PHONY : unoptimised optimised

unoptimised : $(Directories) $(object_files) $(programs)

optimised : $(Directories) $(object_files_optimised) $(programs_optimised)

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

$(dependency_files) : $(aux_dir)/%.d : $(srcdir)/%.cpp
	g++ -MM $(source_libraries) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@.$$$$$$; \
	sed 's,\($*\)\.o \(.*\)\.d [ :]*,\1.o \1$(name_addition).o \2.d : ,g' < $@.$$$$$$ > $@; \
	rm -f $@.$$$$ $@.$$$$$$

$(object_files_optimised) : $(libdir)/%$(name_addition).o : $(srcdir)/%.cpp
	$(CXX) -c -o $@ $(Optimisation_options) $(source_libraries) $<
$(object_files) : $(libdir)/%.o : $(srcdir)/%.cpp
	$(CXX) -c -o $@ $(General_options) $(source_libraries) $<

$(programs) : $(bindir)/% : $(libdir)/%.o
	$(CXX) -o $@ $(General_options) $< $(link_libraries)
$(programs_optimised) : $(bindir)/% : $(libdir)/%.o
	$(CXX) -o $@ $(Optimisation_options) $< $(link_libraries)



# --------------------------------

.PHONY : clean cleanobj cleandef cleanall

Remove := rm -v -f

cleanobj :
	$(Remove) $(object_files) $(object_files_optimised)

cleandef :
	$(Remove) $(dependency_files)

clean : cleanobj cleandef
	$(Remove) test testop

cleanall : clean
	$(Remove) $(programs) $(programs_optimised)
