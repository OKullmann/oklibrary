// Oliver Kullmann, 5.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/Maxima.hpp
  \brief Central docus-file regarding Maxima/Lisp


  <h2> Usage </h2>

  <ul>
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
       starting with "Transitional". </li>
       <li> At each level files "include.mac" are provided to include
       whole (super-)modules. </li>
       <li> So "oklib_load_all()" just is <code>oklib_load("Transitional/ComputerAlgebra/include.mac")</code>. </li>
       <li> But also specific Maxima-files can be loaded. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> Inside a Maxima-file (in the context of the OKlibrary) inclusion of
   other Maxima-files happens via <code>oklib_include(filename)</code>,
   where filename is the relative path starting with "Transitional". </li>
   <li> Access to configuration variables:
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
  </ul>

  <h2> Tutorial </h2>

  <ol>
    <li>Load all OKlibrary functions.
      \verbatim
(%i1) oklib_load_all();
0 errors, 0 warnings
(%o1)                                  1
      \endverbatim
    </li> 
    </li>Graphs
      <ol>
        <li>Creating a graph.
          \verbatim
(%i2) g:[{1,2,3},{{1,2},{1,3}}];
(%o2)                    [{1, 2, 3}, {{1, 2}, {1, 3}}]
          \endverbatim
        </li>
        <li>Basic graph operations.
          <ol>
            <li>Neighbourhood of a vertex:
              \verbatim
(%i3) neighbours(1,g);
(%o3)                               {2, 3}
              \endverbatim
            </li>
	    <li>Remove a vertex:
              \verbatim
(%i4) remove_vertices_graph({2},g);
(%o4)                        [{1, 3}, {{1, 3}}]
              \endverbatim
            </li>
          </ol>
        </li>
        <li>Graph generators
          \verbatim
(%i5) k:Kneser_graph(5,2);
(%o5) [{{1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 4},
{3, 5}, {4, 5}}, {{{1, 2}, {3, 4}}, {{1, 2}, {3, 5}}, {{1, 2}, {4, 5}},
{{1, 3}, {2, 4}}, {{1, 3}, {2, 5}}, {{1, 3}, {4, 5}}, {{1, 4}, {2, 3}},
{{1, 4}, {2, 5}}, {{1, 4}, {3, 5}}, {{1, 5}, {2, 3}}, {{1, 5}, {2, 4}},
{{1, 5}, {3, 4}}, {{2, 3}, {4, 5}}, {{2, 4}, {3, 5}}, {{2, 5}, {3, 4}}}]
          \endverbatim
        </li>
        <li>Graph output
          \verbatim
(%i6) print_graph_dot(k);
graph G {
e1e2;
e1e3;
e1e4;
e1e5;
e2e3;
e2e4;
e2e5;
e3e4;
e3e5;
e4e5;
e1e2 -- e3e4;
e1e2 -- e3e5;
e1e2 -- e4e5;
e1e3 -- e2e4;
e1e3 -- e2e5;
e1e3 -- e4e5;
e1e4 -- e2e3;
e1e4 -- e2e5;
e1e4 -- e3e5;
e1e5 -- e2e3;
e1e5 -- e2e4;
e1e5 -- e3e4;
e2e3 -- e4e5;
e2e4 -- e3e5;
e2e5 -- e3e4;
}
(%o6)                                 }
          \endverbatim
        </li>
      </ol>
    </li>
  </ol>
*/

