// Oliver Kullmann, 29.1.2006 (Swansea)

/*!
  \file Messages/plans/Messages.hpp
  \brief Plans for the messages module (as a whole)

  \todo Doxygen-documentation of message-classes:
  Using the macro OKLIB_MESSAGES_PRINT, doxygen misunderstands it as belonging to
  the following member --- how to correct this behaviour of doxygen? By telling doxygen to replace these macros with the empty string. DONE

  \todo Empty output:
  How can one easily define that for example at basic-level for all languages the output is empty?

  \todo Demonstrations:
  We need demonstrations for all components provided by this module (for example
  LineHandling is not handled yet). Also an overview on the demonstrations is
  needed.

  \todo Testing:
   - We need here a preliminary compiler switch, which disables
     testing of fr_CA for versions different from 4.0.3 and 4.1.1.
     Later we will request compiler version 4.1.1 or higher,
     but first we have to see that there are no performance
     problems with these later gcc-versions (regarding compilation and execution).
   - When removing Messages/Messages_Testapplication.cpp, we must make sure that all these tests are
     incorporated.

*/

/*!
  \namespace OKlib::Messages
  \brief Module for objects representing units of meaning
*/

namespace OKlib {
  namespace Messages {
  }
}

