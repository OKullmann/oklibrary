Program := Backup
ObjectFiles := Backup.o
Headers := Path_Loader.hpp Info_Holder.hpp Command_Caller.hpp Backup.hpp
Sources := Backup.cpp

Remove := rm -v -f

boost_filesystem := -lboost_filesystem-gcc-1_32
boost_date_time := -lboost_date_time-gcc

ifdef BOOST
  boost := -I$(BOOST)
else
  boost :=
endif

ifdef BIBLIOTHEK
  bibliothek := -I$(BIBLIOTHEK)
else
  bibliothek :=
endif

$(Program) : $(ObjectFiles)
	g++ -o $(Program) $(ObjectFiles) ${boost_filesystem} $(boost_date_time)

$(ObjectFiles) : $(Sources) $(Headers)
	g++ -c $(boost) $(bibliothek) $(Sources)

.PHONY : clean cleanall

clean :
	${Remove} ${ObjectFiles} 

cleanall: clean
	${Remove} ${Program}

