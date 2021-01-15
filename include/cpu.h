#ifndef __CPU_H__
#define __CPU_H__

#include "port manager.h"
#include "definitions.h"
#include "logger.h"
#include "mmu.h"

class CPU
{
  public:
    CPU() = default;
    void Reset();
    void Initialize(Logger *logger, MMU*, PortManager*);
    
    byte tick();
    void NMI_request();
    void MI_request(bool);
   
  public:
    PortManager *pm = nullptr;
    Logger *logger = Logger::null;
    MMU *mmu = nullptr;
    
    bool halt = false;
    bool FF1 = false;
    bool FF2 = false;
    bool NMI_servising = false;
    bool NMI_requested = false;
    bool MI_requested = false;
    
    byte interruptsMode = 1;
    byte bits(byte);
    byte extd(byte);
    byte ix(byte);
    byte iy(byte);

	struct {
		union {
			struct {
				byte f = 0x8; //Zero flag is set
				byte a = 0x0;
			};
			word af;
		};
		
		union {
			struct {
				byte c;
				byte b;
			};
			word bc = 0;
		};
		
		union {
			struct {
				byte e;
				byte d;
			};
			word de;
		};
		
		union {
			struct {
				byte l;
				byte h;
			};
			word hl = 0;
		};
		
		union {
		    struct {
		        byte i;
		        byte r;
		    };
		    word ir = 0;
		};
		
		struct {
		    word af = 0;
		    word bc = 0;
		    word de = 0;
		    word hl = 0;
		} shadows;
		
		bit<7> sign = bit<7>(f);
		bit<6> zero = bit<6>(f);
		bit<5> fifth = bit<5>(f);
		bit<4> half_carry = bit<4>(f);
		bit<3> third = bit<3>(f);
		bit<2> PorO = bit<2>(f);
		bit<1> subtract = bit<1>(f);
		bit<0> carry = bit<0>(f);
		
		word ix = 0;
		word iy = 0;
		
		word sp = 0xDFF0;
		word pc = 0x0000;
	} registers { };

    void stackPush(const word);
    word  stackPop();
    
    inline void _rl(byte&);
    inline void _rr(byte&);
    inline void _sla(byte&);
    inline void _sra(byte&);
    inline void _srl(byte&);
    inline void _rlc(byte&);
    inline void _rrc(byte&);
    inline void _sll(byte&);
    inline void _bit(byte, const byte);
    inline void _res(byte&, const byte);
    inline void _set(byte&, const byte);
    inline void _inc(byte&);
    inline void _dec(byte&);
    inline void _cp(byte&, const byte);
    inline void _or(byte&, const byte);
    inline void _and(byte&, const byte);
    inline void _xor(byte&, const byte);
    inline void _sub(byte&, const byte);
    inline void _sbc(byte&, const byte);
    inline void _adc(byte&, const byte);
    inline void _add(byte&, const byte);
    inline void _add(word&, const word);
    
    const static opcode opcodesTable[256];
    
	byte opcode0x00();	byte opcode0x10();	byte opcode0x20();	byte opcode0x30();	byte opcode0x40();	byte opcode0x50();	byte opcode0x60();	byte opcode0x70();	
	byte opcode0x80();	byte opcode0x90();	byte opcode0xA0();	byte opcode0xB0();	byte opcode0xC0();	byte opcode0xD0();	byte opcode0xE0();	byte opcode0xF0();
	byte opcode0x01();	byte opcode0x11();	byte opcode0x21();	byte opcode0x31();	byte opcode0x41();	byte opcode0x51();	byte opcode0x61();	byte opcode0x71();	
	byte opcode0x81();	byte opcode0x91();	byte opcode0xA1();	byte opcode0xB1();	byte opcode0xC1();	byte opcode0xD1();	byte opcode0xE1();	byte opcode0xF1();
	byte opcode0x02();	byte opcode0x12();	byte opcode0x22();	byte opcode0x32();	byte opcode0x42();	byte opcode0x52();	byte opcode0x62();	byte opcode0x72();	
	byte opcode0x82();	byte opcode0x92();	byte opcode0xA2();	byte opcode0xB2();	byte opcode0xC2();	byte opcode0xD2();	byte opcode0xE2();	byte opcode0xF2();
	byte opcode0x03();	byte opcode0x13();	byte opcode0x23();	byte opcode0x33();	byte opcode0x43();	byte opcode0x53();	byte opcode0x63();	byte opcode0x73();	
	byte opcode0x83();	byte opcode0x93();	byte opcode0xA3();	byte opcode0xB3();	byte opcode0xC3();	byte opcode0xD3();	byte opcode0xE3();	byte opcode0xF3();
	byte opcode0x04();	byte opcode0x14();	byte opcode0x24();	byte opcode0x34();	byte opcode0x44();	byte opcode0x54();	byte opcode0x64();	byte opcode0x74();	
	byte opcode0x84();	byte opcode0x94();	byte opcode0xA4();	byte opcode0xB4();	byte opcode0xC4();	byte opcode0xD4();	byte opcode0xE4();	byte opcode0xF4();
	byte opcode0x05();	byte opcode0x15();	byte opcode0x25();	byte opcode0x35();	byte opcode0x45();	byte opcode0x55();	byte opcode0x65();	byte opcode0x75();	
	byte opcode0x85();	byte opcode0x95();	byte opcode0xA5();	byte opcode0xB5();	byte opcode0xC5();	byte opcode0xD5();	byte opcode0xE5();	byte opcode0xF5();
	byte opcode0x06();	byte opcode0x16();	byte opcode0x26();	byte opcode0x36();	byte opcode0x46();	byte opcode0x56();	byte opcode0x66();	byte opcode0x76();	
	byte opcode0x86();	byte opcode0x96();	byte opcode0xA6();	byte opcode0xB6();	byte opcode0xC6();	byte opcode0xD6();	byte opcode0xE6();	byte opcode0xF6();
	byte opcode0x07();	byte opcode0x17();	byte opcode0x27();	byte opcode0x37();	byte opcode0x47();	byte opcode0x57();	byte opcode0x67();	byte opcode0x77();	
	byte opcode0x87();	byte opcode0x97();	byte opcode0xA7();	byte opcode0xB7();	byte opcode0xC7();	byte opcode0xD7();	byte opcode0xE7();	byte opcode0xF7();
	byte opcode0x08();	byte opcode0x18();	byte opcode0x28();	byte opcode0x38();	byte opcode0x48();	byte opcode0x58();	byte opcode0x68();	byte opcode0x78();	
	byte opcode0x88();	byte opcode0x98();	byte opcode0xA8();	byte opcode0xB8();	byte opcode0xC8();	byte opcode0xD8();	byte opcode0xE8();	byte opcode0xF8();
	byte opcode0x09();	byte opcode0x19();	byte opcode0x29();	byte opcode0x39();	byte opcode0x49();	byte opcode0x59();	byte opcode0x69();	byte opcode0x79();	
	byte opcode0x89();	byte opcode0x99();	byte opcode0xA9();	byte opcode0xB9();	byte opcode0xC9();	byte opcode0xD9();	byte opcode0xE9();	byte opcode0xF9();
	byte opcode0x0A();	byte opcode0x1A();	byte opcode0x2A();	byte opcode0x3A();	byte opcode0x4A();	byte opcode0x5A();	byte opcode0x6A();	byte opcode0x7A();	
	byte opcode0x8A();	byte opcode0x9A();	byte opcode0xAA();	byte opcode0xBA();	byte opcode0xCA();	byte opcode0xDA();	byte opcode0xEA();	byte opcode0xFA();
	byte opcode0x0B();	byte opcode0x1B();	byte opcode0x2B();	byte opcode0x3B();	byte opcode0x4B();	byte opcode0x5B();	byte opcode0x6B();	byte opcode0x7B();	
	byte opcode0x8B();	byte opcode0x9B();	byte opcode0xAB();	byte opcode0xBB();	byte opcode0xCB();	byte opcode0xDB();	byte opcode0xEB();	byte opcode0xFB();
	byte opcode0x0C();	byte opcode0x1C();	byte opcode0x2C();	byte opcode0x3C();	byte opcode0x4C();	byte opcode0x5C();	byte opcode0x6C();	byte opcode0x7C();
	byte opcode0x8C();	byte opcode0x9C();	byte opcode0xAC();	byte opcode0xBC();	byte opcode0xCC();	byte opcode0xDC();	byte opcode0xEC();	byte opcode0xFC();
	byte opcode0x0D();	byte opcode0x1D();	byte opcode0x2D();	byte opcode0x3D();	byte opcode0x4D();	byte opcode0x5D();	byte opcode0x6D();	byte opcode0x7D();	
	byte opcode0x8D();	byte opcode0x9D();	byte opcode0xAD();	byte opcode0xBD();	byte opcode0xCD();	byte opcode0xDD();	byte opcode0xED();	byte opcode0xFD();
	byte opcode0x0E();	byte opcode0x1E();	byte opcode0x2E();	byte opcode0x3E();	byte opcode0x4E();	byte opcode0x5E();	byte opcode0x6E();	byte opcode0x7E();	
	byte opcode0x8E();	byte opcode0x9E();	byte opcode0xAE();	byte opcode0xBE();	byte opcode0xCE();	byte opcode0xDE();	byte opcode0xEE();	byte opcode0xFE();
	byte opcode0x0F();	byte opcode0x1F();	byte opcode0x2F();	byte opcode0x3F();	byte opcode0x4F();	byte opcode0x5F();	byte opcode0x6F();	byte opcode0x7F();	
	byte opcode0x8F();	byte opcode0x9F();	byte opcode0xAF();	byte opcode0xBF();	byte opcode0xCF();	byte opcode0xDF();	byte opcode0xEF();	byte opcode0xFF();
};

#endif