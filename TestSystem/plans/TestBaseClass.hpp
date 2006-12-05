// Oliver Kullmann, 22.8.2006 (Swansea)

/*!
  \file TestSystem/plans/TestBaseClass.hpp
  \brief Plans for the module with the (user) base of the test hierarchy

  \todo Log-Messages:
  Replace TestBase::log_message in all tests by TestBase::log (potentially
  updating TestBase::log), and then remove TestBase::log_message :
   - Upgrade all uses of TestBase::log_message to TestBase::log : DONE
   - Eliminate the log-argument from the test-function (after log_message has been eliminated) : DONE

  \todo Messages:
  - Write true message-classes, not just "trivial messages".
  - Use messages also for throwing exceptions.

  \todo Macros:
   - write a macro to ease creating log-messages
   - write a macro to ease defining test-classes.

  \todo Unknown exceptions:
  In case an unknown exception is thrown, there should be a global option to
  let this exception through (perhaps kind of a "handler").

  \todo Encapsulation
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

