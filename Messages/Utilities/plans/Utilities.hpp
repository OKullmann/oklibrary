// Oliver Kullmann, 1.3.2006 (Swansea)

/*!
  \file Messages/Utilities/plans/Utilities.hpp
  \brief General plans for the Messages/Utilities module.

  \todo SimpleMessage:
  Create a class TrivialMessage<X>, which takes a reference to a streamable
  object of type X, and prints this to the stream (independent of language and level) : DONE

  \todo Service for test exceptions:
  As a service for Testsystem/TestException_DesignStudy.hpp
  (and other parts of the test system) and for ProgramOptions we need
  classes (and macros) for file-identification purposes:
   - Per file we have __DATE__ (compilation date), __FILE__ (file name),
     __TIME__ (C) (compilation time), and
     $Date: 2006/12/04 10:45:15 $ (last change date), $Revision: 1.9 $ (current
     revision number) (CVS).
   - For programs we also have the version, the author, name of program etc.

   \todo Line number identification:
   Perhaps we can offer also some help regarding __LINE__.

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

