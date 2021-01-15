#ifndef __CARTRIDGE_H__
#define __CARTRIDGE_H__

#include "definitions.h"
#include "logger.h"

class Cartridge
{
  public:
    Cartridge() = default;
    Cartridge(Logger*);
    bool loadFromFile(const char*);
    uint8 *page(const uint8);
    bool is_codeMasters() const;
        
  private:
    Size rom_size = 0;
    uint8 *rom = nullptr;
    
	struct {
		const struct {
			uint8 sega_string[10];
			uint8 checksum[2];
			uint8 product_code_version[3];
			uint8 region_code_rom_size;
		} *header = nullptr;
		
		const struct {
			uint8 num_of_banks;
			uint8 day, month, year;
			uint8 hour, minute;
			uint16 checksum, compute;
		} *codeMasters_header = nullptr;
	} device_info { };
	
    Logger *logger = Logger::null;
};
    
#endif