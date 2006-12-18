// Oliver Kullmann, 1.3.2006 (Swansea)

/*!
  \file Messages/Utilities/plans/Utilities.hpp
  \brief General plans for the Messages/Utilities module.

    \todo Service for test exceptions:
    As a service for Testsystem/TestException_DesignStudy.hpp
    (and other parts of the test system) and for ProgramOptions we need
    classes (and macros) for identification purposes:
    <ul>
     <li> Handling of type names and line numbers (__LINE__) is needed.
     </li>
     <li> For programs we also have the version, the author, name of program etc.
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

