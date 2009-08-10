// Matthew Gwynne, 4.8.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AES/plans/BreakingAES_R2-6.hpp
  \brief On investigations into breaking reduced round variants of the AES


  \todo Breaking 2 round AES using the canonical translation for Sbox and Mul
  <ul>
    <li> A CNF representing a 2 round AES variant (1 round and the final round)
    can be generated in the following way:
    \verbatim
:lisp (ext:set-limit 'ext:heap-size 3000000000)
:lisp (ext:set-limit 'ext:frame-stack 10000)
:lisp (ext:set-limit 'ext:c-stack 200000)
:lisp (ext:set-limit 'ext:lisp-stack 200000)


oklib_load_all()$

aes_sbox_cp : aes_sbox_ts_cp$
aes_mul3_cp : aes_mul3_ts_cp$

aes_mul2_cp : aes_mul2_ts_cp$
aes_mul9_cp : aes_mul9_ts_cp$
aes_mul11_cp : aes_mul11_ts_cp$
aes_mul13_cp : aes_mul13_ts_cp$
aes_mul14_cp : aes_mul14_ts_cp$

aes_num_rounds : 2$

block([oklib_monitoring : true], F : aes_cnf_fcs())$
output_fcs("AES - 2 Rounds - Using canonical translations for Sbox etc.", F, "AES_r2_ts.cnf")$
    \endverbatim
    </li>
  </ul>
  
  \todo Breaking 2 round AES using Sbox and Mul translations with no new 
  variables
  <ul>
   <li> A CNF representing a 2 round AES variant (1 round and the final round)
   can be generated in the following way:
   \verbatim
:lisp (ext:set-limit 'ext:heap-size 3000000000)
:lisp (ext:set-limit 'ext:frame-stack 10000)
:lisp (ext:set-limit 'ext:c-stack 200000)
:lisp (ext:set-limit 'ext:lisp-stack 200000)


oklib_load_all()$
aes_num_rounds : 2$

block([oklib_monitoring : true], CS : aes_cnf_fcs())$
output_fcs("AES - 2 Rounds - No new variables", F, "AES_r2_nnv.cnf")$
   \endverbatim
   </li>
  </ul>

*/
