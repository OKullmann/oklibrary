// Oliver Kullmann, 22.8.2006 (Swansea)

/*!
  \file TestSystem/plans/TestExceptions.hpp
  \brief Plans for the module providing test macros

  \todo TestSystem::messages::TestException :
  <ol>
   <li> additional_circumstances should only be output for level Full. </li>
   <li> Extend the test macros, so that also an error-message-object is possible, and rewrite
   the existing tests, so that the string is only a short message. </li>
   <li> Shouldn't we allow also at the other places in the call-stack to add a string
   describing the special circumstances (not just at the place where the error occurred)?!
   But this might be hard to achieve, and might be better realised by using the log-facilities?! </li>
   <li> The constructor takes as optional second argument (by default 0) a pointer to
   a message object which provides the full story. DONE</li>
   <li> Use Messages for internal purposes. DONE </li>
   <li> One could make class TestException also a message-class ?! DONE </li>
  </ol>

  \todo TestSystem::messages::ErrorDescription
  (in TestSystem/messages/TestExceptions.hpp) : DONE
  <ul>
   <li> Shouldn't the indentation level be set from the outside (so you can
   have indentation or not --- currently we are forced to have it) ?! DONE </li>
   <li> Use messages. DONE </li>
   <li> Shouldn't ErrorDescription refer to the BasicTestDescription of the
   test class ?! So that it consists of the BasicTestDescription plus
   identification of the place where the exception was thrown?! DONE </li>
   <li> type_test_class is not output in the readable form --- perhaps we
   need a message-class for printing types here : Demangled (but (yet) no
   specialised message class for it). DONE </li>
   <li> Likely ErrorDescription should be derived from MessagesPrePost : DONE </li>
   <li> ErrorDescription should use components from Messages/Utilities
   for identification of lines: DONE </li>
   <li> Ownership of level_description needs to be managed : Now std::shared_ptr used. DONE </li>
   <li> Is the explicit definition of the copy constructor needed? Because of
   the auto-pointer-element, which has only a X&-copy-constructor, the
   implicitely declared copy-constructor has also type X& : Now not
   needed anymore, since std::auto-ptr was replaced by std::shared_ptr. DONE </li>
  </ul>

  \todo Test TestSystem::messages::ErrorDescription
  <ul>
   <li> Apply generic tests for MessagesPrePost. </li>
  </ul>

  \todo OKLIB_TEST_EQUAL :
    Update the implementation according to TestSystem/DesignStudy.hpp.

  \todo An overhaul of the test-macros (like OKLIB_TEST_EQUAL) seems to be needed.

  \todo OKLIB_TEST_RETHROW_NO_LOG :
    Invoking a sub-test but discarding the output. (It seems we should have that, or?)
    Just changing the definition of OKLIB_TEST_RETHROW by replacing log
    by a null-stream needs a global null-stream object ?!

  \todo Once the transition is done, rename "TestExceptions_DesignStudy.hpp" to "TestExceptions.hpp", and check the doxygen-documentation.
*/

