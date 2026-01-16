/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Simple Encryption / Decryption class
*/


#include "Encrypt.h"
#include "Utils.h"

// +--------------------------------------------------------------------+

static long k[4] = {
    0x3B398E26,
    0x40C29501,
    0x614D7630,
    0x7F59409A
};

static void encypher(long* v)
{
    DWORD y=v[0];
    DWORD z=v[1];
    DWORD sum=0;
    DWORD delta=0x9e3779b9;                // a key schedule constant
    DWORD n=32;                            // num iterations

    while (n-->0) {                        // basic cycle start
        sum += delta;
        y += (z<<4)+k[0] ^ z+sum ^ (z>>5)+k[1];
        z += (y<<4)+k[2] ^ y+sum ^ (y>>5)+k[3];
    }

    v[0]=y;
    v[1]=z;
}

static void decypher(long* v)
{
    DWORD y=v[0];
    DWORD z=v[1];
    DWORD sum=0;
    DWORD delta=0x9e3779b9;                // a key schedule constant
    DWORD n=32;                            // num iterations

    sum=delta<<5;

    while (n-->0) {
        z-= (y<<4)+k[2] ^ y+sum ^ (y>>5)+k[3];
        y-= (z<<4)+k[0] ^ z+sum ^ (z>>5)+k[1];
        sum-=delta;
    }

    v[0]=y;
    v[1]=z;
}

// +-------------------------------------------------------------------+

Text
Encryption::Encrypt(Text block)
{
    int len = block.length();

    if (len < 1)
    return Text();

    // pad to eight byte chunks
    if (len & 0x7) {
        len /= 8;
        len *= 8;
        len += 8;
    }

    BYTE* work = new BYTE[len];
    ZeroMemory(work, len);
    CopyMemory(work, block.data(), block.length());

    long* v = (long*) work;
    for (int i = 0; i < len/8; i++) {
        encypher(v);
        v += 2;
    }

    Text cypher((const char*) work, len);
    delete [] work;
    return cypher;
}

// +-------------------------------------------------------------------+

Text
Encryption::Decrypt(Text block)
{
    int  len    = block.length();

    if (len & 0x7) {
        Print("WARNING: attempt to decrypt odd length block (len=%d)\n", len);
        return Text();
    }

    BYTE* work = new BYTE[len];
    CopyMemory(work, block.data(), len);

    long* v = (long*) work;
    for (int i = 0; i < len/8; i++) {
        decypher(v);
        v += 2;
    }

    Text clear((const char*) work, len);
    delete [] work;
    return clear;
}

// +-------------------------------------------------------------------+

static const char* codes = "abcdefghijklmnop";

Text
Encryption::Encode(Text block)
{
    int   len  = block.length() * 2;
    char* work = new char[len + 1];

    for (int i = 0; i < block.length(); i++) {
        BYTE b = (BYTE) (block.data()[i]);
        work[2*i]   = codes[b>>4 & 0xf];
        work[2*i+1] = codes[b    & 0xf];
    }

    work[len] = 0;

    Text code(work, len);
    delete [] work;
    return code;
}

// +-------------------------------------------------------------------+

Text
Encryption::Decode(Text block)
{
    int   len  = block.length() / 2;
    char* work = new char[len + 1];

    for (int i = 0; i < len; i++) {
        char u = block[2*i];
        char l = block[2*i + 1];

        work[i] = (u - codes[0]) << 4 |
        (l - codes[0]);
    }

    work[len] = 0;

    Text clear(work, len);
    delete [] work;
    return clear;
}

