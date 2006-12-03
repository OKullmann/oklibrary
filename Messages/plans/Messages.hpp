// Oliver Kullmann, 29.1.2006 (Swansea)

/*!
  \file Messages/plans/Messages.hpp
  \brief Plans for the messages module (as a whole)

  \todo Update:
   - Move all relevant todos to the plans-directory : DONE.

   - Update the doxygen-documentation.

     Messages/LanguageNames.hpp : DONE
     Messages/messages/Languages.hpp : DONE
     Messages/Languages.hpp : DONE
     Messages/Levels.hpp : DONE
     Messages/messages/Levels.hpp : DONE
     Messages/MessagesBase.hpp : DONE
     Messages/MessagesMain.hpp : DONE
     Messages/Messages_Testapplication.cpp : DONE
     Messages/Utilities/TimeAndDate.hpp : DONE
     Messages/Utilities/Umlaute.hpp : DONE.
     
   - It must be precisely documented how to use and write message classes:
     The simplest way is to write a nice "Messages/demo_Messages_1.cpp" etc. : DONE.

  \todo Testing:
   - We need here a preliminary compiler switch, which disables
     testing of fr_CA for versions different from 4.0.3 and 4.1.1.
     Later we will request compiler version 4.1.1 or higher,
     but first we have to see that there are no performance
     problems with these later gcc-versions (regarding compilation and execution).
   - When removing Messages/Messages_Testapplication.cpp, we must make sure that all these tests are
     incorporated.

  \todo Doxygen-documentation of message-classes
  Using the macro OKLIB_MESSAGES_PRINT, doxygen misunderstands it as belonging to
  the following member --- how to correct this behaviour of doxygen?

*/

/*!
  \namespace OKlib::Messages
  \brief Module for objects representing units of meaning
*/

namespace OKlib {
  namespace Messages {
  }
}

