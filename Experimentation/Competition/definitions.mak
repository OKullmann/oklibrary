# Oliver Kullmann, 13.4.2005 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := TestSATCompetition

programs := Analyse ComputeScores ComputeLexicographicalEvaluation ComputeAnalysis

source_libraries =  $(boost_include_option_okl)

link_libraries := 
