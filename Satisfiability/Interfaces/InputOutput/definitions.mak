# Oliver Kullmann, 13.4.2005 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestInputOutput

programs := ExtendedToStrictDimacs ExtendedDimacsStatistics ExtendedDimacsFullStatistics FilterDimacs SortByClauseLength SortByClauseLengthDescending RandomShuffleDimacs AppendDimacs MinOnes2WeightedMaxSAT MinOnes2PseudoBoolean ManipParam RenameDimacs

source_libraries = $(boost_include_option_okl)

link_libraries := 

