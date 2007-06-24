// Oliver Kullmann, 24.6.2007 (Swansea)

/*!
  \file Transitional/plans/Annotations.hpp
  \brief Plans for the Annotations-part of the OKlibrary, where
  latex-documents are handled

  \todo Partially moving repository Annotations:
  <ul>
   <li> Some parts should move to Transitional, namely
    <ol>
     <li> "FullDocumentation" </li>
     <li> the latex-support (needs clean-up) </li>
    </ol>
    Transitional/Annotations shall be the basis for writing documents and even
    joint papers on the library.
   </li>
   <li> "History_OKsystem" seems obsolete, and should be removed. </li>
   <li> "Release-1_0" perhaps should move (in some form) to Transitional/plans. </li>
   <li> Research articles stay in the separate repository "Annotations" (only for internal
   use). </li>
  </ul>


  \todo Latex macros
  <ul>
   <li> Latex macros need to be shared between several places:
    <ol>
     <li> Transitional/Annotations </li>
     <li> The independent Annotations repository </li>
     <li> OK's own macros </li>
     <li> Other developers macros. </li>
    </ol>
   </li>
   <li> Perhaps at this level one needs to find out how to write a latex-"package" !
    <ol>
     <li> The doc-system should be used for documenting the macros. </li>
    </ol>
   </li>
   <li> What to do with Latex_macros/Basis.tex and Latex_macros/Definitionen.tex ? </li>
  </ul>


  \todo Bibtex-database
  <ul>
   <li> Bibtex-entries need to be shared between several places:
    <ol>
     <li> Transitional/Annotations </li>
     <li> The independent Annotations repository </li>
     <li> OK's own bibtex-database </li>
     <li> Other developers bibtex-database. </li>
    </ol>
   </li>
   <li> What to do with Latex_bib/EigArt.bib ? </li>
   <li> Bibtex-entries can easily be centralised, but what about the annotations? </li>
   <li> Is there some real bibtex-database-system ?
    <ol>
     <li> "bibulus" (XML and Perl) http://www.nongnu.org/bibulus </li>
     <li> "MibibTex" (close to XSLT) http://lifc.univ-fcomte.fr/~hufflen/texts/mlbibtex/mlbibtex/mlbibtex.html </li>
     <li> "BibTex++" (Java) http://bibtex.enstb.org </li>
    </ol>
   </li>
   <li> Useful bibtex-database-management tools?
    <ol>
     <li> http://www.tug.org/tex-archive/biblio/bibtex/utils/ </li>
     <li> http://bibliographic.openoffice.org/biblioo-sw.html </li>
     <li> pybliographer (python-tool) http://pybliographer.org </li>
     <li> JBibtexManagager (Java) http://jabref.sourceforge.net </li>
    </ol>
   </li>
   <li> What to do about online-repositories?
    <ol>
     <li> http://www.math.utah.edu:8080/pub/tex/bib/index-table.html </li>
     <li> http://liinwww.ira.uka.de/bibliography/index.html </li>
    </ol>
   </li>
  </ul>

*/

