// Oliver Kullmann, 19.7.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp
  \brief Plans regarding installation of Sage


  \todo Build Sage
  <ul>
   <li> Manual build
    <ol>
     <li> Download source from:
     \verbatim
http://modular.math.washington.edu/sage/dist/src/sage-2.6.tar
     \endverbatim
     </li>
     <li> In ExternalSources/Sage extract:
     \verbatim
tar xvf sage-2.6.tar
     \endverbatim
     </li>
     <li> Change directory:
     \verbatim
cd sage-2.6
     \endverbatim
     </li>
     <li> Build and install locally:
     \verbatim
make
     \endverbatim
     </li>
     <li> Build documentation:
     \verbatim
cd devel/doc
make pdf
     \endverbatim
     </li>
     <li>
     %Test:
     \verbatim
cd ../..
make test
     \endverbatim
     </li>
    </ol>
   </li>
   <li> We should link at least to the latex-documentation. It speaks about
   it in the README-file, however one can find only index.html (at several
   places!)? Apparently the README-file is not up-to-date, and meanwhile the
   html-documentation became the default. With the above "make pdf" however
   the latex-documentation is generated, and we should also link to it. DONE </li>
   <li> Create a special documentation page for Sage (the internal-link of the local
   home page links then to this special page), which contains all links to html-index
   pages and pdf-files one can find in the Sage-directory. DONE </li>
   <li> No indication of the documentation about "Axioms" etc. ?? DONE (not in the package) </li>
  </ul>

*/
