// Oliver Kullmann, 19.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef MULTIPLEXERTESTOBJECTS_5RReeW
#define MULTIPLEXERTESTOBJECTS_5RReeW

#include <OKlib/Programming/InputOutput/Multiplexer.hpp>
#include <OKlib/Programming/InputOutput/Multiplexer_Tests.hpp>

namespace OKlib {
  namespace GeneralInputOutput {

    ::OKlib::GeneralInputOutput::Test_OStreamDescriptor< ::OKlib::GeneralInputOutput::OStreamDescriptor> test_ostream_descriptor;

    ::OKlib::GeneralInputOutput::Test_OStreamMultiplexer< ::OKlib::GeneralInputOutput::OStreamMultiplexer> test_ostream_multiplexer;

  }
}

#endif
