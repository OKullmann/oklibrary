# Oliver Kullmann, 10.11.2007 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := Test_Transformations

programs := FaronsPuzzle VanderWaerdenCNF Ramsey LinInequal GreenTaoCNF GreenTaoGCNF Smusat_Horn Musatd2

source_libraries = $(boost_include_option_okl)

link_libraries := -lm

