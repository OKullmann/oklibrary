# Oliver Kullmann, 12.7.2011 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

programs := MeasureCPUSpeed MeasureCPUSpeedMem

source_libraries = $(boost_include_option_okl)

link_libraries := ${boost_link_option_okl}
