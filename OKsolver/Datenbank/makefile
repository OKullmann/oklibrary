Bibliothek = $(BIBLIOTHEK)

Special_update : Special_update.cpp $(Bibliothek)/DatabaseHandler01.hpp S$(Bibliothek)/tringHandling.hpp
	g++ -o Special_update -O3 -I$(Bibliothek) -I/usr/include/pgsql Special_update.cpp -lpq

Delete_rows : Delete_rows.cpp $(Bibliothek)/DatabaseHandler01.hpp $(Bibliothek)/StringHandling.hpp
	g++ -o Delete_rows -O3 -I$(Bibliothek) -I/usr/include/pgsql Delete_rows.cpp -lpq

CreateStatistic : CreateStatistic.cpp $(Bibliothek)/DatabaseHandler01.hpp $(Bibliothek)/StringHandling.hpp $(Bibliothek)/TimeHandling.hpp
	g++ -o CreateStatistic -O3 -I$(Bibliothek) -I$(BOOST) -I/usr/include/pgsql CreateStatistic.cpp -lpq
