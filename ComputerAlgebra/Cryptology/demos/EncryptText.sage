#!/usr/bin/sage
# Matthew Gwynne, 5.12.2007 (Swansea)
# Copyright 2007 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

#! \file ComputerAlgebra/Cryptology/demos/EncryptText.sage
#! \brief Demo showcasing the use of the Sage Rijndael Implementation

#
# Use By
#
# First, ensure the Rijndael module has been compiled into it's precompiled
# python state - 
#
# sage $OKplatform/OKsystem/Transitional/ComputerAlgebra/Cryptology/Rijndael.sage
#
# $OKplatform/OKsystem/Transitional/ComputerAlgebra/Cryptology/demos/EncryptText.sage [encrypt|decrypt] keyfile plainfile cryptfile
#
# where $OKPLATFORM has to be replaced by the path to the OKplatform directory
# and
#
#    keyfile is the path to a file containing the hexidecimal key, assumed to be 
#    128bit with no preceding 0x, i.e it should be 32 characters in length in 
#    plaintext.
#
#    plainfile is the path to the file to encrypt. This for the sake of
#    simplicity should be a plain text file or a file unlikely to contain
#    the byte value 16 (DLE) as this is used for padding during encryption.
#
#    cryptfile is the path where the encrypted file should be output
#
# Example of use
#
#    $ ./EncryptText.sage encrypt key plain crypt
#    $ ./EncryptText.sage decrypt key crypt plain2
#    $ diff plain plain2
#    $
#

from sys import *

# Converts a given byte to 2 digit hex string
def charToHexString(char) :
    return hex(ord(char))[2:].rjust(2,'0')

# Converts a character string to a string of the hexidecimal values
# for each character in the input string.
def strToHexString(strTmp) :
    return reduce(lambda x,y : x + charToHexString(y), block,'')

# Function to convert 2 digit hex string to byte
def hexStringToChar(hexStr) :
    return chr(int(hexStr,16))

# Converts a hexidecimal string into a character string
# where each character is the ascii character given
# by the corresponding 2 digit hexidecimal number
def hexStringToChars(hexStr) :
     chrStr = ""
     for i in range(0,len(hexStr),2) :
         chrStr += hexStringToChar(hexStr[i:i+2])
     return chrStr
        

# Define the block and keysize in bytes
BLOCK_SIZE = 16
KEY_SIZE = 16

# Include the Rijndael Encryption Code
load "../Rijndael.py"

# Grab Arguments
if len(sys.argv) < 4 :
    print "Usage : ./EncryptText.sage [encrypt|decrypt] keyfile plainfile cryptfile"
    exit()
mode = argv[1]
keyFile = argv[2]
plainFile = argv[3]
cryptFile = argv[4]

# Define the correct function based on whether we are
# encrypting or decrypting. Also if encrypting, we need
# to pad out the data to a multiple of BLOCK_SIZE (currently
# using the DLE character and assuming it doesn't occur in the
# file, hence the requirement this is a text file, but this
# could be extended to properly handle such measures but would
# involve unnecessary complications). If decrypting, we need
# to define a function to remove the padding characters we have added.
if mode == "decrypt" :
    encryptionFunc = rijndael_de
    padFunc = lambda block,size : block
    unpadFunc = lambda block : block.rstrip(chr(16)) 
else :
    encryptionFunc = rijndael_en
    padFunc = lambda block ,size : block.ljust(size,chr(16))
    unpadFunc = lambda block : block

# Grab the key
# Currently assumes the key is in hex notation
keyFileHandle = open(keyFile, 'r')
hexKey = keyFileHandle.read()
hexKey = hexKey.strip(" \n")

# Check for correct keysize 
if len(hexKey) != (KEY_SIZE * 2) :
    print "Key size must be 128"
    print hexKey
    sys.exit()
keyBlock = hexToGF2t8Mat(hexKey)

# Open the file to be encrypted and read 8 bytes
plainFileHandle = open(plainFile, 'rb')
cryptFileHandle = open(cryptFile, 'wb')
block = plainFileHandle.read(BLOCK_SIZE)
while block :
    block = padFunc(block,BLOCK_SIZE)
    hexBlock = strToHexString(block)
    plainBlock = hexToGF2t8Mat ( hexBlock ) # Pad out 
    cryptBlock = encryptionFunc( plainBlock, keyBlock)
    cryptString = unpadFunc( hexStringToChars( GF2t8MatToHex( cryptBlock ) ) )
    cryptFileHandle.write(cryptString )
    block = plainFileHandle.read(BLOCK_SIZE)

