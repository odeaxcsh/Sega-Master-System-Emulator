#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

using int8 = signed char;
using uint8 = unsigned char;
using int16 = signed short int;
using uint16 = unsigned short int;
using uint32 = unsigned long int;
using int32 = signed long int;
using Address = uint16;
using Size = uint32;
using word = uint16;
using byte = uint8;

//class Logger;
class Cartridge;
class MMU;
class CPU;
class PSG;
class VDP;
class Joypad;
class PortManager;

template <typename dtype>
bool New(dtype *&destPtr, Size size)
{
	void *pointer;
	pointer = malloc(size * sizeof(dtype));
	return ((destPtr = reinterpret_cast<dtype*> (pointer)) != nullptr);
}

template <typename destPtr, typename originPtr>
void reinterpretCast(destPtr *&dest, originPtr *origin)
{
	const void **voidDest = reinterpret_cast<const void**> (&dest);
	const void *voidOrigin = reinterpret_cast<const void*> (origin);
	*voidDest = voidOrigin;
}

void Delete(void *&pointer)
{
	if(pointer != nullptr) 
		free(pointer);
	pointer = nullptr;
}

enum class TRegion
{
	PAL,
	NTSC
};

template <uint8 position>
class bit
{
  public :
    bit(byte& b) : bits(b) { }
    bit(const bit& b) : bits(b.bits) { }
    
    bit& operator= (const bit& b)
    {
        return this->operator=(b.operator byte());
    }
    
    bit& operator= (const byte b)
    {
        if(b == 0) bits &= ~(1 << position);
       else bits &= (1 << position);
       return *this;
    }
    
    operator byte() const
    {
        return (bits >> position) & 1;
    }
  
  private:
    byte& bits;
};

typedef byte (CPU::*opcode)();

#endif