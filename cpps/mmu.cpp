#ifndef __MMU_CPP__
#define __MMU_CPP__

#include "../include/mmu.h"
#include "../include/definitions.h"
#include "../include/cartridge.h"
#include "../include/utilities.h"

#include <stdlib.h>
#include <string.h>

void MMU::Reset()
{
	Delete(ram_bank[0]);
	Delete(ram_bank[1]);
	*(this) = MMU();
}

void MMU::Initialize(Logger *logger, Cartridge *cartridge)
{
	this->Reset();
	this->logger = logger;
    this->cartridge = cartridge;
    
    if(not cartridge->is_codeMasters()) {
        ram_bank[0] = static_cast<uint8*>(malloc(sizeof(uint8) * 0x4000));
        ram_bank[1] = static_cast<uint8*> (malloc(sizeof(uint8) * 0x4000));
        memset(ram_bank[1], 0, 0x4000);
        memset(ram_bank[0], 0, 0x4000);
    } else memset(ram_bank, NULL, 2);
    
    mirrored_ram[0x1FFF] = 2;
    mirrored_ram[0x1FFE] = 1;
    mirrored_ram[0x1FFD] = 0;
    
    rom_bank_1 = cartridge->page(0);
    rom_bank_2 = cartridge->page(1);
    rom_bank_3 = cartridge->page(2);
    switchable_ram = nullptr;
}

word MMU::readWord(Address address) const
{
    return read(address) | read(address + 1) << 8;
}

byte MMU::read(Address address) const
{
    if(address < 0x400 and not cartridge->is_codeMasters())
        return cartridge->page(0)[address];
    
    else switch( ((address & 0xF000) >> 12) & 0xE) {
        case 0x0: case 0x2:
            return rom_bank_1[address];
            
        case 0x4: case 0x6:
            return rom_bank_2[address - 0x4000];
       
       case 0x8: case 0xA:
           return (switchable_ram + (int)rom_bank_3)[address - 0x8000];
       
       case 0xC: 
           return ram[address - 0xC000];
       
       case 0xE:
           return mirrored_ram[address - 0xE000];
    }
    
    return 0; //To slienet warnings
}

void MMU::writeWord(Address address, word data)
{
    write(address, data & 0x00FF);
    write(address + 1, (data & 0xFF00) >> 8);
}

void MMU::write(Address address, byte data)
{
    if(cartridge->is_codeMasters()) {
    	if(cartridge->page(data & 0x3F) == nullptr)
    		//logger->fatal("Indexing out of range of ROM paging[Index: %0d]", data & 0x3F);
        switch(address) {
            case 0x0000:
                rom_bank_1 = cartridge->page(data & 0x3F);
                break;
            
            case 0x4000:
                rom_bank_2 = cartridge->page(data & 0x3F);
                break;
 
            case 0x8000:
                rom_bank_3 = cartridge->page(data & 0x3F);
                break;
        }
    }
    
    switch(((address & 0xF000) >> 12) & 0xE) {
        case 0x0: case 0x2: case 0x4: case 0x6:
            break;
        
        case 0x8: case 0xA:
            if(switchable_ram != nullptr)
            	switchable_ram[address - 0x8000] = data;
            break;
       
       case 0xC:
           ram[address - 0xC000] = data;
           break;
       
       case 0xE:
           mirrored_ram[address - 0xE000] = data;
    }
   
    if(address >= 0xFFFC and not cartridge->is_codeMasters()) {
	    rom_bank_1 = cartridge->page(mirrored_ram[0x1FFD] & 0x3F);
	    rom_bank_2 = cartridge->page(mirrored_ram[0x1FFE] & 0x3F);
		rom_bank_3 = isBitSet(mirrored_ram[0x1FFF], 3) ? nullptr : cartridge->page(mirrored_ram[0x1FFF] & 0x3F);
        switchable_ram = isBitSet(mirrored_ram[0x1FFF], 3) ? ram_bank[isBitSet(mirrored_ram[0x1FFC], 2)] : nullptr;
    }
}
        
#endif         