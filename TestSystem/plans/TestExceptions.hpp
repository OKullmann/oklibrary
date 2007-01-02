// Oliver Kullmann, 22.8.2006 (Swansea)

/*!
  \file TestSystem/plans/TestExceptions.hpp
  \brief Plans for the module providing test macros

  \todo TestSystem::ErrorDescription (in TestSystem/messages/TestExceptions.hpp) :
   - type_test_class is not output in the readable form --- likely we need a
     message-class for printing types here.
   - Use messages.
   - Likely ErrorDescription should be derived from MessagesPrePost.
   - Test it!
   - ErrorDescription should use components from
     Messages/Utilities for identification of lines: DONE
   - Ownership of level_description needs to be managed : Now std::shared_ptr used. DONE
   - Is the explicit definition of the copy constructor needed? Because of the auto-pointer-element,
     which has only a X&-copy-constructor, the implicitely declared copy-constructor has also type
     X& : Now not needed anymore, since std::auto-ptr was replaced by std::shared_ptr. DONE

  \todo TestSystem::TestException :
   Investigate, whether it should use Messages for internal purposes, and (perhaps) also for the
   input of error circumstances (yet the parameter special_circumstances is a plain string variable;
   in this way strings and boost::lexical_cast would be avoided).

  \todo OKLIB_TEST_EQUAL :
    Update the implementation according to TestSystem/DesignStudy.hpp.

  \todo An overhaul of the test-macros (like OKLIB_TEST_EQUAL) seems to be needed.

  \todo OKLIB_TEST_RETHROW_NO_LOG :
    Invoking a sub-test but discarding the output. (It seems we should have that, or?)
    Just changing the definition of OKLIB_TEST_RETHROW by replacing log
    by a null-stream needs a global null-stream object ?!

  \todo Once the transition is done, rename "TestExceptions_DesignStudy.hpp" to "TestExceptions.hpp", and check the doxygen-documentation.
*/

