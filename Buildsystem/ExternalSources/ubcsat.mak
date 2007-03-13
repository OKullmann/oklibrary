# Matthew Lewsey, 9.11.2006 (Swansea)
# filename : BuildSystem/ExternalSources/ubcsat.mak

# ##################################
# Functions
# ##################################

define unarchivefolder
if [ -f $(1).tar.gz ]; then tar --extract --directory=$(2) --file=$(1).tar.gz --ungzip $(3); elif [ -f $(1).tar.bz2 ]; then tar --extract --directory=$(2) --file=$(1).tar.bz2 --bzip2 $(3); else exit 1; fi;
endef

# ##################################
# Targets
# ##################################

include $(OKbuildsystem)/external_sources_versions.mak

# ##################################
# Directory Structure
# ##################################

ubcsat-base-directory := $(ExternalSources)/Ubcsat
ubcsat-extract-directory := $(ubcsat-base-directory)/1-0-0
ubcsat-lib-directory := $(ubcsat-base-directory)/1-0-0/lib
ubcsat-bin-directory := $(ubcsat-base-directory)/1-0-0/bin
ubcsat-src-directory := $(ubcsat-base-directory)/1-0-0/src
ubcsat-tmp-src-directory := $(ubcsat-base-directory)/1-0-0/tmp

ubcsat-directories := $(ubcsat-base-directory) $(ubcsat-extract-directory) $(ubcsat-lib-directory) $(ubcsat-bin-directory) $(ubcsat-src-directory) $(ubcsat-tmp-src-directory)

ubcsat_names := adaptnovelty algorithms gsat gsat-tabu gwsat hsat hwsat irots mt19937ar mylocal novelty parameters reports rnovelty rots samd saps ubcsat ubcsat-help ubcsat-internal ubcsat-io ubcsat-mem ubcsat-reports ubcsat-time ubcsat-triggers walksat walksat-tabu

ubcsat_c_files := $(addsuffix .c, $(ubcsat_names))
ubcsat_c_files_paths := $(addprefix $(ubcsat-src-directory)/, $(ubcsat_c_files))
ubcsat_o_files := $(addsuffix .o, $(ubcsat_names))

$(ubcsat-directories) : % : 
	mkdir $@

paths := $(addprefix $(ubcsat-lib-directory)/, $(ubcsat_o_files))

# #################################
# The Targets
# #################################

$(ubcsat-extract-directory)/tag : | $(ubcsat-base-directory) $(ubcsat-extract-directory) $(ubcsat-src-directory) $(ubcsat-tmp-src-directory)
	$(call unarchivefolder,ubcsat-1-0-0,$(ubcsat-extract-directory),src)
	dos2unix $(ubcsat-src-directory)/*.c
	cp $(ubcsat-src-directory)/* $(ubcsat-tmp-src-directory)
	cp -f $(OKsystem)/Transitional/AutarkySearch/ubcsat-types.h $(ubcsat-tmp-src-directory)
	touch $@

ubcsat : $(ubcsat-extract-directory)/tag $(ubcsat-bin-directory)/ubcsat $(ubcsat-lib-directory)/libubcsat.a cleanup

$(paths) : $(ubcsat-lib-directory)/%.o : $(ubcsat-tmp-src-directory)/%.c | $(ubcsat-installation-directory) $(ubcsat-lib-directory)
	gcc -c $< -o $@

$(ubcsat-bin-directory)/ubcsat : $(paths) | $(ubcsat-bin-directory)
	gcc -O3 -lm -o $(ubcsat-bin-directory)/ubcsat $(ubcsat_c_files_paths)

$(ubcsat-lib-directory)/libubcsat.a : $(paths)
	$(AR) $(ARFLAGS) $@ $^

.PHONY : cleanup

cleanup :
	rm -rf $(ubcsat-tmp-src-directory)


# #################################
# Documentation
# #################################

# USING THE BINARY #

#Running ubcsat without any arguments will yield the following:

#> ubcsat
#
# UBCSAT version 1.0.0 (Grouse Mountain Release)
#
#
# ubcsat -h for help
#

#FATAL ERROR ENCOUNTERED:
#UBCSAT requires you to specify an algorithm with -alg
#ubcsat -ha for a list of valid algorithms

#The binary needs:

#ubcsat [-h] [-ha][-hr] [-alg] [-inst] [-solve] [-find]
#	[-runs] [-cutoff] [-timeout] [-target] [-wtarget]
#       [-seed] [-prestart] [rrestart] [-strestart] [-noimprove]

#OPTIONS
#	Option Summary

#	Help
#	-h -ha -hr

#	Algorithm Options
#	-alg

#	File Options
#	-inst

#	Running Time Options
#	-solve -find -runs -cutoff -timeout -target -wtarget -seed -prestart -rrestart -noimprove




#	-h Help
#	Gives help on general options.

#	-ha Help Algorithms
#	Gives help on algorithm options.

#	-hr
#	Gives help on report options.

#	-alg Algorithm
#	Specifiy which algorithm to be used. For a list of these use -ha.

#	-inst Instance
#	Specify the file path of the input file used.

#	-solve
#	Tells ubcsat to terminate if a solution is found.

#	-find
#	Specify the number of solutions to be found before termination.

#	-runs
#	Specify the amount of times ubcsat runs the algorithm itself before stopping.

#	-cutoff
#	Specify the max amount of search steps ubcsat uses in one run of the algorithm.

#	-timeout
#	Specify maximum number of seconds for all runs.

#	-target
#	Specify the amount of allowed unsatisfied clauses for an assignment to be a solution.

#	-wtarget
#	Specify the amount of weight allowed from unsatisifed weighted clauses for an assignment to be
#	a solution.

#	-seed
#	Force a specific random seed.

#	-prestart
#	Specify the amount of steps before a peridoic restart. 

#	-rrestart
#	Specify probability to restart.

#	-noimprove
#	Specify number of setps to terminate run on when no improvement of current assignment.






# USING THE LIBRARY FILES #

#To use a function or variables from one of the .o files in Ubcsat/1-0-0/lib:

# ubcsat.h must be included
#all .o files in /Ubcsat/1-0-0/lib must be linked together



# WHAT ARE THE LIBRARY FILES #

#library files are unchanged from ubcsat-1-0-0.tar.gz except for ubcsat.o
#ubcsat.o now does not have a main function and instead has the function ubcsatmain.

#Many of the functions and variables are in ubcsat-internal.o and ubcsat-triggers.o.
#These functions and variables are documented in ubcsat-globals.h, ubcsat-triggers.h and ubcsat-internal.h

