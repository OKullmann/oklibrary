# Oliver Kullmann. 6.10.2002 (Swansea)

Test : Test.cpp LinInequal.hpp FaronsPuzzle.hpp
	g++ -o "Test" -I$(BIBLIOTHEK) -I$(BOOST) Test.cpp

Filter : Filter.cpp
	g++ -o "Filter" -I$(BIBLIOTHEK) Filter.cpp

VanderWaerden : VanderWaerden.hpp VanderWaerden.cpp
	g++ -o "VanderWaerden" -I$(BIBLIOTHEK) -I$(BOOST) VanderWaerden.cpp

Ramsey : Ramsey.hpp Ramsey.cpp
	g++ -o "Ramsey" -I$(BIBLIOTHEK) -I$(BOOST) Ramsey.cpp

Test_Transformations : Test_Transformations.cpp Ramsey.hpp Tests_Ramsey.hpp
	g++ -o "Test_Transformations" -I$(BIBLIOTHEK) -I$(BOOST) Test_Transformations.cpp
