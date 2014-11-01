// //////////////////////////////////////////////////////////
// sha256.h
// Copyright (c) 2014 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#pragma once

//#include "hash.h"

/* This include statement has been discarded from the original source code. Not
 * using string object as it is not necessarilly included on the devices that
 * will use this SDK. */
//#include <string>
// define fixed size integer types
#ifdef _MSC_VER
// Windows
typedef unsigned __int8 uint8_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
// GCC
#include <stdint.h>
#endif
// This line added to original source code. defines size_t.
#include "stddef.h"

/// compute SHA256 hash
/** Usage:
 SHA256 sha256;
 std::string myHash  = sha256("Hello World");     // std::string
 std::string myHash2 = sha256("How are you", 11); // arbitrary data, 11 bytes

 // or in a streaming fashion:

 SHA256 sha256;
 while (more data available)
 sha256.add(pointer to fresh data, number of new bytes);
 std::string myHash3 = sha256.getHash();
 */
class SHA256 //: public Hash
{
public:
    /// same as reset()
    SHA256();

    /// compute SHA256 of a memory block
    /* Modified from original source code. Using char* instead of string. */
    char* operator()(const void* data, size_t numBytes);
    /// compute SHA256 of a string, excluding final zero
    /* Modified from original source code. Using char* instead of string. */
    char* operator()(const char* text);

    /// add arbitrary number of bytes
    void add(const void* data, size_t numBytes);

    /// return latest hash as 16 hex characters
    /* Modified from original source code. Using char* instead of string. */
    char* getHash();

    /* This function added to original source. */
    /// return latest hash raw (not as hex)
    char* getHashDec();

    /// restart
    void reset();

private:
    /// process 64 bytes
    void processBlock(const void* data);
    /// process everything left in the internal buffer
    void processBuffer();

    /// split into 64 byte blocks (=> 512 bits)
    enum {
        BlockSize = 512 / 8, HashValues = 8
    };

    /// size of processed data in bytes
    uint64_t m_numBytes;
    /// valid bytes in m_buffer
    size_t m_bufferSize;
    /// bytes not processed yet
    uint8_t m_buffer[BlockSize];
    /// hash, stored as integers
    uint32_t m_hash[8];
};
