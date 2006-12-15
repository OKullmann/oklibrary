# Oliver Kullmann, 13.4.2005 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

test_program :=

programs := Special_update Delete_rows CreateStatistic

source_libraries =  $(Boost) -I/usr/include/pgsql

link_libraries := -lpq

