# Oliver Kullmann, 16.6.2003

programs = Test TestDebug

object_files = Test.o TestDebug.o
definition_files = Test.d TestDebug.d

Bibliothek = $(BIBLIOTHEK)
Boost = $(BOOST)

VPATH = $(Bibliothek) $(Boost)

all : $(programs)


# Test ---------------------------

TestDebug : TestDebug.o
	g++ -o TestDebug TestDebug.o

TestDebug.o : TestDebug.d

TestDebug.d : TestDebug.cpp
	g++ -MM -MG TestDebug.cpp > TestDebug.d; echo "	g++ -c -I$(Bibliothek) -I$(Boost) TestDebug.cpp" >> TestDebug.d

include TestDebug.d


# Transfer -----------------------------------------------------

Transfer :
	mv *.hpp Test.cpp makefile Graphen


.PHONY : clean

clean :
	rm -f $(object_files) $(definition_files)
