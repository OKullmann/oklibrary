// Oliver Kullmann, 22.8.2006 (Swansea)

/*!
  \file TestSystem/plans/TestBaseClass.hpp
  \brief Plans for the module with the (user) base of the test hierarchy

  \todo TestSystem::messages::LogDescription DONE
  (in TestSystem/messages/TestBaseClass.hpp):
  Update; perhaps like messages::ErrorDescription, only
  <ol>
   <li> no indentation </li>
   <li> in basic-level no output </li>
   <li> in full-level all three messages print, but in basic-level </li>
   <li> in extensive-level finally everything also in extensive-level. </li>
  </ol>
  Then also a macro is needed, like "OKLIB_FULL_LOG(m);". (The log-function
  needs only to take the line- and file-identication as parameters (similar
  to what we have now), while the BasicTestDescription is already available.)

  \todo Macros: DONE
   - write a macro to ease creating log-messages DONE
   - write a macro to ease defining test-classes. DONE

  \todo TestSystem::TestBase:
  - Use message-classes.
  - For the full-log-function: Perhaps m should be indented? And in extensive-level perhaps
    message-end should be indicated.
  - When putting the description on the log-stream (in perform_and_catch), then there should
    be a heading (at least for level full).
  - For the typename-output in perform_and_catch use demangling.

  \todo Unknown exceptions:
  In case an unknown exception is thrown, there should be a global option to
  let this exception through (perhaps kind of a "handler").

  \todo Encapsulation:
  It would be desirable to have TestBase::set_depth and TestBase::log_stream private
  (both functions are only used by OKLIB_TEST_RETHROW), since they belong only to the
  test-system. Perhaps we have a protected member-function in TestBase, which is used
  to call sub-tests, and this function then has access to the private data members log_p
  and depth_; the problem with that approach might be, that then it is harder to find out
  in case of a failing sub-tests, where really this sub-test has been called --- perhaps already
  calling this sub-test member-function, then the descriptions have to be given.

  \todo Testing
  Thoroughly test all functionality.

  \todo Transition:
  Once the transition is done, rename "TestBaseClass_DesignStudy.hpp" to
  "TestBaseClass.hpp", and check the doxygen-documentation (which yet doesn't work due
  to multiple definitions of the same entities).
  

*/

