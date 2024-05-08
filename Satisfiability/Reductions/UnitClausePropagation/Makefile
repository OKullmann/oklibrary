# Oliver Kullmann, 16.2.2021 (Swansea)
# Copyright 2021, 2024 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = UnitClausePropagation UnitClausePropagation64 UnitClausePropagationW UnitClausePropagationW64
debug_progs = UnitClausePropagation_debug UnitClausePropagation64_debug UnitClausePropagationW_debug UnitClausePropagationW64_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

UnitClausePropagation : UnitClausePropagation.cpp
UnitClausePropagation : include_options += -I $(oklibloc)/..
UnitClausePropagation_debug : include_options += -I $(oklibloc)/..
UnitClausePropagation_debug : UnitClausePropagation.cpp
UnitClausePropagation64 : UnitClausePropagation.cpp
UnitClausePropagation64 : include_options += -I $(oklibloc)/.. -DLIT_TYPE=std::int64_t
UnitClausePropagation64_debug : include_options += -I $(oklibloc)/.. -DLIT_TYPE=std::int64_t
UnitClausePropagation64_debug : UnitClausePropagation.cpp


UnitClausePropagationW : UnitClausePropagationW.cpp
UnitClausePropagationW : include_options += -I $(oklibloc)/..
UnitClausePropagationW_debug : UnitClausePropagationW.cpp
UnitClausePropagationW_debug : include_options += -I $(oklibloc)/..
UnitClausePropagationW64 : UnitClausePropagationW.cpp
UnitClausePropagationW64 : include_options += -I $(oklibloc)/.. -DLIT_TYPE=std::int64_t
UnitClausePropagationW64_debug : UnitClausePropagationW.cpp
UnitClausePropagationW64_debug : include_options += -I $(oklibloc)/.. -DLIT_TYPE=std::int64_t

regtest :

apptest :

