# Oliver Kullmann, 14.6.2009 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

programs := CountProgressions_GreenTao PdExtend SolutionShift # MinimumTransversals_VanderWaerden

source_libraries =  $(boost_include_option_okl)

link_libraries := 

