# Oliver Kullmann, 9.5.2009 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

programs := RankPrimes GrosswaldHagisFormula

source_libraries = $(gmp_include_option_okl)

link_libraries := 

