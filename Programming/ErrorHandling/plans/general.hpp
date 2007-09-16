// Oliver Kullmann, 17.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ErrorHandling/plans/general.hpp
  \brief Plans for error handling


  \todo Update namespaces.


  \todo Create milestones.


  \todo The Boost method seems reasonable, so we should transfer it.
  But better using own types (a base class for all exceptions thrown
  by the library).
  \todo Transferring General/ErrorHandling.hpp. We should use Messages
  instead of strings.


  \todo Based on facilities in Messages/Utilities, there should be convenience
  macros for throwing exceptions (adding automatically all file information etc.).


  \todo We do NOT provide "cheap" message creators ("value-based"): For every
  (new) message a class has to be written in an appropriate file in a meesage-subdirectory.

*/

/*!
  \namespace OKlib::ErrorHandling
  \brief Tools for error (and exception) handling
*/

namespace OKlib {
  namespace ErrorHandling {
  }
}

