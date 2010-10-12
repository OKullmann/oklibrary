# Oliver Kullmann, 22.5.2009 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

programs := QuineMcCluskey-n4 QuineMcCluskey-n16 QuineMcCluskeySubsumptionHypergraph-n4 QuineMcCluskeySubsumptionHypergraph-n16

source_libraries = $(boost_include_option_okl)

link_libraries := ${boost_link_option_okl} 

