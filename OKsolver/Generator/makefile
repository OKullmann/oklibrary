# Oliver Kullmann, 23.1.2002 (Swansea)

core_programs = OKgenerator interprete_descriptor

# To compile just the core programs (without access to a database) 
# in the same directory as the source files:
# make Prefix="./" Bibliothek="XXX" CPPFLAGS="-DNODATABASE" Generator
# where XXX is the path to the auxiliary library.

# 26.6.2004: TODO -lpq problem

programs = $(core_programs) experiment_preparation process_experiment transfer_experiment

object_files = OKRandGen.o EinKlmGeneratorAES.o VEinKlmGeneratorAES.o LiteralGeneratorAES.o VLiteralGeneratorAES.o BlockChiffAES.o aes.o flint.o flintpp.o DeskriptorVerarbeitung.o ExperimentVorbereitung.o DeskVerarb.o ExperimentDurchfuehrung.o ExperimentUebertragung.o

Bibliothek = $(BIBLIOTHEK)
Boost = $(BOOST)
Loki = $(LOKI)

MainDirectory = $(SATALGORITHMEN)/FormelGen
Prefix = $(MainDirectory)/Executables/
cppc = g++

all : $(programs)

Generator : $(core_programs)

OKgenerator : $(Prefix)/OKgenerator

$(Prefix)/OKgenerator : $(Prefix)/OKRandGen.o $(Bibliothek)/Kommandozeile.o $(Prefix)/EinKlmGeneratorAES.o $(Prefix)/VEinKlmGeneratorAES.o $(Prefix)/LiteralGeneratorAES.o $(Prefix)/VLiteralGeneratorAES.o $(Prefix)/BlockChiffAES.o $(Prefix)/aescrypt.o $(Prefix)/aeskey.o $(Prefix)/aestab2.o $(Prefix)/flint.o $(Prefix)/flintpp.o
	$(cppc) -o $(Prefix)/OKgenerator -O3 $(CPPFLAGS) $(Prefix)/OKRandGen.o $(Bibliothek)/Kommandozeile.o $(Prefix)/EinKlmGeneratorAES.o $(Prefix)/VEinKlmGeneratorAES.o $(Prefix)/LiteralGeneratorAES.o $(Prefix)/VLiteralGeneratorAES.o $(Prefix)/BlockChiffAES.o $(Prefix)/aescrypt.o $(Prefix)/aeskey.o $(Prefix)/aestab2.o $(Prefix)/flint.o $(Prefix)/flintpp.o

$(Bibliothek)/Kommandozeile.o : $(Bibliothek)/Kommandozeile.hpp $(Bibliothek)/Kommandozeile.cpp
	g++ -O3 $(CPPFLAGS) -c $(Bibliothek)/Kommandozeile.cpp 

$(Prefix)/OKRandGen.o : OKRandGen.cpp $(Bibliothek)/Kommandozeile.hpp EinKlmGeneratorAES.hpp LiteralGeneratorAES.hpp flintpp.h
	$(cppc) -c -o $(Prefix)/OKRandGen.o -O3 $(CPPFLAGS) -DFLINTPP_ANSI -I$(Bibliothek) OKRandGen.cpp

$(Prefix)/EinKlmGeneratorAES.o : EinKlmGeneratorAES.cpp EinKlmGeneratorAES.hpp LiteralGeneratorAES.hpp
	$(cppc) -c -o $(Prefix)/EinKlmGeneratorAES.o -O3 $(CPPFLAGS) EinKlmGeneratorAES.cpp

$(Prefix)/VEinKlmGeneratorAES.o : VEinKlmGeneratorAES.cpp VEinKlmGeneratorAES.hpp VLiteralGeneratorAES.hpp
	$(cppc) -c -o $(Prefix)/VEinKlmGeneratorAES.o -O3 $(CPPFLAGS) VEinKlmGeneratorAES.cpp

$(Prefix)/LiteralGeneratorAES.o : LiteralGeneratorAES.cpp LiteralGeneratorAES.hpp BlockChiffAES.hpp
	$(cppc) -c -o $(Prefix)/LiteralGeneratorAES.o -O3 $(CPPFLAGS) LiteralGeneratorAES.cpp

$(Prefix)/VLiteralGeneratorAES.o : VLiteralGeneratorAES.cpp VLiteralGeneratorAES.hpp BlockChiffAES.hpp
	$(cppc) -c -o $(Prefix)/VLiteralGeneratorAES.o -O3 $(CPPFLAGS) VLiteralGeneratorAES.cpp

$(Prefix)/BlockChiffAES.o : BlockChiffAES.cpp BlockChiffAES.hpp aescpp.h
	$(cppc) -c -o $(Prefix)/BlockChiffAES.o -O3 $(CPPFLAGS) BlockChiffAES.cpp

$(Prefix)/aescrypt.o : aescrypt.c aesopt.h aestab.h
	$(cppc) -c -o $(Prefix)/aescrypt.o -O3 $(CPPFLAGS) aescrypt.c

$(Prefix)/aeskey.o : aescrypt.c aesopt.h aestab.h
	$(cppc) -c -o $(Prefix)/aeskey.o -O3 $(CPPFLAGS) aeskey.c

$(Prefix)/aestab2.o : aestab2.c aes.h
	$(cppc) -c -o $(Prefix)/aestab2.o -O3 $(CPPFLAGS) aestab2.c


$(Prefix)/flint.o : flint.c flint.h
	$(cppc) -c -o $(Prefix)/flint.o -O3 $(CPPFLAGS) flint.c

$(Prefix)/flintpp.o : flintpp.h flintpp.cpp
	$(cppc) -c -o $(Prefix)/flintpp.o -DFLINTPP_ANSI -O3 $(CPPFLAGS) flintpp.cpp


interprete_descriptor : $(Prefix)/interprete_descriptor

$(Prefix)/interprete_descriptor : $(Prefix)/DeskriptorVerarbeitung.o $(Prefix)/flint.o $(Prefix)/flintpp.o $(Bibliothek)/EndlAuto.o $(Bibliothek)/Kommandozeile.o
	$(cppc) -o $(Prefix)/interprete_descriptor -O3 $(CPPFLAGS) $(Prefix)/DeskriptorVerarbeitung.o $(Prefix)/flintpp.o $(Prefix)/flint.o $(Bibliothek)/EndlAuto.o $(Bibliothek)/Kommandozeile.o -lpq

$(Bibliothek)/EndlAuto.o : $(Bibliothek)/EndlAuto.cpp $(Bibliothek)/EndlAuto.hpp $(Bibliothek)/Kommandozeile.hpp
	g++ -o $(Bibliothek)/EndlAuto.o -c -O3 $(CPPFLAGS) $(Bibliothek)/EndlAuto.cpp

$(Prefix)/DeskriptorVerarbeitung.o : DeskriptorVerarbeitung.hpp DeskriptorVerarbeitung.cpp flintpp.h $(Bibliothek)/EndlAuto.hpp $(Bibliothek)/DatabaseHandler01.hpp
	$(cppc) -c -o $(Prefix)/DeskriptorVerarbeitung.o -O3 $(CPPFLAGS) -I/usr/include/pgsql -DSELBSTSTAENDIG -DFLINTPP_ANSI -I$(Bibliothek) DeskriptorVerarbeitung.cpp


experiment_preparation : $(Prefix)/experiment_preparation

$(Prefix)/experiment_preparation : $(Prefix)/ExperimentVorbereitung.o $(Prefix)/DeskVerarb.o $(Prefix)/flint.o $(Prefix)/flintpp.o $(Bibliothek)/EndlAuto.o $(Bibliothek)/Kommandozeile.o
	$(cppc) -o $(Prefix)/experiment_preparation -O3 $(CPPFLAGS) $(Prefix)/ExperimentVorbereitung.o $(Prefix)/DeskVerarb.o $(Prefix)/flint.o $(Prefix)/flintpp.o $(Bibliothek)/EndlAuto.o $(Bibliothek)/Kommandozeile.o -lpq

$(Prefix)/ExperimentVorbereitung.o : ExperimentVorbereitung.hpp ExperimentVorbereitung.cpp DeskriptorVerarbeitung.hpp $(Bibliothek)/Kommandozeile.hpp
	$(cppc) -c -o $(Prefix)/ExperimentVorbereitung.o -O3 $(CPPFLAGS) -I$(Bibliothek) ExperimentVorbereitung.cpp

$(Prefix)/DeskVerarb.o : DeskriptorVerarbeitung.hpp DeskriptorVerarbeitung.cpp flintpp.h $(Bibliothek)/EndlAuto.hpp $(Bibliothek)/DatabaseHandler01.hpp
	$(cppc) -c -o $(Prefix)/DeskVerarb.o -O3 $(CPPFLAGS) -DFLINTPP_ANSI -I/usr/include/pgsql -I$(Bibliothek) DeskriptorVerarbeitung.cpp


process_experiment : $(Prefix)/process_experiment

$(Prefix)/process_experiment : $(Prefix)/ExperimentDurchfuehrung.o $(Bibliothek)/Kommandozeile.o
	$(cppc) -o $(Prefix)/process_experiment -O3 $(CPPFLAGS) $(Prefix)/ExperimentDurchfuehrung.o $(Bibliothek)/Kommandozeile.o

$(Prefix)/ExperimentDurchfuehrung.o : ExperimentDurchfuehrung.hpp ExperimentDurchfuehrung.cpp $(Bibliothek)/Kommandozeile.hpp
	$(cppc) -c -o $(Prefix)/ExperimentDurchfuehrung.o -O3 $(CPPFLAGS) -I$(Bibliothek) ExperimentDurchfuehrung.cpp


transfer_experiment : $(Prefix)/transfer_experiment

$(Prefix)/transfer_experiment : $(Prefix)/ExperimentUebertragung.o $(Bibliothek)/Kommandozeile.o
	$(cppc) -o $(Prefix)/transfer_experiment -O3 $(CPPFLAGS) $(Prefix)/ExperimentUebertragung.o $(Bibliothek)/Kommandozeile.o -lxerces-c -lpq

$(Prefix)/ExperimentUebertragung.o : ExperimentUebertragung.cpp ExperimentUebertragung.hpp $(Bibliothek)/DatabaseHandler01.hpp Tools.hpp $(Bibliothek)/Kommandozeile.hpp $(Bibliothek)/TimeHandling.hpp
	$(cppc) -c -o $(Prefix)/ExperimentUebertragung.o -O3 $(CPPFLAGS) -Ixercesc -I/usr/include/pgsql -I$(Bibliothek) ExperimentUebertragung.cpp

# --------------------------------

.PHONY : clean cleanall

clean :
	rm -f $(addprefix $(Prefix),$(object_files))
cleanall : clean
	rm -f $(addprefix $(Prefix),$(programs))
