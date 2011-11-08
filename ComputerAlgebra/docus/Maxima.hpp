// Oliver Kullmann, 5.1.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/Maxima.hpp
  \brief Central docus-file regarding Maxima/Lisp


  <h1> Maxima/Lisp in the OKlibrary </h1>


  <h2> Calling the supported Maxima console </h2>

  <ul>
   <li> For questions related to building Maxima see
   Buildsystem/ExternalSources/SpecialBuilds/docus/Maxima.hpp. </li>
   <li> Via <code>oklib --maxima</code> (see
   Buildsystem/MasterScript/docus/general.hpp for general information on the
   masterscript "oklib") a Maxima-console is opened, with
   support for loading Maxima-components from the OKlibrary:
    <ol>
     <li> By <code>oklib_load_all();</code> then all Maxima-functions in the
     OKlibrary are loaded. </li>
     <li> More general, via <code>oklib_load(filename)</code> a specific
     module is loaded:
      <ul>
       <li> "filename" here is, as with the C++ system, a relative path
       starting with "OKlib". </li>
       <li> At each level files "include.mac" are provided to include
       whole (super-)modules. </li>
       <li> So "oklib_load_all()" just is
       <code>oklib_load("OKlib/ComputerAlgebra/include.mac")</code>. </li>
       <li> But also specific Maxima-files can be loaded. </li>
      </ul>
     </li>
    </ol>
    Further parameter can be provided (they are passed on to Maxima),
    so for example by
    \verbatim
oklib --maxima --batch=FILE
    \endverbatim
    the file FILE is batch-processed by Maxima.
   </li>
   <li> As can be seen from the make-output, "oklib --maxima" just calls the
   Maxima console provided with initial definitions.
    <ol>
     <li> The template for these initial definitions is the file
     <code>$(OKbuildsystem)/MasterScript/SpecialProcessing/maxima-init.mac</code>.
     </li>
     <li> As usual for such templates, configuration variables are accessed
     via "m4_SHELL(variable)". </li>
    </ol>
   </li>
   <li> The redefinition-warnings shown when starting Maxima in this way
   (and using CLisp instead of Ecl) are about a change in behaviour when
   parsing files. Likely this doesn't affect usage, but here are the
   details:
    <ol>
     <li> By default, "oklib_load" and "oklib_include" (see below)
     do not provide debugging information for the parsed functions. </li>
     <li> This saves run-time and storage space. </li>
     <li> If the debugging information (filename and fileline of the
     creating function for list-values) is needed, then it can be
     activated by setting "oklib_load_annotation" to true. </li>
     <li> The standard functions (like "load") are not affected (they
     always provide annotations). </li>
     <li> See the discussion of "oklib_load_annotation" below for further
     information. </li>
    </ol>
   </li>
   <li> If you have in your home directory a directory ".maxima", containing
   a file "maxima-init.mac", then this file will be used <em>instead</em>
   of the OKlibrary initialisation file:
    <ol>
     <li> So in order to work with the OKlibrary, normally you should not
     have a file "~/.maxima/maxima-init.mac". </li>
     <li> However, if you have special needs then using
     "~/.maxima/maxima-init.mac" might be what you want. </li>
    </ol>
   </li>
  </ul>


  <h2> Preloaded functionality </h2>

  <ul>
  <li> For all the details, see the OKlib-Maxima initialisation file
  <code>$(OKbuildsystem)/MasterScript/SpecialProcessing/maxima-init.mac</code>.
  </li>
   <li> Inclusion and loading of files:
    <ol>
    <li> Inside a Maxima-file (in the context of the OKlibrary), inclusion of
    other Maxima-files happens via <code>oklib_include(filename)</code>,
    where filename is the relative path starting with "OKlib".
     <li> In this way multiple inclusions are avoided. </li>
     <li> To avoid multiple inclusions for Maxima modules, use
     <code>oklib_plain_include(modulename)</code> (which does not augment the
     filename). </li>
     <li> On the other hand, via <code>oklib_load(filename)</code> it is
     guaranteed that the specified file is re-loaded (while all other
     includes are again guarded against multiple inclusions). </li>
     <li> Loading a file in batch mode (re-loading it and showing the
     execution of every line) happens via <code>oklib_batch(filename)</code>.
     </li>
     <li> Using <code>oklib_batch</code> is recommended for executing tests
     (that is, loading files from directories <code>testobjects</code>). </li>
     <li> And loading a demo (additionally to "batch" this allows for
     interaction) happens via <code>oklib_demo(filename)</code>.
    </ol>
   </li>
   <li> Access to configuration-variables:
    <ol>
     <li> From a Maxima-session, via "system(string_with_shell_code)" one can
     perform system calls, however to obtain the value of a variable,
     one has to use 'system("echo ${Var}")' and copy the output
     from the screen. </li>
     <li> In this way all configuration-variables from the OKlibrary
     build system are available. </li>
     <li> For convenience <code>OKplatform</code> and <code>OKsystem</code>
     are directly defined as variables. </li>
    </ol>
   </li>
   <li> Storing results at given time intervals:
    <ol>
     <li> The variable "oklib_session_name" contains the name used for
     storing results in a file via "oklib_save". </li>
     <li> Calling <code>oklib_save(arguments)</code> triggers the following
     actions:
      <ol>
       <li> If "oklib_store" is false, then nothing happens. </li>
       <li> If not "oklib_storage_interval"-many run-time minutes have elapsed
       since the last file-storage (or since the start of the session). then
       nothing happens. </li>
       <li> Otherwise, if you used "oklib_save()" then all variables in
       the environment of this call will be saved to file, while if you
       used for example "oklib_save('a,'b)", then only the values of
       variables a and b will be saved. </li>
      </ol>
     </li>
     <li> Via "load(filename)" these variable-values are made accessible in a
     Maxima-session. </li>
    </ol>
   </li>
   <li> Logging of Maxima-output:
    <ol>
     <li> Calling <code>oklib_log(string)</code> starts logging into file
     "oklib_session_name_string" (name as above for saving results, plus
     underscore and additional specification via string; if this file already
     exists, then it is emptied). </li>
     <li> Computation of term t is then best done via
     <code>display(t)$</code>, so that also the input is shown. </li>
     <li> Closing the logging_process by <code>oklib_log_close()</code>. </li>
    </ol>
   </li>
   <li> Variable <code>oklib_test_level</code> controls the test levels,
   similar to the C++ higher-order unit test system:
    <ol>
     <li> <code>oklib_test_level = 0</code> means the "basic" test level (for
     permanent testing). </li>
     <li> <code>oklib_test_level = 1</code> means the "full" test level (to be
     used before submitting to the central repository). </li>
     <li> <code>oklib_test_level = 2</code> means the "extensive" test level
     (to be used before a new release). </li>
     <li> <code>oklib_test_level</code> may have higher values. </li>
     <li> To run absolutely all tests, use <code>oklib_test_level = inf</code>.
     </li>
    </ol>
   </li>
   <li> The boolean variable <code>oklib_load_annotation</code> governs whether
   functions loaded by <code>oklib_load</code> and <code>oklib_include</code>
   annotate their results:
    <ol>
     <li> By default, we turn off ("false") annotation. </li>
     <li> When running the tests, it should be turned on ("true"); this
     debugging information is the reason for the default annotation by
     (plain) Maxima. </li>
     <li> When annotation is turned on (the default for Maxima),
     then every list created by some function coming from a file
     is internally stored with an annotation including the complete path
     for the file and the related line-number. </li>
     <li> When saving results to a file, this can cause a huge increase in
     file size. </li>
     <li> This annotation is hard-coded into the functions when they are
     parsed. </li>
     <li> We redefine the relevant functions in
     ComputerAlgebra/MaximaInternals/optload.lisp. </li>
     <li> With new Maxima versions one needs to check whether the corresponding
     functions in nparse.lisp and mload.lisp have changed, and accordingly
     must update the redefining functions. </li>
    </ol>
   </li>
  </ul>


  <h2> Documentation in general </h2>

  <ul>
   <li> For general Maxima-documentation see
   $(maxima_html_documentation_index_location_tag_okl). All further
   documentation below is on the Maxima-library provided by the OKlibrary.
   </li>
   <li> How to write programs at the Maxima/Lisp level in the OKlibrary
   is discussed in ComputerAlgebra/docus/MaximaTechniques.hpp. </li>
   <li> For a tutorial see ComputerAlgebra/docus/MaximaTutorial.hpp. </li>
   <li> The demonstrations (in <code>demos</code>-subdirectories) can be run
   via <code>oklib_demo(filename)</code> (as to be found in the preamble of the
   respective demos-file). After showing each expression to be evaluated,
   Maxima waits for the user to press the cr-key. </li>
   <li> Another possibility to load a file with protocolling of the expressions
   to be executed is to use <code>oklib_batch</code>, which, different from
   oklib_demo, does not pause after each expression. </li>
  </ul>


  <h2> Docus for specific modules </h2>

  <ul>
   <li> %Combinatorics : ComputerAlgebra/Combinatorics/Lisp/docus/general.hpp
   </li>
   <li> %Graphs : ComputerAlgebra/Graphs/Lisp/docus/general.hpp </li>
   <li> Linear algebra : ComputerAlgebra/LinearAlgebra/Lisp/docus/general.hpp
   </li>
   <li> Satisfiability : ComputerAlgebra/Satisfiability/Lisp/docus/general.hpp
   </li>
  </ul>


  <h2> Building a new Maxima package </h2>

  <ol>
   <li> Assume for example that the current version is 5.17.1, and that
   want to create an update (from CVS), which we name 5.17.1.1. (Further
   updates would be called 5.17.1.2 etc.) </li>
   <li> Now the creation of a package "maxima-5.17.1.1.tar.gz" happens
   as follows. </li>
   <li> Download the appropriate sources; the newest sources by
   \verbatim
> cvs -z3 -d:pserver:anonymous@maxima.cvs.sourceforge.net:/cvsroot/maxima co -P maxima
   \endverbatim
   </li>
   <li> Create an advance in the version number in the fourth digit, by
   changing the line with "AM_INIT_AUTOMAKE" in maxima/configure.in. In our
   example, where the current version is 5.17.1, the new entry is
   \verbatim
AM_INIT_AUTOMAKE(maxima,5.17.1.1)
   \endverbatim
   </li>
   <li> Update information, and create the package: </li>
   \verbatim
cd maxima
./configure
make dist
   \endverbatim
   The package then needs to moved to its proper place </li>
   <li> The program xgettext is needed for that. </li>
  </ol>

*/

