# Oliver Kullmann, 21.2.2003

programs = Test TestDebug

object_files = Test.o TestDebug.o
definition_files = Test.d TestDebug.d

Bibliothek = $(BIBLIOTHEK)
Boost = $(BOOST)

MainDirectory = $(SATALGORITHMEN)/OKlibrary

Warnings = -w

Debug = 

VPATH = $(Bibliothek) $(Boost)

all : $(programs)


# Test ---------------------------

Test : Test.o
	g++ -o Test -O3 Test.o

Test.o : Test.d

Test.d : Test.cpp
	g++ -MM -MG Test.cpp > Test.d; echo "	g++ -O3 -c $(Warnings) $(Debug) -I$(Bibliothek) -I$(Boost) Test.cpp" >> Test.d

include Test.d

TestDebug : TestDebug.o
	g++ -o TestDebug TestDebug.o

TestDebug.o : TestDebug.d

TestDebug.d : TestDebug.cpp
	g++ -MM -MG TestDebug.cpp > TestDebug.d; echo "	g++ -c $(Warnings) $(Debug) -I$(Bibliothek) -I$(Boost) TestDebug.cpp" >> TestDebug.d

include TestDebug.d



# --------------------------------

.PHONY : clean

clean :
	rm -f $(object_files) $(definition_files)
