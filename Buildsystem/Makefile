# Oliver Kullmann, 15.4.2005 (Swansea)
# Copyright 2005-2007, 2009, 2011 Oliver Kullmann
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

# the (non-recursive) makefiles for compiling C/C++ programs
generic_dirs := $(OKlib)/Concepts $(OKlib)/General $(OKlib)/TestExecutionTools $(OKlib)/TestSystem $(OKlib)/traits $(OKlib)/Satisfiability/Solvers/OKsolver/SAT2002 $(OKlib)/Experimentation/Competition $(OKlib)/Structures/Cryptology $(OKlib)/Programming/Sequences $(OKlib)/Satisfiability/ProofSystems/DPv $(OKlib)/Satisfiability/Solvers/Backtracking $(OKlib)/Satisfiability/Algorithms/Backtracking $(OKlib)/Applications/QuantumPhysics $(OKlib)/Satisfiability/ProblemInstances/ActiveClauses/InjectivityConstraints $(OKlib)/Satisfiability/Interfaces/InputOutput $(OKlib)/Satisfiability/ProblemInstances/Conditions/AtomicConditions $(OKlib)/Satisfiability/Assignments/PartialAssignments $(OKlib)/Satisfiability/Assignments/TotalAssignments $(OKlib)/Satisfiability/Algorithms/LocalSearch $(OKlib)/Programming/Tools/Backup $(OKlib)/Combinatorics/Hypergraphs/Colourings $(OKlib)/Programming/Parallelism $(OKlib)/Satisfiability/Transformers/Generators $(OKlib)/Satisfiability/ProblemInstances/Literals $(OKlib)/Satisfiability/ProblemInstances/Variables $(OKlib)/Combinatorics/Graphs/BoostSupport $(OKlib)/Combinatorics/Graphs/Isomorphisms $(OKlib)/Applications/LatinSquares $(OKlib)/Structures/LinearAlgebra/Matrices $(OKlib)/Programming/Refactoring $(OKlib)/Programming/Iterators $(OKlib)/Programming/Parsing $(OKlib)/Programming/Messages $(OKlib)/Combinatorics/Graphs/TreeDecompositions $(OKlib)/Programming/SystemSpecifics $(OKlib)/Structures/Sets/SetAlgorithms $(OKlib)/Programming/MetaProgramming $(OKlib)/Programming/Utilities/OrderRelations $(OKlib)/Programming/InputOutput $(OKlib)/DataStructures/Search $(OKlib)/Satisfiability/Algorithms/Autarkies/Search $(OKlib)/ComputerAlgebra $(OKlib)/OKsolver/Experimental/AllgKlassen200203 $(OKlib)/Structures/NumberTheory/PrimeNumbers $(OKlib)/Satisfiability/FiniteFunctions $(OKlib)/Combinatorics/Hypergraphs/Transversals/Bounded $(OKlib)/Combinatorics/Hypergraphs/Generators $(OKlib)/Applications/RamseyTheory $(OKlib)/Satisfiability/ProblemInstances/Clauses $(OKlib)/Satisfiability/Reductions/UnitClausePropagation $(OKlib)/Satisfiability/Reductions/KLevelForcedAssignments $(OKlib)/Satisfiability/Reductions/Bases $(OKlib)/Experimentation/Investigations/BooleanFunctions $(OKlib)/Satisfiability/Interfaces/DistributedSolving $(OKlib)/System/ComplexitySystem

generic_links := $(addsuffix /makefile, $(generic_dirs))


# The recursive makefiles :

makefile_oksystem := $(OKsystem)/makefile

makefile_Annotations := $(Annotations_dir)/makefile

makefile_annotations := $(OKlib)/annotations/makefile

makefile_oklib := $(OKlib)/makefile
makefile_satisfiability := $(OKlib)/Satisfiability/makefile
makefile_assignments := $(OKlib)/Satisfiability/Assignments/makefile
makefile_autarkies := $(OKlib)/Satisfiability/Algorithms/Autarkies/makefile
makefile_proofsystems := $(OKlib)/Satisfiability/ProofSystems/makefile
makefile_solvers := $(OKlib)/Satisfiability/Solvers/makefile
makefile_oksolver := $(OKlib)/Satisfiability/Solvers/OKsolver/makefile
makefile_probleminstances := $(OKlib)/Satisfiability/ProblemInstances/makefile
makefile_activeclauses := $(OKlib)/Satisfiability/ProblemInstances/ActiveClauses/makefile
makefile_conditions := $(OKlib)/Satisfiability/ProblemInstances/Conditions/makefile
makefile_reductions := $(OKlib)/Satisfiability/Reductions/makefile
makefile_experimentation := $(OKlib)/Experimentation/makefile
makefile_investigations := $(OKlib)/Experimentation/Investigations/makefile
makefile_booleanfunctions := $(OKlib)/Experimentation/Investigations/BooleanFunctions
makefile_structures := $(OKlib)/Structures/makefile
makefile_sets := $(OKlib)/Structures/Sets/makefile
makefile_linearalgebra := $(OKlib)/Structures/LinearAlgebra/makefile
makefile_programming := $(OKlib)/Programming/makefile
makefile_tools := $(OKlib)/Programming/Tools/makefile
makefile_utilities := $(OKlib)/Programming/Utilities/makefile
makefile_applications := $(OKlib)/Applications/makefile
makefile_algorithms := $(OKlib)/Satisfiability/Algorithms/makefile
makefile_interfaces := $(OKlib)/Satisfiability/Interfaces/makefile
makefile_transformers := $(OKlib)/Satisfiability/Transformers/makefile
makefile_combinatorics := $(OKlib)/Combinatorics/makefile
makefile_hypergraphs := $(OKlib)/Combinatorics/Hypergraphs/makefile
makefile_trhypergraphs := $(OKlib)/Combinatorics/Hypergraphs/Transversals/makefile
makefile_graphs := $(OKlib)/Combinatorics/Graphs/makefile
makefile_datastructures := $(OKlib)/DataStructures/makefile
makefile_oldoksolver := $(OKlib)/OKsolver/makefile
makefile_oldoksolver_experimental := $(OKlib)/OKsolver/Experimental/makefile
makefile_numbertheory := $(OKlib)/Structures/NumberTheory/makefile
makefile_complexitysystem := $(OKlib)/System/makefile

OKsystem_links := $(makefile_oksystem)
Annotations_links := $(makefile_Annotations)
annotations_links := $(makefile_annotations)
OKlib_links := $(makefile_oklib) $(makefile_satisfiability) $(makefile_solvers) $(makefile_oksolver) $(makefile_algorithms) $(makefile_experimentation) $(makefile_investigations) $(makefile_booleanfunctions) $(makefile_structures) $(makefile_sets) $(makefile_programming) $(makefile_applications) $(makefile_proofsystems) $(makefile_assignments) $(makefile_autarkies) $(makefile_probleminstances) $(makefile_activeclauses) $(makefile_interfaces) $(makefile_conditions) $(makefile_reductions) $(makefile_tools) $(makefile_combinatorics) $(makefile_hypergraphs) $(makefile_trhypergraphs) $(makefile_transformers) $(makefile_graphs) $(makefile_linearalgebra) $(OKlib)/Programming/Utilities/makefile $(makefile_datastructures) $(makefile_oldoksolver) $(makefile_oldoksolver_experimental) $(makefile_numbertheory) $(makefile_complexitysystem)


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

