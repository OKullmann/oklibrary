// Oliver Kullmann, 31.3.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

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
    <td> <code> doxygen_documentation </code> </td>
    <td> Creates the complete doxygen-documentation (wherever called). </td>
   </tr>
   <tr>
    <td> <code> documentation_index </code> </td>
    <td> Creates local html index pages (wherever called). </td>
   </tr>
   <tr>
    <td> <code> html </code> </td>
    <td> Calls <code>doxygen_documentation</code> and <code>documentation_index</code> (wherever called). </td>
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
    <td> Use the locally installed gcc with version V (for example
    <code>gcc_version_number=4.2.0</code>); default value is the
    recommended version. </td>
   </tr>
   <tr>
    <td> <code> boost_version_number=V </code> </td>
    <td> Use the locally installed %boost with version V (for example
    <code>boost_version_number=1_34_0</code>); default value is the
    recommended version. </td>
   </tr>
   <tr>
    <td> <code> Test_tool=T </code> </td>
    <td> For example T=<code>"valgrind"</code> or T=<code>"valgrind --quiet"</code>;
    default value is T=<code>time -p</code> </td>
   </tr>
  </table>


  <h2> Settings for the new test system </h2>

  The basic parameters are:
  <table>
   <tr>
    <td> <code> test_level=L </code> </td>
    <td> <code>L</code> is one of "basic", "full", "extensive"
    (the test effort); default is "basic" </td>
   </tr>
   <tr>
    <td> <code>error_level=l</code>, <code>message_level=l</code>,
    <code>error_level=l</code> </td>
    <td> <code>l</code> is one of 0,1,2 (the level of verbosity for
    the three output streams); default is 0 </td>
   </tr>
  </table>

  The three output streams with their meanings:
  <ul>
   <li> The <strong> error stream </strong> contains all output related to
   test failures. It is set by default to
   \verbatim
error_stream = "cerr|ofstream-w=$(error_file)|ofstream-w=$(log_file)"
   \endverbatim
   that is, error output goes at the same time to standard error, to the error
   file, and to the log file. </li>
   <li> The <strong> message stream </strong> contans all output which informs
   about the general sequence of tests (it is produced by the test system
   itself). It is set by default to
   \verbatim
message_stream = "cout|ofstream-w=$(message_file)|ofstream-w=$(log_file)"
   \endverbatim
   that is, message output goes at the same time to standard output, to the
   message file, and to the log file. </li>
   <li> The <strong> log stream </strong> contains all output specifically
   produced by the test functions to inform about the tests. It is set by default to
   \verbatim
log_stream = "ofstream-w=$(log_file)"
   \endverbatim
   that is, log output only goes to the log file. </li>
  </ul>
  The default values for the three output files are
  \verbatim
error_file = $(test_aux_dir)/Error
message_file = $(test_aux_dir)/Message
log_file = $(test_aux_dir)/Log
   \endverbatim
   where the current value of configuration variable "test_aux_dir" (for this
   run) is <code>$(test_aux_dir)</code>.

   <p>
   Finally for each stream the language can be set by <code>error_lang</code>,
   <code>message_lang</code> and <code>log_lang</code>; the default value is
   <code>en_GB</code>. </p>


  \todo Complete the information on customisation.
  \todo Once the move to the new test system has been completed, references to the
  old test system need to be removed.
*/

