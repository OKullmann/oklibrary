// Oliver Kullmann, 8.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file LegalIssues/plans/Licence.hpp
  \brief Plans regarding licences

  The basic decision is to use GPLv3 (http://www.fsf.org/licensing/licenses/gpl.html)


  \todo Problems with licence
  <ul>
   <li> Part of the licence must be that the full history is handed down. Is this
   guaranteed by the original text, or do we need to amend it?
    <ol>
     <li> E-mail was sent to FSL. </li>
    </ol>
   </li>
   <li> What about the copyright statements?
    <ol>
     <li> Initially every file has OK's copyright. </li>
     <li> But once other external developers have contributed essentially to some file,
     then perhaps they should also have copyright on this file (that is, the corresponding
     group leader)? Perhaps we should ask the FSF how to handle such situations. </li>
     <li> Perhaps for legal reasons it is not feasible to have such a fractured
     copyright-situation, but then at least perhaps in every plans-directory we have
     a files "contributors.hpp" where just the developers are mentioned which contributed
     to the module (they enter themselves)? </li>
     <li> The possibility, that each files contains its own special copyright list,
     where each contributor enters himself, does not seem feasible to me due to the
     following reasons:
      <ol>
       <li> In each group, there is a hierarchy, and often students and research
       assistants do the work as instructed by their supervisors --- not it wouldn't
       be right that the student or assistant gets the copyright, but it must be
       the supervisor who gets the copyright --- now who shall control this?? </li>
       <li> How to make the distinction: Shall everybody who just corrects a little
       spelling mistake get also the copyright? In our situation, perhaps different
       from traditional (centralised) open-source development, everybody is
       encouraged (and enabled) to do tiny contributions. </li>
       <li> Especially this "continuous model" of tiny steps, a lot of them just
       regarding the documentation, is not compatible with the permanent trouble
       of updating copyrights. </li>
       <li> If in the future due to changed legislation etc. a licence change
       is needed to maintain the open source character, then only the single
       copyright guarantees that it can be done. </li>
       <li> The only thing that could go wrong is that OK, the single copyright holder,
       at some point changes the licence to a proprietary licence, and tries to get
       rich. So well, this possibility seems only fair to me, given that incomparable
       amount of work spent on the OKlibrary. The released code --- and there shall
       be continous releases will always stay free (of course). </li>
       <li> The possibility, that OK creates patents (which would disable others
       to use the library) is excluded by the licence --- if later the licence would
       be changed, then patents could only be issued on later code. </li>
       <li> According the the Berne convention, the contributors do not
       loose their "natural copyright" (as partial authors), it is only
       that I (OK) express *my* copyright. More information is needed here XXX </li>
       <li> Does it make a difference whether contributors submit files already
       with OK's copyright, or perhaps would it be better if the copyright
       would be left blank, and then OK's copyright is filled in? </li>
       <li> What about the contributor's file --- don't we run their into
       similar problems, who shall be entered? And what is the scope of
       contribution --- always the directory below it?? Seems to create also
       trouble. Perhaps we do without it, and leave it to the source control
       to log the contributors (would be more precise)? (But, of course, we
       have the central developers file.) </li>
       <li> Can we make the OKlibrary "officially" available? Or "more
       official"?? Perhaps we need a Sourceforge-account??? The Git-history should
       be well-secured. </li>
       <li> Having each file OK-copyright would emphasise the responsibility
       of OK for the unity of the library. A statement shall be made that OK tries
       his best to maintain and develop all code accepted into the library. </li>
       <li> Also the submissions which are not yet accepted are made visible somewhere. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> It seems LGPL is suitable; we have to check this (is it compatible
   with all other licenses (for external sources) involved? I guess so). DONE (for the research-platform, where everybody contributes ideas, we need stronger protection) </li>
  </ul>


  \todo Plans on license installation
  <ul>
   <li> Every file gets a short version as header, with copyright statement
   and reference to the main text. </li>
   <li> When external developers check in new files, then those must have the OK-copyright
   etc. clause (otherwise the submission is rejected). </li>
   <li> The head of every file would look like as follows:
   \verbatim
// Name of file creator, date of file creation (place of file creation)
// Copyright 20XX Oliver Kullmann
// This file is part of the OKlibrary.
//  OKlibrary is free software; you can redistribute it and/or modify it under the terms of
//  the GNU General Public License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//  The OKlibrary is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details. You should have received a copy of the
//  GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
   \endverbatim
   where "XX" is to be replaced by the current year. </li>
   <li> Where to put the GPL-text? Here. </li>
   <li> For every release, the year of the release must be added to every file's copyright notice
   (if not already present). </li>
   <li> In case of shell scripts, there are two leadings lines (one as above, and before it the
   specification of the shell). </li>
   <li> Data files don't get a licence. </li>
   <li> What about the plans-files? Also get a licence-statement, since they are as
   important as the rest. </li>
   <li> Can't we use a shortened header for very simple files?
   The following version looks more sensible:
   \verbatim
// Name of file creator, date of file creation (place of file creation)
// Copyright 20XX Oliver Kullmann
// This file is part of the OKlibrary.
//  OKlibrary is free software; you can redistribute it and/or modify it under the terms of
//  the GNU General Public License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
s/>.
   \endverbatim
   The above seems to make a fetish out of these "legal formalities". We should use this version
   for every file!
   </li>
   <li> Download GPLv3 and put under Configuration. DONE </li>
  </ul>


  \todo Licence installation
  <ul>
   <li> First, we should make all non-data files conform to the standard,
   that the first line shows the file-creator and file-creation-date:
    <ol>
     <li> DONE .c-files:
     \verbatim
Transitional> for F in $(find * -name "*.c"); do Z=$(head -1 ${F}); if [[ ! ${Z} =~ "// *" ]]; then echo ${F}; fi; done
     \endverbatim
     </li>
     <li> DONE .cpp-files:
     \verbatim
Transitional> for F in $(find * -name "*.cpp"); do Z=$(head -1 ${F}); if [[ ! ${Z} =~ "// *" ]]; then echo ${F}; fi; done
     \endverbatim
     </li>
     <li> DONE .h-files:
     \verbatim
Transitional> for F in $(find * -name "*.h"); do Z=$(head -1 ${F}); if [[ ! ${Z} =~ "// *" ]]; then echo ${F}; fi; done
     \endverbatim
     </li>
     <li> DONE .hpp-files: Extraction with
     \verbatim
Transitional> for F in $(find * -name "*.hpp"); do Z=$(head -1 ${F}); if [[ ! ${Z} =~ "// *" ]]; then echo ${F}; fi; done
     \endverbatim
     </li>
    </ol>
   <li> In one go, every files gets the licence statement.
    <ol>
     <li> First the C/C++-like files.
     \verbatim
Transitional> for F in $(find * -name "*.hpp" -or -name "*.cpp" -or -name "*.h" -or -name "*.c"); do System/LegalIssues/AddLicence1 ${F}; done
     \endverbatim
     </li>
    </ol>
   </li>
   <li> DONE (we use different tools for the different insertion cases;
   AddLicence1 is now ready to handle C/C++-like files)
   Can we write a little tool?
    <ol>
     <li> The simple shell scripts "AddLicense1/2", which take
     the first 1/2 lines, then insert the licence header, and
     then add the rest of the file, need more intelligence. </li>
     <li> First, whether 1 or 2 lines are taken at the beginning, should
     be automatically detected: 2 lines only if we find "#!/bin/bash"
     in the first line. </li>
     <li> From the line with the creation year XXXX we extract this year, and
     the copyright statement then becomes "Copyright XXXX-2007 Oliver Kullmann". </li>
     <li> And from the first line we also obtain the commenting-style: Either
     a C-style for the whole block, or a script-style comment for each line. </li>
    </ol>
   </li>
   <li> DONE (since we don't have much "dangerous intelligence" in the tool,
   we don't need confirmation)
   The tool should run through all files, each time showing the new beginning, asking
   for confirmation. </li>
   <li> DONE (we use the tool only if the file does not have the licence statement)
   If the file has already the licence statement, then possibly only the current year
   is added to the copyright statement. </li>
  </ul>


  \todo Licence documentation
  <ul>
   <li> Basic motivation for the GPL: Research platform. </li>
   <li> Create a special html-page, and link to it from the local documentation page. </li>
  </ul>

*/

