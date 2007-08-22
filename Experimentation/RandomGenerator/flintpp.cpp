//*****************************************************************************/
//                                                                            */
//  Functions for arithmetic and number theory with large integers in C       */
//  Software supplement to the book "Cryptography in C and C++"               */
//  by Michael Welschenbach, published by Apress Berkeley CA, 2001            */
//                                                                            */
//  Module flintpp.cpp      Revision: 19.12.2000                              */
//                                                                            */
//  Copyright (C) 1998-2001 by Michael Welschenbach                           */
//  Copyright (C) 1998-2001 by Springer-Verlag Berlin, Heidelberg             */
//  Copyright (C) 2001      by Apress L.P., Berkeley, CA                      */
//                                                                            */
//  All Rights Reserved                                                       */
//                                                                            */
//  The software may be used for noncommercial purposes and may be altered,   */
//  as long as the following conditions are accepted without any              */
//  qualification:                                                            */
//                                                                            */
//  (1) All changes to the sources must be identified in such a way that the  */
//      changed software cannot be misinterpreted as the original software.   */
//                                                                            */
//  (2) The statements of copyright may not removed or altered.               */
//                                                                            */
//  (3) The following DISCLAIMER is accepted:                                 */
//                                                                            */
//  DISCLAIMER:                                                               */
//                                                                            */
//  There is no warranty for the software contained on this CD-ROM, to the    */
//  extent permitted by applicable law. The copyright holders provide the     */
//  software `as is' without warranty of any kind, either expressed or        */
//  implied, including, but not limited to, the implied warranty of fitness   */
//  for a particular purpose. The entire risk as to the quality and           */
//  performance of the program is with you.                                   */
//                                                                            */
//  In no event unless required by applicable law or agreed to in writing     */
//  will the copyright holders, or any of the individual authors named in     */
//  the source files, be liable to you for damages, including any general,    */
//  special, incidental or consequential damages arising out of any use of    */
//  the software or out of inability to use the software (including but not   */
//  limited to any financial losses, loss of data or data being rendered      */
//  inaccurate or losses sustained by you or by third parties as a result of  */
//  a failure of the software to operate with any other programs), even if    */
//  such holder or other party has been advised of the possibility of such    */
//  damages.                                                                  */
//                                                                            */
//*****************************************************************************/

#include "flintpp.h"

#if defined FLINTPP_ANSI
#define NOTHROW (nothrow)
#else
#define NOTHROW
#endif

#define NO_ASSERTS

#ifdef FLINT_DEBUG
#undef NO_ASSERTS
#define ASSERT_LOG_AND_QUIT
#include "_assert.h"
#endif

#ifdef NO_ASSERTS
#define Assert(a) (void)0
#endif

// Version control

#define FLINTCPPVMAJ  2
#define FLINTCPPVMIN  1

#if ((FLINTCPPVMIN != FLINTCPPHVMIN) || (FLINTCPPVMAJ != FLINTCPPHVMAJ))
#error Version error: FLINTPP.CPP not compatibel to FLINTPP.H
#endif

//lint -wlib(0)
//lint -e537 (Don't complain about "repeated include files")

#include <stdlib.h>
#include <string.h>

//lint -wlib(4)


////////////////////////////////////////////////////////////////////////////////
//           Constructors                                                     //
////////////////////////////////////////////////////////////////////////////////

// Constructor 1
// Default-constructor without assigment
LINT::LINT (void)
{
  n_l = new NOTHROW CLINT;

  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "Default constructor", 0, __LINE__);
    }

  maxlen = CLINTMAXDIGIT;
  init = 0;
  status = E_LINT_OK;
}


// Constructor 2
// LINT is constructed from character string 
LINT::LINT (const char* const str, const int base)
{
  int error;

  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 2", 0, __LINE__);
    }

  error = str2clint_l (n_l , (char*)str, base);

  switch (error)
   {
      case E_CLINT_OK:
        maxlen = CLINTMAXDIGIT;
        init = 1;
        status = E_LINT_OK;
        break;
      case E_CLINT_NPT:
        status = E_LINT_NPT;
        panic (E_LINT_NPT, "constructor 2", 1, __LINE__);
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        panic (E_LINT_OFL, "constructor 2", 1, __LINE__);
        break;
      case E_CLINT_BOR:
        status = E_LINT_BOR;
        panic (E_LINT_BOR, "constructor 2", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "constructor 2", error, __LINE__);
   }
} //lint !e1541 (maxlen, init, status possibly not initialized)


// Constructor 3
// LINT is constructed from byte array with digits to base 2^8
// according to IEEE P1363, significance of bytes increasing from left to right 
LINT::LINT (const UCHAR* const bytevector, const int length)
{
  int error;

  if (NULL == bytevector)
    {
      panic (E_LINT_VAL, "constructor 3", 1, __LINE__);
    }

  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 3", 0, __LINE__);
    }

  error = byte2clint_l (n_l , (UCHAR*)bytevector, length);

  switch (error)
   {
      case E_CLINT_OK:
        maxlen = CLINTMAXDIGIT;
        init = 1;
        status = E_LINT_OK;
        break;
      case E_CLINT_NPT:
        status = E_LINT_NPT;
        panic (E_LINT_NPT, "constructor 3", 1, __LINE__);
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        panic (E_LINT_OFL, "constructor 3", 1, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "constructor 3", error, __LINE__);
   }
} //lint !e1541 (maxlen, init, status possibly not initialized)


// Constructor 4
// LINT is constructed from ASCII-string with C syntax.
// Syntax:
//   "0[x|X]{0123456789abcdefABCDEF}" : HEX number
//   "0[b|B]{01}"                     : Binary number
//   "{0123456789}" : Characters are interpreted as decimal digits
LINT::LINT (const char* const str)
{
  int error;

  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 5", 0, __LINE__);
    }

  //lint -e668 -e613 (str is not NULL)
  if (strncmp (str, "0x", 2) == 0 || strncmp (str, "0X", 2) == 0)
    {
      error = str2clint_l (n_l, (char*)str+2, 16);
    }
  else
    {
      if (strncmp (str, "0b", 2) == 0 || strncmp (str, "0B", 2) == 0)
        {
          error = str2clint_l (n_l, (char*)str+2, 2);
        }
      else
        {
          error = str2clint_l (n_l, (char*)str, 10);
        }
    }

  switch (error)
    {
      case E_CLINT_OK:
        maxlen = CLINTMAXDIGIT;
        init = 1;
        status = E_LINT_OK;
        break;
      case E_CLINT_NPT:
        status = E_LINT_NPT;
        panic (E_LINT_NPT, "constructor 4", 1, __LINE__);
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        panic (E_LINT_OFL, "constructor 4", 1, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "constructor 4", error, __LINE__);
    }
} //lint !e1541 +e668


// Constructor 5
// LINT is constructed from LINT
LINT::LINT (const LINT& ln)
{
  if (!ln.init) panic (E_LINT_VAL, "constructor 5", 1, __LINE__);

  n_l = new NOTHROW clint[ln.maxlen + 1]; //lint !e737
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 5", 0, __LINE__);
    }
  cpy_l (n_l, ln.n_l);
  maxlen = ln.maxlen;
  init = 1;
  status = ln.status;
} //lint !e1541


// Constructor 6
// LINT is constructed from int (w/o sign extension)
LINT::LINT (const signed int i)
{
  unsigned long ul;
  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 8", 0, __LINE__);
    }
  maxlen = CLINTMAXDIGIT;
  init = 1;
  status = E_LINT_OK;
  ul = (unsigned)abs(i);
  ul2clint_l (n_l, ul);
}


// Constructor 7
// LINT is constructed from long (w/o sign extension)
LINT::LINT (const signed long l)
{
  unsigned long ul;
  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 9", 0, __LINE__);
    }
  maxlen = CLINTMAXDIGIT;
  init = 1;
  status = E_LINT_OK;
  ul = (ULONG)abs(l);
  ul2clint_l (n_l, ul);
}


// Constructor 8
// LINT is constructed from unsigned char 
LINT::LINT (const unsigned char uc)
{
  unsigned long ul;
  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 10", 0, __LINE__);
    }
  maxlen = CLINTMAXDIGIT;
  init = 1;
  status = E_LINT_OK;
  ul = uc;
  ul2clint_l (n_l, ul);
}


// Constructor 9
// LINT is constructed from unsigned short
LINT::LINT (const unsigned short us)
{
  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 11", 0, __LINE__);
    }
  maxlen = CLINTMAXDIGIT;
  init = 1;
  status = E_LINT_OK;
  u2clint_l (n_l, us);
}


// Constructor 10
// LINT is constructed from unsigned int
LINT::LINT (const unsigned int ui)
{
  unsigned long ul;
  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 12", 0, __LINE__);
    }
  maxlen = CLINTMAXDIGIT;
  init = 1;
  status = E_LINT_OK;
  ul = ui;
  ul2clint_l (n_l, ul);
}


// Constructor 11
// LINT is constructed from unsigned long
LINT::LINT (const unsigned long ul)
{
  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 13", 0, __LINE__);
    }
  maxlen = CLINTMAXDIGIT;
  init = 1;
  status = E_LINT_OK;
  ul2clint_l (n_l, ul);
}


// Constructor 12
// LINT is constructed from type CLINT
LINT::LINT (const CLINT m_l)
{
  if (vcheck_l ((clint*)(m_l)) < 0)
    {
      panic (E_LINT_VAL, "constructor 14", 1, __LINE__);
    }

  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor 14", 0, __LINE__);
    }

  cpy_l (n_l, (clint*)m_l);             //lint !e613 (m_l is not NULL)
  maxlen = CLINTMAXDIGIT;
  init = 1;
  status = E_LINT_OK;
}                                       //lint !e1541


// Oliver Kullmann, 23.2.2002
// Constructor 2 abgewandelt fuer den Typ String
LINT::LINT (const std::string D, const int B)
{
  int error;

  const char * str = D.c_str();
  const int base = B;

  n_l = new NOTHROW CLINT;
  if (NULL == n_l)
    {
      panic (E_LINT_NHP, "constructor Neu", 0, __LINE__);
    }

  error = str2clint_l (n_l , (char*)str, base);

  switch (error)
   {
      case E_CLINT_OK:
        maxlen = CLINTMAXDIGIT;
        init = 1;
        status = E_LINT_OK;
        break;
      case E_CLINT_NPT:
        status = E_LINT_NPT;
        panic (E_LINT_NPT, "constructor Neu", 1, __LINE__);
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        panic (E_LINT_OFL, "constructor Neu", 1, __LINE__);
        break;
      case E_CLINT_BOR:
        status = E_LINT_BOR;
        panic (E_LINT_BOR, "constructor Neu", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "constructor Neu", error, __LINE__);
   }
}



////////////////////////////////////////////////////////////////////////////////
//           Overloaded operators                                             //
////////////////////////////////////////////////////////////////////////////////

// Assignment

const LINT& LINT::operator= (const LINT& ln)
{
  if (!ln.init) panic (E_LINT_VAL, "=", 1, __LINE__);
  if (maxlen < DIGITS_L (ln.n_l)) panic (E_LINT_OFL, "=, Assignment", 1, __LINE__);

  if (&ln != this)                      // Don't copy object to itself
    {
      cpy_l (n_l, ln.n_l);
      init = 1;
      status = ln.status;
    }
  return *this;
} /*lint !e1539*/


// Arithmetic

const LINT operator+ (const LINT& lm, const LINT& ln)
{
  LINT sum;
  int err;
  if (!lm.init) LINT::panic (E_LINT_VAL, "+", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "+", 2, __LINE__);
  err = add_l (ln.n_l, lm.n_l, sum.n_l);
  switch (err)
    {
      case E_CLINT_OK:
        sum.init = 1;
        break;
      case E_CLINT_OFL:
        sum.status = E_LINT_OFL;
        sum.init = 1;
        break;
      default:
        LINT::panic (E_LINT_ERR, "+", err, __LINE__);
    }
  return sum;
}


const LINT operator- (const LINT& lm, const LINT& ln)
{
  LINT diff;
  int err;
  if (!lm.init) LINT::panic (E_LINT_VAL, "-", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "-", 2, __LINE__);
  err = sub_l (lm.n_l, ln.n_l, diff.n_l);
  switch (err)
    {
      case E_CLINT_OK:
        diff.init = 1;
        break;
      case E_CLINT_UFL:
        diff.status = E_LINT_UFL;
        diff.init = 1;
        break;
      default:
        LINT::panic (E_LINT_ERR, "-", err, __LINE__);
    }

  return diff;
}


const LINT operator* (const LINT& lm, const LINT& ln)
{
  LINT prd;
  int err;
  if (!lm.init) LINT::panic (E_LINT_VAL, "*", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "*", 2, __LINE__);

  if (&lm == &ln)     //lint !e506
      err = sqr_l (lm.n_l, prd.n_l);    // Use squaring function sqr_l for lm*lm
  else
      err = mul_l (lm.n_l, ln.n_l, prd.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        prd.init = 1;
        break;
      case E_CLINT_OFL:
        prd.status = E_LINT_OFL;
        prd.init = 1;
        break;
      default:
        LINT::panic (E_LINT_ERR, "*", err, __LINE__);
    }

  return prd;
}


const LINT operator/ (const LINT& lm, const LINT& ln)
{
  LINT quot;
  CLINT junk_l;
  int err;
  if (!lm.init) LINT::panic (E_LINT_VAL, "/", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "/", 2, __LINE__);

  err = div_l (lm.n_l, ln.n_l, quot.n_l, junk_l);

  switch (err)
    {
      case E_CLINT_OK:
        quot.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "/", 2, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "/", err, __LINE__);
    }

  ZEROCLINT_L (junk_l);
  return quot;
}


const LINT operator% (const LINT& lm, const LINT& ln)
{
  LINT rem;
  CLINT junk_l;
  int err;
  if (!lm.init) LINT::panic (E_LINT_VAL, "%", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "%", 2, __LINE__);

  err = div_l (lm.n_l, ln.n_l, junk_l, rem.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        rem.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "%", 2, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "%", err, __LINE__);
    }

  ZEROCLINT_L (junk_l);
  return rem;
}


const LINT& LINT::operator++ (void) // Prefix operation
{
  int err;
  if (!init) panic (E_LINT_VAL, "++", 0, __LINE__);

  err = inc_l (n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        break;
      default:
        panic (E_LINT_ERR, "++", err, __LINE__);
    }

  return *this;
}


const LINT LINT::operator++ (int i) // Postfix operation
{
  LINT tmp = *this;
  int err;
  if (!init) panic (E_LINT_VAL, "++", 0, __LINE__);

  err = inc_l (n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        break;
      default:
        panic (E_LINT_ERR, "++", err, __LINE__);
    }

  return tmp;
} //lint !e715


const LINT& LINT::operator-- (void) // Prefix operation
{
  int err;
  if (!init) panic (E_LINT_VAL, "--", 0, __LINE__);

  err = dec_l (n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_UFL:
        status = E_LINT_UFL;
        break;
      default:
        panic (E_LINT_ERR, "--", err, __LINE__);
    }

  return *this;
}


const LINT LINT::operator-- (int i) // Postfix operation
{
  LINT tmp = *this;
  int err;
  if (!init) panic (E_LINT_VAL, "--", 0, __LINE__);

  err = dec_l (n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_UFL;
        break;
      default:
        panic (E_LINT_ERR, "--", err, __LINE__);
    }

  return tmp;
} //lint !e715


const LINT& LINT::operator+= (const LINT& ln)
{
  int err;
  if (!init) panic (E_LINT_VAL, "+=", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "+=", 1, __LINE__);

  err = add_l (n_l, ln.n_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        break;
      default:
        panic (E_LINT_ERR, "+=", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::operator-= (const LINT& ln)
{
  int err;
  if (!init) panic (E_LINT_VAL, "-=", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "-=", 1, __LINE__);

  err = sub_l (n_l, ln.n_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_UFL:
        status = E_LINT_UFL;
        break;
      default:
        panic (E_LINT_ERR, "-=", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::operator*= (const LINT& ln)
{
  int err;
  if (!init) panic (E_LINT_VAL, "*=", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "*=", 1, __LINE__);

  if (&ln == this)
      err = sqr_l (n_l, n_l); // Use squaring function sqr_l for ln*=ln
  else
      err = mul_l (n_l, ln.n_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        break;
      default:
        panic (E_LINT_ERR, "*=", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::operator/= (const LINT& ln)
{
  CLINT junk_l;
  int err;
  if (!init) panic (E_LINT_VAL, "/=", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "/=", 1, __LINE__);

  err = div_l (n_l, ln.n_l, n_l, junk_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "/=", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "/=", err, __LINE__);
    }

  ZEROCLINT_L (junk_l);
  return *this;
}


const LINT& LINT::operator%= (const LINT& ln)
{
  CLINT junk_l;
  int err;
  if (!init) panic (E_LINT_VAL, "%=", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "%=", 1, __LINE__);

  err = div_l (n_l, ln.n_l, junk_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "%=", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "%=", err, __LINE__);
    }

  ZEROCLINT_L (junk_l);
  return *this;
}


// Bitwise operators


const LINT operator<< (const LINT& ln, const int times)
{
  int err;
  if (!ln.init) LINT::panic (E_LINT_VAL, "<<", 1, __LINE__);
  LINT shft = ln;

  err = shift_l (shft.n_l, times);

  switch (err)
    {
      case E_CLINT_OK:
        shft.init = 1;
        shft.status = E_LINT_OK;  // status = E_LINT_OK is set
        break;                    // because shft = ln was executed in advance
      case E_CLINT_OFL:
        shft.init = 1;
        shft.status = E_LINT_OFL;
        break;
      case E_CLINT_UFL:
        shft.init = 1;
        shft.status = E_LINT_UFL;
        break;
      default:
        LINT::panic (E_LINT_ERR, "<<", err, __LINE__);
    }

  return shft;
}


const LINT operator>> (const LINT& ln, const int times)
{
  int err;
  if (!ln.init) LINT::panic (E_LINT_VAL, ">>", 1, __LINE__);
  LINT shft = ln;

  err = shift_l (shft.n_l, -times);

  switch (err)
    {
      case E_CLINT_OK:
        shft.init = 1;
        shft.status = E_LINT_OK;  // status = E_LINT_OK is set
        break;                    // because shft = ln was executed in advance
      case E_CLINT_OFL:
        shft.init = 1;
        shft.status = E_LINT_OFL;
        break;
      case E_CLINT_UFL:
        shft.init = 1;
        shft.status = E_LINT_UFL;
        break;
      default:
        LINT::panic (E_LINT_ERR, ">>", err, __LINE__);
    }

  return shft;
}


const LINT& LINT::operator<<= (const int times)
{
  int err;
  if (!init) panic (E_LINT_VAL, "<<=", 0, __LINE__);

  err = shift_l (n_l, times);

  switch (err)
    {
      case E_CLINT_OK:
        init = 1;
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        init = 1;
        status = E_LINT_OFL;
        break;
      case E_CLINT_UFL:
        init = 1;
        status = E_LINT_UFL;
        break;
      default:
        panic (E_LINT_ERR, "<<=", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::operator>>= (const int times)
{
  int err;
  if (!init) panic (E_LINT_VAL, ">>=", 0, __LINE__);

  err = shift_l (n_l, -times);

  switch (err)
    {
      case E_CLINT_OK:
        init = 1;
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        init = 1;
        status = E_LINT_OFL;
        break;
      case E_CLINT_UFL:
        init = 1;
        status = E_LINT_UFL;
        break;
      default:
        panic (E_LINT_ERR, ">>=", err, __LINE__);
    }

  return *this;
}


const LINT operator^ (const LINT& lm, const LINT& ln)
{
  LINT lr;
  if (!lm.init) LINT::panic (E_LINT_VAL, "^", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "^", 2, __LINE__);

  xor_l (lm.n_l, ln.n_l, lr.n_l);

  lr.init = 1;
  return lr;
}


const LINT operator| (const LINT& lm, const LINT& ln)
{
  LINT lr;
  if (!lm.init) LINT::panic (E_LINT_VAL, "|", 0, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "|", 1, __LINE__);

  or_l (lm.n_l, ln.n_l, lr.n_l);

  lr.init = 1;
  return lr;
}


const LINT operator& (const LINT& lm, const LINT& ln)
{
  LINT lr;
  if (!lm.init) LINT::panic (E_LINT_VAL, "&", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "&", 2, __LINE__);

  and_l (lm.n_l, ln.n_l, lr.n_l);

  lr.init = 1;
  return lr;
}


const LINT& LINT::operator|= (const LINT& ln)
{
  if (!init) panic (E_LINT_VAL, "|=", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "|=", 1, __LINE__);

  or_l (n_l, ln.n_l, n_l);

  status = E_LINT_OK;
  return *this;
}


const LINT& LINT::operator&= (const LINT& ln)
{
  if (!init) panic (E_LINT_VAL, "&=", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "&=", 1, __LINE__);

  and_l (n_l, ln.n_l, n_l);

  status = E_LINT_OK;
  return *this;
}


const LINT& LINT::operator^= (const LINT& ln)
{
  if (!init) panic (E_LINT_VAL, "^=", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "^=", 1, __LINE__);

  xor_l (n_l, ln.n_l, n_l);
  status = E_LINT_OK;
  return *this;
}


////////////////////////////////////////////////////////////////////////////////
//           Logical operators as friend functions                            //
////////////////////////////////////////////////////////////////////////////////

const int operator== (const LINT& lm, const LINT& ln)
{
  if (!lm.init) LINT::panic (E_LINT_VAL, "==", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "==", 2, __LINE__);

  if (&lm == &ln)       //lint !e506
    {
      return 1;
    }

  return equ_l (lm.n_l, ln.n_l);
  // Operator == returns 1 if lm == ln, 0 else
}


const int operator!= (const LINT& lm, const LINT& ln)
{
  if (!lm.init) LINT::panic (E_LINT_VAL, "!=", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "!=", 2, __LINE__);

  if (&lm == &ln)       //lint !e506
    {
      return 0;
    }

  return (equ_l (lm.n_l, ln.n_l) == 0);
  // Operator != returns 1 if lm != ln, 0 else
}


const int operator< (const LINT& lm, const LINT& ln)
{
  if (!lm.init)  LINT::panic (E_LINT_VAL, "<", 1, __LINE__);
  if (!ln.init)  LINT::panic (E_LINT_VAL, "<", 2, __LINE__);

  if (&lm == &ln)       //lint !e506
    {
      return 0;
    }

  return (cmp_l (lm.n_l, ln.n_l) == -1);
}


const int operator> (const LINT& lm, const LINT& ln)
{
  if (!lm.init)  LINT::panic (E_LINT_VAL, ">", 1, __LINE__);
  if (!ln.init)  LINT::panic (E_LINT_VAL, ">", 2, __LINE__);

  if (&lm == &ln)       //lint !e506
    {
      return 0;
    }

  return (cmp_l (lm.n_l, ln.n_l) == 1);
}


const int operator<= (const LINT& lm, const LINT& ln)
{
  int i;
  if (!lm.init)  LINT::panic (E_LINT_VAL, "<=", 1, __LINE__);
  if (!ln.init)  LINT::panic (E_LINT_VAL, "<=", 2, __LINE__);

  if (&lm == &ln)       //lint !e506
    {
      return 1;
    }

  i = cmp_l (lm.n_l, ln.n_l);
  return ((i == -1) || (i == 0));
}


const int operator>= (const LINT& lm, const LINT& ln)
{
  int i;
  if (!lm.init)  LINT::panic (E_LINT_VAL, ">=", 1, __LINE__);
  if (!ln.init)  LINT::panic (E_LINT_VAL, ">=", 2, __LINE__);

  if (&lm == &ln)       //lint !e506
    {
      return 1;
    }

  i = cmp_l (lm.n_l, ln.n_l);
  return ((i == 1) || (i == 0));
}


////////////////////////////////////////////////////////////////////////////////
//           Bit access with member functions                                 //
////////////////////////////////////////////////////////////////////////////////

const LINT& LINT::setbit (const unsigned int pos)
{
  int err;
  if (!init)
    { // If not initialized, set *this to 0 !
      SETZERO_L (n_l);
      init = 1;
    }

  err = setbit_l (n_l, pos);

  switch (err)
    {
      case E_CLINT_OFL:
        init = 1;
        status = E_LINT_OFL;
        break;
      default:
        init = 1;
        status = E_LINT_OK;
    }

  return *this;
}


const LINT& LINT::clearbit (const unsigned int pos)
{
  if (!init) panic (E_LINT_VAL, "clearbit()", 0, __LINE__);

  clearbit_l (n_l, pos);

  status = E_LINT_OK;
  return *this;
}


const int LINT::testbit (const unsigned int pos) const
{
  if (!init) panic (E_LINT_VAL, "testbit()", 0, __LINE__);
  return (testbit_l (n_l, pos));
}


////////////////////////////////////////////////////////////////////////////////
//           Swapping, purging                                                //
////////////////////////////////////////////////////////////////////////////////

// member-swap

LINT& LINT::fswap (LINT& b)
{
  LINT tmp;
  if (!init) panic (E_LINT_VAL, "fswap", 0, __LINE__);
  if (!b.init) panic (E_LINT_VAL, "fswap", 1, __LINE__);
  tmp = *this;
  *this = b;
  b = tmp;
  return *this;
}


// friend-swap

void fswap (LINT& a, LINT& b)
{
  LINT tmp;
  if (!a.init) LINT::panic (E_LINT_VAL, "fswap", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "fswap", 2, __LINE__);
  tmp = a;
  a = b;
  b = tmp;
}


// Purging of LINT variables, overwriting with 0

void LINT::purge (void)
{
  if (!init) LINT::panic (E_LINT_VAL, "purge", 0, __LINE__);
  purge_l (n_l);           //lint !e613
  SETDIGITS_L (n_l, 0);    //lint !e613
}


// friend-purge

void purge (LINT& a)
{
  if (!a.init) LINT::panic (E_LINT_VAL, "purge", 1, __LINE__);
  purge_l (a.n_l);         //lint !e613
  SETDIGITS_L (a.n_l, 0);  //lint !e613
}


////////////////////////////////////////////////////////////////////////////////
//          Arithmetic member functions                                       //
//          Accumulator mode                                                  //
////////////////////////////////////////////////////////////////////////////////

const LINT& LINT::add (const LINT& b)
{
  int err;
  if (!init) panic (E_LINT_VAL, "add", 0, __LINE__);
  if (!b.init) panic (E_LINT_VAL, "add", 1, __LINE__);

  err = add_l (n_l, b.n_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        break;
      default:
        panic (E_LINT_ERR, "add", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::sub (const LINT& b)
{
  int err;
  if (!init) panic (E_LINT_VAL, "sub", 0, __LINE__);
  if (!b.init) panic (E_LINT_VAL, "sub", 1, __LINE__);

  err = sub_l (n_l, b.n_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_UFL:
        status = E_LINT_UFL;
        break;
      default:
        panic (E_LINT_ERR, "sub", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mul (const LINT& b)
{
  int err;
  if (!init) panic (E_LINT_VAL, "mul", 0, __LINE__);
  if (!b.init) panic (E_LINT_VAL, "mul", 1, __LINE__);

  if (&b == this)
      err = sqr_l (n_l, n_l);
  else
      err = mul_l (n_l, b.n_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        break;
      default:
        panic (E_LINT_ERR, "mul", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::sqr (void)
{
  int err;
  if (!init) panic (E_LINT_VAL, "sqr", 0, __LINE__);

  err = sqr_l (n_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        break;
      default:
        panic (E_LINT_ERR, "sqr", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::divr (const LINT& d, LINT& r)
{
  int err;
  if (!init) panic (E_LINT_VAL, "divr", 0, __LINE__);
  if (!d.init) panic (E_LINT_VAL, "divr", 1, __LINE__);

  err = div_l (n_l, d.n_l, n_l, r.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        r.init = 1;
        status = E_LINT_OK;
        r.status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "divr", 1, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "divr", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mod (const LINT& d)
{
  LINT junk;
  int err;
  if (!init) panic (E_LINT_VAL, "mod", 0, __LINE__);
  if (!d.init) panic (E_LINT_VAL, "mod", 1, __LINE__);

  err = div_l (n_l, d.n_l, junk.n_l, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "mod", 1, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "mod", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mod2 (const USHORT m) // mod 2^m
{
  int err;
  if (!init) panic (E_LINT_VAL, "mod2", 0, __LINE__);

  err = mod2_l (n_l, m, n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      default:
        panic (E_LINT_ERR, "mod2", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::madd (const LINT& ln, const LINT& m)
{
  int err;
  if (!init) panic (E_LINT_VAL, "madd", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "madd", 1, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "madd", 2, __LINE__);

  err = madd_l (n_l, ln.n_l, n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "madd", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "madd", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::msub (const LINT& ln, const LINT& m)
{
  int err;
  if (!init) panic (E_LINT_VAL, "msub", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "msub", 1, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "msub", 2, __LINE__);

  err = msub_l (n_l, ln.n_l, n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "msub", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "msub", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mmul (const LINT& ln, const LINT& m)
{
  int err;
  if (!init) panic (E_LINT_VAL, "mmul", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "mmul", 1, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "mmul", 2, __LINE__);

  if (&ln == this)
      err = msqr_l (n_l, n_l, m.n_l);
  else
      err = mmul_l (n_l, ln.n_l, n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "mmul", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "mmul", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::msqr (const LINT& m)
{
  int err;
  if (!init) panic (E_LINT_VAL, "msqr", 0, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "msqr", 1, __LINE__);

  err = msqr_l (n_l, n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "msqr", 1, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "msqr", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mexp (const USHORT e, const LINT& m)
{
  int err;
  if (!init) panic (E_LINT_VAL, "mexp", 0, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "mexp", 1, __LINE__);

  if (m.isodd ())
    {
      err = umexpm_l (n_l, e, n_l, m.n_l);
    }
  else
    {
      err = umexp_l (n_l, e, n_l, m.n_l);
    }

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "mexp", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "mexp", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mexp (const LINT& ln, const LINT& m)
{
  int err;
  if (!init) panic (E_LINT_VAL, "mexp", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "mexp", 1, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "mexp", 2, __LINE__);

  err = mexp_l (n_l, ln.n_l, n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "mexp", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "mexp", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mexp5m (const LINT& ln, const LINT& m)
{
  int err;
  if (!init) panic (E_LINT_VAL, "mexp5m", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "mexp5m", 1, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "mexp5m", 2, __LINE__);

  err = mexp5m_l (n_l, ln.n_l, n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_MOD:
        panic (E_LINT_MOD, "mexp5m", 2, __LINE__);
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "mexp5m", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "mexp5m", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mexpkm (const LINT& ln, const LINT& m)
{
  int err;
  if (!init) panic (E_LINT_VAL, "mexpkm", 0, __LINE__);
  if (!ln.init) panic (E_LINT_VAL, "mexpkm", 1, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "mexpkm", 2, __LINE__);

  err = mexpkm_l (n_l, ln.n_l, n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_MOD:
        panic (E_LINT_MOD, "mexpkm", 2, __LINE__);
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "mexpkm", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "mexpkm", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::mexp2 (const USHORT e, const LINT& m) // *this^(2^e)
{
  int err;
  if (!init) panic (E_LINT_VAL, "mexp2", 0, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "mexp2", 1, __LINE__);

  err = mexp2_l (n_l, e, n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "mexp2", 2, __LINE__);
        break;
      default:
        panic (E_LINT_ERR, "mexp2", err, __LINE__);
    }

  return *this;
}


const LINT& LINT::shift (const int noofbits)
{
  int err;
  if (!init) panic (E_LINT_VAL, "shift", 0, __LINE__);

  err = shift_l (n_l, noofbits);

  switch (err)
    {
      case E_CLINT_OK:
        status = E_LINT_OK;
        break;
      case E_CLINT_OFL:
        status = E_LINT_OFL;
        break;
      case E_CLINT_UFL:
        status = E_LINT_UFL;
        break;
      default:
        panic (E_LINT_ERR, "shift", err, __LINE__);
    }

  return *this;
}


////////////////////////////////////////////////////////////////////////////////
//           Number-theoretic member functions                                //
////////////////////////////////////////////////////////////////////////////////


const unsigned int LINT::ld (void) const
{
  if (!init) panic (E_LINT_VAL, "ld", 0, __LINE__);
  return ld_l (n_l);
}


const int LINT::iseven (void) const
{
  if (!init) LINT::panic (E_LINT_VAL, "iseven", 0, __LINE__);
  return (ISEVEN_L (n_l)); //lint !e613
}


const int LINT::isodd (void) const
{
  if (!init) LINT::panic (E_LINT_VAL, "isodd", 0, __LINE__);
  return (ISODD_L (n_l)); //lint !e613
}


const int LINT::isprime (void) const
{
  if (!init) panic (E_LINT_VAL, "isprime", 0, __LINE__);
  return (ISPRIME_L (n_l));
}


const LINT LINT::issqr (void) const
{
  LINT sqroot;
  if (!init) panic (E_LINT_VAL, "issqr", 0, __LINE__);

  issqr_l (n_l, sqroot.n_l);
  sqroot.init = 1;

  return sqroot;
}


const LINT LINT::root (void) const
{
  LINT sqroot;
  if (!init) panic (E_LINT_VAL, "root", 0, __LINE__);

  iroot_l (n_l, sqroot.n_l);
  sqroot.init = 1;

  return sqroot;
}


const LINT LINT::gcd (const LINT& b) const
{
  LINT gcd;
  if (!init) panic (E_LINT_VAL, "gcd", 0, __LINE__);
  if (!b.init) panic (E_LINT_VAL, "gcd", 1, __LINE__);

  gcd_l (n_l, b.n_l, gcd.n_l);
  gcd.init = 1;

  return gcd;
}


const LINT LINT::lcm (const LINT& b) const
{
  LINT lcm;
  int err;
  if (!init) panic (E_LINT_VAL, "lcm", 0, __LINE__);
  if (!b.init) panic (E_LINT_VAL, "lcm", 1, __LINE__);

  err = lcm_l (n_l, b.n_l, lcm.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        lcm.init = 1;
        break;
      case E_CLINT_OFL:
        lcm.status = E_LINT_OFL;
        lcm.init = 1;
        break;
      default:
        LINT::panic (E_LINT_ERR, "lcm", err, __LINE__);
    }

  return lcm;
}


const LINT LINT::xgcd (const LINT& b , LINT& u, int& sign_u, LINT& v, int& sign_v) const
{
  LINT g, hlp;
  if (!init) LINT::panic (E_LINT_VAL, "xgcd", 0, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "xgcd", 1, __LINE__);

  xgcd_l (n_l, b.n_l, g.n_l, u.n_l, &sign_u, v.n_l, &sign_v);
  g.init = 1;
  u.init = 1;
  v.init = 1;

  return g;
}


const LINT LINT::inv (const LINT& b) const
{
  LINT invers, hlp;
  if (!init) panic (E_LINT_VAL, "inv", 0, __LINE__);
  if (!b.init) panic (E_LINT_VAL, "inv", 1, __LINE__);

  inv_l (n_l, b.n_l, hlp.n_l, invers.n_l);
  invers.init = 1;

  return invers;
}


const int LINT::jacobi (const LINT& q) const
{
  if (!init) panic (E_LINT_VAL, "jacobi", 0, __LINE__);
  if (!q.init) panic (E_LINT_VAL, "jacobi", 1, __LINE__);

  return (jacobi_l (n_l, q.n_l));
}


const LINT LINT::root (const LINT& p, const LINT& q) const
{
  LINT rt;
  if (!init) panic (E_LINT_VAL, "root", 0, __LINE__);
  if (!p.init) panic (E_LINT_VAL, "root", 1, __LINE__);
  if (!q.init) panic (E_LINT_VAL, "root", 2, __LINE__);

  if (root_l (n_l, p.n_l, q.n_l, rt.n_l) == 0)
    {
      rt.init = 1;
    }
  else
    {
      rt = 0;
      rt.status = E_LINT_ERR;
    }
  return rt;
}


const LINT LINT::root (const LINT& p) const
{
  LINT rt;
  if (!init) panic (E_LINT_VAL, "root", 0, __LINE__);
  if (!p.init) panic (E_LINT_VAL, "root", 1, __LINE__);

  if (proot_l (n_l, p.n_l, rt.n_l) == 0)
    {
      rt.init = 1;
    }
  else
    {
      rt = 0;
      rt.status = E_LINT_ERR;
    }
  return rt;
}


const int LINT::twofact (LINT& odd) const
{
  if (!init) panic (E_LINT_VAL, "twofact", 0, __LINE__);
  odd.init = 1;
  return (twofact_l (n_l, odd.n_l));
}


const int LINT::mequ (const LINT& b, const LINT& m) const
{
  int err;
  if (!init) panic (E_LINT_VAL, "mequ", 0, __LINE__);
  if (!b.init) panic (E_LINT_VAL, "mequ", 1, __LINE__);
  if (!m.init) panic (E_LINT_VAL, "mequ", 2, __LINE__);

  err = mequ_l (n_l, b.n_l, m.n_l);

  switch (err)
    {
      case 0:
      case 1:
        break;
      case E_CLINT_DBZ:
        panic (E_LINT_DBZ, "mequ", 2, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mequ", err, __LINE__);
    }

  return err;
}


const LINT LINT::chinrem (const LINT& m, const LINT& b, const LINT& u) const
{
  LINT x;
  if (!init) LINT::panic (E_LINT_VAL, "chinrem", 0, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "chinrem", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "chinrem", 2, __LINE__);
  if (!u.init) LINT::panic (E_LINT_VAL, "chinrem", 3, __LINE__);

  clint* koeff_l[4];
  koeff_l[0] = n_l;
  koeff_l[1] = m.n_l;
  koeff_l[2] = b.n_l;
  koeff_l[3] = u.n_l;

  if (chinrem_l (2, koeff_l, x.n_l) == 0)
    {
      x.init = 1;
    }
  else
    {
      x = 0;
      x.status = E_LINT_ERR;
    }
  return x;
}


////////////////////////////////////////////////////////////////////////////////
//          Arithmetic friend functions                                       //
////////////////////////////////////////////////////////////////////////////////

const LINT add (const LINT& a, const LINT& b)
{
  LINT sum;
  int err;
  if (!a.init) LINT::panic (E_LINT_VAL, "add", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "add", 2, __LINE__);

  err = add_l (a.n_l, b.n_l, sum.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        sum.init = 1;
        break;
      case E_CLINT_OFL:
        sum.init = 1;
        sum.status = E_LINT_OFL;
        break;
      default:
        LINT::panic (E_LINT_ERR, "add", err, __LINE__);
    }

  return sum;
}


const LINT sub (const LINT& a, const LINT& b)
{
  LINT dif;
  int err;
  if (!a.init) LINT::panic (E_LINT_VAL, "sub", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "sub", 2, __LINE__);

  err = sub_l (a.n_l, b.n_l, dif.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        dif.init = 1;
        break;
      case E_CLINT_UFL:
        dif.init = 1;
        dif.status = E_LINT_UFL;
        break;
      default:
        LINT::panic (E_LINT_ERR, "sub", err, __LINE__);
    }

  return dif;
}


const LINT mul (const LINT& a, const LINT& b)
{
  LINT p;
  int err;
  if (!a.init) LINT::panic (E_LINT_VAL, "mul", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "mul", 2, __LINE__);

  if (&a == &b) //lint !e506
      err = sqr_l (a.n_l, p.n_l);
  else
      err = mul_l (a.n_l, b.n_l, p.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        p.init = 1;
        break;
      case E_CLINT_OFL:
        p.init = 1;
        p.status = E_LINT_OFL;
        break;
      default:
        LINT::panic (E_LINT_ERR, "mul", err, __LINE__);
    }

  return p;
}


const LINT sqr (const LINT& a)
{
  LINT p;
  int err;
  if (!a.init) LINT::panic (E_LINT_VAL, "sqr", 1, __LINE__);

  err = sqr_l (a.n_l, p.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        p.init = 1;
        break;
      case E_CLINT_OFL:
        p.init = 1;
        p.status = E_LINT_OFL;
        break;
      default:
        LINT::panic (E_LINT_ERR, "sqr", err, __LINE__);
    }

  return p;
}


const LINT divr (const LINT& a, const LINT& b, LINT& r)
{
  LINT q;
  int err;
  if (!a.init) LINT::panic (E_LINT_VAL, "divr", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "divr", 2, __LINE__);

  err = div_l (a.n_l, b.n_l, q.n_l, r.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        q.init = 1;
        r.init = 1;
        r.status = E_LINT_OK;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "divr", 2, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "div", err, __LINE__);
    }

  return q;
}


const LINT mod (const LINT& a, const LINT& n)
{
  LINT r, junk;
  int err;
  if (!a.init)  LINT::panic (E_LINT_VAL, "mod", 1, __LINE__);
  if (!n.init) LINT::panic (E_LINT_VAL, "mod", 2, __LINE__);

  err = div_l (a.n_l, n.n_l, junk.n_l, r.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        r.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mod", 2, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mod", err, __LINE__);
    }

  return r;
}


const LINT mod2 (const LINT& a, const USHORT k)
{
  LINT r;
  int err;
  if (!a.init)  LINT::panic (E_LINT_VAL, "mod2", 1, __LINE__);

  err = mod2_l (a.n_l, k, r.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        r.init = 1;
        break;
      default:
        LINT::panic (E_LINT_ERR, "mod2", err, __LINE__);
    }

  return r;
}


const LINT madd (const LINT& lr, const LINT& ln, const LINT& m)
{
  LINT sum;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "madd", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "madd", 2, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "madd", 3, __LINE__);

  err = madd_l (lr.n_l, ln.n_l, sum.n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        sum.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "madd", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "madd", err, __LINE__);
    }

  return sum;
}


const LINT msub (const LINT& lr, const LINT& ln, const LINT& m)
{
  LINT dif;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "msub", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "msub", 2, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "msub", 3, __LINE__);

  err = msub_l (lr.n_l, ln.n_l, dif.n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        dif.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "msub", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "msub", err, __LINE__);
    }

  return dif;
}


const LINT mmul (const LINT& lr, const LINT& ln, const LINT& m)
{
  LINT p;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "mmul", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "mmul", 2, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "mmul", 3, __LINE__);

  if (&lr == &ln) //lint !e506
    {
      err = msqr_l (lr.n_l, p.n_l, m.n_l);
    }
  else
    {
      err = mmul_l (lr.n_l, ln.n_l, p.n_l, m.n_l);
    }

  switch (err)
    {
      case E_CLINT_OK:
        p.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mmul", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mmul", err, __LINE__);
    }

  return p;
}


const LINT msqr (const LINT& lr, const LINT& m)
{
  LINT p;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "msqr", 1, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "msqr", 2, __LINE__);

  err = msqr_l (lr.n_l, p.n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        p.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "msqr", 2, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "msqr", err, __LINE__);
    }

  return p;
}


const LINT mexp (const LINT& lr, const LINT& ln, const LINT& m)
{
  LINT pot;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "mexp", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "mexp", 2, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "mexp", 3, __LINE__);

  err = mexp_l (lr.n_l, ln.n_l, pot.n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        pot.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mexp", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mexp", err, __LINE__);
    }

  return pot;
}


const LINT mexp (const USHORT b, const LINT& ln, const LINT& m)
{
  LINT pot;
  int err;
  if (!ln.init) LINT::panic (E_LINT_VAL, "mexp", 2, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "mexp", 3, __LINE__);
  
  if (m.isodd ())
    {
      err = wmexpm_l (b, ln.n_l, pot.n_l, m.n_l);
    }
  else
    {
      err = wmexp_l (b, ln.n_l, pot.n_l, m.n_l);
    }

  switch (err)
    {
      case E_CLINT_OK:
        pot.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mexp", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mexp", err, __LINE__);
    }

  return pot;
}


const LINT mexp (const LINT& lr, const USHORT e, const LINT& m)
{
  LINT pot;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "mexp", 1, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "mexp", 3, __LINE__);

  if (m.isodd())
    {
      err = umexpm_l (lr.n_l, e, pot.n_l, m.n_l);
    }
  else
    {
      err = umexp_l (lr.n_l, e, pot.n_l, m.n_l);
    }

  switch (err)
    {
      case E_CLINT_OK:
        pot.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mexp", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mexp", err, __LINE__);
    }

  return pot;
}


const LINT mexp5m (const LINT& lr, const LINT& ln, const LINT& m)
{
  LINT pot;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "mexp5m", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "mexp5m", 2, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "mexp5m", 3, __LINE__);

  err = mexp5m_l (lr.n_l, ln.n_l, pot.n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        pot.init = 1;
        break;
      case E_CLINT_MOD:
        LINT::panic (E_LINT_MOD, "mexp5m", 3, __LINE__);
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mexp5m", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mexp5m", err, __LINE__);
    }

  return pot;
}


const LINT mexpkm (const LINT& lr, const LINT& ln, const LINT& m)
{
  LINT pot;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "mexpkm", 1, __LINE__);
  if (!ln.init) LINT::panic (E_LINT_VAL, "mexpkm", 2, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "mexpkm", 3, __LINE__);

  err = mexpkm_l (lr.n_l, ln.n_l, pot.n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        pot.init = 1;
        break;
      case E_CLINT_MOD:
        LINT::panic (E_LINT_MOD, "mexpkm", 3, __LINE__);
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mexpkm", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mexpkm", err, __LINE__);
    }

  return pot;
}


const LINT mexp2 (const LINT& lr, const USHORT e, const LINT& m)
{
  LINT pot;
  int err;
  if (!lr.init) LINT::panic (E_LINT_VAL, "mexp", 1, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "mexp", 3, __LINE__);

  err = mexp2_l (lr.n_l, e, pot.n_l, m.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        pot.init = 1;
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mexp2", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mexp2", err, __LINE__);
    }

  return pot;
}


const LINT shift (const LINT& a, const int noofbits)
{
  int err;
  if (!a.init) LINT::panic (E_LINT_VAL, "shift", 1, __LINE__);

  LINT shft = a;
  err = shift_l (shft.n_l, noofbits);

  switch (err)
    {
      case E_CLINT_OK:
        shft.init = 1;
        shft.status = E_LINT_OK; // status = E_LINT_OK is set 
        break;
      case E_CLINT_OFL:
        shft.init = 1;
        shft.status = E_LINT_OFL;
        break;
      case E_CLINT_UFL:
        shft.init = 1;
        shft.status = E_LINT_UFL;
        break;
      default:
        LINT::panic (E_LINT_ERR, ">>", err, __LINE__);
    }

  return shft;
}


////////////////////////////////////////////////////////////////////////////////
//           Number-theoretic friend functions                                //
////////////////////////////////////////////////////////////////////////////////


const unsigned int ld (const LINT& a)
{
  if (!a.init) LINT::panic (E_LINT_VAL, "ld", 1, __LINE__);
  return ld_l (a.n_l);
}


const int isprime (const LINT& p)
{
  if (!p.init) LINT::panic (E_LINT_VAL, "isprime", 1, __LINE__);
  return (ISPRIME_L (p.n_l));
}


const int iseven (const LINT& p)
{
  if (!p.init) LINT::panic (E_LINT_VAL, "iseven", 1, __LINE__);
  return (ISEVEN_L (p.n_l));
}


const int isodd (const LINT& p)
{
  if (!p.init) LINT::panic (E_LINT_VAL, "isodd", 1, __LINE__);
  return (ISODD_L (p.n_l)); 
}


const LINT issqr (const LINT& lr)
{
  LINT sqroot;
  if (!lr.init) LINT::panic (E_LINT_VAL, "issqr", 1, __LINE__);

  issqr_l (lr.n_l, sqroot.n_l);
  sqroot.init = 1;

  return sqroot;
}


const LINT root (const LINT& lr)
{
  LINT sqroot;
  if (!lr.init) LINT::panic (E_LINT_VAL, "root", 1, __LINE__);

  iroot_l (lr.n_l, sqroot.n_l);
  sqroot.init = 1;

  return sqroot;
}


const LINT gcd (const LINT& a, const LINT& b)
{
  LINT gcd, hlp;
  if (!a.init) LINT::panic (E_LINT_VAL, "gcd", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "gcd", 2, __LINE__);

  gcd_l (a.n_l, b.n_l, gcd.n_l);
  gcd.init = 1;

  return gcd;
}


const LINT lcm (const LINT& a, const LINT& b)
{
  LINT lcm, hlp;
  int err;
  if (!a.init) LINT::panic (E_LINT_VAL, "lcm", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "lcm", 2, __LINE__);

  err = lcm_l (a.n_l, b.n_l, lcm.n_l);

  switch (err)
    {
      case E_CLINT_OK:
        lcm.init = 1;
        break;
      case E_CLINT_OFL:
        lcm.status = E_LINT_OFL;
        lcm.init = 1;
        break;
      default:
        LINT::panic (E_LINT_ERR, "lcm", err, __LINE__);
    }

  return lcm;
}


const LINT xgcd (const LINT& a, const LINT& b , LINT& u, int& sign_u, LINT& v, int& sign_v)
{
  LINT g, hlp;
  if (!a.init) LINT::panic (E_LINT_VAL, "xgcd", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "xgcd", 2, __LINE__);

  xgcd_l (a.n_l, b.n_l, g.n_l, u.n_l, &sign_u, v.n_l, &sign_v);

  g.init = 1;
  u.init = 1;
  v.init = 1;

  return g;
}


const LINT inv (const LINT& a, const LINT& b)
{
  LINT invers, hlp;
  if (!a.init) LINT::panic (E_LINT_VAL, "inv", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "inv", 2, __LINE__);

  inv_l (a.n_l, b.n_l, hlp.n_l, invers.n_l);
  invers.init = 1;

  return invers;
}


const int jacobi (const LINT& p, const LINT& q)
{
  if (!p.init) LINT::panic (E_LINT_VAL, "jacobi", 1, __LINE__);
  if (!q.init) LINT::panic (E_LINT_VAL, "jacobi", 2, __LINE__);

  return (jacobi_l (p.n_l, q.n_l));
}


const LINT root (const LINT& a, const LINT& p)
{
  LINT rt;
  if (!a.init) LINT::panic (E_LINT_VAL, "root", 1, __LINE__);
  if (!p.init) LINT::panic (E_LINT_VAL, "root", 2, __LINE__);

  if (proot_l (a.n_l, p.n_l, rt.n_l) == 0)
    {
      rt.init = 1;
    }
  else
    {
      rt = 0;
      rt.status = E_LINT_ERR;
    }
  return rt;
}


const LINT root (const LINT& a, const LINT& p, const LINT& q)
{
  LINT rt;
  if (!a.init) LINT::panic (E_LINT_VAL, "root", 1, __LINE__);
  if (!p.init) LINT::panic (E_LINT_VAL, "root", 2, __LINE__);
  if (!q.init) LINT::panic (E_LINT_VAL, "root", 3, __LINE__);

  if (root_l (a.n_l, p.n_l, q.n_l, rt.n_l) == 0)
    {
      rt.init = 1;
    }
  else
    {
      rt = 0;
      rt.status = E_LINT_ERR;
    }
  return rt;
}


const LINT chinrem (const unsigned int noofeq, LINT** koeff)
{
  LINT x;
  clint** koeff_l = new clint* [noofeq << 1];
  if (NULL == koeff_l)
    {
      return LINT(0);
    }
  clint** k_l = koeff_l;

  for (unsigned i = 0; i < (noofeq << 1); koeff_l++, koeff++, i++)
    {
      if (!(*koeff)->init) LINT::panic (E_LINT_VAL, "chinrem", i, __LINE__);
      *koeff_l = (*koeff)->n_l;
    }

  if (chinrem_l (noofeq, k_l, x.n_l) == 0)
    {
      x.init = 1;
    }
  else
    {
      x = 0;
      x.status = E_LINT_ERR;
    }

  delete [] k_l;
  return x;
}


const LINT primroot (const unsigned int noofprimes, LINT** primes)
{
  LINT x;
  clint** primes_l = new clint* [noofprimes + 1];
  if (NULL == primes_l)
    {
      return LINT(0);
    }
  clint** pr_l = primes_l;

  for (unsigned int i = 0; i <= noofprimes; primes_l++, primes++, i++)
    {
      if (!(*primes)->init) LINT::panic (E_LINT_VAL, "primroot", i, __LINE__);
      *primes_l = (*primes)->n_l;
    }

  if (primroot_l (x.n_l, noofprimes, pr_l) == 0)
    {
      x.init = 1;
    }
  else
    {
      x = 0UL;
    }

  delete [] pr_l;
  return x;
}


const int twofact (const LINT& even, LINT& odd)
{
  if (!even.init) LINT::panic (E_LINT_VAL, "twofact", 1, __LINE__);
  odd.init = 1;
  return (twofact_l (even.n_l, odd.n_l));
}


// Generation of a prime number p with 2^(l-1) <= p < 2^l

const LINT findprime (const USHORT l)
{
  return findprime (l, 1);
}


// Generation of a prime number p with 2^(l-1) <= p < 2^l
// and gcd (p - 1, f) = 1

const LINT findprime (const USHORT l, const LINT& f)
{
  if (0 == l || l > CLINTMAXBIT) LINT::panic (E_LINT_OFL, "findprime", 1, __LINE__);

  LINT pmin (0);
  LINT pmax (0);
  pmin.setbit (l - 1);
  pmax.setbit (l);
  --pmax;

  return findprime (pmin, pmax, f);
}


// Generation of a prime number p pmin <= p <= pmax
// and gcd (p - 1, f) = 1

const LINT findprime (const LINT& pmin, const LINT& pmax, const LINT& f)
{
  if (!pmin.init) LINT::panic (E_LINT_VAL, "findprime", 1, __LINE__);
  if (!pmax.init) LINT::panic (E_LINT_VAL, "findprime", 2, __LINE__);
  if (!f.init) LINT::panic (E_LINT_VAL, "findprime", 3, __LINE__);

  if (pmin > pmax) LINT::panic (E_LINT_VAL, "findprime", 1, __LINE__);

  // 0 < f must be odd
  if (f.iseven ()) LINT::panic (E_LINT_VAL, "findprime", 3, __LINE__);

  LINT p = randBBS (pmin, pmax);
  LINT t = pmax - pmin;

  if  (p < pmin)
    {
      p += pmin;
    }

  if (p.iseven ())
    {
      ++p;
    }

  if (p > pmax)
    {
      p = pmin + p % (t + 1);
    }

  while ((gcd (p - 1, f) != 1) || !p.isprime ())
    {
      ++p;
      ++p;

      while (p > pmax)
        {
          p = pmin + p % (t + 1);

          if (p.iseven ())
            {
              ++p;
            }
        }
    }

  return p;
}


// Find smallest prime number p >= a with gcd(p - 1, f) = 1

const LINT nextprime (const LINT& a, const LINT& f)
{
  LINT p(a);
  if (p.iseven ()) ++p;
  while (!p.isprime () || (gcd (p - 1, f) != 1))
    {
      ++p;
      ++p;
    }
  return p; // p is smallest prime number p >= a with gcd(p - 1, f) = 1
}


// Find prime number p of length 2^(l-1) <= p <= 2^l - 1
// with p = a mod q and gcd (p - 1, f) = 1.
// Input parameter: 2 < q prime, a mod q != 0, 0 < f odd

const LINT extendprime (const USHORT l,
                        const LINT& a,
                        const LINT& q,
                        const LINT& f)
{
  if (l > CLINTMAXBIT) LINT::panic (E_LINT_VAL, "extendprime", 1, __LINE__);

  LINT pmin = LINT(0).setbit (l - 1);
  LINT pmax = LINT(0).setbit (l);
  --pmax;

  return (extendprime (pmin, pmax, a, q, f));
}


// Find prime number p of length pmin <= p <= pmax
// with p = a mod q and gcd (p - 1, f) = 1.
// Input parameter: 2 < q prime, a mod q != 0, 0 < f odd

const LINT extendprime (const LINT& pmin,
                        const LINT& pmax,
                        const LINT& a,
                        const LINT& q,
                        const LINT& f)
{
  if (!pmin.init) LINT::panic (E_LINT_VAL, "extendprime", 1, __LINE__);
  if (!pmax.init) LINT::panic (E_LINT_VAL, "extendprime", 2, __LINE__);
  if (!a.init) LINT::panic (E_LINT_VAL, "extendprime", 3, __LINE__);
  if (!q.init) LINT::panic (E_LINT_VAL, "extendprime", 4, __LINE__);
  if (!f.init) LINT::panic (E_LINT_VAL, "extendprime", 5, __LINE__);

  if (pmin > pmax) LINT::panic (E_LINT_VAL, "extendprime", 1, __LINE__);

  // q required to be odd
  if (q.iseven ()) LINT::panic (E_LINT_VAL, "extendprime", 3, __LINE__);

  // a must not be divided by q
  if (a.mequ (0,q)) LINT::panic (E_LINT_VAL, "extendprime", 2, __LINE__);

  // 0 < f required to be odd
  if (f.iseven ()) LINT::panic (E_LINT_VAL, "extendprime", 4, __LINE__);

  LINT p = randBBS (pmin, pmax);
  LINT twotimesq = q << 1;
  LINT t = pmax - pmin;

  if (p < pmin)
    {
      p += pmin;
    }

  LINT w = p % twotimesq;

  p += (twotimesq - w) + a;

  if (p.iseven ())
    {
      p += q;
    }

  while (p > pmax)
    {
      p = pmin + p % (t + 1);
      w = p % twotimesq;
      p += (twotimesq - w) + a;
      if (p.iseven ())
        {
          p += q;
        }
    }

  while (1 != gcd (p - 1, f) || !p.isprime ())
    {

      Assert (twotimesq < pmax);

      p += twotimesq;

      while (p > pmax)
        {
          p += usrandBBS_l ();
          p = pmin + p % (t + 1);

          Assert (p < pmax);

          w = p % twotimesq;
          p += (twotimesq - w) + a;

          Assert (p < pmax);

          if (p.iseven ())
            {
              p += q;
            }

          Assert (p < pmax);

        }
    }

  Assert ((p - a) % q == 0);

  return p; // p is prime number with p = a mod q, gcd (p - 1, f) = 1
}


// Find strong prime p of length 2^(l-1) <= p <= 2^l - 1
// with primes r, s, t, such that
//    r divides p - 1
//    t divides r - 1
//    s divides p + 1
//
// Input parameter: Binary length l of p

const LINT strongprime (const USHORT l)
{
  if (l >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 1, __LINE__);

  LINT pmin = LINT(0).setbit (l - 1);
  LINT pmax = LINT(0).setbit (l);
  --pmax;

  return (strongprime (pmin, pmax, (l>>2)-8, (l>>1)-8, (l>>1)-8, 1));
}


// Find strong prime p of length 2^(l-1) <= p <= 2^l - 1
// with gcd (p - 1, f) = 1 and primes r, s, t such that
//    r divides p - 1
//    t divides r - 1
//    s divides p + 1
//
// Input parameter: Binary length l of p,
//                  0 < f odd

const LINT strongprime (const USHORT l, const LINT& f)
{
  if (l >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 1, __LINE__);

  LINT pmin = LINT(0).setbit (l - 1);
  LINT pmax = LINT(0).setbit (l);
  --pmax;

  return (strongprime (pmin, pmax, (l>>2)-8, (l>>1)-8, (l>>1)-8, f));
}


// Find strong prime p of length 2^(l-1) <= p <= 2^l - 1
// with gcd (p - 1, f) = 1 and primes r, s, t such that
//    r divides p - 1
//    t divides r - 1
//    s divides p + 1
//
// Input parameter: Binary length l of p,
//                  Lengths lt, lr and ls of primes t, r and s resp.
//                     lt <~ l/4, lr ~ ls <~ l/2 of l
//                  <~ means: smaller than, close to
//                  0 < f odd

const LINT strongprime (const USHORT l,
                        const USHORT lt,
                        const USHORT lr,
                        const USHORT ls,
                        const LINT& f)
{
  if (l >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 1, __LINE__);
  if (lt >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 2, __LINE__);
  if (lr >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 3, __LINE__);
  if (ls >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 4, __LINE__);

  LINT pmin = LINT(0).setbit (l - 1);
  LINT pmax = LINT(0).setbit (l);
  --pmax;

  return (strongprime (pmin, pmax, lt, ls, lr, f));
}


// Find strong prime p with pmin <= p <= pmax
// with gcd (p - 1, f) = 1 and primes r, s, t such that
//    r divides p - 1
//    t divides r - 1
//    s divides p + 1
//
// Input parameter: pmin, pmax
//                  0 < f odd

const LINT strongprime (const LINT& pmin,
                        const LINT& pmax,
                        const LINT& f)
{
  if (!pmin.init) LINT::panic (E_LINT_VAL, "strongprime", 1, __LINE__);
  if (!pmax.init) LINT::panic (E_LINT_VAL, "strongprime", 2, __LINE__);
  if (!f.init) LINT::panic (E_LINT_VAL, "strongprime", 6, __LINE__);

  // 0 < f required to be odd
  if (f.iseven ()) LINT::panic (E_LINT_VAL, "strongprime", 5, __LINE__);

  int lt = (ld (pmin) >> 2) - 8;
  int lr = (ld (pmin) >> 1) - 8;
  int ls = lr;

  return strongprime (pmin, pmax, lt, ls, lr, f);
}


// Find strong prime p with pmin <= p <= pmax
// with gcd (p - 1, f) = 1 and primes r, s, t such that
//    r divides p - 1
//    t divides r - 1
//    s divides p + 1
//
// Input parameters: pmin, pmax,
//                   Lengths lt, lr and ls of primes t, r and s resp.
//                     lt <~ l/4, lr ~ ls <~ l/2 of l
//                   <~ means: smaller than, close to
//                   0 < f odd

const LINT strongprime (const LINT& pmin,
                        const LINT& pmax,
                        const USHORT lt,
                        const USHORT lr,
                        const USHORT ls,
                        const LINT& f)
{
  if (!pmin.init) LINT::panic (E_LINT_VAL, "strongprime", 1, __LINE__);
  if (!pmax.init) LINT::panic (E_LINT_VAL, "strongprime", 2, __LINE__);
  if (lt >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 3, __LINE__);
  if (lr >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 4, __LINE__);
  if (ls >= CLINTMAXBIT) LINT::panic (E_LINT_OFL, "strongprime", 5, __LINE__);
  if (!f.init) LINT::panic (E_LINT_VAL, "strongprime", 6, __LINE__);

  // 0 < f required to be odd
  if (f.iseven ()) LINT::panic (E_LINT_VAL, "strongprime", 5, __LINE__);

  LINT t = findprime (lt, 1);
  LINT r = extendprime (lr, 1, t, 1);
  LINT s = findprime (ls, 1);

  LINT p = inv (r,s);  // p := r^(-1) mod s
  p *= r;              // p := r^(-1) * r
  p <<= 1;             // p := 2*r^(-1) * r
  LINT rs = r*s;
  p = msub (1,p,rs);   // p := 1 - 2*r^(-1) * r mod r*s = s*u - r*v mod r*s
                       // with u := s^(-1) mod r and v := r^(-1) mod s

  Assert ((p - 1) % r == 0);
  Assert ((p + 1) % s == 0);

  p = extendprime (pmin, pmax, p, rs, f);

  Assert ((p - 1) % r == 0);
  Assert ((p + 1) % s == 0);

  return p;
}


const int mequ (const LINT& a, const LINT& b, const LINT& m)
{
  int err;
  if (!a.init) LINT::panic (E_LINT_VAL, "mequ", 1, __LINE__);
  if (!b.init) LINT::panic (E_LINT_VAL, "mequ", 2, __LINE__);
  if (!m.init) LINT::panic (E_LINT_VAL, "mequ", 3, __LINE__);

  err = mequ_l (a.n_l, b.n_l, m.n_l);

  switch (err)
    {
      case 0:
      case 1:
        break;
      case E_CLINT_DBZ:
        LINT::panic (E_LINT_DBZ, "mequ", 3, __LINE__);
        break;
      default:
        LINT::panic (E_LINT_ERR, "mequ", err, __LINE__);
    }

  return err;
}


////////////////////////////////////////////////////////////////////////////////
//                    Pseudorandom number generators                          //
////////////////////////////////////////////////////////////////////////////////


// Linear congruential generator

void seedl (const LINT& seed)
{
  if (seed.init) seed64_l (seed.n_l);
  else LINT::panic (E_LINT_VAL, "seed64", 0, __LINE__);
}


LINT randl (const int l)
{
  LINT random;
  rand_l (random.n_l, MIN (l, (int)CLINTMAXBIT));
  random.init = 1;
  return random;
}


LINT randl (const LINT& rmin, const LINT& rmax)
{
  if (!rmin.init) LINT::panic (E_LINT_VAL, "randl", 1, __LINE__);
  if (!rmax.init) LINT::panic (E_LINT_VAL, "randl", 2, __LINE__);
  if (rmax < rmin) LINT::panic (E_LINT_VAL, "randl", 1, __LINE__);

  LINT random;
  LINT t = rmax - rmin;
  USHORT l = (ld (rmin) + ld (rmax)) >> 1;

  rand_l (random.n_l, MIN (l, (int)CLINTMAXBIT));
  random.init = 1;

  if (random < rmin)
    {
      random += rmin;
    }

  if (random > rmax)
    {
      random = rmin + random % (t + 1);
    }

  Assert ((random >= rmin) && (random <= rmax));

  return random;
}




// Blum-Blum-Shub Generator

int seedBBS (const LINT& seed)
{
  if (!seed.init) LINT::panic (E_LINT_VAL, "seedBBS", 0, __LINE__);
  return (seedBBS_l (seed.n_l));
}


LINT randBBS (const int l)
{
  LINT random;
  randBBS_l (random.n_l, MIN (l, (int)CLINTMAXBIT));
  random.init = 1;
  return random;
}


LINT randBBS (const LINT& rmin, const LINT& rmax)
{
  if (!rmin.init) LINT::panic (E_LINT_VAL, "randBBS", 1, __LINE__);
  if (!rmax.init) LINT::panic (E_LINT_VAL, "randBBS", 2, __LINE__);
  if (rmax < rmin) LINT::panic (E_LINT_VAL, "randBBS", 1, __LINE__);

  LINT random;
  LINT t = rmax - rmin;
  USHORT l = (ld (rmin) + ld (rmax)) >> 1;

  randBBS_l (random.n_l, MIN (l, (int)CLINTMAXBIT));
  random.init = 1;

  if (random < rmin)
    {
      random += rmin;
    }

  if (random > rmax)
    {
      random = rmin + random % (t + 1);
    }

  Assert ((random >= rmin) && (random <= rmax));

  return random;
}

////////////////////////////////////////////////////////////////////////////////
//                         Input/Output                                       //
////////////////////////////////////////////////////////////////////////////////


int LINT::flagsindex;
// changed from long to int; Oliver Kullmann, 28.10.2004

// Initialization of class member setup (class LintInit)
// The initialization of LINT::setup causes a call to the constructor 
// LintInit(), which in turn initializes the static member flagsindex as 
// a pointer to ios-internal memory: 

LintInit LINT::setup; /*lint !e1502*/


// Constructor LintInit() sets the ios-internal variable ios::iword (flagsindex)
// to the LINT default values:

LintInit::LintInit (void)
{
  // Get index to status variable of type long in class ios
  LINT::flagsindex = ios::xalloc ();

  // Set default status in cout and cerr
  cout.iword (LINT::flagsindex) = cerr.iword (LINT::flagsindex) =
           LINT::lintshowlength | LINT::linthex | LINT::lintshowbase;
}


// Read static LINT status variable in ios for standard output stream cout

long LINT::flags (void)
{
  return cout.iword (flagsindex);
}


// Read static LINT status variable in ios for output stream ostream s

long LINT::flags (ostream& s)
{
  return s.iword (flagsindex);
}


// Set static LINT status variable in ios for output stream ostream s

long LINT::setf (ostream& s, long flag)
{
  long t = s.iword (flagsindex);

  // Flags lintdec, linthex, lintbin, lintoct are mutually exclusive:
  if (flag & LINT::lintdec)
    {
      s.iword (flagsindex) = 
        (t & ~LINT::linthex & ~LINT::lintoct & ~LINT::lintbin) | LINT::lintdec;
      flag ^= LINT::lintdec;
    }

  if (flag & LINT::linthex)
    {
      s.iword (flagsindex) =
        (t & ~LINT::lintdec & ~LINT::lintoct & ~LINT::lintbin) | LINT::linthex;
      flag ^= LINT::linthex;
    }

  if (flag & LINT::lintoct)
    {
      s.iword (flagsindex) = 
        (t & ~LINT::lintdec & ~LINT::linthex & ~LINT::lintbin) | LINT::lintoct;
      flag ^= LINT::lintoct;
    }
  
  if (flag & LINT::lintbin)
    {
      s.iword (flagsindex) = 
        (t & ~LINT::lintdec & ~LINT::lintoct & ~LINT::linthex) | LINT::lintbin;
      flag ^= LINT::lintbin;
    } 

  // All remaining flags are mutually compatible:
  s.iword (flagsindex) |= flag;

  return t;
}


// Set static LINT status variable in ios for standard output stream cout

long LINT::setf (long flag)
{
  return LINT::setf (cout, flag);
}


// Switch off status bits in ostream s

long LINT::unsetf (ostream& s, long flag)
{
  long t = s.iword (flagsindex);
  // changed from int to long; Oliver Kullmann, 28.10.2004
  s.iword (flagsindex) = (t | flag) ^ flag;
  return t;
}


// Switch off status bits in stream cout

long LINT::unsetf (long flag)
{
  return LINT::unsetf (cout, flag);
}


// Reset of LINT status variable for stream ostream s

long LINT::restoref (ostream& s, long flag)
{
  int t = s.iword (flagsindex);
  LINT::unsetf (s, t);
  s.iword (flagsindex) = flag;
  return t;
}


// Reset of LINT status variable for standard stream ostream cout

long LINT::restoref (long flag)
{
  return LINT::restoref (cout, flag);
}


// Manipulators

ostream& LintHex (ostream& s)
{
  LINT::setf (s, LINT::linthex);
  return s;
}

ostream& LintDec (ostream& s)
{
  LINT::setf (s, LINT::lintdec);
  return s;
}

ostream& LintOct (ostream& s)
{
  LINT::setf (s, LINT::lintoct);
  return s;
}

ostream& LintBin (ostream& s)
{
  LINT::setf (s, LINT::lintbin);
  return s;
}

ostream& LintUpr (ostream& s)
{
  LINT::setf (s, LINT::lintuppercase);
  return s;
}

ostream& LintLwr (ostream& s)
{
  LINT::unsetf (s, LINT::lintuppercase);
  return s;
}

ostream& LintShowbase (ostream& s)
{
  LINT::setf (s, LINT::lintshowbase);
  return s;
}

ostream& LintNobase (ostream& s)
{
  LINT::unsetf (s, LINT::lintshowbase);
  return s;
}

ostream& LintShowlength (ostream& s)
{
  LINT::setf (s, LINT::lintshowlength);
  return s;
}

ostream& LintNolength (ostream& s)
{
  LINT::unsetf (s, LINT::lintshowlength);
  return s;
}


// Manipulators SetLintFlags, ResetLintFlags

ostream& _SetLintFlags (ostream& s, int flag)
{
  LINT::setf (s, flag);
  return s;
}

ostream& _ResetLintFlags (ostream& s, int flag)
{
  LINT::unsetf (s, flag);
  return s;
}

LINT_omanip <int> SetLintFlags (int flag)
{
  return LINT_omanip <int> (&_SetLintFlags, flag);
}

LINT_omanip <int> ResetLintFlags (int flag)
{
  return LINT_omanip <int> (&_ResetLintFlags, flag);
}


// Overloading of ostream operator << for ouput of LINT objects

ostream& operator << (ostream& s, const LINT& ln)
{
  unsigned short base = 16;
  long flags = LINT::flags (s);
  char* formatted_lint;

  if (!ln.init) LINT::panic (E_LINT_VAL, "ostream operator<<", 0, __LINE__);

  if (flags & LINT::linthex)
    {
      base = 16;
    }
  else
    {
      if (flags & LINT::lintdec)
        {
          base = 10;
        }
      else
        {
          if (flags & LINT::lintoct)
            {
              base = 8;
            }
          else
            {
              if (flags & LINT::lintbin)
                {
                  base = 2;
                }
            }
        }
    }
 
  if (flags & LINT::lintshowbase)
    {
      formatted_lint = lint2str (ln, base, 1);
    }
  else
    {
      formatted_lint = lint2str (ln, base, 0);
    }

  if (flags & LINT::lintuppercase)
    {
      strupr_l (formatted_lint);
    }

  s << formatted_lint << flush;

  if (flags & LINT::lintshowlength)
    {
      ios::fmtflags _flags = s.flags ();  //get current flag settings
      // Oliver Kullmann, 17.4.2002: long -> ios::fmtflags
      s.setf (ios::dec);         //lint !e641
      s << endl << ld (ln) << " bit\n" << endl;
      s.setf (_flags);           //restore flags
    }

  return s;
}


// Overloading of ifstream/ofstream operators >>/<< for file input/ouput 
// of LINT objects

ofstream& operator<< (ofstream& s, const LINT& ln)
{
  if (!ln.init) LINT::panic (E_LINT_VAL, "ofstream operator <<", 0, __LINE__);
  for (int i = 0; i <= (int) DIGITS_L (ln.n_l); i++)
    {
      if (write_ind_ushort (s, ln.n_l[i]))
        {
          LINT::panic (E_LINT_EOF, "ofstream operator <<", 0, __LINE__);
        }
    }
  return s;
}


fstream& operator<< (fstream& s, const LINT& ln)
{
  if (!ln.init) LINT::panic (E_LINT_VAL, "fstream operator <<", 0, __LINE__);
  for (int i = 0; i <= (int) DIGITS_L (ln.n_l); i++)
    {
      if (write_ind_ushort (s, ln.n_l[i]))
        {
          LINT::panic (E_LINT_EOF, "fstream operator <<", 0, __LINE__);
        }
    }
  return s;
}


ifstream& operator>> (ifstream& s, LINT& ln)
{
  if (read_ind_ushort (s, ln.n_l))
    {
      LINT::panic (E_LINT_EOF, "ifstream operator >>", 0, __LINE__);
    }

  if (DIGITS_L (ln.n_l) < CLINTMAXSHORT)
    {
      for (int i = 1; i <= (int) DIGITS_L (ln.n_l); i++)
        {
          if (read_ind_ushort (s, &ln.n_l[i]))
            {
              LINT::panic (E_LINT_EOF, "ifstream operator >>", 0, __LINE__);
            }
        }
    }
  if (vcheck_l (ln.n_l) == 0)    // be paranoid - check imported values!
    {
      ln.init = 1;
    }
  else
    {
      ln.init = 0;
    }
  return s;
}


fstream& operator>> (fstream&s, LINT& ln)
{
  if (read_ind_ushort (s, ln.n_l))
    {
      LINT::panic (E_LINT_EOF, "fstream operator >>", 0, __LINE__);
    }

  if (DIGITS_L (ln.n_l) < CLINTMAXSHORT)
    {
      for (int i = 1; i <= (int) DIGITS_L (ln.n_l); i++)
        {
          if (read_ind_ushort (s, &ln.n_l[i]))
            {
              LINT::panic (E_LINT_EOF, "fstream operator >>", 0, __LINE__);
            }
        }
    }
  if (vcheck_l (ln.n_l) == 0)
    { // Be paranoid ...
      ln.init = 1;
    }
  else
    {
      ln.init = 0;
    }
  return s;
}


// Conversion of type LINT to character string by lint2str

// lint2str as member function:

char* LINT::lint2str (const USHORT base, const int showbase) const
{
  if (!init) LINT::panic (E_LINT_VAL, "lint2str", 0, __LINE__);
  return xclint2str_l (n_l, base, showbase);
}


// lint2str as friend function

char* lint2str (const LINT& ln, const USHORT  base, const int showbase)
{
  if (!ln.init) LINT::panic (E_LINT_VAL, "lint2str", 0, __LINE__);
  return xclint2str_l (ln.n_l, base, showbase);
}


// Conversion of type LINT to byte array by lint2byte

// lint2byte as member function:

UCHAR* LINT::lint2byte (int* len) const
{
  if (!init) LINT::panic (E_LINT_VAL, "lint2byte", 0, __LINE__);
  return clint2byte_l (n_l, len);
}


// lint2byte as friend function

UCHAR* lint2byte (const LINT& ln, int* len)
{
  if (!ln.init) LINT::panic (E_LINT_VAL, "lint2byte", 0, __LINE__);
  return clint2byte_l (ln.n_l, len);
}


////////////////////////////////////////////////////////////////////////////////
//                         Error-Handling                                     //
////////////////////////////////////////////////////////////////////////////////

//Get error status

LINT_ERRORS LINT::Get_Warning_Status (void)
{
  return status;
}

//Set user defined error handler

static void (*LINT_User_Error_Handler)(LINT_ERRORS, const char* const, const int, const int) = NULL;

void LINT::Set_LINT_Error_Handler (void (*Error_Handler)(LINT_ERRORS,
                                     const char* const, const int, const int))
{
  LINT_User_Error_Handler = Error_Handler;
}


// Set default error handler (member function)

void LINT::panic (LINT_ERRORS error, const char* const func, const int arg, const int line)
{
  if (LINT_User_Error_Handler)
    {
      LINT_User_Error_Handler (error, func, arg, line);
    }
  else
    {
      cerr << "Critical runtime error detected by class LINT:\n";
      switch (error)
        {
          case E_LINT_OK:
            // O.K., do nothing
            break;

          case E_LINT_DBZ:
            cerr << "Division by zero, operator/function " << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_DivByZero (func, line);
#endif
            break;

          case E_LINT_EOF:
            cerr << "Error in file I/O, operator/function " << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_File (func, line);
#endif
            break;

          case E_LINT_OFL:
            cerr << "Overflow, operator/function " << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_OFL (func, line);
#endif
            break;

          case E_LINT_UFL:
            cerr << "Underflow, operator/function " << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_UFL (func, line);
#endif
            break;

          case E_LINT_NHP:
            cerr << "Error in new, function/operator " << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_Heap (func, line);
#endif
            break;

          case E_LINT_VAL:
            cerr << "Argument " << arg << " in operator/function " << func;
            cerr << " uninitialized" << endl;
            cerr << "or has invalid value, line ";
            cerr << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_Init (func, arg, line);
#endif
            break;

          case E_LINT_BOR:
            cerr << "Base invalid, operator/function " << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_Base (func, line);
#endif
            break;

          case E_LINT_MOD:
            cerr << "Modulus is even, operator/function " << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_Emod (func, line);
#endif
            break;

          case E_LINT_NPT:
            cerr << "Argument " << arg << " is Null-pointer in operator/function" << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_Nullptr (func, arg, line);
#endif
            break;

          case E_LINT_ERR:
          default:
            cerr << "Unexpected Error in operator/function " << func;
            cerr << ", line " << line << ", module " << __FILE__ << endl;
#ifdef LINT_EX
            throw LINT_Mystic (func, arg, line);
#endif
        }
      abort ();
    }
}

///////////////////////////////////////////////////////////////////////////////
//    Exception handling                                                     //
///////////////////////////////////////////////////////////////////////////////

LINT_DivByZero::LINT_DivByZero (const char* const func, const int line)
{
  function = func;
  lineno = line;
  argno = 0;
}

void LINT_DivByZero::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Division by zero in operator/function " << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_File::LINT_File (const char* const func, const int line)
{
  function = func;
  lineno = line;
  argno = 0;
}

void LINT_File::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Error in file I/O, operator " << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_Init::LINT_Init (const char* const func, const int arg, const int line)
{
  function = func;
  lineno = line;
  argno = arg;
}

void LINT_Init::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Argument " << argno << " in operator/function "
       << function << " is not initialized" << endl;
  cerr << "or has invalid value" << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_Heap::LINT_Heap (const char* const func, const int line)
{
  function = func;
  lineno = line;
  argno = 0;
}

void LINT_Heap::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Error in new in operator/function " << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_OFL::LINT_OFL (const char* const func, const int line)
{
  function = func;
  lineno = line;
  argno = 0;
}

void LINT_OFL::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Overflow in operator/function " << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_UFL::LINT_UFL (const char* const func, const int line)
{
  function = func;
  lineno = line;
  argno = 0;
}

void LINT_UFL::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Underflow in operator/function " << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_Base::LINT_Base (const char* const func, const int line)
{
  function = func;
  lineno = line;
  argno = 0;
}

void LINT_Base::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Base invalid in operator/function " << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_Emod::LINT_Emod (const char* const func, const int line)
{
  function = func;
  lineno = line;
  argno = 0;
}

void LINT_Emod::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Modulus even in Montgomery-function " << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_Nullptr::LINT_Nullptr (const char* const func, const int arg, const int line)
{
  function = func;
  lineno = line;
  argno = arg;
}

void LINT_Nullptr::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Argument " << argno << " is Null-pointer in operator/Function ";
  cerr << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}


LINT_Mystic::LINT_Mystic (const char* const func, const int errcode, const int line)
{
  function = func;
  lineno = line;
  argno = errcode;
}

void LINT_Mystic::debug_print (void) const
{
  cerr << "LINT-Exception:" << endl;
  cerr << "Unexpected error " << argno << " in operator/function "
       << function << endl;
  cerr << "Module: " << __FILE__ << ", Line: " << lineno << endl;
}

////////////////////// End of flintpp.cpp //////////////////////////////////////







