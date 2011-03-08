// Matthew Gwynne, 8.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
  \brief On investigations into the Data Encryption Standard


  \todo Understanding DES
  <ul>
   <li> First implementations of encryption/decryption at the Maxima-level
   are needed. </li>
   <li> At least in two versions, as follows:
    <ol>
     <li> Following the specific descriptions of the various permutations
     and S-boxes. </li>
     <li> Emphasising the abstract flow information (abstracting away of the
     specific nature of the special functions involved). </li>
    </ol>
   </li>
   <li> We also need to think about different organisations of the round.
   Perhaps the key-involvement could happen at the beginning or end of a round,
   instead of in the middle. </li>
  </ul>


  \todo Basic translation
  <ul>
   <li> Just following the definitions, of course "ignoring" the permutations,
   that is, they just are interpreted as "rewirings". </li>
   <li> Since the key schedule thus now is basically trivial (only input key
   bits are used), all what remains are the S-boxes and binary additions. </li>
   <li> The S-boxes have 6 input and 4 output bits, and thus yield 10-bit
   boolean functions. </li>
   <li> These can be analysed completely, and we can study the various
   representations (at least minimum, canonical, r_0- and r_1-bases). </li>
   <li> Do we need also to include decryption? Can the basic scheme encrypt
   and decrypt by just unit-clause propagation? </li>
   <li> We should obtain a considerably smaller formula than what Massacci and
   Marraro obtained. To be comparable, for the minimum translation we also need
   two versions: one treating an S-box as one 6-to-4 bit function, and one
   treating it as 4 6-bit functions. </li>
  </ul>


  \todo r_2-bases of a round
  <ul>
   <li> It seems not effective, and also not needed for obtaining an r_1-base,
   to integrate the addition of 4 (selected) bits to the S-box output,
   yielding a 14-bit boolean function: this should create a lot of clauses,
   which are also obtained, as r_1-bases, by just translating the additions.
   </li>
   <li> However on the input-side of an S-box more interesting things happen:
   4 (selected) bits yield 6 bits by partial repetition, they are added with
   6 key bits, and yield the input of the S-box. This yields also a 14-bit
   boolean function, whose use should improve the above basic translation.
   </li>
   <li> Again we consider at least minimum, canonical and r_0-, r_1-based
   representations. </li>
   <li> We should obtain r_2-based representations of a round. </li>
  </ul>


  <li> Understanding the Massacci-Marraro translation
  <ul>
   <li> The reference is [SAT 2000, editors Gent, van Maaren, Walsh, pages
   343-375]. </li>
   <li> The article speaks about the "commercial version" ? Likely this means
   just full DES? </li>
   <li> It seems superfluous variables are used, since the "superstructure",
   the permutations and the whole structure, is not trivialised, but is
   represented by "logical" formulas. </li>
   <li> On pages 356-361 one finds accordingly strange considerations regarding
   various circuits. </li>
   <li> The S-boxes:
    <ol>
     <li> On page 351, last paragraph it says that just by unit-clause
     propagation encryption can be performed. </li>
     <li> However on pages 356-357 Espresso and minimisation is mentioned,
     in two versions, considering an S-box as a 6-to-4 bit functions and
     as 4 6-bit functions. </li>
     <li> Apparently they go for a minimum representation. </li>
     <li> If they use the 6-to-4 bit representation, then the output bit
     can't be inferred. </li>
     <li> But using the 4 6-bit functions, the output bit is trivially inferred
     since it's the only variable left. </li>
     <li> So that seems to be it. </li>
    </ol>
   </li>
  </ul>


  \todo More advanced treatments
  <ul>
   <li> It seems complicated to "break" the barriers between the rounds, but
   we should try. </li>
   <li> The point is that even if we have a very good representation of a
   round, the sixteen rounds together likely still have enough confusion
   power to make the problem very hard for a SAT solver. </li>
  </ul>

*/
