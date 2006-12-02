// Oliver Kullmann, 1.3.2006 (Swansea)

/*!
  \file Messages/Utilities/plans/Utilities.hpp
  \brief General plans for the Messages/Utilities module.

  \todo SimpleMessage:
  Create a SimpleMessage class (for quick creation of message objects out of strings):
    - SimpleMessage(mstring) creates a message object with mstring the message in the default
      language at basic level; there are two further parameters for full and extensive level,
      with default values the empty string.
      All three levels for the default language are implemented.
    - The question is now how to specify further languages ?
        With a type-list the languages are specified, and then at run-time
        values are provided: SimpleMessageF<Language-list> (derived from SimpleMessage;
        "F" like "foreign").
      The class template SimpleMessageF provides for each language implementations
      for all three levels (similar to SimpleMessage), where in case of the empty string the message
      from the level below is chosen.
    This all in a new file Utilities/SimpleMessages.

  \todo Service for test exceptions:
  As a service for Testsystem/TestException_DesignStudy.hpp
  (and other parts of the test system) and for ProgramOptions we need
  classes (and macros) for file-identification purposes:
   - Per file we have __DATE__ (compilation date), __FILE__ (file name),
     __TIME__ (C) (compilation time), and
     $Date: 2006/12/02 17:44:33 $ (last change date), $Revision: 1.7 $ (current
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
   in this module.

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

