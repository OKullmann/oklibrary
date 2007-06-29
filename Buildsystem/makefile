SHELL = /bin/sh
.SUFFIXES :

# Special makefile for the buildsystem

# ################################################################
# Original definitions of OKplatform and OKbuildsystem, are 
# in Transtional/Buildsystem/generic.mak and cut-and-pasted
# to :
#  Transitional/Buildsystem/ExternalSources.mak
#  Transitional/Buildsystem/makefile
#  Transitional/Buildsystem/OKsystem.mak
#  Transitional/Buildsystem/recursive.mak
#  Transitional/makefile
#  Annotations/makefile
# ################################################################

ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKsystem
  ifdef OKSYSTEM
    OKsystem := $(OKSYSTEM)
  else
    OKsystem := $(OKplatform)/OKsystem
  endif
endif

ifndef OKbuildsystem
  ifdef OKBUILDSYSTEM
    OKbuildsystem := $(OKBUILDSYSTEM)
  else
    OKbuildsystem := $(OKsystem)/Transitional/Buildsystem
  endif
endif

# ######################################################################

include $(OKbuildsystem)/system_definitions.mak

include $(OKbuildsystem)/standardgoals.mak

externalsourcesdir := $(OKplatform)/ExternalSources

Directories :=  $(externalsourcesdir)

generic_dirs := $(OKsystem)/Transitional/AtomicConditions $(OKsystem)/Transitional/AutarkySearch $(OKsystem)/Transitional/Backup $(OKsystem)/Transitional/BigIntegers $(OKsystem)/Transitional/Combinatorics $(OKsystem)/Transitional/ComplexitySystem $(OKsystem)/Transitional/Concepts $(OKsystem)/Transitional/CrispCSP $(OKsystem)/Transitional/DPv $(OKsystem)/Transitional/ExperimentSystem $(OKsystem)/Transitional/General $(OKsystem)/Transitional/GeneralInputOutput $(OKsystem)/Transitional/Generators $(OKsystem)/Transitional/GraphDecomposition $(OKsystem)/Transitional/GraphDrawing $(OKsystem)/Transitional/HittingClauseSets  $(OKsystem)/Transitional/HypergraphColouring $(OKsystem)/Transitional/HypergraphTransversals $(OKsystem)/Transitional/InjectivityConstraints $(OKsystem)/Transitional/InputOutput $(OKsystem)/Transitional/IOTools $(OKsystem)/Transitional/Isomorphisms $(OKsystem)/Transitional/Iterators $(OKsystem)/Transitional/KLevelForcedAssignments $(OKsystem)/Transitional/Kryptologie $(OKsystem)/Transitional/LaTeX $(OKsystem)/Transitional/LatinSquares $(OKsystem)/Transitional/Literals $(OKsystem)/Transitional/LocalSearch $(OKsystem)/Transitional/Matrices $(OKsystem)/Transitional/Messages $(OKsystem)/Transitional/MetaProgramming $(OKsystem)/Transitional/NumberTheory $(OKsystem)/Transitional/OKDatabase $(OKsystem)/Transitional/OrderRelations $(OKsystem)/Transitional/Parallelism $(OKsystem)/Transitional/Parsing $(OKsystem)/Transitional/PartialAssignments $(OKsystem)/Transitional/ProgramOptions $(OKsystem)/Transitional/PseudoBoolean $(OKsystem)/Transitional/QuantumPhysics $(OKsystem)/Transitional/RamseyTheory $(OKsystem)/Transitional/RandomGenerator $(OKsystem)/Transitional/Refactoring $(OKsystem)/Transitional/SATAlgorithms $(OKsystem)/Transitional/SATCompetition $(OKsystem)/Transitional/SATModuloTheories $(OKsystem)/Transitional/SearchDataStructures $(OKsystem)/Transitional/SetAlgorithms $(OKsystem)/Transitional/Statistics $(OKsystem)/Transitional/SystemSpecifics $(OKsystem)/Transitional/TestExecutionTools $(OKsystem)/Transitional/TestSystem $(OKsystem)/Transitional/traits $(OKsystem)/Transitional/TreeDecompositions $(OKsystem)/Transitional/UnitClausePropagation $(OKsystem)/Transitional/Variables

generic_links = $(addsuffix /makefile, $(generic_dirs))

makefile_oksystem := $(OKsystem)/makefile
makefile_externalsources := $(externalsourcesdir)/makefile
makefile_annotations := $(Annotations_dir)/makefile
makefile_transitional := $(OKsystem)/Transitional/makefile

$(test_goals) $(new_test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) :

prebuild : $(Directories) $(makefiles) $(makefile_oksystem) $(makefile_externalsources) $(doxy_file) $(generic_links) $(makefile_annotations) $(makefile_transitional)

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

$(makefile_externalsources) : $(OKbuildsystem)/ExternalSources/ExternalSources.mak
	- ln -s $< $@

clean_links :
	- rm $(generic_links) $(makefile_oksystem) $(makefile_annotations) $(makefile_transitional) $(makefile_externalsources)
