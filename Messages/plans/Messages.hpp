// Oliver Kullmann, 29.1.2006 (Swansea)

/*!
  \file Messages/plans/Messages.hpp
  \brief Plans for the messages module (as a whole)

  \todo The general concept of a "message":
  There need to be clear definitions and examples for what a "message" is:
  <ol>
   <li> A basic message is like a sentence:
   <ul>
    <li> It starts with a capital letter. </li>
    <li> It just continues on the given line, that is, it does not invoke anything at
         the begin of the message or at the end (and it does not span multiple lines). </li>
   </ul>
   </li>
   <li> For multi-line messages MessagePrePost is to be used (perhaps better "MessagesMultiLines" ?):
   <ul>
    <li> Still nothing at the begin or the end, but for line breaks
     <code> l_end, l_start </code>
    are to be used, so that indentation of messages is possible. </li>
    <li> For convenience, functions
     <code> s(), e(), se() </code>
     switch on the l-start and l-end at the beginning respectively at the end of the
     (whole) message (can this be automatically achieved?). </li>
    <li> To facilitate indentation, an additional indent-member functions
    (2 overloads: string and int) is made available, which appends to the prefix
    (and returns a reference; the point is easier use for temporary message objects). </li>
   </ul>
   </li>
   <li> These basic requirements shall go to the general documentation (and also to the doxygen
        documentation). </li>
   <li> All existed messages need to be overhauled. </li>
  </ol>
  

  \todo Empty output:
  How can one easily define that for example at basic-level for all languages the output is empty?
  Easiest would be a function template, but then overload-ambiguities arise.
  In the cases where it is applicable (see for example OKlib::TestSystem::messages::LogDescription),
  one has to see whether code bloat arises.
  A similar problem arises, when a nested message-class is only called for example for level
  "extensive", but nevertheless all print-version need to be defined (!) (declared is not enough!).
  The solution should be to have more macros like OKLIB_MESSAGES_PRINT, which allow to handle
  these special cases. Perhaps OKLIB_LEVEL_yUhTr6 and OKLIB_LANG_77TgVf can be utilised.

  \todo Internal use of messages:
  If in for example
  \code print(std::ostream& out, L<en_GB>, S<Basic>) const \endcode
  a message is output on stream <code>out</code>, then, despite the knowledge about the
  language and the level, at runtime language and level must be extracted from <code>out</code>
  to output the message: Can this runtime-overhead be avoided?

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
   - When removing Messages/Messages_Testapplication.cpp, we must make sure that all the tests there
     are incorporated.

  \todo Memory management:
  Are intelligent pointer enough to handle messages? There should be a documentation about handling
  messages (regarding their storage duration: automatic and dynamic are most important; and references
  are also important).

*/

/*!
  \namespace OKlib::Messages
  \brief Module for objects representing units of meaning
*/

namespace OKlib {
  namespace Messages {
  }
}

