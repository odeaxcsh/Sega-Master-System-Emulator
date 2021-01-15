#ifndef __MMU_H__
#define __MMU_H__

#include "logger.h"
#include "definitions.h"
#include "cartridge.h"

class MMU
{
  public:
    MMU() = default;
    void Reset();
    void Initialize(Logger*, Cartridge*);
    byte read(Address) const;
    void write(Address, byte);
    word readWord(Address) const;
    void writeWord(Address, word);
  
  private:
    Logger *logger = nullptr;
    Cartridge *cartridge = nullptr;
    uint8 *rom_bank_1 = nullptr;
    uint8 *rom_bank_2 = nullptr;
    uint8 *rom_bank_3 = nullptr;
    uint8 *switchable_ram = nullptr;
    
    uint8 ram[0x2000] = { 0 };
    uint8 mirrored_ram[0x2000] = { 0 };
    uint8 *ram_bank[2] = { nullptr, nullptr };
};

#endif