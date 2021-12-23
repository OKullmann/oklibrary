# Oliver Kullmann, 3.3.2019 (Swansea)
# Copyright 2019, 2020, 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../..

opt_progs = ExperimentsTau GMP_MPFR_test Boost_int_test Boost_dec_test BBOpt
test_progs = Test TestStatistics
debug_progs = $(addsuffix _debug,$(opt_progs)) $(addsuffix _debug,$(test_progs))
unit_targets = $(addsuffix _debug_run,$(test_progs))

.PHONY : $(unit_targets)

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

ExperimentsTau : LDFLAGS += -lmpfr -lgmp
ExperimentsTau : Optimisation_plus =
ExperimentsTau : ExperimentsTau.cpp Tau.hpp Tau_mpfr.hpp FloatingPoint.hpp
ExperimentsTau_debug : LDFLAGS += -lmpfr -lgmp
ExperimentsTau_debug : Optimisation_plus =
ExperimentsTau_debug : ExperimentsTau.cpp Tau.hpp Tau_mpfr.hpp FloatingPoint.hpp

GMP_MPFR_test : LDFLAGS += -lmpfr -lgmp
GMP_MPFR_test : Optimisation_plus =
GMP_MPFR_test : GMP_MPFR_test.cpp
GMP_MPFR_test_debug : LDFLAGS += -lmpfr -lgmp
GMP_MPFR_test_debug : Optimisation_plus =
GMP_MPFR_test_debug : GMP_MPFR_test.cpp

Boost_int_test : Boost_int_test.cpp
Boost_int_test_debug : Boost_int_test.cpp
Boost_dec_test : Boost_dec_test.cpp
Boost_dec_test_debug : Boost_dec_test.cpp

BBOpt : LDFLAGS += -pthread
BBOpt : Optimisation_plus = -fwhole-program
BBOpt : BBOpt.cpp Optimisation.hpp
BBOpt_debug : LDFLAGS += -pthread
BBOpt_debug : BBOpt.cpp Optimisation.hpp

# -------------------------

Test_debug : LDFLAGS += -lmpfr -lgmp -pthread
Test_debug : Test.cpp NumTypes.hpp Conversions.hpp NumInOut.hpp FloatingPoint.hpp Tau.hpp Tau_mpfr.hpp Optimisation.hpp

TestStatistics_debug : TestStatistics.cpp Statistics.hpp


# -------------------------

regtest : $(unit_targets)

$(unit_targets) : %_run : %
	./$*

apptest :

