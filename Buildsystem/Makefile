# Oliver Kullmann, 15.4.2005 (Swansea)
# Copyright 2005-2007, 2009 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# For creating basic directories and for planting symbolic links to makefiles (the latter
# should become superfluous soon).

SHELL = /bin/bash
.SUFFIXES :

# Special makefile for preparing the buildsystem

# The oklib-masterscript defines OKplatform.
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

Directories := $(ExternalSources) $(Annotations_dir)

generic_dirs := $(OKsystem)/OKlib/Concepts $(OKsystem)/OKlib/General $(OKsystem)/OKlib/TestExecutionTools $(OKsystem)/OKlib/TestSystem $(OKsystem)/OKlib/traits $(OKsystem)/OKlib/Satisfiability/Solvers/OKsolver/SAT2002 $(OKsystem)/OKlib/Experimentation/Competition $(OKsystem)/OKlib/Structures/Cryptology $(OKsystem)/OKlib/Programming/Sequences $(OKsystem)/OKlib/Satisfiability/ProofSystems/DPv $(OKsystem)/OKlib/Satisfiability/Solvers/Backtracking $(OKsystem)/OKlib/Satisfiability/Algorithms/Backtracking $(OKsystem)/OKlib/Applications/QuantumPhysics $(OKsystem)/OKlib/Satisfiability/ProblemInstances/ActiveClauses/InjectivityConstraints $(OKsystem)/OKlib/Satisfiability/Interfaces/InputOutput $(OKsystem)/OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions $(OKsystem)/OKlib/Satisfiability/Assignments/PartialAssignments $(OKsystem)/OKlib/Satisfiability/Assignments/TotalAssignments $(OKsystem)/OKlib/Satisfiability/Algorithms/LocalSearch $(OKsystem)/OKlib/Programming/Tools/Backup $(OKsystem)/OKlib/Combinatorics/Hypergraphs/Colourings $(OKsystem)/OKlib/Programming/Parallelism $(OKsystem)/OKlib/Satisfiability/Transformers/Generators $(OKsystem)/OKlib/Satisfiability/ProblemInstances/Literals $(OKsystem)/OKlib/Satisfiability/ProblemInstances/Variables $(OKsystem)/OKlib/Combinatorics/Graphs/BoostSupport $(OKsystem)/OKlib/Combinatorics/Graphs/Isomorphisms $(OKsystem)/OKlib/Applications/LatinSquares $(OKsystem)/OKlib/Structures/LinearAlgebra/Matrices $(OKsystem)/OKlib/Programming/Refactoring $(OKsystem)/OKlib/Programming/Iterators $(OKsystem)/OKlib/Programming/Parsing $(OKsystem)/OKlib/Programming/Messages $(OKsystem)/OKlib/Combinatorics/Graphs/TreeDecompositions $(OKsystem)/OKlib/Programming/SystemSpecifics $(OKsystem)/OKlib/Structures/Sets/SetAlgorithms $(OKsystem)/OKlib/Programming/MetaProgramming $(OKsystem)/OKlib/Programming/Utilities/OrderRelations $(OKsystem)/OKlib/Programming/InputOutput $(OKsystem)/OKlib/DataStructures/Search $(OKsystem)/OKlib/Satisfiability/Algorithms/Autarkies/Search $(OKsystem)/OKlib/ComputerAlgebra $(OKsystem)/OKlib/OKsolver/Experimental/AllgKlassen200203 $(OKsystem)/OKlib/Structures/NumberTheory/PrimeNumbers $(OKsystem)/OKlib/Satisfiability/FiniteFunctions $(OKsystem)/OKlib/Combinatorics/Hypergraphs/Transversals/Bounded $(OKsystem)/OKlib/Combinatorics/Hypergraphs/Generators $(OKsystem)/OKlib/Applications/RamseyTheory $(OKsystem)/OKlib/Satisfiability/ProblemInstances/Clauses $(OKsystem)/OKlib/Satisfiability/Reductions/UnitClausePropagation $(OKsystem)/OKlib/Satisfiability/Reductions/KLevelForcedAssignments $(OKsystem)/OKlib/Satisfiability/Reductions/Bases

generic_links := $(addsuffix /makefile, $(generic_dirs))

makefile_oksystem := $(OKsystem)/makefile

makefile_Annotations := $(Annotations_dir)/makefile

makefile_annotations := $(OKsystem)/OKlib/annotations/makefile

makefile_oklib := $(OKsystem)/OKlib/makefile
makefile_satisfiability := $(OKsystem)/OKlib/Satisfiability/makefile
makefile_assignments := $(OKsystem)/OKlib/Satisfiability/Assignments/makefile
makefile_autarkies := $(OKsystem)/OKlib/Satisfiability/Algorithms/Autarkies/makefile
makefile_proofsystems := $(OKsystem)/OKlib/Satisfiability/ProofSystems/makefile
makefile_solvers := $(OKsystem)/OKlib/Satisfiability/Solvers/makefile
makefile_oksolver := $(OKsystem)/OKlib/Satisfiability/Solvers/OKsolver/makefile
makefile_probleminstances := $(OKsystem)/OKlib/Satisfiability/ProblemInstances/makefile
makefile_activeclauses := $(OKsystem)/OKlib/Satisfiability/ProblemInstances/ActiveClauses/makefile
makefile_conditions := $(OKsystem)/OKlib/Satisfiability/ProblemInstances/Conditions/makefile
makefile_reductions := $(OKsystem)/OKlib/Satisfiability/Reductions/makefile
makefile_experimentation := $(OKsystem)/OKlib/Experimentation/makefile
makefile_structures := $(OKsystem)/OKlib/Structures/makefile
makefile_sets := $(OKsystem)/OKlib/Structures/Sets/makefile
makefile_linearalgebra := $(OKsystem)/OKlib/Structures/LinearAlgebra/makefile
makefile_programming := $(OKsystem)/OKlib/Programming/makefile
makefile_tools := $(OKsystem)/OKlib/Programming/Tools/makefile
makefile_utilities := $(OKsystem)/OKlib/Programming/Utilities/makefile
makefile_applications := $(OKsystem)/OKlib/Applications/makefile
makefile_algorithms := $(OKsystem)/OKlib/Satisfiability/Algorithms/makefile
makefile_interfaces := $(OKsystem)/OKlib/Satisfiability/Interfaces/makefile
makefile_transformers := $(OKsystem)/OKlib/Satisfiability/Transformers/makefile
makefile_combinatorics := $(OKsystem)/OKlib/Combinatorics/makefile
makefile_hypergraphs := $(OKsystem)/OKlib/Combinatorics/Hypergraphs/makefile
makefile_trhypergraphs := $(OKsystem)/OKlib/Combinatorics/Hypergraphs/Transversals/makefile
makefile_graphs := $(OKsystem)/OKlib/Combinatorics/Graphs/makefile
makefile_datastructures := $(OKsystem)/OKlib/DataStructures/makefile
makefile_oldoksolver := $(OKsystem)/OKlib/OKsolver/makefile
makefile_oldoksolver_experimental := $(OKsystem)/OKlib/OKsolver/Experimental/makefile
makefile_numbertheory := $(OKsystem)/OKlib/Structures/NumberTheory/makefile

OKsystem_links := $(makefile_oksystem)
Annotations_links := $(makefile_Annotations)
annotations_links := $(makefile_annotations)
OKlib_links := $(makefile_oklib) $(makefile_satisfiability) $(makefile_solvers) $(makefile_oksolver) $(makefile_algorithms) $(makefile_experimentation) $(makefile_structures) $(makefile_sets) $(makefile_programming) $(makefile_applications) $(makefile_proofsystems) $(makefile_assignments) $(makefile_autarkies) $(makefile_probleminstances) $(makefile_activeclauses) $(makefile_interfaces) $(makefile_conditions) $(makefile_reductions) $(makefile_tools) $(makefile_combinatorics) $(makefile_hypergraphs) $(makefile_trhypergraphs) $(makefile_transformers) $(makefile_graphs) $(makefile_linearalgebra) $(OKsystem)/OKlib/Programming/Utilities/makefile $(makefile_datastructures) $(makefile_oldoksolver) $(makefile_oldoksolver_experimental) $(makefile_numbertheory)


########### Rules ######################

$(test_goals) $(new_test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) :

.PHONY : make_directories create_links clean_links create_public_links

prebuild : make_directories create_links create_public_links

make_directories : $(build_directories) $(Directories)

$(Directories) $(build_directories) : % :
	- mkdir -p $@

create_links :  $(generic_links) $(OKsystem_links) $(Annotations_links) $(annotations_links) $(OKlib_links)

$(generic_links) : % :
	- ln -s $(OKbuildsystem)/OKlibBuilding/Makefile $@

$(OKsystem_links) : $(OKbuildsystem)/OKsystem.mak
	- ln -s $< $@

$(Annotations_links) : $(OKbuildsystem)/Annotations.mak
	- ln -s $< $@

$(annotations_links) : $(OKbuildsystem)/annotations.mak
	- ln -s $< $@

$(OKlib_links) : $(OKbuildsystem)/OKlib.mak
	- ln -s $< $@

create_public_links :
	cd $(public_bin_dir_okl); for L in $(public_link_list_okl); do ln -s --force $${L}; done


clean_links :
	- rm $(generic_links) $(OKsystem_links) $(Annotations_links) $(annotations_links) $(OKlib_links)

