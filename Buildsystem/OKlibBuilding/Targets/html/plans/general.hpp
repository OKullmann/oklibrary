// Matthew Henderson, 15.11.2006 (Swansea)

/*!
  \file Buildsystem/OKlibBuilding/Targets/html/plans/general.hpp
  \brief Plans for the documentation-extraction from the source-code


  \todo Move
  <ul>
   <li> Currently a part of Buildsystem/OKlibBuilding, this make-functionality
   will move to Buildsystem/Html; see Buildsystem/plans/MasterScript.hpp. </li>
   <li> The target "html" will then be split into targets for the local home
   page and for the doxygen-system; this module here will be reponsible for
   the doxygen-system. </li>
  </ul>

  \todo Doxygen error messages:
  <ul>
   <li> Move aux/DoxygenErrorMessages to directory
   system_directories/log (and add suffix .txt; this filename
   is a configuration-variable). </li>
   <li> Perhaps the output of doxygen could be configured (only error
   messages, all output, etc.). </li>
  </ul>
  

  \todo Environment-variable expansion
  <ul>
   <li> Describe more precisely the features of "$(VAR)"-usage:
    <ol>
     <li> "$(VAR)" in a file to be processed by doxygen cannot occur
     on a "\par"-line. </li>
     <li> "$(VAR)" in the Doxyfile can appear within quotation marks. </li>
    </ol>
   </li>
   <li> Ask the doxygen-mailing list! </li>
   <li> Compare with "Definitions for doxygen" in Buildsystem/Configuration/plans/general.hpp. DONE </li>
   <li> Apparently doxygen replaces make-variable-evaluations
   like "$(source_libraries)" or "$(HOME)" by their value, except if these terms are in
   quotes (as here, to avoid evaluation) ?!?! This could be useful, but is not documented? DONE (decided to use this feature) </li>
  </ul>


  \todo Doxygen general
  <ul>
   <li> Instead of defining the exceptional macros by Doxyfile-tag PREDEFINED, we should use
     the tag EXPAND_AS_DEFINED (since we do not alter the definitions), however this
     does not work? </li>
   <li> In OKDatabase/CreateStatistic.cpp we have, within an unnamed namespace, the private
   member v of class Densities --- nevertheless doxygen claims, that at every other place,
   where an object is called v, this private member is referenced??? This looks like a
   clear doxygen-bug to me. </li>
   <li> Can doxygen tell which other files include a file (not in graph form, but in text form)? </li>
   <li> For functions there is a "callergraph" --- shall we use it? </li>
   <li> How to avoid that a leading "include" in a Doxygen-comment is interpreted as
     a doxygen-command? And how to avoid that apparently certain "keywords" are captured by
     doxygen? The keyword-interpretation can be avoided by prefixing them with "%". </li>
   <li> How to obtain general statistics: About the number of classes, lines of code, etc. (best
     with some statistics on the change over time)
     and then also on the version numbers of the modules (again with changes over time).
     If doxygen does not support it yet, then we should submit some feature request, and postphone
     this item. See what' in 1.5.3. </li>
   <li> It appears that all .cpp-files are considered as linked together? </li>
   <li> Can makefiles (or cmake-files) be incorporated?! </li>
   <li> What is the role of the doxygen configuration tag BUILTIN_STL_SUPPORT?
     Should we enable this? : Enabled; it should help --- check. </li>
   <li> Can we have nested lists? Numbered lists? These things should go into
     our general documentation --- or we have some example files, which demonstrate
     our use of doxygen :
     For simple lists we do it as here, otherwise we use the html-elements. Documented in the main documentation-document.
     However, how do they description-lists work? </li>
   <li> Doxygen can create "man pages" --- what does this %mean? </li>
   <li> See the doxygen-related problems in Messages (should be solvable by expanding those
     macros) : DONE </li>
   <li> How to integrate a *general* todo list into Doxygen? With xrefitem. However it seems we no longer need it : DONE </li>
   <li> Can doxygen extract a list with all modules and their current version numbers? Installed module_version : DONE </li>
   <li> We should get the newest configuration file, and transfer our information from the old configuration file : DONE </li>
  </ul>


  \todo Doxygen menu and main page review
  <ul>
   <li> Sorting of file/directory lists should distinguish between capital and small
   first letters (as it is possible with Unix/Linux), so that "plans" etc. comes at the end.
   </li>
   <li> Is the file list still needed (with version 0.1.4 of Transitional it
     was kind of useful to see for example how many files are documented)?
     Otherwise dropping it could make the system more user-friendly, by
     restricting the attention to the most appropriate choice? </li>
   <li> Sometimes it would be good to show also other files (especially for Buildsystem).
     This is mainly the issue of how to get doxygen showing makefiles etc. (perhaps
     just as plain text files, without any special formatting). </li>
   <li> The page "Module versions" should be sortable by version numbers (to see the most
   mature modules). Or how else can we (automatically) display the levels of development? </li>
  </ul>


  \todo Search engine
  <ul>
   <li> Doxygen can also use a "searchengine" (see activation in the "Doxyfile"):
   This would be very useful, but apparently needs some php-stuff? </li>
   <li> Moreover, a web server needs to be started? And then --- I installed php and
   appache on csltok, and put all pages to /srv/www/htdocs, but still konqueror just
   returns the search.php-file instead of doing something with it --- how should
   konqueror know that also for local files the web server should be used? </li>
  </ul>


  \todo Other code
  <ul>
   <li> We should also integrate (somehow) documentation of other code (for
   example all the computer-algebra code) into the doxygen-system (that is,
   giving access also to the code, not just to the plans). </li>
   <li> At least a simple "dump" should be possible ("as is"). </li>
   <li> Can doxygen be extended? We need some easy solutions. </li>
  </ul>


  \todo Directory structure: DONE (complete update)
  The directory structure under Buildsystem/Generic/documentation_building needs review:
  <ul>
   <li> Upper-case should be used (the names are not "systematic"). </li>
   <li> One nesting-level likely should be eliminated. </li>
  </ul>

*/
