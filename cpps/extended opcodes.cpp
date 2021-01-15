#ifndef __EXTENDED_OPCODES__
#define __EXTENDED_OPCODES__

#include "../include/definitions.h"
#include "../include/utilities.h"
#include "../include/cpu.h"
    
byte CPU::bits(byte opcode)
{
	 auto position = [](const byte opcode) -> byte { return (opcode & 0x38) >> 3; };
	 byte buffer = 0;
	const auto regmap = [&](const byte opcode) -> byte& {
	    switch(opcode & 0x07) {
	        case 0: return registers.b;
	        case 1: return registers.c;
	        case 2: return registers.d;
	        case 3: return registers.e;
	        case 4: return registers.h;
	        case 5: return registers.l;
	        case 6: return buffer = 0;
	        case 7: return registers.a;
	        default: throw "logicError(\"Read it BUG ://\");";
	    }
	};
	if((opcode & 0x07) == 0x06) {
		byte hlPointsTo = mmu->read(registers.hl);
		switch(opcode & 0xF8) {
	       case 0x00: _rlc(hlPointsTo); break;
	       case 0x08: _rrc(hlPointsTo); break;
	       case 0x10: _rl(hlPointsTo); break;
	       case 0x18: _rr(hlPointsTo); break;
	       case 0x20: _sla(hlPointsTo); break;
	       case 0x28: _sra(hlPointsTo); break;
	       case 0x30: _sll(hlPointsTo); break;
	       case 0x38: _srl(hlPointsTo); break;
	  	 default: 
	  	 	switch(opcode & 0xC0) {
					case 0x40: _bit(hlPointsTo, position(opcode)); break;
			    	case 0x80: _res(hlPointsTo, position(opcode)); break;
					case 0xC0: _set(hlPointsTo, position(opcode)); break;
			  }
		}
	  mmu->write(registers.hl, hlPointsTo);
	  return 15 - 3 * ((opcode & 0xC0) == 0x40);
	} else {
		switch(opcode & 0xF8) {
	       case 0x00: _rlc(regmap(opcode)); break;
	       case 0x08: _rrc(regmap(opcode)); break;
	       case 0x10: _rl(regmap(opcode)); break;
	       case 0x18: _rr(regmap(opcode)); break;
	       case 0x20: _sla(regmap(opcode)); break;
	       case 0x28: _sra(regmap(opcode)); break;
	       case 0x30: _sll(regmap(opcode)); break;
	       case 0x38: _srl(regmap(opcode)); break;
	   	default:
	   		switch(opcode & 0xC0) {
					case 0x40: _bit(regmap(opcode), position(opcode)); break;
			    	case 0x80: _res(regmap(opcode), position(opcode)); break;
					case 0xC0: _set(regmap(opcode), position(opcode)); break;
			  }
		}
		return 8;
	}
}

byte CPU::extd(byte opcode)
{

	const auto bigregmap = [&](const byte opcode) -> word& {
		switch(opcode & 0x03) {
			case 0x00: return registers.bc;
			case 0x01: return registers.de;
			case 0x02: return registers.hl;
			case 0x03: return registers.sp;
		};
	};

	byte buffer = 0;
	const auto regmap = [&](const byte opcode) -> byte& {
	    switch(opcode & 0x07) {
	        case 0: return registers.b;
	        case 1: return registers.c;
	        case 2: return registers.d;
	        case 3: return registers.e;
	        case 4: return registers.h;
	        case 5: return registers.l;
	        case 6: return buffer = 0;
	        case 7: return registers.a;
	        default: throw "logicError(\"Read it BUG ://\");";
	    }
	};

	byte value;
	switch(opcode & 0xC7) {
		case 0x40: //IN
			value = regmap(opcode >> 3) = pm->read(registers.c);
			registers.sign = (value & 0x80) == 0 ? 0 : 1;
			registers.zero = (value == 0) ? 1 : 0;
			registers.PorO = value & 0x01;
			registers.half_carry = 0;
			registers.subtract = 0; 
			return 12;
		
		case 0x41: //OUT
			pm->write(registers.c, regmap(opcode >> 3));
			return 12;
			
		case 0x42:
			if(not isBitSet(opcode, 4)) {
				byte carry = registers.carry;
				registers.carry = (registers.hl < bigregmap(opcode >> 4) + carry) ? 1 : 0;
				registers.half_carry = ((registers.hl & 0x0FFF) < (bigregmap(opcode >> 4) & 0x0FFF) + carry) ? 1 : 0;
				registers.hl -= bigregmap(opcode >> 4) + carry;
				registers.subtract = 1;
				registers.zero = (registers.hl == 0) ? 1 : 0;
				registers.sign = isBitSet(registers.hl, 15);
			} else {
				byte carry = registers.carry;
				registers.carry = (registers.hl + bigregmap(opcode >> 4) + carry) > 0xFFFF ? 1 : 0;
				registers.half_carry = ((registers.hl & 0x0FFF) + (bigregmap(opcode >> 4) & 0x0FFF) + carry) > 0x0FFF ? 1 : 0;
				registers.hl += bigregmap(opcode >> 4) + carry;
				registers.subtract = 0;
				registers.zero = (registers.hl == 0) ? 1 : 0;
				registers.sign = isBitSet(registers.hl, 15);	
			} return 15;
		
		case 0x43: {
			Address immediatePointsTo = mmu->readWord(registers.pc);
			registers.pc += 2;
			if(not isBitSet(opcode, 4))
				mmu->writeWord(immediatePointsTo, bigregmap(opcode >> 4));
			else bigregmap(opcode >> 4) = mmu->read(immediatePointsTo);
			return 20;
		}
		
		case 0x44:
			registers.PorO = (registers.a == 0x80 ? 1 : 0);
			registers.carry = (registers.a == 0x80 ? 0 : 1);
			registers.half_carry = ((registers.a & 0x0F) == 0 ? 0 : 1);
			registers.a = 0 - registers.a;
			registers.zero = (registers.a == 0 ? 1 : 0);
			registers.sign = isBitSet(registers.a, 7);
			registers.subtract = 1;
			return 8;
		
		case 0x45:
			registers.pc = stackPop();
			FF1 = FF2;
			NMI_servising = false;
			return 14;
		
		case 0x46:
			switch((opcode >> 3) & 0x07) {
				case 0x2: case 0x6:
					interruptsMode = 1;
					break;
				case 0x0: case 0x4:
					interruptsMode = 0;
					throw "Unsupported";
				case 0x3: case 0x5:
					interruptsMode = 2;
					throw "Unsupported";
			} return 8;
		
		case 0x47:
			switch((opcode >> 3) & 0x07) {
				case 0x0: registers.i = registers.a; return 9;
				case 0x1: registers.r = registers.a; return 9;
				case 0x2: registers.a = registers.i; return 9;
				case 0x3: registers.a = registers.r; return 9;
				case 0x4: { //RRD
					byte var = mmu->read(registers.hl);
					var = (var >> 4) | (registers.a << 4);
					registers.a = (registers.a & 0xF0) | (mmu->read(registers.hl) & 0x0F);
					mmu->write(registers.hl, var);
					registers.subtract = 0;
					registers.half_carry = 0;
					registers.zero = (registers.a == 0) ? 1 : 0;
					registers.sign = isBitSet(registers.a, 7);
					registers.PorO = (registers.a & 0x1) == 0 ? 1 : 0;
					return 18;
				} case 0x5: { //RLD
					byte var = mmu->read(registers.hl);
					var = (var << 4) | (registers.a & 0xF);
					registers.a = (registers.a & 0xF0) | ((mmu->read(registers.hl) & 0xF0) >> 4);
					mmu->write(registers.hl, var);
					registers.subtract = 0;
					registers.half_carry = 0;
					registers.zero = (registers.a == 0) ? 1 : 0;
					registers.sign = isBitSet(registers.a, 7);
					registers.PorO = (registers.a & 0x1) == 0 ? 1 : 0;
					return 18;
				}
			}
	}
	
	byte cycles = 16;

	byte pointedTo = mmu->read(registers.hl);

	byte sign = (isBitSet(opcode, 3) * -2) + 1;
	switch(opcode & 0x03) {
		case 0x00: switch(opcode >> 4) {
			case 0x0B:
				if(registers.bc - 1) {
					cycles = 21;
					registers.pc -= 2;
				}
			case 0x0A:
				mmu->write(registers.de, mmu->read(registers.hl));
				--registers.bc;
				registers.de += sign;
				registers.hl += sign;
				registers.PorO = (registers.bc == 0) ? 0 : 1;
				registers.half_carry = 0;
				registers.subtract = 0;
				return cycles;
		}
		case 0x01: switch(opcode >> 4) {
			case 0xB:
				if((registers.bc - 1) and (pointedTo - registers.a)) {
					cycles = 21;
					registers.pc -= 2;
				}
			case 0xA:
				registers.zero = (pointedTo == registers.a) ? 1 : 0;
				registers.half_carry = ((registers.a & 0x0F) < (pointedTo & 0x0F));
				registers.sign = (registers.a < pointedTo) ? 1 : 0;
				registers.PorO = (registers.bc == 0) ? 0 : 1;
				registers.subtract = 1;
				--registers.bc;
				registers.hl += sign;
				return cycles;
		}
		case 0x02: switch(opcode >> 4) {
			case 0xB:
				if(registers.b - 1) {
					cycles = 21;
					registers.pc -= 2;
				}
			
			case 0xA:
				mmu->write(registers.hl, pm->read(registers.c));
				registers.zero = (registers.b == 1) ? 0 : 1;
				--registers.b;
				registers.hl += sign;
				registers.subtract = 1;
				return cycles;
		}
		case 0x03: switch(opcode >> 4) {
			case 0xB:
				if(registers.b - 1) {
					cycles = 21;
					registers.pc -= 2;
				}
			
			case 0xA:
				pm->write(registers.c, mmu->read(registers.hl));
				registers.zero = (registers.b == 1) ? 0 : 1;
				--registers.b;
				registers.hl += sign;
				registers.subtract = 1;
				return cycles;
		}
	}
	return 8; //NOP
}

byte CPU::ix(byte opcode)
{
	byte buffer;
	Address address;
	switch(opcode) {
		case 0x09:
			_add(registers.ix, registers.bc);
			return 15;
		
		case 0x19:
			_add(registers.ix, registers.de);
			return 15;
		
		case 0x11:
			registers.ix = mmu->readWord(registers.pc);
			registers.pc += 2;
			return 14;
		
		case 0x12:
			mmu->write(mmu->readWord(registers.pc), registers.ix);
			registers.pc += 2;
			return 20;
		
		case 0x13:
			++registers.ix;
			return 10;
		
		case 0x29:
			_add(registers.ix, registers.ix);
			return 15;
		
		case 0x2A:
			registers.ix = mmu->readWord(mmu->readWord(registers.pc));
			registers.pc += 2;
			return 20;
		
		case 0x2B:
			--registers.ix;
			return 10;
		
		case 0x34:
			address = mmu->read(registers.pc++) + registers.ix;
			buffer = mmu->read(address);
			_inc(buffer);
			mmu->write(address, buffer);
			return 23;
		
		case 0x35:
			address = mmu->read(registers.pc++) + registers.ix;
			buffer = mmu->read(address);
			_dec(buffer);
			mmu->write(address, buffer);
			return 23;
		
		case 0x36:
			address = mmu->read(registers.pc++) + registers.ix;
			buffer = mmu->read(registers.pc++);
			mmu->write(address, buffer);
			return 19;
		
		case 0x39:
			_add(registers.ix, registers.sp);
			return 15;
		
		case 0x46:
			address = mmu->read(registers.pc++) + registers.ix;
			registers.b = mmu->read(address);
			return 19;
		
		case 0x4E:
			address = mmu->read(registers.pc++) + registers.ix;
			registers.c = mmu->read(address);
			return 19;

		case 0x56:
			address = mmu->read(registers.pc++) + registers.ix;
			registers.d = mmu->read(address);
			return 19;
		
		case 0x5E:
			address = mmu->read(registers.pc++) + registers.ix;
			registers.e = mmu->read(address);
			return 19;

		case 0x66:
			address = mmu->read(registers.pc++) + registers.ix;
			registers.h = mmu->read(address);
			return 19;
		
		case 0x6E:
			address = mmu->read(registers.pc++) + registers.ix;
			registers.l = mmu->read(address);
			return 19;
		
		case 0x70:
			mmu->write(registers.ix + mmu->read(registers.pc++), registers.b);
			return 19;
	
		case 0x71:
			mmu->write(registers.ix + mmu->read(registers.pc++), registers.c);
			return 19;
			
		case 0x72:
			mmu->write(registers.ix + mmu->read(registers.pc++), registers.d);
			return 19;
	
		case 0x73:
			mmu->write(registers.ix + mmu->read(registers.pc++), registers.e);
			return 19;
	
		case 0x74:
			mmu->write(registers.ix + mmu->read(registers.pc++), registers.h);
			return 19;
			
		case 0x75:
			mmu->write(registers.ix + mmu->read(registers.pc++), registers.l);
			return 19;

		case 0x77:
			mmu->write(registers.ix + mmu->read(registers.pc++), registers.a);
			return 19;
	
		case 0x7E:
			address = mmu->read(registers.pc++) + registers.ix;
			registers.a = mmu->read(address);
			return 19;
		
		case 0x86:
			address = registers.ix + mmu->read(registers.pc++);
			_add(registers.a, mmu->read(address));
			return 19;
		
		case 0x8E:
			address = registers.ix + mmu->read(registers.pc++);
			_adc(registers.a, mmu->read(address));
			return 19;
		
		case 0x96:
			address = registers.ix + mmu->read(registers.pc++);
			_sub(registers.a, mmu->read(address));
			return 19;
		
		case 0x9E:
			address = registers.ix + mmu->read(registers.pc++);
			_sbc(registers.a, mmu->read(address));
			return 19;

		case 0xA6:
			address = registers.ix + mmu->read(registers.pc++);
			_and(registers.a, mmu->read(address));
			return 19;
		
		case 0xAE:
			address = registers.ix + mmu->read(registers.pc++);
			_xor(registers.a, mmu->read(address));
			return 19;

		case 0xB6:
			address = registers.ix + mmu->read(registers.pc++);
			_or(registers.a, mmu->read(address));
			return 19;
		
		case 0xBE:
			address = registers.ix + mmu->read(registers.pc++);
			_cp(registers.a, mmu->read(address));
			return 19;
		
		case 0xCB:
		{
			auto position = [](const byte opcode) -> byte { return (opcode & 0x38) >> 3; };
			byte opcode = mmu->read(registers.pc++);
			if((opcode & 0x07) == 0x06) {
				address = registers.ix + mmu->read(registers.pc++);
				buffer = mmu->read(address);
				switch(opcode & 0xF8) {
			       case 0x00: _rlc(buffer); break;
			       case 0x08: _rrc(buffer); break;
			       case 0x10: _rl(buffer); break;
			       case 0x18: _rr(buffer); break;
			       case 0x20: _sla(buffer); break;
			       case 0x28: _sra(buffer); break;
			       case 0x30: _sll(buffer); break;
			       case 0x38: _srl(buffer); break;
			  	 default: 
			  	 	switch(opcode & 0xC0) {
							case 0x40: _bit(buffer, position(opcode)); break;
					    	case 0x80: _res(buffer, position(opcode)); break;
							case 0xC0: _set(buffer, position(opcode)); break;
				 	}
				}
				mmu->write(address, buffer);
			} else return 12; //NOP	
		}
		
		case 0xE1:
			registers.ix = stackPop();
			return 14;
		
		case 0xE3: {
			word tmp = mmu->readWord(registers.sp);
			mmu->writeWord(registers.sp, registers.ix);
			registers.ix = tmp;
			return 23;
		}
		
		case 0xE5:
			stackPush(registers.ix);
			return 15;
		
		case 0xE9:
			registers.pc = registers.ix;
			return 8;
		
		case 0xF9:
			registers.sp = registers.ix;
			return 10;
		
		default: return 8;	//NOP
	}		
}

byte CPU::iy(byte opcode)
{
	byte buffer;
	Address address;
	switch(opcode) {
		case 0x09:
			_add(registers.iy, registers.bc);
			return 15;
		
		case 0x19:
			_add(registers.iy, registers.de);
			return 15;
		
		case 0x11:
			registers.iy = mmu->readWord(registers.pc);
			registers.pc += 2;
			return 14;
		
		case 0x12:
			mmu->write(mmu->readWord(registers.pc), registers.iy);
			registers.pc += 2;
			return 20;
		
		case 0x13:
			++registers.iy;
			return 10;
		
		case 0x29:
			_add(registers.iy, registers.iy);
			return 15;
		
		case 0x2A:
			registers.iy = mmu->readWord(mmu->readWord(registers.pc));
			registers.pc += 2;
			return 20;
		
		case 0x2B:
			--registers.iy;
			return 10;
		
		case 0x34:
			address = mmu->read(registers.pc++) + registers.iy;
			buffer = mmu->read(address);
			_inc(buffer);
			mmu->write(address, buffer);
			return 23;
		
		case 0x35:
			address = mmu->read(registers.pc++) + registers.iy;
			buffer = mmu->read(address);
			_dec(buffer);
			mmu->write(address, buffer);
			return 23;
		
		case 0x36:
			address = mmu->read(registers.pc++) + registers.iy;
			buffer = mmu->read(registers.pc++);
			mmu->write(address, buffer);
			return 19;
		
		case 0x39:
			_add(registers.iy, registers.sp);
			return 15;
		
		case 0x46:
			address = mmu->read(registers.pc++) + registers.iy;
			registers.b = mmu->read(address);
			return 19;
		
		case 0x4E:
			address = mmu->read(registers.pc++) + registers.iy;
			registers.c = mmu->read(address);
			return 19;

		case 0x56:
			address = mmu->read(registers.pc++) + registers.iy;
			registers.d = mmu->read(address);
			return 19;
		
		case 0x5E:
			address = mmu->read(registers.pc++) + registers.iy;
			registers.e = mmu->read(address);
			return 19;

		case 0x66:
			address = mmu->read(registers.pc++) + registers.iy;
			registers.h = mmu->read(address);
			return 19;
		
		case 0x6E:
			address = mmu->read(registers.pc++) + registers.iy;
			registers.l = mmu->read(address);
			return 19;
		
		case 0x70:
			mmu->write(registers.iy + mmu->read(registers.pc++), registers.b);
			return 19;
	
		case 0x71:
			mmu->write(registers.iy + mmu->read(registers.pc++), registers.c);
			return 19;
			
		case 0x72:
			mmu->write(registers.iy + mmu->read(registers.pc++), registers.d);
			return 19;
	
		case 0x73:
			mmu->write(registers.iy + mmu->read(registers.pc++), registers.e);
			return 19;
	
		case 0x74:
			mmu->write(registers.iy + mmu->read(registers.pc++), registers.h);
			return 19;
			
		case 0x75:
			mmu->write(registers.iy + mmu->read(registers.pc++), registers.l);
			return 19;

		case 0x77:
			mmu->write(registers.iy + mmu->read(registers.pc++), registers.a);
			return 19;
	
		case 0x7E:
			address = mmu->read(registers.pc++) + registers.iy;
			registers.a = mmu->read(address);
			return 19;
		
		case 0x86:
			address = registers.iy + mmu->read(registers.pc++);
			_add(registers.a, mmu->read(address));
			return 19;
		
		case 0x8E:
			address = registers.iy + mmu->read(registers.pc++);
			_adc(registers.a, mmu->read(address));
			return 19;
		
		case 0x96:
			address = registers.iy + mmu->read(registers.pc++);
			_sub(registers.a, mmu->read(address));
			return 19;
		
		case 0x9E:
			address = registers.iy + mmu->read(registers.pc++);
			_sbc(registers.a, mmu->read(address));
			return 19;

		case 0xA6:
			address = registers.iy + mmu->read(registers.pc++);
			_and(registers.a, mmu->read(address));
			return 19;
		
		case 0xAE:
			address = registers.iy + mmu->read(registers.pc++);
			_xor(registers.a, mmu->read(address));
			return 19;

		case 0xB6:
			address = registers.iy + mmu->read(registers.pc++);
			_or(registers.a, mmu->read(address));
			return 19;
		
		case 0xBE:
			address = registers.iy + mmu->read(registers.pc++);
			_cp(registers.a, mmu->read(address));
			return 19;
		
		case 0xCB:
		{
			auto position = [](const byte opcode) -> byte { return (opcode & 0x38) >> 3; };
			byte opcode = mmu->read(registers.pc++);
			if((opcode & 0x07) == 0x06) {
				address = registers.iy + mmu->read(registers.pc++);
				buffer = mmu->read(address);
				switch(opcode & 0xF8) {
			       case 0x00: _rlc(buffer); break;
			       case 0x08: _rrc(buffer); break;
			       case 0x10: _rl(buffer); break;
			       case 0x18: _rr(buffer); break;
			       case 0x20: _sla(buffer); break;
			       case 0x28: _sra(buffer); break;
			       case 0x30: _sll(buffer); break;
			       case 0x38: _srl(buffer); break;
			  	 default: 
			  	 	switch(opcode & 0xC0) {
							case 0x40: _bit(buffer, position(opcode)); break;
					    	case 0x80: _res(buffer, position(opcode)); break;
							case 0xC0: _set(buffer, position(opcode)); break;
				 	}
				}
				mmu->write(address, buffer);
			} else return 12; //NOP
		}
		
		case 0xE1:
			registers.iy = stackPop();
			return 14;
		
		case 0xE3: {
			word tmp = mmu->readWord(registers.sp);
			mmu->writeWord(registers.sp, registers.iy);
			registers.iy = tmp;
			return 23;
		}
		
		case 0xE5:
			stackPush(registers.iy);
			return 15;
		
		case 0xE9:
			registers.pc = registers.iy;
			return 8;
		
		case 0xF9:
			registers.sp = registers.iy;
			return 10;
		
		default: return 8;	//NOP
	}		
}
#endif	