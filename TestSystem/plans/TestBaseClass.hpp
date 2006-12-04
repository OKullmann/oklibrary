// Oliver Kullmann, 22.8.2006 (Swansea)

/*!
  \file TestSystem/plans/TestBaseClass.hpp
  \brief Plans for the module with the (user) base of the test hierarchy

  \todo Log-Messages:
  Replace TestBase::log_message in all tests by TestBase::log (potentially
  updating TestBase::log), and then remove TestBase::log_message :
   - TestBase::log implementation finished, but not tested yet.
   - After having created Messages::QuickMessage, upgrade all uses of
     TestBase::log_message to TestBase::log.
   - Why is the log-stream passed to the test-function? In this way it is always
     possible to output directly to the stream; this argument should
     be cancelled (after log_message has been eliminated).

  Files to handle:
   - TestSystem/tests/TestBaseClass.hpp
   - TestSystem/tests/Testfunctions.hpp (only elimination of log-parameter)
   - TestSystem/tests/Testfunctions2.hpp (only elimination of log-parameter).

  \todo Testing:
  Try to extend the level hierarchy (whether it "works").

  \todo Messages:
  Use Messages for all output.

  \todo Unknown exceptions:
  In case an unknown exception is thrown, there should be a global option to
  let this exception through (perhaps kind of a "handler").

  \todo Transition:
  Once the transition is done, rename "TestBaseClass_DesignStudy.hpp" to
  "TestBaseClass.hpp", and check the doxygen-documentation (which yet doesn't work due
  to multiple definitions of the same entities).
  

*/

