// Oliver Kullmann, 1.3.2006 (Swansea)

/*!
  \file Messages/Utilities/plans/Utilities.hpp
  \brief General plans for the Messages/Utilities module.

  \todo QuickMessage:
  Create a QuickMessage class (for quick creation of message objects out of strings).

  \todo Service for test exceptions:
  As a service for Testsystem/TestException_DesignStudy.hpp
  (and other parts of the test system) and for ProgramOptions we need
  classes (and macros) for file-identification purposes:
   - Per file we have __DATE__ (compilation date), __FILE__ (file name),
     __TIME__ (C) (compilation time), and
     $Date: 2006/09/06 12:54:34 $ (last change date), $Revision: 1.4 $ (current
     revision number) (CVS).
   - For programs we also have the version, the author, name of program etc.

 \todo Line number identification:
 Perhaps we can offer also some help regarding __LINE__.

 \todo Warning and error messages:
 We need components (message classes and handlers) for creating warning and error
 messages (using the above facilities)

 \todo Time and Date:
 Representations of time and date should move from General to Messages.

 \todo Error handling:
 Module ErrorHandling should offer a base class using message objects.

 \todo Testing:
 Where to put the tests for module Messages/Utilities ? We need tests for all the components
 in this module.

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

