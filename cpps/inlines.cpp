#ifndef __CPU_INLINES_CPP__
#define __CPU_INLINES_CPP__

#include "../include/definitions.h"
#include "../include/utilities.h"
#include "../include/cpu.h"

inline void CPU::stackPush(const word top)
{
    registers.sp -= 2;
    mmu->writeWord(registers.sp, top);
}

inline word CPU::stackPop()
{
    word top = mmu->readWord(registers.sp);
    registers.sp += 2;
    return top;
}

inline void CPU::_res(byte& reg, const byte pos)
{
	clearBit(reg, pos);
}

inline void CPU::_set(byte& reg, const byte pos)
{
	setBit(reg, pos);
}

inline void CPU::_bit(byte reg, const byte pos)
{
	registers.zero = isBitSet(reg, pos);
	registers.half_carry = 1;
	registers.subtract = 0;
}

inline void CPU::_sra(byte& reg)
{
    byte bit = isBitSet(reg, 0);
    reg = (reg >> 1) | (reg & 0x80);
    registers.zero = (reg == 0) ? 1 : 0;
    registers.carry = bit;
    registers.sign = isBitSet(reg, 7);
    registers.subtract = 0;
    registers.half_carry = 0;
    registers.PorO = bit;
}

inline void CPU::_sll(byte& reg)
{
	_sla(reg);
}

inline void CPU::_srl(byte& reg)
{
    byte bit = isBitSet(reg, 0);
    reg >>= 1;
    registers.zero = (reg == 0) ? 1 : 0;
    registers.carry = bit;
    registers.sign = 0;
    registers.subtract = 0;
    registers.half_carry = 0;
}

inline void CPU::_sla(byte& reg)
{
    byte bit = isBitSet(reg, 7);
    reg <<= 1;
    registers.zero = (reg == 0) ? 1 : 0;
    registers.carry = bit;
    registers.sign = bit;
    registers.subtract = 0;
    registers.half_carry = 0;
}

inline void CPU::_rr(byte& reg)
{
    byte bit = isBitSet(reg, 0);
    reg = (reg >> 1) | (registers.carry << 7);
    registers.zero = (reg == 0) ? 1 : 0;
    registers.carry = bit;
    registers.sign = isBitSet(reg, 7);
    registers.subtract = 0;
    registers.half_carry = 0;
}

inline void CPU::_rl(byte& reg)
{
    byte bit = isBitSet(reg, 7);
    reg = (reg << 1) | (registers.carry);
    registers.zero = (reg == 0) ? 1 : 0;
    registers.carry = bit;
    registers.sign = bit;
    registers.subtract = 0;
    registers.half_carry = 0;
}

inline void CPU::_rrc(byte& reg)
{
    byte bit = isBitSet(reg, 0);
    reg >>= 1;
    reg |= (bit << 7);
    registers.zero = (reg == 0) ? 1 : 0;
    registers.carry = bit;
    registers.sign = isBitSet(reg, 7);
    registers.subtract = 0;
    registers.half_carry = 0;
}

inline void CPU::_rlc(byte& reg)
{
    byte bit = isBitSet(reg, 7);
    reg <<= 1;
    reg |= bit;
    registers.zero = (reg == 0) ? 1 : 0;
    registers.carry = bit;
    registers.sign = bit;
    registers.subtract = 0;
    registers.half_carry = 0;
}

inline void CPU::_inc(byte& reg)
{
    ++reg;
    registers.subtract = 0;
    registers.zero = (reg == 0) ? 1 : 0;
    registers.half_carry = (reg & 0x0F) == 0 ? 1 : 0;
    registers.sign = isBitSet(reg, 7);
}

inline void CPU::_dec(byte& reg)
{
   --reg;
    registers.subtract = 1;
    registers.zero = (reg == 0 ? 1 : 0);
    registers.half_carry = ((reg & 0x0F) == 0x0F ? 1 : 0);
    registers.sign = isBitSet(reg, 7);
}

inline void CPU::_cp(byte& acc, const byte data)
{
    registers.subtract = 1;
    registers.PorO = 0;
    registers.zero = (acc == data) ? 1 : 0;
    registers.carry = (acc < data) ? 1 : 0;
    registers.half_carry = (acc & 0x0F) < (acc & 0x0F) ? 1 : 0;
    registers.sign = (acc < data) ? 1 : 0;
}

inline void CPU::_or(byte& acc, const byte data)
{
    registers.subtract = 0;
    registers.carry = 0;
    registers.half_carry = 0;
    acc |= data;
    registers.zero = (acc == 0) ? 1 : 0;
    registers.sign = isBitSet(acc, 7);
}

inline void CPU::_and(byte &acc, const byte data)
{
    registers.carry = 0;
    registers.half_carry = 0;
    registers.subtract = 1;
    acc &= data;
    registers.zero = (acc == 0) ? 1 : 0;
    registers.sign = isBitSet(acc, 7);
}

inline void CPU::_xor(byte &acc, const byte data)
{
    registers.subtract = 0;
    registers.carry = 0;
    registers.half_carry = 0;
    acc ^= data;
    registers.zero = (acc == 0) ? 1 : 0;
    registers.sign = isBitSet(acc, 7);
    registers.PorO = isBitSet(acc, 0);
}

inline void CPU::_sub(byte& acc, const byte toSubtract)
{
    registers.subtract = 1;
    registers.carry = (acc < toSubtract) ? 1 : 0;
    registers.half_carry = ((acc & 0x0F) < (toSubtract & 0x0F)) ? 1 : 0;
    acc = acc - toSubtract;
    registers.zero = (acc == 0) ? 1 : 0;
    registers.sign = isBitSet(acc, 7);
    registers.PorO = registers.carry;
}
    
    
inline void CPU::_sbc(byte &acc, const byte toSubtract) 
{
    int carryValue = registers.carry;
    registers.subtract = 1;
    registers.carry = (acc < toSubtract + carryValue) ? 1 : 0;
    registers.half_carry = ((acc & 0x0F) < (toSubtract & 0x0F) + carryValue) ? 1 : 0;
    acc = acc - toSubtract;
    registers.zero = (acc == 0) ? 1 : 0;
    registers.sign = isBitSet(acc, 7);
}

inline void CPU::_adc(byte &origin, const byte adding)
{
    byte carryValue = registers.carry;
     registers.subtract = 0;
    registers.carry =  (origin + adding + carryValue)  > 0x00FF;
    registers.half_carry = ((origin & 0x000F) + (adding & 0x000F) + carryValue) > 0x000F ? 1 : 0;
    origin += adding;
    registers.zero = origin == 0 ? 1 : 0;
    registers.sign = isBitSet(origin, 7);
    registers.PorO = registers.carry;
}
    
    
inline void CPU::_add(byte &origin, const byte adding) 
{
    registers.subtract = 0;
    registers.carry =  (origin + adding)  > 0x00FF;
    registers.half_carry = ((origin & 0x000F) + (adding & 0x000F)) > 0x000F ? 1 : 0;
    origin += adding;
    registers.zero = origin == 0 ? 1 : 0;
    registers.sign = isBitSet(origin, 7);
    registers.PorO = registers.carry;
}

inline void CPU::_add(word &origin, const word adding)
{
    registers.subtract = 0;
    registers.carry =  (origin + adding)  > 0xFFFF;
    registers.half_carry = ((origin & 0x0FFF) + (adding & 0x0FFF)) > 0x0FFF;
    origin += adding;
    registers.zero = origin == 0 ? 1 : 0;
    registers.sign = isBitSet(origin, 7);
    registers.PorO = registers.carry;
}

#endif
