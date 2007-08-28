/*!
  \file rijndael.hpp
  \brief Implementation of the Rijndael cipher by Szymon Stefanek

  Stylistic improvements for the OKlibrary.
  \todo Proper testing!
  \todo With C++09 stdint.h likely is to be replaced by cstdint.
*/

#ifndef _RIJNDAEL_H_8uuHgt
#define _RIJNDAEL_H_8uuHgt

//
// File : rijndael.h
// Creation date : Sun Nov 5 2000 03:21:05 CEST
// Author : Szymon Stefanek (stefanek@tin.it)
//
// Another implementation of the Rijndael cipher.
// This is intended to be an easily usable library file.
// This code is public domain.
// Based on the Vincent Rijmen and K.U.Leuven implementation 2.4.
//
// Oliver Kullmann (O.Kullmann@swansea.ac.uk), 1.1.2006: Coding style improvements

//
// Original Copyright notice:
//
//    rijndael-alg-fst.c   v2.4   April '2000
//    rijndael-alg-fst.h
//    rijndael-api-fst.c
//    rijndael-api-fst.h
//
//    Optimised ANSI C code
//
//    authors: v1.0: Antoon Bosselaers
//             v2.0: Vincent Rijmen, K.U.Leuven
//             v2.3: Paulo Barreto
//             v2.4: Vincent Rijmen, K.U.Leuven
//
//    This code is placed in the public domain.
//

//
// This implementation works on 128 , 192 , 256 bit keys
// and on 128 bit blocks
//

//
// Example of usage:
//
//  // Input data
//  unsigned char key[32];                       // The key
//  initializeYour256BitKey();                   // Obviously initialized with sth
//  const unsigned char * plainText = getYourPlainText(); // Your plain text
//  int plainTextLen = strlen(plainText);        // Plain text length
//
//  // Encrypting
//  Rijndael rin;
//  unsigned char output[plainTextLen + 16];
//
//  rin.init(Rijndael::CBC,Rijndael::Encrypt,key,Rijndael::Key32Bytes);
//  // It is a good idea to check the error code
//  int len = rin.padEncrypt(plainText,len,output);
//  if(len >= 0)useYourEncryptedText();
//  else encryptError(len);
//
//  // Decrypting: we can reuse the same object
//  unsigned char output2[len];
//  rin.init(Rijndael::CBC,Rijndael::Decrypt,key,Rijndael::Key32Bytes));
//  len = rin.padDecrypt(output,len,output2);
//  if(len >= 0)useYourDecryptedText();
//  else decryptError(len);
//

#include <stdint.h>

namespace OKlib {
  namespace Rijndael {

    const int max_key_columns = 256/32;
    const int max_rounds = 14;
    const int max_iv_size = 16;

    typedef uint8_t UInt8;
    typedef uint32_t UInt32;

    // Error codes
    const int rijndael_success = 0;
    const int rijndael_unsupported_mode = -1;
    const int rijndael_unsupported_direction = -2;
    const int rijndael_unsupported_key_length = -3;
    const int rijndael_bad_key = -4;
    const int rijndael_not_initialized = -5;
    const int rijndael_bad_direction = -6;
    const int rijndael_corrupted_data = -7;

    class Rijndael
    {	
    public:
      enum Direction { Encrypt , Decrypt };
      enum Mode { ECB , CBC , CFB1 };
      enum KeyLength { Key16Bytes , Key24Bytes , Key32Bytes };
      //
      // Creates a Rijndael cipher object
      // You have to call init() before you can encrypt or decrypt stuff
      //
      Rijndael();
      ~Rijndael();
    protected:
      // Internal stuff
      enum State { Valid , Invalid };

      State     m_state;
      Mode      m_mode;
      Direction m_direction;
      UInt8     m_initVector[max_iv_size];
      UInt32    m_uRounds;
      UInt8     m_expandedKey[max_rounds+1][4][4];
    public:
      //////////////////////////////////////////////////////////////////////////////////////////
      // API
      //////////////////////////////////////////////////////////////////////////////////////////
      
      // init(): Initializes the crypt session
      // Returns rijndael_success or an error code
      // mode      : Rijndael::ECB, Rijndael::CBC or Rijndael::CFB1
      //             You have to use the same mode for encrypting and decrypting
      // dir       : Rijndael::Encrypt or Rijndael::Decrypt
      //             A cipher instance works only in one direction
      //             (Well , it could be easily modified to work in both
      //             directions with a single init() call, but it looks
      //             useless to me...anyway , it is a matter of generating
      //             two expanded keys)
      // key       : array of unsigned octets , it can be 16 , 24 or 32 bytes long
      //             this CAN be binary data (it is not expected to be null terminated)
      // keyLen    : Rijndael::Key16Bytes , Rijndael::Key24Bytes or Rijndael::Key32Bytes
      // initVector: initialization vector, you will usually use 0 here
      int init(Mode mode,Direction dir,const UInt8 *key,KeyLength keyLen,UInt8 * initVector = 0);
      // Encrypts the input array (can be binary data)
      // The input array length must be a multiple of 16 bytes, the remaining part
      // is DISCARDED.
      // so it actually encrypts inputLen / 128 blocks of input and puts it in outBuffer
      // Input len is in BITS!
      // outBuffer must be at least inputLen / 8 bytes long.
      // Returns the encrypted buffer length in BITS or an error code < 0 in case of error
      int blockEncrypt(const UInt8 *input, int inputLen, UInt8 *outBuffer);
      // Encrypts the input array (can be binary data)
      // The input array can be any length , it is automatically padded on a 16 byte boundary.
      // Input len is in BYTES!
      // outBuffer must be at least (inputLen + 16) bytes long
      // Returns the encrypted buffer length in BYTES or an error code < 0 in case of error
      int padEncrypt(const UInt8 *input, int inputOctets, UInt8 *outBuffer);
      // Decrypts the input vector
      // Input len is in BITS!
      // outBuffer must be at least inputLen / 8 bytes long
      // Returns the decrypted buffer length in BITS and an error code < 0 in case of error
      int blockDecrypt(const UInt8 *input, int inputLen, UInt8 *outBuffer);
      // Decrypts the input vector
      // Input len is in BYTES!
      // outBuffer must be at least inputLen bytes long
      // Returns the decrypted buffer length in BYTES and an error code < 0 in case of error
      int padDecrypt(const UInt8 *input, int inputOctets, UInt8 *outBuffer);
    protected:
      void keySched(UInt8 key[max_key_columns][4]);
      void keyEncToDec();
      void encrypt(const UInt8 a[16], UInt8 b[16]);
      void decrypt(const UInt8 a[16], UInt8 b[16]);
    };
    
  }
  
}

#endif
