# Oliver Kullmann, 23.11.2007 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

programs := Test demo_read_graph

source_libraries = $(boost_include_option_okl)

link_libraries := ${boost_link_option_okl} -lboost_graph -lboost_regex

