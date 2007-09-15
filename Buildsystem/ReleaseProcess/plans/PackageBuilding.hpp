// Oliver Kullmann, 26.6.2007 (Swansea)

/*!
  \file Buildsystem/ReleaseProcess/plans/PackageBuilding.hpp
  \brief Plans regarding package building


  \todo General
  <ul>
   <li> Checking is separate, has to be done (fully) in advance
   see Buildsystem/plans/CheckBuildSystem.hpp. </li>
   <li> The appropriate "user clone" has to be created in advance
   (see Buildsystem/ReleaseProcess/plans/Release.hpp) --- and also this clone has
   been tested. </li>
   <li> Packages must be stored in an accessible depot. At least the
   core-packages need to be backed-up on cs-oksvr. See
   "ExternalSources repository" in Buildsystem/ReleaseProcess/plans/Release.hpp. </li>
   <li> Package data must be on the Internet-page
   (as automatical as possible).
    <ol>
     <li> md5-checksums </li>
     <li> construction dates and times </li>
    </ol>
   </li>
   <li> Also a ((semi-)automatical) e-mail to the notification list. </li>
   <li> Package meta-data:
    <ol>
     <li> In Configuration/ReleaseProcess we have the file "ReleaseHistory"
     with line-records of the form
     \verbatim
running-number version date SHA md5sum
     \endverbatim
     where
      <ol>
       <li> running number of left-padded with 5 digits (starting with "00001"), </li>
       <li> version is the respective value of make-variable "transitional_version", </li>
       <li> date is the respective value of make-variable "current_date", </li>
       <li> SHA is given by
       \verbatim
git log HEAD | head -1 | cut --fields=2 --delimiter=" "
       \verbatim
       </li>
       <li> md5sum is the md5sum of the finalised package. </li>
      </ol>
     </li>
     <li> The package name is for example
     \verbatim
OKlibrary-0.2.0.0_00001.tar.bz2
     \verbatim
     (we need the running number, since many packages will be created,
     and this is clearest to distinguish packages, and to decide which
     is later). </li>
    </ol>
   </li>
   <li> DONE (basic ideas are clear now; updated above)
   Packages are called like
   \verbatim
OKlibrary-0.2.0.8
   \endverbatim
   where "0.2.0" is the main version number and ".8" the current "stage"
   (the whole string "0.2.0.8" is given by make-variable "transitional_version").
    <ol>
     <li> So a new package is only created when at least the current stage
     is advanced; users who wish to by fully update use Git. </li>
     <li> Parts (without the external libraries etc.) should be suffixed
     like "-minimal". </li>
     <li> Regarding the date formatting, we should stick to this
     European form. DONE (no dates in the name; otherwise we use for
     example "04.09.2007") </li>
     <li> Should the name be "OKplatform" ? DONE (no, yet no use
     for OKplatform) </li>
     <li> DONE (no time included; this is available via the history)
     Should also the time be included? Likely not (it must not
     matter). </li>
    </ol>
   </li>
   <li> DONE (explained in more details in the next item)
   A package exists for every new version number of Transitional. </li>
   <li> DONE (question answered; to be entered into the full documentation)
   Shall we make a distinction between "major" and "minor" versions? Yes,
   in the documentation and announcements we make a differences between the
   four types of advances (related to the four digits): Advancing to
   "1.0.0.0" should be a "big thing", advancing to "0.3.0.0" still gets
   some publicity, advancing to "0.2.1.0" is announced on internal mailing
   lists, still with explanations what did change, while finally advancing
   to "0.2.0.4" only creates automated messages. </li>
   <li> DONE (no, we use standard package names, not such home-cooked)
   Packages need precise names; proposal (for the next release):
   \verbatim
OKlib_0.1.6_31072007
   \endverbatim
   this is, after the name comes the version number, and then the date
   (day) of packaging. </li>
   </ul>


  \todo The script
  <ul>
   <li> Synchronise the following with "Distributing the library" in
   Buildsystem/ReleaseProcess/plans/Release.hpp. </li>
   <li> DONE Name of this Bash script: "CreatePackage". </li>
   <li>  DONE The script is invoked by oklib, and thus all configuration data
   is available via the environment --- but for this another
   makefile ReleaseProcess/Makefile is needed. </li>
   <li> DONE (only the makefile gets parameters; thus all communication happens via make-variables)
   Syntax "oklib --create-package"; all further parameters are passed to
   CreatePackage. </li>
   <li> Main steps for the full package:
    <ol>
     <li> DONE In the current system_directories, create a package directory called for example
     "OKlibrary-0.2.0.0_000001" (extracting first the current version number,
     and incrementing the running number of the last entry in ReleaseHistory). </li>
     <li> DONE In the package directory (pd) do
          mkdir OKplatform OKplatform/OKsystem OKplatform/system_directories
          OKplatform/ExternalSources </li>
     <li> DONE Make a clone of the current repository in pd:OKplatform/OKsystem.
      <ol>
       <li> What kind of clone? Does it have the address of the mother-clone inside? </li>
       <li> It seems that
       \verbatim
git clone --no-hardlinks ${Transitional}
       \endverbatim
       should be reasonable; we have then always a clone of the working repository. </li>
       <li> As a parameter one can pass an argument to option "--origin", to specify
       a different repository as the "upstream" repository (to pull from).
       DONE (set git_upstream) </li>
       <li> And another parameter makes it possible to clone from a different repository.
       DONE (set get_repository) </li>
      </ol>
     </li>
     <li> DONE (via 'oklib --create-package ExternalSources=""' copying of external sources is prevented)
     Copy ExternalSources/sources to pd:ExternalSources; however it must be possible
     to leave this out. </li>
     <li> DONE Copy OKsystem/documents to pd:OKsystem. </li>
     <li> DONE Run pd:Buildsystem/SetUp.mak with target oklibrary_initialisation and with
     OKplatform set. </li>
     <li> DONE pd:oklib --prebuild </li>
     <li> pd:oklib html </li>
     <li> Create a symbolic link in pd:OKplatform to pk:oklib. </li>
     <li> Create the current README-file in pd:OKplatform; this contains the information
     that one can perform "oklib --setup" to create the link to oklib. </li>
     <li> Compress. </li>
     <li> Create log-entry in file ReleaseHistory. </li>
    </ol>
   </li>
   <li> So a complete package comes with documentation, and for the rest
   the standard build system can be used (no need for additional makefiles).
   However, there should be additional targets for the oklib-masterscript
   to "compile and test" everything. </li>
   <li> Variations:
    <ol>
     <li> "minimal" without external sources and html. </li>
     <li> "extern" only contains the external sources. </li>
     <li> "doc" only with the documentation. </li>
    </ol>
   </li>
   <li> DONE For this to work we need to make the internal url's relative.
   See "Install configuration system" in Buildsystem/Html/plans/general.hpp. </li>
  </ul>

*/
