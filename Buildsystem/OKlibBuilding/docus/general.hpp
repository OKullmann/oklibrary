// Oliver Kullmann, 31.3.2007 (Swansea)

/*!
  \file Buildsystem/OKlibBuilding/docus/general.hpp
  \brief General documentation for the central makefile Buildsystem/OKlibBuilding/Makefile

  <h1> Make targets </h1>

  <h2> Applications </h2>

  <table>
   <tr>
    <td> <code> all </code> </td>
    <td>  First call <code>unoptimised</code>, then call <code>optimised</code>. </td>
   </tr>
   <tr>
    <td> <code> unoptimised </code> </td>
    <td>  Compile unoptimised object files and programs (including test-programs for the old test system, but not for the new one). </td>
   </tr>
   <tr>
    <td> <code> optimised </code> </td>
    <td>  Compile optimised object files and programs (including test-programs for the old test system, but not for the new one). </td>
   </tr>
  </table>


  <h2> %Test system </h2>

  <h3> The old test system (deprecated) </h3>

  <table>
   <tr>
    <td> <code> check </code> </td>
    <td> First call <code>test</code>, then call <code>testop</code>. </td>
   </tr>
   <tr>
    <td> <code> test </code> </td>
    <td> Perform unoptimised tests (recompiling code if necessary). </td>
   </tr>
   <tr>
    <td> <code> testop </code> </td>
    <td> Perform optimised tests (recompiling code if necessary). </td>
   </tr>
  </table>

  <h3> The new test system </h3>

  <table>
   <tr>
    <td> <code> new_check </code> </td>
    <td> First call <code>new_test</code>, then call <code>new_testop</code>. </td>
   </tr>
   <tr>
    <td> <code> new_test </code> </td>
    <td> Perform unoptimised tests (recompiling code if necessary). </td>
   </tr>
   <tr>
    <td> <code> new_testop </code> </td>
    <td> Perform optimised tests (recompiling code if necessary). </td>
   </tr>
  </table>

  <h3> Application tests </h3>

  <table>
   <tr>
    <td> <code> app_tests </code> </td>
    <td> Performs (unconditionally) application tests. </td>
   </tr>
  </table>


  <h2> Documentation </h2>

  <table>
   <tr>
    <td> <code> html </code> </td>
    <td> Creates the complete doxygen-documentation (wherever called). </td>
   </tr>
   <tr>
    <td> <code> internet </code> </td>
    <td> Creates the Internet pages (wherever called). </td>
   </tr>
  </table>


  <h2> Cleaning </h2>

  <h3> Regarding the old test system </h3>

  <table>
   <tr>
    <td> <code> cleantest </code> </td>
    <td> Removes the test timestamp file for the current module.
         The next <code> make test </code> will re-run the tests for the current module. </td>
   </tr>
   <tr>
    <td> <code> cleantestop </code> </td>
    <td> Removes the optimised test timestamp file for the current module.
         The next <code> make testop </code> will re-run the tests for the current module. </td>
   </tr>
   <tr>
    <td> <code> cleanalltest </code> </td>
    <td> Calls first <code>cleantest</code>, then <code>cleantestop</code>. </td>
   </tr>
  </table>

  <h3> Regarding the new test system </h3>

  <table>
   <tr>
    <td> <code> new_cleantest </code> </td>
    <td> Removes the test timestamp file for the current module. The next
         <code> make new_test </code> will re-run the tests for the current module. </td>
   </tr>
   <tr>
    <td> <code> new_cleantestop </code> </td>
    <td> Removes the test timestamp file for the current module. The next
         <code> make new_testop </code> will re-run the tests for the current module. </td>
   </tr>
   <tr>
    <td> <code> new_cleanalltests </code> </td>
    <td> Calls first <code>new_cleantest</code>, then <code> new_cleantestop </code>. </td>
   </tr>
   <tr>
    <td> <code> cleantestobj </code> </td>
    <td> Removes all test object files and optimised test object files for the
         current module. </td>
   </tr>
   <tr>
    <td> <code> cleantestprograms </code> </td>
    <td> Removes all test program executables (optimised and unoptimised)
         for the current module. </td>
   </tr>
   <tr>
    <td> <code> cleanmessages </code> </td>
    <td>  Removes error, log and message files for the current module. </td>
   </tr>
  </table>

  <h3> General cleaning </h3>

  <table>
   <tr>
    <td> <code> cleanobj </code> </td>
    <td> Removes all object files and optimised object files for the current module.
         This includes the object-files from the old test-system
         (but not for the new one). </td>
   </tr>
   <tr>
    <td> <code> cleandep </code> </td>
    <td> Removes all dependency files for the current module. </td>
   </tr>
   <tr>
    <td> <code> clean </code> </td>
   <td> Calls <code>cleanobj, cleantestobj, cleandep, cleanalltest, new_cleanalltests</code>. </td>
   </tr>
   <tr>
    <td> <code> cleanprograms </code> </td>
    <td>  Removes all program executables (optimised and unoptimised) for the
          current module. For the old test system, this includes the test-programs,
          but not for the new test system. </td>
   </tr>
   <tr>
    <td> <code> cleanall </code> </td>
    <td> Calls <code>clean, cleanprograms, cleantestprograms, cleanmessages</code>. </td>
   </tr>
  </table>


  <h1> Customisation </h1>

  The following make-variables can be used (on the command line):
  <table>
   <tr>
    <td> <code> gcc_version_number=V </code> </td>
    <td> Use the locally installed gcc with version V (for example <code>gcc_version_number=4.2.0</code>); default value is the recommended version. </td>
   </tr>
   <tr>
    <td> <code> boost_version_number=V </code> </td>
    <td> Use the locally installed %boost with version V (for example <code>boost_version_number=1_34_0</code>); default value is the recommended version. </td>
   </tr>
   <tr>
    <td> <code> Test_tool=T </code> </td>
    <td> For example T=<code>"valgrind"</code> or T=<code>"valgrind --quiet"</code>; default value is T=<code>time -p</code> </td>
   </tr>
  </table>


  \todo Complete the information on customisation.
  \todo Once the move to the new test system has been completed, references to the
  old test system need to be removed.
*/

