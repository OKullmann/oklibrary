// Oliver Kullmann, 8.7.2007 (Swansea)

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
       <li> According the the Berne convention, the contributors do not
       loose their "natural copyright" (as partial authors), it is only
       that I (OK) express *my* copyright. More information is needed here XXX </li>
       <li> Does it make a difference whether contributors submit files already
       with OK's copyright, or perhaps would it be better if the copyright
       would be left blank, and then OK's copyright is filled in? </li>
       <li> What about the developer's file --- don't we run their into
       similar problems, who shall be entered? And what is the scope of
       contribution --- always the directory below it?? Seems to create also
       trouble. Perhaps we do without it, and leave it to the source control
       to log the contributors (would be more precise)? </li>
       <li> Can we make the OKlibrary "officially" available? Or "more
       official"?? Perhaps we need a Sourceforge-account??? </li>
       <li> Having each file OK-copyright would emphasise the responsibility
       of OK for the unity of the library. </li>
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
   <li> Where to put the GPL-text? </li>
   <li> For every release, the year of the release must be added to every file's copyright notice
   (if not already present). </li>
   <li> Download GPLv3 and put under Configuration. DONE </li>
  </ul>


  \todo Licence installation
  <ul>
   <li> In one go, every files gets the licence statement. </li>
  </ul>


  \todo Licence documentation
  <ul>
   <li> Basic motivation for the GPL: Research platform. </li>
   <li> Create a special html-page, and link to it from the local documentation page. </li>
  </ul>

*/

