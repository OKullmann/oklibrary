# LICENCE

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


source_libraries := $(Bibliothek) $(Loki) $(Boost)
link_libraries :=

General_options := -g
Optimisation_options :=
programs := Test

compilation_units := $(wildcard *.cpp)
definition_files := $(compilation_units:.cpp=.d)
object_files := $(compilation_units:.cpp=.o)


all : $(object_files) $(programs)


include $(definition_files)

%.d : %.cpp
	g++ -MM $(source_libraries) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o : %.cpp
	$(CXX) -c -o $@ $(General_options) $(Optimisation_options) $(source_libraries) $<

% : %.o
	$(CXX) -o $@ $(General_options) $(Optimisation_options) $< $(link_libraries)


# --------------------------------

.PHONY : clean cleanobj cleandef cleanall

cleanobj :
	rm -f $(object_files)

cleandef :
	rm -f $(definition_files)

clean : cleanobj cleandef
	rm -f test testop

cleanall : clean
	rm -f $(programs)
