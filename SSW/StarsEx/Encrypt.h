/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Simple Encryption / Decryption class
*/


#ifndef Encrypt_h
#define Encrypt_h

#include "Types.h"
#include "Text.h"

// +-------------------------------------------------------------------+

class Encryption
{
public:
    // private key encryption / decryption of
    // arbitrary blocks of data
    static Text Encrypt(Text block);
    static Text Decrypt(Text block);

    // encode / decode binary blocks into
    // ascii strings for use in text files
    static Text Encode(Text block);
    static Text Decode(Text block);
};

#endif  // Encrypt_h