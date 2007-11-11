# Oliver Kullmann, 10.11.2007 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program := Test_Transformations

programs := FaronsPuzzle VanderWaerden Ramsey

source_libraries = $(Boost)

link_libraries := -lm

