// Matthew Henderson, 14.11.2006 (Swansea)

/*!
  \file Buildsystem/Generic/documentation_building/plans/documentation_index.mak.hpp
  \brief Plans for the documentation index makefile

  \bug This file should be renamed to documentation_index.hpp.

  \todo Boost-documentation
  <ul>
   <li> Include documentation for version 1_34_0:
    <ol>
     <li> 15 of the 16 pages are loaded quickly, while the last takes very long. ?? </li>
     <li> Use the recommended version number. DONE </li>
    </ol>
   </li>
   <li> We should make available documentation for all supported versions.
   (This is useful for transition periods.) But the main documentation is
   for the recommended version (the rest is provided by links). </li>
  </ul>

  \todo GCC-documentation
  <ul>
   <li> Include documentation for version 4.2.0 (make sure that everything
   is linked).
    <ol>
     <li> Find all the root-pages. </li>
     <li> Use the recommended version number. </li>
    </ol>
   </li>
   <li> We should make available documentation for all supported versions.
   However the main documentations should be for the recommended version,
   while for the other pages we provide links. </li>
  </ul>  

  \todo Page organisation
  <ul>
   <li> Further links:
    <ol>
     <li> Internet page </li>
    </ol>
   </li>
   <li>  On my laptop the columns are arranged in two rows (independent of
   the horizontal space available (a lot)). So something with the dimensions
   seems to be wrong.
     OK: I changed some settings, so that it works now on my laptop, but
     not the three columns with the links are not centered anymore on
     cs-wsok (they appear left-aligned to the subtitle)?? </li>
   <li> Using Mozilla or Firefox the arrangement is completely different (and
     not as wished) ?? </li>
   <li> What kind of style-sheet is used?? Syntax like "#content" I (OK)
   cannot find in any documentation?? </li>
   <li> In documentation_index_style.css one finds hard-coded dimensions ---
   this doesn't look right. </li>
   <li> Documentation is needed (in the form of inline-comments for the
   css-file). </li>
  </ul>

  \todo Using css:
  <ul>
   <li> Is it really worth it? The main page of the OKlibrary documentation
   likely will forevery just stay one page, so the generality enabled by css
   will likely never be used. But it seems that creating web pages is much
   more complicated in the css-way?
   What tools exist to create easily such web pages? And how to check them??
   </li>
   <li> Perhaps we have a few general style-elements, and the rest is
   ordinary html?? </li>
   <li> There is the general problem of how to create these web-pages
   (based on make-variables): Perhaps asciidoc can help? </li>
   <li> The question arises, why to create the index-page at all??
   It would be much easier to just create a fixed (version-controlled)
   web-page, which is adjusted accordingly?!! </li>
  </ul>

  \todo Meta tags:
   - All the documentation should be available (finally) on the web; thus the index page will become
     a publically available web page, and needs all the meta tags, and also all the other declarations
     available, about encoding, last change date etc..

  \todo Improved logo:
   - yet the colour is a bit too lavenderish (a touch of darker blue perhaps)
   - perhaps a tiny bit of space between "OK" and "library" (looks squeezed)
   - some comments on how the logo has been created are needed.

*/
