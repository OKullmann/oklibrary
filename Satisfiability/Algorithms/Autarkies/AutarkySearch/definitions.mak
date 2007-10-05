# Oliver Kullmann, 2.11.2006 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

programs := AnalyseTotalAssignment

source_libraries = -I- -I$(OKsystem)/Transitional/Satisfiability/Algorithms/LocalSearch/Ubcsat/local -I$(OKsystem)/Transitional/Satisfiability/Algorithms/LocalSearch/Ubcsat/corrected $(Ubcsat)

link_libraries := 
