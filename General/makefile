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

source_libraries := $(Bibliothek) $(Xerces_h) $(Loki) $(Boost)
link_libraries := -lboost_date_time-gcc $(Xerces_so)

General_options :=
Optimisation_options := -O3
programs := TestBibliothek NumberTheory_Application_gcd Algebra_Applications_ModulareExponentiation TimeHandling_Applications_DaysDifference TimeHandling_Applications_DifferenceList
test_program := TestBibliothek

# ---------------------------------------------------

name_addition := $(strip $(Optimisation_options))
All_options = $(General_options) $(Optimisation_options)

programs_optimised := $(programs:=$(name_addition))
compilation_units := $(wildcard *.cpp)
definition_files := $(compilation_units:.cpp=.d)
object_files := $(compilation_units:.cpp=.o)
object_files_optimised := $(compilation_units:.cpp=$(name_addition).o)

.PHONY : unoptimised optimised

unoptimised : $(object_files) $(programs)

optimised : $(object_files_optimised) $(programs_optimised)

test : $(test_program)
	./$(test_program)
	touch test
testop : $(test_program)$(name_addition)
	./$(test_program)$(name_addition)
	touch testop


include $(definition_files)

%.d : %.cpp
	g++ -MM $(source_libraries) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@.$$$$$$; \
	sed 's,\($*\)\.o \(.*\)\.d [ :]*,\1.o \1$(name_addition).o \2.d : ,g' < $@.$$$$$$ > $@; \
	rm -f $@.$$$$ $@.$$$$$$

%$(name_addition).o : %.cpp
	$(CXX) -c -o $@ $(Optimisation_options) $(source_libraries) $<
%.o : %.cpp
	$(CXX) -c -o $@ $(General_options) $(source_libraries) $<

%$(name_addition) : %$(name_addition).o
	$(CXX) -o $@ $(Optimisation_options) $< $(link_libraries)
% : %.o
	$(CXX) -o $@ $(General_options) $< $(link_libraries)




# --------------------------------

.PHONY : clean cleanobj cleandef cleanall

cleanobj :
	rm -f $(object_files) $(object_files_optimised)

cleandef :
	rm -f $(definition_files)

clean : cleanobj cleandef
	rm -f test testop

cleanall : clean
	rm -f $(programs) $(programs_optimised)
