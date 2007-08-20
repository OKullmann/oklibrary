// Oliver Kullmann, 17.8.2007 (Swansea)

/*!
  \file Solvers/OKsolver/SAT2002/plans/general.hpp
  \brief Plans on the maintenance of the code for the
  old OKsolver


  \todo Language standards
  <ul>
   <li> The old OKsolver is a C99 program. </li>
   <li> Use standard include-guards. </li>
   <li> Why are there these comments that string.h is included because of C++? Seems we should
   remove them. </li>
   <li> For the xml-output we should then state that C is the programming language. </li>
   <li> Where we touch the code, we change the style to the standard
   (otherwise we leave it). </li>
   <li> All includes need to be changed to the library-style. Or? Perhaps, due to the
   exceptional "historical" character, we don't do this here? Emphasising, that there
   are no reusable components here?! </li>
   <li> Deeper changes like const-introduction only later. </li>
   <li> DONE (stick to C) Perhaps move everything to C++ (but no real changes to any data structures, etc.,
   only using C++ header files etc.). Or?? </li>
   <li> DONE (GesamtOKs.cpp removed) Why does GesamtOKs.cpp want to be a C++ program --- maybe we just stick to C?
   Would be more honest! On the other hand, we are more knowledgeable with C++.)
   The purpose of GesamtOKs.cpp likely was to enable the use of C++ inlining --- but
   also C99 has the inline keyword. </li>
   <li>  DONE (stick to C) The program uses typical C-methods to simulate abstract data types (a functional
   interface is build, hiding all pointer access, which happens in the implementation files).
   So it appears that we better stick to C. </li>
  </ul>


  \todo Buildsystem (replacing Buildtools/UebersetzungOKs.plx)
  <ul>
   <li> That Perl-script had the following purpose:
    <ol>
     <li> Reflect the settings of macro, compilation-options and
     the choice of heuristics etc. in the name of the
     executable (with suffix ".sa"). </li>
     <li> Compile always a standard selection of combinations of
     macro-settings (for the different variants of the OKsolver). </li>
     <li> Create directories with a running number, containing
     the created executables, for ease of archiving. </li>
     <li> Take all compilation units, and insert them into one big
     stand-alone file "GesamtOKs.cpp". </li>
    </ol>
   </li>
   <li> We need to incorporate this functionality with our
   build system. </li>
   <li> Since we do not do any more experiments on variations of
   the old OKsolver, we do not need to create the sequence
   of directories anymore here. </li>
   <li> The compilation of different variants could be handled
   by creating files with the corresponding names to be created,
   making them symbolic links to the main program, while
   the compiler-options are set special for each of these variants. </li>
   <li> There are three programs:
    <ol>
     <li> OKsolver_2002.c : standard C program (assumes linking with the other compilation units) </li>
     <li> OKsolver_2002_lnk.c : Same as OKsolver_2002.c, but includes all other compilation units, and thus
     needs no linking. </li>
     <li> DONE (removed) GesamtOKs.cpp : Produced by UebersetzungOKs.plx out of all the compilation units
     (and is treated as C++). (However OK.h and Parameter.h are not inserted, but included.) </li>
    </ol>
   <li> There is no need anymore for the created file GesamtOKs.cpp, however we need some
   tests on the running times of GesamtOKs, OKsolver_2002 and OKsolver_2002_lnk, and this also
   compared with the existing OKsolver-binaries. Optimally, we could do this simply with our build system
   (as planned), but perhaps for the time being we just do some ad-hoc testing.
   </li>
   <li> The difference between OKsolver_2002.c and OKsolver_2002_lnk.c is that the latter enables more inlining ---
   can't this be achieved otherwise (through inline-specifications)? </li>
   <li> The build system links the C programs as C++ programs; shouldn't make a big
   difference for now, but should be rectified with the new system. </li>
   <li> The build system doesn't know about the dependency of OKsolver_2002.c on the other .c-programs
   (to which it links) --- this needs to be addressed!
    <ol>
     <li> For each program P, one additional dependency file has to be created which contains
     the rule with target P, where the prerequisites are all the files P links together. </li>
     <li> See the plans under Buildsystem. </li>
    </ol>
   </li>
   <li> Compilation of lokalesLernen.c on its own should only happen with LOKALLERNEN defined
   (without it a compile-time error should ensue). </li>
  </ul>


  \todo Complete the help facilities of the OKsolver
  <ul>
   <li> There are quite a few options available, and these should be documented. </li>
  </ul>


  \todo Add doxygen-documentation
  <ul>
   <li> We want to leave the code mainly as it is, but we can add doxygen
   documentation. </li>
  </ul>


  \todo Write docus-pages
  <ul>
   <li> Should explain the history and usage. </li>
   <li> Also the extension possibilities. </li>
  </ul>


  \todo Add asserts throughout


  \todo Create systematic application tests
  <ul>
   <li> Differentiate between solver runs which are expected to succeed and runs which
   are expected to fail (so that in the latter case the output to stderr can be suppressed). </li>
   <li> All possible syntactic errors diagnosed by the OKsolver need be checked. </li>
   <li> Tautological clauses and repeated literals in clauses need to be checked. </li>
   <li> Output of satisfying assignments needs to be checked. </li>
   <li> We need also to check the other output formats. </li>
  </ul>


  \todo Investigate unit-testing
  <ul>
   <li> Optimally, we write unit tests for all main components. </li>
   <li> Possibly, the code doesn't allow easy unit testing? </li>
  </ul>


  \todo Apply time-measurements
  <ul>
   <li> Apply the time-measurement-system (assuming it is available by now). </li>
  </ul>


  \todo Declare variable as close to their first usage as possible
  <ul>
   <li> Since C99 also allows to appear declaration everywhere, one
   should move many variable declarations to their first usage point. </li>
   <li> And, as with C++, one can now declare loop-variables inside the loop. </li>
  </ul>


  \todo Use const-qualification
  <ul>
   <li> Where possible, const-qualification should be applied. </li>
  </ul>


  \todo Use restrict-qualification
  <ul>
   <li> We should investigate at which places this pointer-qualification can be used. </li>
  </ul>


  \todo Correct counting
  <ul>
   <li> Counting for example test_cases/TwoUnit.cnf is not correct. </li>
   <li> See whether this can be easily rectified. </li>
   <li> Possibly this is not the case; then we need a precise specification what
   actually is counted, so that then the output can be declared as correct. </li>
  </ul>

*/


