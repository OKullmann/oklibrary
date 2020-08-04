# Oliver Kullmann, 25.5.2018 (Swansea)
# Copyright 2018, 2019, 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../../../..

gcc_version = $(shell g++ -dumpfullversion | cut -d. -f 1)
opt_progs17 = qcount qcount_ct wqcount pqcount Queens_RUCP_ct
ifeq ($(gcc_version),10)
opt_progs20 = Queens_RUCP_ct20
else
opt_progs20 =
endif
opt_progs = $(opt_progs17) $(opt_progs20)
debug_progs = $(addsuffix _debug, $(opt_progs)) TestQueens_debug
popt_progs = $(addprefix p-, $(opt_progs))


include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

Optimisation_plus = -fwhole-program -funsafe-loop-optimizations

SETN=-DNUMQUEENS=16
SETBTYPES=-DBTYPES=1

# -------------------------

qcount : Warning_options += -Wno-parentheses
qcount : NQueens.cpp NQueens.hpp

qcount_debug : Warning_options += -Wno-parentheses
qcount_debug : NQueens.cpp NQueens.hpp

qcount_ct : CXXFLAGS+=$(SETN)
qcount_ct : NQueens_ct.cpp

qcount_ct_debug : CXXFLAGS += $(SETN)
qcount_ct_debug : NQueens_ct.cpp

wqcount : Warning_options += -Wno-parentheses
wqcount : NQueens_w.cpp

wqcount_debug : Warning_options += -Wno-parentheses
wqcount_debug : NQueens_w.cpp

pqcount : Warning_options += -Wno-parentheses
pqcount : LDFLAGS += -pthread
pqcount : NQueenspar.cpp NQueens.hpp

pqcount_debug : Warning_options += -Wno-parentheses
pqcount_debug : LDFLAGS += -pthread
pqcount_debug : NQueenspar.cpp NQueens.hpp

Queens_RUCP_ct : CXXFLAGS+=$(SETN) $(SETBTYPES)
Queens_RUCP_ct : LDFLAGS += -pthread
Queens_RUCP_ct : Queens_RUCP_ct.cpp Dimensions.hpp Rows.hpp ExtRows.hpp Statistics.hpp Board.hpp Backtracking.hpp

Queens_RUCP_ct20 : CXXFLAGS+=$(SETN) $(SETBTYPES)
Queens_RUCP_ct20 : LStandard_options+=--std=c++20
Queens_RUCP_ct20 : LDFLAGS += -pthread
Queens_RUCP_ct20 : Queens_RUCP_ct.cpp Dimensions.hpp Rows.hpp ExtRows.hpp Statistics.hpp Board.hpp Backtracking.hpp

Queens_RUCP_ct_debug : CXXFLAGS+=$(SETN) $(SETBTYPES)
Queens_RUCP_ct_debug : LDFLAGS += -pthread
Queens_RUCP_ct_debug : Queens_RUCP_ct.cpp Dimensions.hpp Rows.hpp ExtRows.hpp Statistics.hpp Board.hpp Backtracking.hpp

Queens_RUCP_ct20_debug : CXXFLAGS+=$(SETN) $(SETBTYPES)
Queens_RUCP_ct20_debug : LStandard_options+=--std=c++20
Queens_RUCP_ct20_debug : LDFLAGS += -pthread
Queens_RUCP_ct20_debug : Queens_RUCP_ct.cpp Dimensions.hpp Rows.hpp ExtRows.hpp Statistics.hpp Board.hpp Backtracking.hpp


TestQueens_debug : TestQueens.cpp Dimensions.hpp Rows.hpp ExtRows.hpp Statistics.hpp Board.hpp Backtracking.hpp


.PHONY : TestQueensdebug


TestQueensdebug : TestQueens_debug
	./$<
