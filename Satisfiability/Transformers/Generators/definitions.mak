# Oliver Kullmann, 10.11.2007 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := Test_Transformations

programs := FaronsPuzzle VanderWaerden Ramsey LinInequal GreenTaoCNF GreenTaoGCNF

source_libraries = $(boost_include_option_okl)

link_libraries := -lm

