# Oliver Kullmann, 25.2.2002 (Swansea)

DLL-Implementations : DLL-Implementations.o DLL_Algorithms.o Kommandozeile.o
	g++ -o DLL-Implementations -O3 DLL-Implementations.o DLL_Algorithms.o

DLL-Implementations.o : DLL-Implementations.cpp VarSet.hpp Literal.hpp Clause.hpp ClauseSet.hpp PartAssign.hpp DLL_Algorithms.hpp Kommandozeile.hpp
	g++ -c -O3 DLL-Implementations.cpp

Kommandozeile.o : Kommandozeile.hpp Kommandozeile.cpp
	g++ -c -O3 Kommandozeile.cpp

DLL_Algorithms.o : VarSet.hpp Literal.hpp Clause.hpp ClauseSet.hpp PartAssign.hpp DLL_Algorithms.hpp DLL_Algorithms.cpp
	g++ -c -O3 DLL_Algorithms.cpp

clean :
	rm DLL-Implementations.o DLL_Algorithms.o

