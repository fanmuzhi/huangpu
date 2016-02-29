#ifndef __CRC32_H__
#define __CRC32_H__
////////////////////////////////////////////////////////////////////////////////
//Crc32.h
//
//Copyright (c) 2012 Synaptics.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////


#include "stdint.h"



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class Crc32
{
public:
  static unsigned long
  fast_crc32(unsigned long sum, unsigned char *p, unsigned long len);
  
  #define POLY 0x4C11DB7
  
  // note remember to feed size (4 for crc32) zeroes through the slow
  // crc algorithm to make the fast and the slow compute the same checksum
  
  static unsigned long
  slow_crc32(unsigned long sum, unsigned char *p, unsigned long len);

private:
  static const unsigned long crc32_table[256];
};

#endif //__CRC32_H__
