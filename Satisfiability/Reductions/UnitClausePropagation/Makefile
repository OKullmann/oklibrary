# Oliver Kullmann, 16.2.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

oklibloc = ../../..

opt_progs = UnitClausePropagation UnitClausePropagation64 UnitClausePropagationW UnitClausePropagationW64
debug_progs = UnitClausePropagation_debug UnitClausePropagation64_debug UnitClausePropagationW_debug UnitClausePropagationW64_debug

include $(oklibloc)/Buildsystem/OKlibBuilding/LocalMakefile

# -------------------------

# The followoing programs use headers from boost; compilation with C++20
# yields the compilation-error:
# /usr/include/boost/iostreams/detail/buffer.hpp:42:56: error: no class template named -F¡rebind¢ in ¡class std::allocator<char>¢-A
# rebind was deprecated in C++17.

UnitClausePropagation : UnitClausePropagation.cpp
UnitClausePropagation : CXXFLAGS+=-I $(oklibloc)/..
UnitClausePropagation : LStandard_options=-pedantic
UnitClausePropagation_debug : CXXFLAGS+=-I $(oklibloc)/..
UnitClausePropagation_debug : LStandard_options=-pedantic
UnitClausePropagation_debug : UnitClausePropagation.cpp
UnitClausePropagation64 : UnitClausePropagation.cpp
UnitClausePropagation64 : CXXFLAGS+=-I $(oklibloc)/.. -DLIT_TYPE=std::int64_t
UnitClausePropagation64 : LStandard_options=-pedantic
UnitClausePropagation64_debug : CXXFLAGS+=-I $(oklibloc)/.. -DLIT_TYPE=std::int64_t
UnitClausePropagation64_debug : LStandard_options=-pedantic
UnitClausePropagation64_debug : UnitClausePropagation.cpp


UnitClausePropagationW : UnitClausePropagationW.cpp
UnitClausePropagationW : CXXFLAGS+=-I $(oklibloc)/..
UnitClausePropagationW : LStandard_options=-pedantic
UnitClausePropagationW_debug : UnitClausePropagationW.cpp
UnitClausePropagationW_debug : CXXFLAGS+=-I $(oklibloc)/..
UnitClausePropagationW_debug : LStandard_options=-pedantic
UnitClausePropagationW64 : UnitClausePropagationW.cpp
UnitClausePropagationW64 : CXXFLAGS+=-I $(oklibloc)/.. -DLIT_TYPE=std::int64_t
UnitClausePropagationW64 : LStandard_options=-pedantic
UnitClausePropagationW64_debug : UnitClausePropagationW.cpp
UnitClausePropagationW64_debug : CXXFLAGS+=-I $(oklibloc)/.. -DLIT_TYPE=std::int64_t
UnitClausePropagationW64_debug : LStandard_options=-pedantic

regtest :

apptest :

