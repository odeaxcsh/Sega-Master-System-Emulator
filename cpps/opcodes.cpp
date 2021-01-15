#ifndef __OPCODES_CPP__
#define __OPCODES_CPP__

#include "../include/definitions.h"
#include "../include/cpu.h"
#include <cmath>

byte CPU::opcode0x00 ()//NOP
{
    return 4;
}

byte CPU::opcode0x01()//LD BC (PC)
{
    registers.bc = mmu->readWord(registers.pc);
    registers.pc += 2;
    return 10;
}

byte CPU::opcode0x02()//LD (BC) A
{
    mmu->write(registers.bc, registers.a);
    return 7;
}

byte CPU::opcode0x03()//INC BC
{
    ++registers.bc;
    return 6;
}

byte CPU::opcode0x04()//INC B
{
    _inc(registers.b);
    return 4;
}

byte CPU::opcode0x05()//DEC B
{
    _dec(registers.b);
    return 4;
}

byte CPU::opcode0x06()//LD B (PC)
{
    registers.b = mmu->read(registers.pc ++);
    return 7;
}

byte CPU::opcode0x07()//RLCA
{
    _rlc(registers.a);
    return 4;
}

byte CPU::opcode0x08()//EX AF AF'
{
    std::swap(registers.af, registers.shadows.af);
    return 4;
}

byte CPU::opcode0x09()//ADD HL BC
{
    _add(registers.hl, registers.bc);
    return 11;
}

byte CPU::opcode0x0A()//LD A (BC)
{
    registers.a = mmu->read(registers.bc);
    return 7;
}

byte CPU::opcode0x0B()//DEC BC
{
    --registers.bc;
    return 6;
}

byte CPU::opcode0x0C()//INC C
{
    _inc(registers.c);
    return 4;
}

byte CPU::opcode0x0D()//DEC C
{
    _dec(registers.c);
    return 4;
}

byte CPU::opcode0x0E()//LD C (PC)
{
    registers.c = mmu->read(registers.pc++);
    return 7;
}

byte CPU::opcode0x0F()//RRCA
{
    _rrc(registers.a);
    return 4;
}

byte CPU::opcode0x10 ()//DJNZ
{
    --registers.b;
    if(registers.b == 0) {
        ++registers.pc;
        return 8;
    } else {
        registers.pc += (static_cast<int8>(mmu->read(registers.pc)) + 1);
        return 13;
    }
}

byte CPU::opcode0x11()//LD DE (PC)
{
    registers.de = mmu->readWord(registers.pc);
    registers.pc += 2;
    return 10;
}

byte CPU::opcode0x12()//LD (DE) A
{
    mmu->write(registers.de, registers.a);
    return 7;
}

byte CPU::opcode0x13()//INC DE
{
    ++registers.de;
    return 6;
}

byte CPU::opcode0x14()//INC D
{
    _inc(registers.d);
    return 4;
}

byte CPU::opcode0x15()//DEC D
{
    _dec(registers.d);
    return 4;
}

byte CPU::opcode0x16()//LD D (BC)
{
    registers.d = mmu->read(registers.pc++);
    return 7;
}

byte CPU::opcode0x17()//RL A
{
    _rl(registers.a);
    return 4;
}

byte CPU::opcode0x18()//JR (PC)
{
    registers.pc += (static_cast<int8>(mmu->read(registers.pc)) + 1);
    return 12;
}

byte CPU::opcode0x19()//ADD HL DE
{
    _add(registers.hl, registers.de);
    return 11;
}

byte CPU::opcode0x1A()//LD A (DE)
{
    registers.a = mmu->read(registers.de);
    return 7;
}

byte CPU::opcode0x1B()//DEC DE
{
    --registers.de;
    return 6;
}

byte CPU::opcode0x1C()//INC E
{
    _inc(registers.e);
    return 4;
}

byte CPU::opcode0x1D()//DEC E
{
    _dec(registers.e);
    return 4;
}

byte CPU::opcode0x1E()//LD E (PC)
{
    registers.e = mmu->read(registers.pc++);
    return 7;
}

byte CPU::opcode0x1F()//RR A
{
    _rr(registers.a);
    return 4;
}

byte CPU::opcode0x20 ()//JR NZ
{
    if(registers.zero == 0) {
        registers.pc += (static_cast<int8>(mmu->read(registers.pc)) + 1);
        return 12;
    } else {
        ++registers.pc;
        return 7;
    }
}

byte CPU::opcode0x21()//LD HL (PC)
{
    registers.hl = mmu->readWord(registers.pc);
    registers.pc += 2;
    return 10;
}

byte CPU::opcode0x22()//LD ((PC)) HL
{
    Address pcPointsTo = mmu->readWord(registers.pc);
    mmu->writeWord(pcPointsTo, registers.hl);
    registers.pc += 2;
    return 16;
}

byte CPU::opcode0x23()//INC HL
{
    ++registers.hl;
    return 6;
}

byte CPU::opcode0x24()//INC H
{
    _inc(registers.h);
    return 4;
}

byte CPU::opcode0x25()//DEC H
{
    _dec(registers.h);
    return 4;
}

byte CPU::opcode0x26()//LD H (PC)
{
    registers.h = mmu->read(registers.pc++);
    return 7;
}

byte CPU::opcode0x27()//DAA
{
    //throw "struct notImplementedYet { };";
    return 4;
}

byte CPU::opcode0x28()//JR Z
{
    if(registers.zero == 0) {
        ++registers.pc;
        return 7;
    } else {
        registers.pc += (static_cast<int8>(mmu->read(registers.pc)) + 1);
        return 12;
    }
}

byte CPU::opcode0x29()//ADD HL HL
{
    _add(registers.hl, registers.hl);
    return 11;
}

byte CPU::opcode0x2A()//LD HL ((PC))
{
    Address pcPointsTo = mmu->readWord(registers.pc);
    registers.hl = mmu->readWord(pcPointsTo);
    registers.pc += 2;
    return 16;
}

byte CPU::opcode0x2B()//DEC HL
{
    --registers.hl;
    return 6;
}

byte CPU::opcode0x2C()//INC L
{
    _inc(registers.l);
    return 4;
}

byte CPU::opcode0x2D()//DEC L
{
    _dec(registers.l);
    return 4;
}

byte CPU::opcode0x2E()//LD L (PC)
{
    registers.l = mmu->read(registers.pc++);
    return 7;
}

byte CPU::opcode0x2F()//CP L
{
    throw "struct IDontKnowWhatIsThis { };";
}

byte CPU::opcode0x30 ()//JR NC
{
    if(registers.carry == 0) {
        registers.pc += (static_cast<int8>(mmu->read(registers.pc)) + 1);
        return 12;
    } else {
        ++registers.pc;
        return 7;
    }
}

byte CPU::opcode0x31()//LD SP (PC)
{
    registers.sp = mmu->readWord(registers.pc);
    registers.pc += 2;
    return 10;
}

byte CPU::opcode0x32()//LD ((PC)) A
{
    word pcPointsTo = mmu->readWord(registers.pc);
    mmu->write(pcPointsTo, registers.a);
    registers.pc += 2;
    return 13;
}

byte CPU::opcode0x33()//INC SP
{
    ++registers.sp;
    return 6;
}

byte CPU::opcode0x34()//INC (HL)
{
    byte hlPointsTo = mmu->read(registers.hl);
    _dec(hlPointsTo);
    mmu->write(registers.hl, hlPointsTo);
    return 11;
}

byte CPU::opcode0x35()//DEC (HL)
{
    byte hlPointsTo = mmu->read(registers.hl);
    _dec(hlPointsTo);
    mmu->write(registers.hl, hlPointsTo);
    return 11;
}

byte CPU::opcode0x36()//LD (HL) (PC)
{
    byte value = mmu->read(registers.pc++);
    mmu->write(registers.hl, value);
    return 10;
}

byte CPU::opcode0x37()//SCF
{
    registers.carry = 1;
    registers.subtract = 0;
    registers.half_carry = 0;
    return 4;
}

byte CPU::opcode0x38()//JR C
{
    if(registers.c == 0) {
        ++registers.pc;
        return 7;
    } else {
        registers.pc += (static_cast<int8>(mmu->read(registers.pc)) + 1);
        return 12;
    }
}

byte CPU::opcode0x39()//ADD HL SP
{
    _add(registers.hl, registers.sp);
    return 11;
}

byte CPU::opcode0x3A()//LD A ((PC))
{
    Address pcPointsTo = mmu->readWord(registers.pc);
    registers.a = mmu->read(pcPointsTo);
    registers.pc += 2;
    return 13;
}

byte CPU::opcode0x3B()//DEC SP
{
    --registers.sp;
    return 6;
}

byte CPU::opcode0x3C()//INC A
{
    _inc(registers.a);
    return 4;
}

byte CPU::opcode0x3D()//DEC A
{
    _dec(registers.a);
    return 4;
}

byte CPU::opcode0x3E()//LD A (PC)
{
    registers.a = mmu->read(registers.pc++);
    return 7;
}

byte CPU::opcode0x3F()//CC F
{
    throw "struct IDontKnowWhatThisIs{};";
}

byte CPU::opcode0x40()//LD B, B
{
    registers.b = registers.b;
    return 4;
}

byte CPU::opcode0x41()//LD B, C
{
    registers.b = registers.c;
    return 4;
}

byte CPU::opcode0x42()//LD B, D
{
    registers.b = registers.d;
    return 4;
}

byte CPU::opcode0x43()//LD B, E
{
    registers.b = registers.e;
    return 4;
}

byte CPU::opcode0x44()//LD B, H
{
    registers.b = registers.h;
    return 4;
}

byte CPU::opcode0x45()//LD B, L
{
    registers.b = registers.l;
    return 4;
}

byte CPU::opcode0x46()//LD B, (HL)
{
    registers.b = mmu->read(registers.hl);
    return 7;
}

byte CPU::opcode0x47()//LD B, A
{
    registers.b = registers.a;
    return 4;
}

byte CPU::opcode0x48()//LD C, B
{
    registers.c = registers.b;
    return 4;
}

byte CPU::opcode0x49()//LD C, C
{
    registers.c = registers.c;
    return 4;
}

byte CPU::opcode0x4A()//LD C, D
{
    registers.c = registers.d;
    return 4;
}

byte CPU::opcode0x4B()//LD C, E
{
    registers.c = registers.e;
    return 4;
}

byte CPU::opcode0x4C()//LD C, H
{
    registers.c = registers.h;
    return 4;
}

byte CPU::opcode0x4D()//LD C, L
{
    registers.c = registers.l;
    return 4;
}

byte CPU::opcode0x4E()//LD C, (HL)
{
    registers.c = mmu->read(registers.hl);
    return 7;
}

byte CPU::opcode0x4F()//LD C, A
{
    registers.c = registers.a;
    return 4;
}

byte CPU::opcode0x50()//LD D, B
{
    registers.d = registers.b;
    return 4;
}

byte CPU::opcode0x51()//LD D, C
{
    registers.d = registers.c;
    return 4;
}

byte CPU::opcode0x52()//LD D, D
{
    registers.d = registers.d;
    return 4;
}

byte CPU::opcode0x53()//LD D, E
{
    registers.d = registers.e;
    return 4;
}

byte CPU::opcode0x54()//LD D, H
{
    registers.d = registers.h;
    return 4;
}

byte CPU::opcode0x55()//LD D, L
{
    registers.d = registers.l;
    return 4;
}

byte CPU::opcode0x56()//LD D, (HL)
{
    registers.d = mmu->read(registers.hl);
    return 7;
}

byte CPU::opcode0x57()//LD D, A
{
    registers.d = registers.a;
    return 4;
}

byte CPU::opcode0x58()//LD E, B
{
    registers.e = registers.b;
    return 4;
}

byte CPU::opcode0x59()//LD E, C
{
    registers.e = registers.c;
    return 4;
}

byte CPU::opcode0x5A()//LD E, D
{
    registers.e = registers.d;
    return 4;
}

byte CPU::opcode0x5B()//LD E, E
{
    registers.e = registers.e;
    return 4;
}

byte CPU::opcode0x5C()//LD E, H
{
    registers.e = registers.h;
    return 4;
}

byte CPU::opcode0x5D()//LD E, L
{
    registers.e = registers.l;
    return 4;
}

byte CPU::opcode0x5E()//LD E, (HL)
{
    registers.e = mmu->read(registers.hl);
    return 7;
}

byte CPU::opcode0x5F()//LD E, A
{
    registers.e = registers.a;
    return 4;
}

byte CPU::opcode0x60()//LD H, B
{
    registers.h = registers.b;
    return 4;
}

byte CPU::opcode0x61()//LD H, C
{
    registers.h = registers.c;
    return 4;
}

byte CPU::opcode0x62()//LD H, D
{
    registers.h = registers.d;
    return 4;
}

byte CPU::opcode0x63()//LD H, E
{
    registers.h = registers.e;
    return 4;
}

byte CPU::opcode0x64()//LD H, H
{
    registers.h = registers.h;
    return 4;
}

byte CPU::opcode0x65()//LD H, L
{
    registers.h = registers.l;
    return 4;
}

byte CPU::opcode0x66()//LD H, (HL)
{
    registers.h = mmu->read(registers.hl);
    return 7;
}

byte CPU::opcode0x67()//LD H, A
{
    registers.h = registers.a;
    return 4;
}

byte CPU::opcode0x68()//LD L, B
{
    registers.l = registers.b;
    return 4;
}

byte CPU::opcode0x69()//LD L, C
{
    registers.l = registers.c;
    return 4;
}

byte CPU::opcode0x6A()//LD L, D
{
    registers.l = registers.d;
    return 4;
}

byte CPU::opcode0x6B()//LD L, E
{
    registers.l = registers.e;
    return 4;
}

byte CPU::opcode0x6C()//LD L, H
{
    registers.l = registers.h;
    return 4;
}

byte CPU::opcode0x6D()//LD L, L
{
    registers.l = registers.l;
    return 4;
}

byte CPU::opcode0x6E()//LD L, (HL)
{
    registers.l = mmu->read(registers.hl);
    return 7;
}

byte CPU::opcode0x6F()//LD L, A
{
    registers.l = registers.a;
    return 4;
}

byte CPU::opcode0x70()//(HL), B
{
    mmu->write(registers.hl, registers.b);
    return 7;
}

byte CPU::opcode0x71()//(HL), C
{
    mmu->write(registers.hl, registers.c);
    return 7;
}

byte CPU::opcode0x72()//(HL), D
{
    mmu->write(registers.hl, registers.d);
    return 7;
}

byte CPU::opcode0x73()//(HL), E
{
    mmu->write(registers.hl, registers.e);
    return 7;
}

byte CPU::opcode0x74()//(HL), H
{
    mmu->write(registers.hl, registers.h);
    return 7;
}

byte CPU::opcode0x75()//(HL), L
{
    mmu->write(registers.hl, registers.a);
    return 7;
}

byte CPU::opcode0x76()//HALT
{
    halt = true;
    return 4;
}

byte CPU::opcode0x77()//(HL), A
{
    mmu->write(registers.hl, registers.a);
    return 7;
}

byte CPU::opcode0x78()//LD A, B
{
    registers.a = registers.b;
    return 4;
}

byte CPU::opcode0x79()//LD A, C
{
    registers.a = registers.c;
    return 4;
}

byte CPU::opcode0x7A()//LD A, D
{
    registers.a = registers.d;
    return 4;
}

byte CPU::opcode0x7B()//LD A, E
{
    registers.a = registers.e;
    return 4;
}

byte CPU::opcode0x7C()//LD A, H
{
    registers.a = registers.h;
    return 4;
}

byte CPU::opcode0x7D()//LD A, L
{
    registers.a = registers.l;
    return 4;
}

byte CPU::opcode0x7E()//LD A, (HL)
{
    registers.a = mmu->read(registers.hl);
    return 7;
}

byte CPU::opcode0x7F()//LD A, A
{
    registers.a = registers.a;
    return 4;
}

byte CPU::opcode0x80()//ADD A, B
{
    _add(registers.a, registers.b);
    return 4;
}

byte CPU::opcode0x81()//ADD A, C
{
    _add(registers.a, registers.c);
    return 4;
}

byte CPU::opcode0x82()//ADD A, D
{
    _add(registers.a, registers.d);
    return 4;
}

byte CPU::opcode0x83()//ADD A, E
{
    _add(registers.a, registers.e);
    return 4;
}

byte CPU::opcode0x84()//ADD A, H
{
    _add(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0x85()//ADD A, L
{
    _add(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0x86()//ADD A, (HL)
{
    _add(registers.a, mmu->read(registers.hl));
    return 7;
}

byte CPU::opcode0x87()//ADD A, A
{
    _add(registers.a, registers.a);
    return 4;
}

byte CPU::opcode0x88()//ADC A, B
{
    _adc(registers.a, registers.b);
    return 4;
}

byte CPU::opcode0x89()//ADC A, C
{
    _adc(registers.a, registers.c);
    return 4;
}

byte CPU::opcode0x8A()//ADC A, D
{
    _adc(registers.a, registers.d);
    return 4;
}

byte CPU::opcode0x8B()//ADC A, E
{
    _adc(registers.a, registers.e);
    return 4;
}

byte CPU::opcode0x8C()//ADC A, H
{
    _adc(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0x8D()//ADC A, L
{
    _adc(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0x8E()//ADC A, (HL)
{
    _adc(registers.a, mmu->read(registers.hl));
    return 7;
}

byte CPU::opcode0x8F()//ADC A, A
{
    _adc(registers.a, registers.a);
    return 4;
}

byte CPU::opcode0x90()//SUB A, B
{
    _sub(registers.a, registers.b);
    return 4;
}

byte CPU::opcode0x91()//SUB A, C
{
    _sub(registers.a, registers.c);
    return 4;
}

byte CPU::opcode0x92()//SUB A, D
{
    _sub(registers.a, registers.d);
    return 4;
}

byte CPU::opcode0x93()//SUB A, E
{
    _sub(registers.a, registers.e);
    return 4;
}

byte CPU::opcode0x94()//SUB A, H
{
    _sub(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0x95()//SUB A, L
{
    _sub(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0x96()//SUB A, (HL)
{
    _sub(registers.a, mmu->read(registers.hl));
    return 7;
}

byte CPU::opcode0x97()//SUB A, A
{
    _sub(registers.a, registers.a);
    return 4;
}

byte CPU::opcode0x98()//SBC A, B
{
    _sbc(registers.a, registers.b);
    return 4;
}

byte CPU::opcode0x99()//SBC A, C
{
    _sbc(registers.a, registers.c);
    return 4;
}

byte CPU::opcode0x9A()//SBC A, D
{
    _sbc(registers.a, registers.d);
    return 4;
}

byte CPU::opcode0x9B()//SBC A, E
{
    _sbc(registers.a, registers.e);
    return 4;
}

byte CPU::opcode0x9C()//SBC A, H
{
    _sbc(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0x9D()//SBC A, L
{
    _sbc(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0x9E()//SBC A, (HL)
{
    _sbc(registers.a, mmu->read(registers.hl));
    return 7;
}

byte CPU::opcode0x9F()//SBC A, A
{
    _sbc(registers.a, registers.a);
    return 4;
}

byte CPU::opcode0xA0()//AND A, B
{
    _and(registers.a, registers.b);
    return 4;
}

byte CPU::opcode0xA1()//AND A, C
{
    _and(registers.a, registers.c);
    return 4;
}

byte CPU::opcode0xA2()//AND A, D
{
    _and(registers.a, registers.d);
    return 4;
}

byte CPU::opcode0xA3()//AND A, E
{
    _and(registers.a, registers.e);
    return 4;
}

byte CPU::opcode0xA4()//AND A, H
{
    _and(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0xA5()//AND A, L
{
    _and(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0xA6()//AND A, (HL)
{
    _and(registers.a, mmu->read(registers.hl));
    return 7;
}

byte CPU::opcode0xA7()//AND A, A
{
    _and(registers.a, registers.a);
    return 4;
}

byte CPU::opcode0xA8()//XOR A, B
{
    _xor(registers.a, registers.b);
    return 4;
}

byte CPU::opcode0xA9()//XOR A, C
{
    _xor(registers.a, registers.c);
    return 4;
}

byte CPU::opcode0xAA()//XOR A, D
{
    _xor(registers.a, registers.d);
    return 4;
}

byte CPU::opcode0xAB()//XOR A, E
{
    _xor(registers.a, registers.e);
    return 4;
}

byte CPU::opcode0xAC()//XOR A, H
{
    _xor(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0xAD()//XOR A, L
{
    _xor(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0xAE()//XOR A, (HL)
{
    _xor(registers.a, mmu->read(registers.hl));
    return 7;
}

byte CPU::opcode0xAF()//XOR A, A
{
    _xor(registers.a, registers.a);
    return 4;
}

byte CPU::opcode0xB0()//OR A, B
{
    _or(registers.a, registers.b);
    return 4;
}

byte CPU::opcode0xB1()//OR A, C
{
    _or(registers.a, registers.c);
    return 4;
}

byte CPU::opcode0xB2()//OR A, D
{
    _or(registers.a, registers.d);
    return 4;
}

byte CPU::opcode0xB3()//OR A, E
{
    _or(registers.a, registers.e);
    return 4;
}

byte CPU::opcode0xB4()//OR A, H
{
    _or(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0xB5()//OR A, L
{
    _or(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0xB6()//OR A, (HL)
{
    _or(registers.a, mmu->read(registers.hl));
    return 7;
}

byte CPU::opcode0xB7()//OR A, A
{
    _or(registers.a, registers.a);
    return 4;
}

byte CPU::opcode0xB8()//CP A, B
{
    _cp(registers.a, registers.b);
    return 4;
}

byte CPU::opcode0xB9()//CP A, C
{
    _cp(registers.a, registers.c);
    return 4;
}

byte CPU::opcode0xBA()//CP A, D
{
    _cp(registers.a, registers.d);
    return 4;
}

byte CPU::opcode0xBB()//CP A, E
{
    _cp(registers.a, registers.e);
    return 4;
}

byte CPU::opcode0xBC()//CP A, H
{
    _cp(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0xBD()//CP A, L
{
    _cp(registers.a, registers.h);
    return 4;
}

byte CPU::opcode0xBE()//CP A, (HL)
{
    _cp(registers.a, mmu->read(registers.hl));
    return 7;
}

byte CPU::opcode0xBF()//CP A, A
{
    _cp(registers.a, registers.a);
    return 4;
}

byte CPU::opcode0xC0()//RET NZ
{
    if(registers.zero == 0) {
        registers.pc = stackPop();
        return 11;
    } else return 5;
}

byte CPU::opcode0xC1()//POP BC
{
    registers.bc = stackPop();
    return 10;
}

byte CPU::opcode0xC2()//JP NZ, (PC)
{
    if(registers.zero == 0)
        registers.pc = mmu->readWord(registers.pc);
    else registers.pc += 2;
    return 10;
}

byte CPU::opcode0xC3()//JP TR, (PC)
{
    registers.pc = mmu->readWord(registers.pc);
    return 10;
}

byte CPU::opcode0xC4()//CALL NZ, (PC)
{
    if(registers.zero == 0) {
        stackPush(registers.pc + 2);
        registers.pc = mmu->readWord(registers.pc);
        return 17;
    } else {
        registers.pc += 2;
        return 11;
    }
}

byte CPU::opcode0xC5()//PUSH BC
{
    stackPush(registers.bc);
    return 11;
}

byte CPU::opcode0xC6()//ADD A, (PC)
{
    _add(registers.a, mmu->read(registers.pc++));
    return 7;
}

byte CPU::opcode0xC7()//RST 00
{
    stackPush(registers.pc);
    registers.pc = 0x00;
    return 11;
}

byte CPU::opcode0xC8()//RET Z
{
    if(registers.zero == 0)
        return 5;
    else {
        registers.pc = stackPop();
        return 11;
    }
}

byte CPU::opcode0xC9()//RET
{
    registers.pc = stackPop();
    return 10;
}

byte CPU::opcode0xCA()//JP Z, (PC)
{
    if(registers.zero == 0)
        registers.pc += 2;
    else registers.pc = mmu->readWord(registers.pc);
    return 10;
}

byte CPU::opcode0xCB()//BITS
{
    return bits(mmu->read(registers.pc++));
}

byte CPU::opcode0xCC()//CALL Z, (PC)
{
    if(registers.zero == 0) {
        registers.pc += 2;
        return 10;
    } else {
        stackPush(registers.pc + 2);
        registers.pc = mmu->readWord(registers.pc);
        return 17;
    }
}

byte CPU::opcode0xCD()//CALL TR (PC)
{
    stackPush(registers.pc + 2);
    registers.pc = mmu->readWord(registers.pc);
    return 17;
}
    
byte CPU::opcode0xCE()//ADC A, (PC)
{
    _adc(registers.a, mmu->read(registers.pc++));
    return 7;
}

byte CPU::opcode0xCF()//RST 08
{
    stackPush(registers.pc);
    registers.pc = 0x08;
    return 11;
}

byte CPU::opcode0xD0()//RET NC
{
    if(registers.carry == 0) {
        registers.pc = stackPop();
        return 11;
    } else return 5;
}

byte CPU::opcode0xD1()//POP DE
{
    registers.de = stackPop();
    return 10;
}

byte CPU::opcode0xD2()//JP NC, (PC)
{
    if(registers.carry == 0)
        registers.pc = mmu->readWord(registers.pc);
    else registers.pc += 2;
    return 10;
}

byte CPU::opcode0xD3()//OUT (PC), A
{
    pm->write(mmu->read(registers.pc++), registers.a);
    return 11;
}

byte CPU::opcode0xD4()//CALL NC, (PC)
{
    if(registers.carry == 0) {
        stackPush(registers.pc + 2);
        registers.pc = mmu->readWord(registers.pc);
        return 17;
    } else {
        registers.pc += 2;
        return 11;
    }
}

byte CPU::opcode0xD5()//PUSH DE
{
    stackPush(registers.de);
    return 11;
}

byte CPU::opcode0xD6()//SUB A, (PC)
{
    _sub(registers.a, mmu->read(registers.pc++));
    return 7;
}

byte CPU::opcode0xD7()//RST 10
{
    stackPush(registers.pc);
    registers.pc = 0x10;
    return 11;
}

byte CPU::opcode0xD8()//RET C
{
    if(registers.carry == 0)
        return 5;
    else {
        registers.pc = stackPop();
        return 11;
    }
}

byte CPU::opcode0xD9()//EXX
{
    std::swap(registers.bc, registers.shadows.bc);
    std::swap(registers.de, registers.shadows.de);
    std::swap(registers.hl, registers.shadows.de);
    return 4;
}

byte CPU::opcode0xDA()//JP C, (PC)
{
    if(registers.carry == 0)
        registers.pc += 2;
    else registers.pc = mmu->readWord(registers.pc);
    return 10;
}

byte CPU::opcode0xDB()//IN A, (PC)
{
    registers.a = pm->read(mmu->read(registers.pc++));
    return 11;
}

byte CPU::opcode0xDC()//CALL C, (PC)
{
    if(registers.carry == 0) {
        registers.pc += 2;
        return 10;
    } else {
        stackPush(registers.pc + 2);
        registers.pc = mmu->readWord(registers.pc);
        return 17;
    }
}

byte CPU::opcode0xDD()//IX
{
    return ix(mmu->read(registers.pc++));
}
    
byte CPU::opcode0xDE()//SBC A, (PC)
{
    _sbc(registers.a, mmu->read(registers.pc++));
    return 7;
}

byte CPU::opcode0xDF()//RST 18
{
    stackPush(registers.pc);
    registers.pc = 0x18;
    return 11;
}

byte CPU::opcode0xE0()//RET PO
{
    if(registers.PorO == 0) {
        registers.pc = stackPop();
        return 11;
    } else return 5;
}

byte CPU::opcode0xE1()//POP HL
{
    registers.hl = stackPop();
    return 10;
}

byte CPU::opcode0xE2()//JP PO, (PC)
{
    if(registers.PorO == 0)
        registers.pc = mmu->readWord(registers.pc);
    else registers.pc += 2;
    return 10;
}

byte CPU::opcode0xE3()//EX (SP), HL
{
    word atHl = registers.hl;
    registers.hl = mmu->readWord(registers.sp);
    mmu->writeWord(registers.sp, atHl);
    return 19;
}

byte CPU::opcode0xE4()//CALL PO, (PC)
{
    if(registers.PorO == 0) {
        stackPush(registers.pc + 2);
        registers.pc = mmu->readWord(registers.pc);
        return 17;
    } else {
        registers.pc += 2;
        return 11;
    }
}

byte CPU::opcode0xE5()//PUSH HL
{
    stackPush(registers.hl);
    return 11;
}

byte CPU::opcode0xE6()//AND A, (PC)
{
    _and(registers.a, mmu->read(registers.pc++));
    return 7;
}

byte CPU::opcode0xE7()//RST 20
{
    stackPush(registers.pc);
    registers.pc = 0x20;
    return 11;
}

byte CPU::opcode0xE8()//RET PE
{
    if(registers.PorO == 0)
        return 5;
    else {
        registers.pc = stackPop();
        return 11;
    }
}

byte CPU::opcode0xE9()//JP (HL)
{
    registers.pc = registers.hl;
    return 4;
}

byte CPU::opcode0xEA()//JP PE, (PC)
{
    if(registers.PorO == 0)
        registers.pc += 2;
    else registers.pc = mmu->readWord(registers.pc);
    return 10;
}

byte CPU::opcode0xEB()//EX DE, HL
{
    std::swap(registers.hl, registers.de);
    return 4;
}

byte CPU::opcode0xEC()//CALL PE, (PC)
{
    if(registers.PorO == 0) {
        registers.pc += 2;
        return 10;
    } else {
        stackPush(registers.pc + 2);
        registers.pc = mmu->readWord(registers.pc);
        return 17;
    }
}

byte CPU::opcode0xED()//EXTD
{
    return extd(mmu->read(registers.pc++));
}
    
byte CPU::opcode0xEE()//XOR A, (PC)
{
    _xor(registers.a, mmu->read(registers.pc++));
    return 7;
}

byte CPU::opcode0xEF()//RST 28
{
    stackPush(registers.pc);
    registers.pc = 0x28;
    return 11;
}

byte CPU::opcode0xF0()//RET P
{
    if(registers.sign == 0) {
        registers.pc = stackPop();
        return 11;
    } else return 5;
}

byte CPU::opcode0xF1()//POP AF
{
    registers.af = stackPop();
    return 10;
}

byte CPU::opcode0xF2()//JP P, (PC)
{
    if(registers.sign == 0)
        registers.pc = mmu->readWord(registers.pc);
    else registers.pc += 2;
    return 10;
}

byte CPU::opcode0xF3()//DI
{
    FF1 = FF2 = false;
    return 4;
}

byte CPU::opcode0xF4()//CALL P, (PC)
{
    if(registers.sign == 0) {
        stackPush(registers.pc + 2);
        registers.pc = mmu->readWord(registers.pc);
        return 17;
    } else {
        registers.pc += 2;
        return 11;
    }
}

byte CPU::opcode0xF5()//PUSH Af
{
    stackPush(registers.af);
    return 11;
}

byte CPU::opcode0xF6()//OR A, (PC)
{
    _or(registers.a, mmu->read(registers.pc++));
    return 7;
}

byte CPU::opcode0xF7()//RST 30
{
    stackPush(registers.pc);
    registers.pc = 0x30;
    return 11;
}

byte CPU::opcode0xF8()//RET M
{
    if(registers.sign == 0)
        return 5;
    else {
        registers.pc = stackPop();
        return 11;
    }
}

byte CPU::opcode0xF9()//LD SP, HL
{
    registers.sp = registers.hl;
    return 6;
}

byte CPU::opcode0xFA()//JP M, (PC)
{
    if(registers.sign == 0)
        registers.pc += 2;
    else registers.pc = mmu->readWord(registers.pc);
    return 10;
}

byte CPU::opcode0xFB()//EI
{
    FF1 = FF2 = true;
    return 4;
}

byte CPU::opcode0xFC()//CALL M, (PC)
{
    if(registers.sign == 0) {
        registers.pc += 2;
        return 10;
    } else {
        stackPush(registers.pc + 2);
        registers.pc = mmu->readWord(registers.pc);
        return 17;
    }
}

byte CPU::opcode0xFD()//IY
{
    return iy(mmu->read(registers.pc++));
}
    
byte CPU::opcode0xFE()//CP A, (PC)
{
    _cp(registers.a, mmu->read(registers.pc++));
    return 7;
}

byte CPU::opcode0xFF()//RST 38
{
    stackPush(registers.pc);
    registers.pc = 0x38;
    return 11;
}

#endif