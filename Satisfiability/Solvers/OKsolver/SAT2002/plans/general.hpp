// Oliver Kullmann, 17.8.2007 (Swansea)

/*!
  \file Solvers/OKsolver/SAT2002/plans/general.hpp
  \brief Plans on the maintenance of the code for the
  old OKsolver


  \bug Uninitialised values with optimised inlined versions
  <ul>
   <li> On cs-wsok, from the 2*3=6 executables for the OKsolver,
   when run on the example data/uuf250-011.cnf, exactly those two inlined
   "Gesamt" and "GesamtOKs" and compiled with optimisation fail (apparently
   get into an infinite loop; the other programs produce the same
   results, and valgrind can find no fail with them), yielding
   \verbatim
bin> valgrind ./Gesamt-O3-DNDEBUG uuf250-011.cnf
==30110== Conditional jump or move depends on uninitialised value(s)
==30110==    at 0x407597: Reduktion1 (in /home/csoliver/SAT-Algorithmen/OKplatform/system_directories/bin/Gesamt-O3-DNDEBUG)
==30110==    by 0x409E95: main (in /home/csoliver/SAT-Algorithmen/OKplatform/system_directories/bin/Gesamt-O3-DNDEBUG)
   \endverbatim
   There could be further uninitialised values, or it could be a compiler
   but (but suspicious that it's again in Reduktion1). The correct result is
   \verbatim
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=3 initial_number_of_variables=250 initial_number_of_clauses=1065 initial_number_of_literal_occurrences=3195 running_time(s)=8.6 number_of_nodes=9681 number_of_single_nodes=0 number_of_quasi_single_nodes=0 number_of_2-reductions=65579 number_of_pure_literals=1402 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=25 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=45382 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=0 file_name=uuf250-011.cnf
   \endverbatim
   To emphasise, the program OK runs correct in both versions (also OK-O3-DNDEBUG)).
   </li>
  </ul>


  \todo Tests
  <ul>
   <li> Test the example file of Marijn Heule, on cs-wsok and csltok. </li>
   <li> Now under SAT2002/data. </li>
  </ul>


  \todo Language standards
  <ul>
   <li> Perhaps move everything to C++ (but no real changes to any data structures, etc.,
   only using C++ header files etc.). Or?? </li>
   <li> Why does GesamtOKs.cpp want to be a C++ program --- maybe we just stick to C?
   Would be more honest! On the other hand, we are more knowledgeable with C++.) </li>
   <li> The program uses typical C-methods to simulate abstract data types (a functional
   interface is build, hiding all pointer access, which hapens in the implementation files).
   So it appears that we better stick to C. </li>
   <li> Eliminate all warnings. </li>
   <li> Use standard include-guards. </li>
   <li> Why are there these comments that string.h is included because of C++? Seems we should
   remove them. </li>
   <li> For the xml-output we should then state that C is the programming language. </li>
   <li> All includes need to be changed to the library-style. Or? Perhaps, due to the
   exceptional "historical" character, we don't do this here? Emphasising, that there
   are no reusable components here?! </li>
   <li> Deeper changes like const-introduction only later. </li>
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
     <li> OK.c : standard C program (assumes linking with the other compilation units) </li>
     <li> Gesamt.c : Same as OK.c, but includes all other compilation units, and thus
     needs no linking. </li>
     <li> GesamtOKs.cpp : Produced by UebersetzungOKs.plx out of all the compilation units
     (and is treated as C++). (However OK.h and Parameter.h are not inserted, but included.) </li>
    </ol>
   <li> There should be no need anymore for the created file GesamtOKs.cpp, however we need some
   tests on the running times of GesamtOKs, Gesamt and OK, and this also compared with
   the existing OKsolver-binaries. Optimally, we could do this simply with our build system
   (as planned), but perhaps for the time being we just do some ad-hoc testing.
   </li>
   <li> The difference between OK.c and Gesamt.c is that the latter enables more inlining ---
   can't this be achieved otherwise (through inline-specifications)? </li>
   <li> The build system links the C programs as C++ programs; shouldn't make a big
   difference for now, but should be rectified with the new system. </li>
   <li> The build system doesn't know about the dependency of OK.c on the other .c-programs
   (to which it links) --- this needs to be adressed! </li>
  </ul>


  \todo Complete the help facilities of the OKsolver
  <ul>
   <li> There are quite a few options availabe, and these should be documented. </li>
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


  \todo Add asserts


  \todo Write application tests


  \todo Investigate unit-testing
  <ul>
   <li> Optimally, we write unit tests for all main components. </li>
   <li> Possibly, the code doesn't allow easy unit testing? </li>
  </ul>


  \todo Apply time-measurements
  <ul>
   <li> Apply the time-measurement-system (assuming it is available by now). </li>
  </ul>


  \todo Use const-qualification
  <ul>
   <li> Where possible, const-qualification should be applied. </li>
  </ul>


  \todo Use restrict-qualification
  <ul>
   <li> We should investigate at which places this pointer-qualification can be used. </li>
  </ul>


  \bug Uninitialised values with the old OKsolver : DONE (when initialising
  the variables, then now also all are set to "unassigned")

  Running the OKsolver (for example the executable "Gesamt", but the same arises with
  the two other versions "OK" and "GesamtOKs") with valgrind we get
  \verbatim
system_directories/bin> valgrind ./OK ${OKPLATFORM}/OKsystem/Transitional/QuantumPhysics/data/Peres33PointConfiguration.cnf
...
==8114== Conditional jump or move depends on uninitialised value(s)
==8114==    at 0x804AC12: Reduktion1 (Reduktion.c:501)
==8114==    by 0x804C832: SATEntscheidung (OK.c:481)
==8114==    by 0x804FA94: main (OK.c:1379)
   \endverbatim
   The first guess is that the data member "belegt" is not properly (0-)initialised (apparently this
   did not happen with older versions of gcc). Perhaps we eliminate first all warnings. How does valgrind know about the uninitialised values?
   Attaching gdb and inspecting the value of v does show anything?

*/


