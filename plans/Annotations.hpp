// Oliver Kullmann, 24.6.2007 (Swansea)
/* Copyright 2007, 2008, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/plans/Annotations.hpp
  \brief Plans for the Annotations-part of the OKlibrary, where
  latex-documents are handled


  \bug Bibtex-problem
  <ul>
   <li> On cs-wsok OKsystems/Annotations does not use the local bibtex files,
   but the ones of the user. </li>
  </ul>


  \todo Partially moving repository Annotations:
  <ul>
   <li> Some parts should move to OKlib, namely
    <ol>
     <li> "FullDocumentation" : DONE </li>
     <li> the latex-support (needs clean-up) </li>
    </ol>
    OKlib/annotations shall be the basis for writing documents and even
    joint papers on the library.
   </li>
   <li> "History_OKsystem" seems obsolete, and should be removed. </li>
   <li> "Release-1_0" perhaps should move (in some form) to OKlib/plans. </li>
   <li> Research articles stay in the separate repository "Annotations" (only for internal
   use). </li>
  </ul>


  \todo Latex macros
  <ul>
   <li> Latex macros need to be shared between several places:
    <ol>
     <li> OKlib/Annotations </li>
     <li> The independent Annotations repository </li>
     <li> OK's own macros </li>
     <li> Other developers macros. </li>
    </ol>
   </li>
   <li> Perhaps at this level one needs to find out how to write a
   latex-"package" !
    <ol>
     <li> The doc-system should be used for documenting the macros. </li>
    </ol>
   </li>
   <li> What to do with Latex_macros/Basis.tex and
   Latex_macros/Definitionen.tex ? </li>
  </ul>


  \todo Bibtex-database
  <ul>
   <li> Bibtex-entries need to be shared between several places:
    <ol>
     <li> OKlib/Annotations </li>
     <li> The independent Annotations repository </li>
     <li> OK's own bibtex-database </li>
     <li> Other developers bibtex-database. </li>
    </ol>
    ?!?
   </li>
   <li> What to do with Latex_bib/EigArt.bib ? </li>
   <li> Bibtex-entries can easily be centralised, but what about the
   annotations? </li>
   <li> Is there some real bibtex-database-system ?
    <ol>
     <li> "Aigaion - A Web based bibliography management system"
     http://www.aigaion.nl/
      <ul>
       <li> Seems unfortunately not the right thing for us, since it is
       web-centric (while we need just local access). </li>
       <li> The web-server though could be installed (and accessed) locally.
       </li>
       <li> Perhaps more important, it seems centralised, which
       seems not to fit into our decentralised model? </li>
       <li> And then there is the whole user administration, which we
       don't need, and which perhaps creates problems? </li>
       <li> The display, organisation, import and export of bibliographical
       data is fine, but we need a system which works locally and decentralised
       (and where updates of bibliographies just happen via updating the
       OKlibrary). </li>
       <li> And the same item might have different annotations in different
       contexts. </li>
      </ul>
     </li>
     <li> "bibulus" (XML and Perl) http://www.nongnu.org/bibulus: Project
     apparently not completed. </li>
     <li> "MibibTex" (close to XSLT)
     http://lifc.univ-fcomte.fr/~hufflen/texts/mlbibtex/mlbibtex/mlbibtex.html:
     There are some publications regarding it, but can't find something
     usable. </li>
     <li> "BibTex++" (Java) http://bibtex.enstb.org: Looks quite experimental.
     </li>
    </ol>
   </li>
   <li> Since bibtex-entries typically don't need updates, the simplest
   solution is to have only minimal bibtex-information in
   OKlib/Annotations, the rest in Annotations, and just
   to update the .bib-files if needed. No sharing with for examples OK's own
   bibtex-database.
   </li>
  </ul>

*/

