# Oliver Kullmann, 15.4.2005 (Swansea)
# Copyright 2005-2007 Oliver Kullmann
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

generic_dirs := $(OKsystem)/Transitional/Concepts $(OKsystem)/Transitional/General $(OKsystem)/Transitional/TestExecutionTools $(OKsystem)/Transitional/TestSystem $(OKsystem)/Transitional/traits $(OKsystem)/Transitional/Satisfiability/Solvers/OKsolver/SAT2002 $(OKsystem)/Transitional/Experimentation/Database $(OKsystem)/Transitional/Experimentation/Competition $(OKsystem)/Transitional/Structures/Cryptology $(OKsystem)/Transitional/Programming/Sequences $(OKsystem)/Transitional/Applications/RamseyTheory $(OKsystem)/Transitional/Satisfiability/ProofSystems/DPv $(OKsystem)/Transitional/Satisfiability/Solvers/Backtracking $(OKsystem)/Transitional/Satisfiability/Algorithms/Backtracking $(OKsystem)/Transitional/Applications/QuantumPhysics $(OKsystem)/Transitional/Satisfiability/ProblemInstances/ActiveClauses/InjectivityConstraints $(OKsystem)/Transitional/Satisfiability/Interfaces/InputOutput $(OKsystem)/Transitional/Satisfiability/ProblemInstances/Conditions/AtomicConditions $(OKsystem)/Transitional/Satisfiability/Assignments/PartialAssignments $(OKsystem)/Transitional/Satisfiability/Algorithms/LocalSearch $(OKsystem)/Transitional/Programming/Tools/Backup $(OKsystem)/Transitional/Combinatorics/Hypergraphs/Colourings $(OKsystem)/Transitional/Programming/Parallelism $(OKsystem)/Transitional/Satisfiability/Transformers/Generators $(OKsystem)/Transitional/Satisfiability/ProblemInstances/Literals $(OKsystem)/Transitional/Satisfiability/ProblemInstances/Variables $(OKsystem)/Transitional/Combinatorics/Graphs/BoostSupport $(OKsystem)/Transitional/Combinatorics/Graphs/Isomorphisms $(OKsystem)/Transitional/Applications/LatinSquares $(OKsystem)/Transitional/Structures/LinearAlgebra/Matrices $(OKsystem)/Transitional/Programming/Refactoring $(OKsystem)/Transitional/Programming/Iterators $(OKsystem)/Transitional/Programming/Parsing $(OKsystem)/Transitional/Programming/Messages $(OKsystem)/Transitional/Combinatorics/Graphs/TreeDecompositions $(OKsystem)/Transitional/Programming/SystemSpecifics $(OKsystem)/Transitional/Structures/Sets/SetAlgorithms $(OKsystem)/Transitional/Programming/MetaProgramming $(OKsystem)/Transitional/Programming/Utilities/OrderRelations $(OKsystem)/Transitional/Programming/InputOutput $(OKsystem)/Transitional/DataStructures/Search $(OKsystem)/Transitional/Satisfiability/Algorithms/Autarkies/Search $(OKsystem)/Transitional/ComputerAlgebra $(OKsystem)/Transitional/OKsolver/Experimental/AllgKlassen200203

generic_links := $(addsuffix /makefile, $(generic_dirs))

makefile_oksystem := $(OKsystem)/makefile

makefile_Annotations := $(Annotations_dir)/makefile

makefile_annotations := $(OKsystem)/Transitional/annotations/makefile

makefile_transitional := $(OKsystem)/Transitional/makefile
makefile_satisfiability := $(OKsystem)/Transitional/Satisfiability/makefile
makefile_assignments := $(OKsystem)/Transitional/Satisfiability/Assignments/makefile
makefile_autarkies := $(OKsystem)/Transitional/Satisfiability/Algorithms/Autarkies/makefile
makefile_proofsystems := $(OKsystem)/Transitional/Satisfiability/ProofSystems/makefile
makefile_solvers := $(OKsystem)/Transitional/Satisfiability/Solvers/makefile
makefile_oksolver := $(OKsystem)/Transitional/Satisfiability/Solvers/OKsolver/makefile
makefile_probleminstances := $(OKsystem)/Transitional/Satisfiability/ProblemInstances/makefile
makefile_activeclauses := $(OKsystem)/Transitional/Satisfiability/ProblemInstances/ActiveClauses/makefile
makefile_conditions := $(OKsystem)/Transitional/Satisfiability/ProblemInstances/Conditions/makefile
makefile_experimentation := $(OKsystem)/Transitional/Experimentation/makefile
makefile_structures := $(OKsystem)/Transitional/Structures/makefile
makefile_sets := $(OKsystem)/Transitional/Structures/Sets/makefile
makefile_linearalgebra := $(OKsystem)/Transitional/Structures/LinearAlgebra/makefile
makefile_programming := $(OKsystem)/Transitional/Programming/makefile
makefile_tools := $(OKsystem)/Transitional/Programming/Tools/makefile
makefile_utilities := $(OKsystem)/Transitional/Programming/Utilities/makefile
makefile_applications := $(OKsystem)/Transitional/Applications/makefile
makefile_algorithms := $(OKsystem)/Transitional/Satisfiability/Algorithms/makefile
makefile_interfaces := $(OKsystem)/Transitional/Satisfiability/Interfaces/makefile
makefile_transformers := $(OKsystem)/Transitional/Satisfiability/Transformers/makefile
makefile_combinatorics := $(OKsystem)/Transitional/Combinatorics/makefile
makefile_hypergraphs := $(OKsystem)/Transitional/Combinatorics/Hypergraphs/makefile
makefile_graphs := $(OKsystem)/Transitional/Combinatorics/Graphs/makefile
makefile_datastructures := $(OKsystem)/Transitional/DataStructures/makefile

OKsystem_links := $(makefile_oksystem)
Annotations_links := $(makefile_Annotations)
annotations_links := $(makefile_annotations)
Transitional_links := $(makefile_transitional) $(makefile_satisfiability) $(makefile_solvers) $(makefile_oksolver) $(makefile_algorithms) $(makefile_experimentation) $(makefile_structures) $(makefile_sets) $(makefile_programming) $(makefile_applications) $(makefile_proofsystems) $(makefile_assignments) $(makefile_autarkies) $(makefile_probleminstances) $(makefile_activeclauses) $(makefile_interfaces) $(makefile_conditions) $(makefile_tools) $(makefile_combinatorics) $(makefile_hypergraphs) $(makefile_transformers) $(makefile_graphs) $(makefile_linearalgebra) $(OKsystem)/Transitional/Programming/Utilities/makefile $(makefile_datastructures)


########### Rules ######################

$(test_goals) $(new_test_goals) $(normal_goals) $(cleaning_goals) $(forced_goals) :

.PHONY : make_directories create_links clean_links create_public_links

prebuild : make_directories create_links create_public_links

make_directories : $(build_directories) $(Directories)

$(Directories) $(build_directories) : % :
	- mkdir -p $@

create_links :  $(generic_links) $(OKsystem_links) $(Annotations_links) $(annotations_links) $(Transitional_links)

$(generic_links) : % :
	- ln -s $(OKbuildsystem)/OKlibBuilding/Makefile $@

$(OKsystem_links) : $(OKbuildsystem)/OKsystem.mak
	- ln -s $< $@

$(Annotations_links) : $(OKbuildsystem)/Annotations.mak
	- ln -s $< $@

$(annotations_links) : $(OKbuildsystem)/annotations.mak
	- ln -s $< $@

$(Transitional_links) : $(OKbuildsystem)/Transitional.mak
	- ln -s $< $@

create_public_links :
	cd $(public_bin_dir_okl); for L in $(public_link_list_okl); do ln -s --force $${L}; done


clean_links :
	- rm $(generic_links) $(OKsystem_links) $(Annotations_links) $(annotations_links) $(Transitional_links)

