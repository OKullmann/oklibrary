// Oliver Kullmann, 1.3.2006 (Swansea)

/*!
  \file Messages/Utilities/plans/Utilities.hpp
  \brief General plans for the Messages/Utilities module.

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


   \todo Warning and error messages:
   We need components (message classes and handlers) for creating warning and error
   messages (using the above facilities)

   \todo Error handling:
   Module ErrorHandling should offer a base class using message objects.

   \todo Testing:
   Where to put the tests for module Messages/Utilities ? We need tests for all the components
   in this module. With the current system we have to put them to Messages/tests and
   Messages/testobjects. But this seems ugly --- we need a simple extension of the build system,
   which allows also subdirectories "tests" and "testobjects" in sub-modules. Perhaps a simple
   make-variable, which contains the list of sub-modules with own test-facilities. The question
   then however is where to put the test-object-files and programs? We need then corresponding
   sub-directories.

   \todo Umlaute
   Is it guaranteed that the UTF-8 encoding of a file will translate into the same encoding as given
   by the macros in Messages/Utilities/Umlaute.hpp for the string literals?

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

