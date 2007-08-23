# Oliver Kullmann, 15.4.2005 (Swansea)

SHELL = /bin/sh
.SUFFIXES :

# Special makefile for preparing the buildsystem

# The following definitions are temporary: The oklib-masterscript will define OKplatform and OKconfiguration.
ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
  endif
endif
ifndef OKconfiguration
  OKconfiguration := $(OKplatform)/.oklib/Configuration
endif

include $(OKconfiguration)/configuration_data.mak
export

# ######################################################################

include $(OKbuildsystem)/standardgoals.mak


Directories := $(ExternalSources)

generic_dirs := $(OKsystem)/Transitional/AtomicConditions $(OKsystem)/Transitional/AutarkySearch $(OKsystem)/Transitional/Backup $(OKsystem)/Transitional/Concepts $(OKsystem)/Transitional/General $(OKsystem)/Transitional/GeneralInputOutput $(OKsystem)/Transitional/InjectivityConstraints $(OKsystem)/Transitional/InputOutput $(OKsystem)/Transitional/IOTools $(OKsystem)/Transitional/Iterators $(OKsystem)/Transitional/Kryptologie $(OKsystem)/Transitional/LaTeX $(OKsystem)/Transitional/LatinSquares $(OKsystem)/Transitional/Literals $(OKsystem)/Transitional/LocalSearch $(OKsystem)/Transitional/Matrices $(OKsystem)/Transitional/Messages $(OKsystem)/Transitional/MetaProgramming $(OKsystem)/Transitional/OrderRelations $(OKsystem)/Transitional/Parallelism $(OKsystem)/Transitional/Parsing $(OKsystem)/Transitional/PartialAssignments $(OKsystem)/Transitional/QuantumPhysics $(OKsystem)/Transitional/Refactoring $(OKsystem)/Transitional/SATAlgorithms $(OKsystem)/Transitional/SATCompetition $(OKsystem)/Transitional/SearchDataStructures $(OKsystem)/Transitional/SetAlgorithms $(OKsystem)/Transitional/SystemSpecifics $(OKsystem)/Transitional/TestExecutionTools $(OKsystem)/Transitional/TestSystem $(OKsystem)/Transitional/traits $(OKsystem)/Transitional/TreeDecompositions $(OKsystem)/Transitional/Variables $(OKsystem)/Transitional/Satisfiability/Solvers/OKsolver/SAT2002 $(OKsystem)/Transitional/Experimentation/Database

generic_links = $(addsuffix /makefile, $(generic_dirs))

makefile_oksystem := $(OKsystem)/makefile
makefile_externalsources := $(ExternalSources)/Makefile
makefile_annotations := $(Annotations_dir)/makefile
makefile_transitional := $(OKsystem)/Transitional/makefile
makefile_satisfiability := $(OKsystem)/Transitional/Satisfiability/makefile
makefile_solvers := $(OKsystem)/Transitional/Satisfiability/Solvers/makefile
makefile_oksolver := $(OKsystem)/Transitional/Satisfiability/Solvers/OKsolver/makefile
makefile_experimentation := $(OKsystem)/Transitional/Experimentation/makefile

$(test_goals) $(new_test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) :

prebuild : $(Directories) $(makefile_oksystem) $(makefile_externalsources) $(generic_links) $(makefile_annotations) $(makefile_transitional) $(makefile_satisfiability) $(makefile_solvers) $(makefile_oksolver) $(makefile_experimentation)

$(generic_links) : % :
	- ln -s $(OKbuildsystem)/generic.mak $@

$(Directories) : % :
	- mkdir $@


$(makefile_oksystem) : $(OKbuildsystem)/OKsystem.mak
	- ln -s $< $@

$(makefile_annotations) : $(OKbuildsystem)/Annotations.mak
	- ln -s $< $@

$(makefile_transitional) : $(OKbuildsystem)/Transitional.mak
	- ln -s $< $@
$(makefile_satisfiability) : $(OKbuildsystem)/Transitional.mak
	- ln -s $< $@
$(makefile_solvers) : $(OKbuildsystem)/Transitional.mak
	- ln -s $< $@
$(makefile_oksolver) : $(OKbuildsystem)/Transitional.mak
	- ln -s $< $@
$(makefile_experimentation) : $(OKbuildsystem)/Transitional.mak
	- ln -s $< $@



$(makefile_externalsources) : $(OKbuildsystem)/ExternalSources/Makefile
	- ln -s $< $@


clean_links :
	- rm $(generic_links) $(makefile_oksystem) $(makefile_annotations) $(makefile_transitional) $(makefile_externalsources)
