# Oliver Kullmann, 25.7.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

#! \file ComputerAlgebra/Cryptology/Rijndael.sage
#! \brief Implementation of Rijndael with the sage-iphython-system
#! \deprecated To be replaced by genuine computer algebra

#
# Use By
#
# attach $OKplatform/OKsystem/Transitional/ComputerAlgebra/Cryptology/Rijndael.sage 
#
# where $OKPLATFORM has to be replaced by the path to the OKplatform directory. 
#
# Example of use
#
# sage: k = hexToGF2t8Mat('00000000000000000000000000000000')                
# sage: p = hexToGF2t8Mat('00000000000000000000000000000000')                
# sage: c = rijndael_en(p,k)                                                 
# sage: GF2t8MatToHex(c)                                                     
# 66E94BD4EF8A2C3B884CFA59CA342B2E
# sage: GF2t8MatToHex( rijndael_de(c,k) )                                    
# 00000000000000000000000000000000
# sage: k = hexToGF2t8Mat('000000000000000000000000000000000000000000000000')
# sage: GF2t8MatToHex( rijndael_en(p,k) )                                    
# AAE06992ACBF52A3E8F4A96EC9300BD7
#
# The hex notation used above is the same as used in the text Design of Rijndael,
# a columnwise byte representation in hex (2 digits per byte).

import copy
# Creates a field GF(2) with the free variable x over it
# and then uses this to create the field GF(2^8) with
# the modulus defined for Rijndael.
BIT.<z> = GF(2)[]
BYTE.<a> = GF(2**8, 'a', z^8 + z^4 + z^3 + z + 1)

def natToGF2t8(n) :
    """ Converts From a number in Nat to the corresponding value in GF(2^8) given 
        that the number is a binary value and the binary bits are seen to be 
        the coefficients in GF(2) or the polynomials making up GF(2^8)"""
    return BYTE.fetch_int(n)
 
def GF2t8ToNat(n) :
    """ Converts to a number in Nat from the corresponding value in GF(2^8) 
        given that the number is a binary value and the binary bits are 
        seen to be the coefficients in GF(2) or the polynomials making up GF(2^8)"""
    return int(n)

def BitVToGF2t8(v) :
    """ Converts a vector of bits to an element in GF(2^8) """
    return (v[0] + v[1] * a^1 + v[2] * a^2 + v[3] * a^3 + v[4] * a^4 + 
            v[5] * a^5 + v[6] * a^6 + v[7] * a^7)

def GF2t8ToBitV(re) :
    """ Converts an element in GF(2^8) to a vector of bits """
    return re.vector()


def listToGF2t8Mat(rl) :
    """ Converts a list of natural numbers to a matrix of elements in GF(2^8) """
    return matrix(BYTE,4, map(natToGF2t8, rl) )

def listFromGF2t8Mat(rm) :
    """ Converts a matrix of GF(2^8) elements to a list of natural numbers """
    return map(GF2t8ToNat, rm.list())

def hexToGF2t8Mat(n) :
    """ Converts a hexidecimal string representing column-wise bits
    in a Rijndael block into a matrix of GF(2^8) elements used in
    rijndael_en """
    rl = []
    for i in range(0,len(n),2) :
            rl.append(int(n[i:i+2],16))
    return matrix(BYTE, len(rl)/4, map(natToGF2t8, rl)).transpose()

def GF2t8MatToHex(rm) :
    """ Converts a 4xn matrix of GF(2^8) elements to a list of 2 digit hexidecimal 
    strings where the hexidecimal value is the integer representation of the bitstring
    representation of the coefficients of the GF(2^8) element's polynomial, where the
    MSB is the coefficient of the highest order. """
    print reduce(lambda x,y : x + y[2:].rjust(2,'0'), map(hex, listFromGF2t8Mat(rm.transpose())),'').upper()

def numberOfRounds(nb,nk) :
    """ Given the size of the plaintext and the key in bytes, returns
        the number of rounds suggested by rijndael """
    nb = nb / 4 - 4
    nk = nk / 4 - 4
    return 10 + max(nb,nk)

# Rijndael Mix Columns Matrix
# Equivalent to multiplication by constant in the 4-byte ring.
RMCM = listToGF2t8Mat( 
    [2 , 3 , 1 , 1,
     1 , 2 , 3 , 1,
     1 , 1 , 2 , 3,
     3 , 1 , 1 , 2])

# Rijndael SBox
SBOX_MATRIX = matrix(GF(2),8,
    [1,0,0,0,1,1,1,1,
     1,1,0,0,0,1,1,1,
     1,1,1,0,0,0,1,1,
     1,1,1,1,0,0,0,1,
     1,1,1,1,1,0,0,0,
     0,1,1,1,1,1,0,0,
     0,0,1,1,1,1,1,0,
     0,0,0,1,1,1,1,1])

def aesPerm(n, nb) :
    """ Given a value n, maps n to m where m is the new position that the Rijndael 
        byte should map to in the block during the ShiftRows operation """
    nb = nb / 4
    row = floor(n / nb)
    if row == 3 and nb >= 7 :
        offset = 4
    elif row == 2 and nb == 8 :
        offset = 3
    else :
        offset = row
    position = int(n) % int(nb)
    return ((row * nb) + ((position - offset) % nb))

def S_rd(x) :
    """ Takes a given byte in GF(2^8) and applies the subbytes operation to it """
    return BitVToGF2t8(SBOX_MATRIX * GF2t8ToBitV(x ^ -1)) + BitVToGF2t8((1,1,0,0,0,1,1,0))

def InvS_rd(x) :
    return (BitVToGF2t8((SBOX_MATRIX^-1) * GF2t8ToBitV(x)) + BitVToGF2t8((1,0,1,0,0,0,0,0))) ^ -1

############## Main Round Operations ###################

def MixColumns(rv) :
    """ Takes a 4xn matrix of GF(2^8) elements and returns
    a matrix of GF(2^8) elements after the MixColumns Step """
    return RMCM * rv

def InvMixColumns(rv) :
    """ Takes a 4xn matrix of GF(2^8) elements and returns
    a matrix of GF(2^8) elements after the Inverse MixColumns Step """
    return (RMCM ^ -1) * rv

def ShiftRows(rm, perm = aesPerm) :
    """ Takes a 4xn matrix of GF(2^8) elements and optionally
    an arbitrary integer permutation function (f(x) = x') and returns
    a matrix of GF(2^8) elements after the ShiftRows (or the custom) permutation
    has been applied """
    rml = rm.list()
    rl = [1] * len(rml)
    for x in range(0,len(rml)) :
        rl[perm(x,len(rml))] = rml[x]
    return matrix(BYTE,4,rl)

def InvShiftRows(rm, perm = aesPerm) :
    """ Takes a 4xn matrix of GF(2^8) elements and optionally
    an arbitrary integer permutation function (f(x) = x') and returns
    a matrix of GF(2^8) elements after the Inverse ShiftRows (or the custom) permutation
    has been applied. 
    
    The arbitrary permutation function is applied in the inverse manner to ShiftRows, so
    the same permutation function should be used in each. """
    rml = rm.list()
    rl = [1] * len(rml)
    for x in range(0,len(rml)) :
        rl[x] = rml[perm(x,len(rml))]
    return matrix(BYTE,4,rl)

def SubBytes(rm) :
    """ Takes a 4xn matrix of GF(2^8) elements and applies the Rijndael
    Sbox pointwise to each element, returning a 4xn matrix of GF(2^8)
    elements """
    return matrix(BYTE,4,map(S_rd, rm.list()))

def InvSubBytes(rm) :
    """ Takes a 4xn matrix of GF(2^8) elements and applies the Inverse of the Rijndael
    Sbox pointwise to each element, returning a 4xn matrix of GF(2^8)
    elements """
    return matrix(BYTE, 4, map(InvS_rd, rm.list()))

def getExpandedKey(km, pl, r) :
    """ Takes in a key matrix, the plaintext length (in bytes) and the
        number of rounds and produces a list of bytes representing the ExpandedKey """
    ekrl = map (lambda x : x.list(), km.rows()) # Get a list of rows
    nk = len(km.list())
    prkrl = copy.deepcopy(ekrl)
    i = 1
    while (( len(ekrl[0])*4 ) <= ( pl * (r+1) )) :
        prkrl = nextKey(prkrl, i)
        ekrl = map( lambda (x,y) : x + y, zip(ekrl, prkrl)) # Add the new key to the block 
        i += 1
    return matrix(ekrl).list()

def getRoundKey(ekl, pl, r) :
    """ Given the expanded key (list), the length of the plain text (in bytes) and the round
        to get the key for """
    ekm = matrix(BYTE,4,ekl)
    rkl = map(lambda x : x.list()[(r * (pl/4)): ((r+1) * (pl/4))], ekm.rows())
    return matrix(BYTE,rkl)

def nextKey(prkl, r) :
    """ Produces the next round key from the previous round key (given as a list of rows) given the number
        of the current round (i). This method works for key sizes <= 196bit """
    nk = len(prkl[0])
    nrkl = copy.deepcopy(prkl)
    # First Column 
    nrkl[0][0] = prkl[0][0] + S_rd(prkl[1][nk-1]) + (a^(r-1))
    for i in range(1,4) :
        nrkl[i][0] = prkl[i][0] + S_rd(prkl[(i+1)%4][(nk - 1)])
    # Subsequent Columns
    if nk <= 6 :
        for col in range(1,nk) :
            for i in range(0,4) :
                nrkl[i][col] = nrkl[i][col-1] + prkl[i][col]
    else :
        for col in range(1,4) :
            for i in range(0,4) :
                nrkl[i][col] = nrkl[i][(col-1)] + prkl[i][col]
        for i in range(0,4) :
            nrkl[i][4] = S_rd(nrkl[i][(4 - 1)]) + prkl[i][4]
        for col in range(5,nk) :
            for i in range(0,4) :
                nrkl[i][col] = nrkl[i][(col-1)] + prkl[i][col]
    return nrkl

def rijndael_en(pm,km,r = -1) :
    """ Takes a 4xn matrix of GF(2^8) elements plaintext,
    a 4xn matrix of GF(2^8) elements key and optionally
    an integer (>0) number of rounds to apply and returns
    a 4xn matrix of GF(2^8) elements representing the results
    of the Rijndael block cipher given the parameters."""
    nb = len(pm.list())
    if r == -1 :
        r = numberOfRounds(nb, len(km.list()))
    ekl = getExpandedKey(km, nb, r)
    rkm = getRoundKey(ekl, nb, 0)
    # Initial Round
    rm = pm + rkm
    # Middle Rounds
    for i in range(1,r) :
        rm = SubBytes(rm)
        rm = ShiftRows(rm)
        rm = MixColumns(rm)
        rkm = getRoundKey(ekl, nb, i)
        rm = rm + rkm
    # Final Round
    rm = SubBytes(rm)
    rm = ShiftRows(rm)
    rkm = getRoundKey(ekl, nb, r)
    rm = rm + rkm
    return rm

def rijndael_de(cm,km,r = -1) :
    """ Takes a 4xn matrix of GF(2^8) elements plaintext,
    a 4xn matrix of GF(2^8) elements key and optionally
    an integer (>0) number of rounds to apply and returns
    a 4xn matrix of GF(2^8) elements representing the results
    of the Rijndael block cipher given the parameters."""
    nb = len(cm.list())
    if r == -1 :
        r = numberOfRounds(nb, len(km.list()))
    ekl = getExpandedKey(km, nb, r)
    # Final Round
    rkm = getRoundKey(ekl, nb, r)
    rm = cm + rkm
    rm = InvShiftRows(rm)
    rm = InvSubBytes(rm)
    # Middle Rounds
    for i in range(r-1,0,-1) :
        rkm = getRoundKey(ekl, nb, i)
        rm = rm + rkm
        rm = InvMixColumns(rm)
        rm = InvShiftRows(rm)
        rm = InvSubBytes(rm)
    # Initial Round
    rkm = getRoundKey(ekl, nb, 0)
    rm = rm + rkm
    return rm

###################### Tests ##################################

# Test vectors from Design of Rijndael - Appendix D

def RijndaelTest () :
    """ Rijndael Test Suite. 
    
    This can be run to perform a set of tests to try to ensure the correctness of the implementation. A message
    will be printed to screen in the event of each failure, and in the event of no failure, the final message
    should read 'n/n Tests Passed' where n is a positive integer. """
    numTests = 6
    numFailed = 0
    # Just to make sure the two above functions are the inverses of each other
    if (not (reduce( lambda x, y : x and y ,[ GF2t8ToNat(natToGF2t8(x)) == x for x in range(0,256)]))) :
        print "GF2t8ToNat Conversion Test failed. GF2t8ToNat is not the inverse of natToGF2t8!"
        numFailed += 1
    # Test the Sbox/subbytes works for a given test vector
    sbox1 = listToGF2t8Mat([25, 61, 227, 190, 160, 244, 226, 43, 154, 198, 141, 42, 233, 248, 72, 8]).transpose()
    if  (not (listFromGF2t8Mat(SubBytes(sbox1).transpose()) == 
        [212, 39, 17, 174, 224, 191, 152, 241, 184, 180, 93, 229, 30, 65, 82, 48])) :
        print "Sbox Test Failed!...."
        numFailed += 1
    # Test to ensure the encryption function works
    pm1 = listToGF2t8Mat([50, 67, 246, 168, 136, 90, 48, 141, 49, 49, 152, 162, 224, 55, 7, 52]).transpose()
    km1 = listToGF2t8Mat([43, 126, 21, 22, 40, 174, 210, 166, 171, 247, 21, 136, 9, 207, 79, 60]).transpose()
    cm1 = rijndael_en(pm1, km1, 10)
    if (not (listFromGF2t8Mat(cm1) == [57, 2, 220, 25, 37, 220, 17, 106, 132, 9, 133, 11, 29, 251, 151, 50])) :
        print "AES Encryption Test Failed!...."
        numFailed += 1
    if (not (rijndael_de(cm1, km1, 10) == pm1)) :
        print "AES Decryption Test Failed!...."
        numFailed += 1
    cm2 = rijndael_en(matrix(BYTE,4,[0]*32), matrix(BYTE,4,[0]*28))
    if (not (listFromGF2t8Mat(cm2) == 
        [188, 54, 187, 221, 53, 51, 80, 28, 24, 156, 39, 102, 109, 192, 210, 97, 191, 
        149, 28, 195, 186, 0, 50, 126, 109, 91, 188, 104, 91, 85, 11, 33])) :
        print "Rijndael 256 bit plaintext, 192 bit key null Encryption Test Failed!...."
        numFailed += 1
    if (not (listFromGF2t8Mat(rijndael_de(cm2,matrix(BYTE,4,[0]*28))) == ([0] * 32))) :
        print "Rijndael 256 bit plaintext, 192 bit key null Decryption Test Failed!...."
        numFailed += 1
    print str(numTests - numFailed) + "/" + str(numTests) + " Tests Passed"

