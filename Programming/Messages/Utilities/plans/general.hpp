// Oliver Kullmann, 1.3.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/Utilities/plans/general.hpp
  \brief General plans for the Messages/Utilities module.


  \todo Update namespace


  \todo Update namespace usage.


   \todo Identification services:
   We need further classes (and macros) for identification purposes:
    <ul>
     <li> Document the handling of line numbers (including macro OKLIB_LINE).
     </li>
     <li> A versatile message classes for type-names is needed:
      <ul>
       <li> Study the existing solutions in TestSystem. </li>
      </ul>
     </li>
     <li> For programs we need the version, the author, name of program etc.
     </li>
    </ul>


   \todo Warning and error messages
   <ul>
    <li> We need components (message classes and handlers) for creating warning
    and error messages (using the above facilities). </li>
   </ul>


   \todo Error handling
   <ul>
    <li> Module ErrorHandling should offer a base class using message
    objects. </li>
   </ul>


   \todo Testing
   <ul>
    <li> Where to put the tests for module Messages/Utilities ? </li>
    <li> We need tests for all the components in this module. </li>
    <li> With the current system we have to put them to Messages/tests and
    Messages/testobjects. </li>
    <li> But this seems ugly --- we need a simple extension of the build system,
    which allows also subdirectories "tests" and "testobjects" in
    sub-modules. </li>
    <li> Perhaps a simple make-variable, which contains the list of sub-modules
    with own test-facilities. </li>
    <li> The question then however is where to put the test-object-files and
    programs? We need then corresponding sub-directories. </li>
   </ul>


   \todo Umlaute
   <ul>
    <li> Is it guaranteed that the UTF-8 encoding of a file will translate
    into the same encoding as given by the macros in
    Messages/Utilities/Umlaute.hpp for the string literals? </li>
   </ul>

*/

/*!
  \namespace OKlib::Messages::Utilities
  \brief Components for creating standard message objects
*/

namespace OKlib {
  namespace Messages {
    namespace Utilities {
    }
  }
}

