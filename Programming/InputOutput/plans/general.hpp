// Oliver Kullmann, 26.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/plans/general.hpp
  \brief Plans for the module on general input and output facilities


  \todo Compiler options for Programming/InputOutput/PathDifference.cpp
  <ul>
   <li> Would "-m32", supplied via PathDifference.compiler_options,
   still run on AMD-64-bit machines? </li>
   <li> Or are there other options which would make it running on
   both 32-bit and 64-bit machines? </li>
   <li> See "bug The binary coming with the package" in
   Buildsystem/ReleaseProcess/plans/PackageBuilding.hpp. </li>
  </ul>


  \todo Update namespaces


  \todo Update namespace-usage


  \todo Complete doxygen-documentation


  \todo Update plans
  Write plans for all submodules and transfer all todo's:
  <ul>
   <li> IOStreamFilters : </li>
   <li> IOStreamSinks : </li>
   <li> Multiplexer : </li>
   <li> RecursiveDirectoryIteration : (%boost can now be used here,
        and the bug should be handled) </li>
   <li> MatchFiles : (shall move to applications-sub-directory,
        or perhaps to a Tools-directory) </li>
   <li> PathDifference : (same as MatchFiles) </li>
  </ul>


  \todo Move Refactoring/BoostPathCorrected.hpp to here.


  \todo Write docus


  \todo Write demos


  \todo Transfer the tests to the new test system


  \todo Generic inserters and extractors
  <ul>
   <li> In [Standard C++ IOStreams and Locales], Section 3.1.5, general
   facilities for inserting and extracting to/from streams are discussed
   (with full error handling), so support insertion/extraction of arbitrary
   types. </li>
   <li> First implementation of a generic inserter (as in the book):
   \code
#include <ios>
#include <new>
template <typename charT, class Traits, class Argument>
std::basic_ostream<charT, Traits>& generic_inserter (
  std::basic_ostream<charT, Traits>& os,
  const Argument& arg
) {
  typedef typename std::basic_ostream<charT, Traits> ostream;
  std::ios_base::iostate error(std::ios_base::goodbit);
  try {
    typename ostream::sentry sentry_object(os);
    if (sentry_object) {
      error = arg.print(os);
      os.width(0);
    }
  }
  // now error != 0 iff (width threw and print_on != 0)
  catch(const std::bad_alloc&) { // setting badbit
    error |= std::ios_base::badbit;
    const std::ios_base::iostate exception_mask = os.exceptions(); // possibly throws ?!
    const bool throw_if_failbit = exception_mask & std::ios_base::failbit;
    const bool throw_if_badbit = exception_mask & std::ios_base::badbit;
    if (throw_if_failbit and not throw_if_badbit)
      os.setstate(error); // possibly throws std::ios_base::failure
    else if (throw_if_badbit) {
      try { os.setstate(error); } // throws std::ios_base::failure
      catch(const std::ios_base::failure&) {}
      throw;
    }
  }
  catch(...) { // setting failbit
    error |= std::ios_base::failbit;
    const bool badbit_in_error = error & std::ios_base::badbit;
    const std::ios_base::iostate exception_mask = os.exceptions(); // possibly throws ?!
    const bool throw_if_failbit = exception_mask & std::ios_base::failbit;
    const bool throw_if_badbit = exception_mask & std::ios_base::badbit;
    if (throw_if_badbit and badbit_in_error)
      os.setstate(error); // throws std::ios_base::failure; why not rethrow?!
    else if (throw_if_failbit) {
      try { os.setstate(error); } // throws std::ios_base::failure
      catch(const std::ios_base::failure&) {}
      throw;
    }
  }
  if (error) os.setstate(error); // possibly throws std::ios_base::failure
  return os;
}
   \endcode
   </li>
  </ul>

*/

/*!
  \namespace OKlib::GeneralInputOutput
  \brief Tools for handling input and output.
*/

namespace OKlib {
  namespace GeneralInputOutput {
  }
}
