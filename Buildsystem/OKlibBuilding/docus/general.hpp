// Oliver Kullmann, 31.3.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/OKlibBuilding/docus/general.hpp
  \brief General documentation for the central makefile Buildsystem/OKlibBuilding/Makefile

  <h1> Usage </h1>

  These targets are to be used together with the <code>oklib</code>-command
  (see Buildsystem/MasterScript/docus/general.hpp).


  <h1> Make targets </h1>


  <h2> Applications </h2>

  The compilation-targets apply to the current directory and everything below.

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

  The test-targets apply to the current directory and everything below.

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
    <td> First call <code>new_test</code>, then <code>new_testop</code>, then
    <code>maxima_test</code>. </td>
   </tr>
   <tr>
    <td> <code> new_test </code> </td>
    <td> Perform unoptimised tests (recompiling code if necessary). </td>
   </tr>
   <tr>
    <td> <code> new_testop </code> </td>
    <td> Perform optimised tests (recompiling code if necessary). </td>
   </tr>
   <tr>
    <td> <code> maxima_test </code> </td>
    <td> Run the tests for the Maxima/Lisp level. </td>
   </tr>
  </table>

  <h3> Application tests </h3>

  <table>
   <tr>
    <td> <code> app_tests </code> </td>
    <td> Performs (unconditionally) application tests. </td>
   </tr>
  </table>


  <h2> Local documentation </h2>

  The following targets are independent from the calling-directory.

  <table>
   <tr>
    <td> <code> doxygen_documentation </code> </td>
    <td> Creates the complete doxygen-documentation. </td>
   </tr>
   <tr>
    <td> <code> documentation_index </code> </td>
    <td> Creates local html index pages. </td>
   </tr>
   <tr>
    <td> <code> html </code> </td>
    <td> Calls <code>doxygen_documentation</code> and <code>documentation_index</code>. </td>
   </tr>
  </table>


  <h2> Cleaning </h2>

  The cleaning-targets apply to the current directory and everything below.

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


  <h1> Customisation of gcc and Boost </h1>

  Regarding the usage of the main compiler "gcc" and main external library
  "Boost", currently we have:
  <ul>
   <li> The current default is to use the locally installed gcc as well as the
   locally installed %boost (under $(ExternalSources)), each in the
   recommended version. </li>
   <li> Locally installed %boost versions are compiled with some locally
   installed gcc. </li>
  </ul>
  The versions of gcc and %boost can be changed as follows:
  <ul>
   <li> Either change the local version of %boost (typically only useful in
   transition periods), while the build system chooses the recommended
   local gcc. </li>
   <li> Or change the local version of gcc, while the build system
   chooses the appropriate local variant of the recommended %boost version. </li>
   <li> Or change both; in case the appropriate versions of gcc and Boost
   (the latter compiled with that gcc-version) need to available. </li>
  </ul>

  The make-variables to be set on the command-line are as follows:
  <table>
   <tr>
    <td> <code> gcc_recommended_version_number_okl=V </code> </td>
    <td> Use the locally installed gcc with version V (for example
    <code>gcc_recommended_version_number_okl=4.2.4</code>); default value
    is the recommended version. </td>
   </tr>
   <tr>
    <td> <code> %boost_version_number=V </code> </td>
    <td> Use the locally installed %boost with version V (for example
    <code>boost_version_number=1_34_0</code>); default value is the
    recommended version. </td>
   </tr>
  </table>

  <p /> Examples:
  <ul>
   <li>
    \verbatim
oklib all check new_check app_tests html boost_version_number=1_34_1 test_level=full
    \endverbatim
    is for using the locally installed Boost-library, where we first compile
    everything, then run all tests, and finally create the documentation,
    and this all with test-level "full". </li>
  </ul>


  <h1> Customisation of compilation, linking and testing </h1>

  By the following command-line settings compilation, linking and running tests
  can be influenced:
  <table>
   <tr>
    <td> <code> Compile_tool=T </code> </td>
    <td> Command to prefix each call of the compiler;
    default value is T=<code>"time -p"</code> </td>
   </tr>
   <tr>
    <td> <code> Link_tool=T </code> </td>
    <td> Command to prefix each call of the linker;
    default value is T=<code>"time -p"</code> </td>
   </tr>
   <tr>
    <td> <code> Test_tool=T </code> </td>
    <td> For example T=<code>"valgrind"</code> or T=<code>"valgrind --quiet"</code>;
    default value is T=<code>"time -p"</code> </td>
   </tr>
  </table>


  <h1> Settings for the new test system </h1>

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


  <h1> Internet pages </h1>

  The following targets are independent from the calling-directory.

  <table>
   <tr>
    <td> <code> internet_local </code> </td>
    <td> Creates the (movable) Internet web pages first under the local doc-directory. </td>
   </tr>
   <tr>
    <td> <code> internet_global </code> </td>
    <td> Moves the Internet-web-pages-directory from the local place to the "global" place. </td>
   </tr>
   <tr>
    <td> <code> internet_packages </code> </td>
    <td> Moves the content of the (local) packages-directory to their "global" place. </td>
   </tr>
   <tr>
    <td> <code> internet </code> </td>
    <td> Calls <code>internet_local</code>, <code>internet_global</code> and <code>internet_packages</code>. </td>
   </tr>
  </table>

  Remarks:
  <ol>
   <li> For the Internet web pages a version of the local documentation is
   created from scratch, with appropriate movable url's. </li>
   <li> The local directory with the (movable) Internet pages is
   "internet_html_dir" = <code>$(internet_html_dir)</code>. </li>
   <li> The directory providing the "global" place is
   "internet_dir" = <code>$(internet_dir)</code>. This variable is to be set
   in the configuration-override file. </li>
   <li> The local packages-directory is
   "packages_dir" = <code>$(packages_dir)</code>, the "global"
   packages-directory is "internet_dir/packages". </li>
   <li> When testing the internet pages and their creation with a local
   web server, one can proceed as follows, using the Apache web server and
   testing the central ok-library-web-page:
    <ol>
     <li> The default directory for placing web pages is
     <code>/usr/local/apache2/htdocs</code>. </li>
     <li> In this directory root creates the directory
     <code>ok-sat-library</code>, with group-ownership "users" and
     group-writable, but not accessible, readable or writable for "others".
     </li>
     <li> Now after running <code>oklib internet</code> (of course as a normal
     user) we get the Internet home page at
     "http://localhost/ok-sat-library/internet_html/index.html". </li>
     <li> The packages need to be created before (by
     <code>oklib --create-packages</code>). </li>
    </ol>
   </li>
  </ol>


  \todo Complete the information on customisation.
  \todo Once the move to the new test system has been completed, references to
  the old test system need to be removed.
*/

