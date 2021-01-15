#ifndef __CARTRIDGE_CPP__
#define __CARTRIDGE_CPP__

#include "../include/cartridge.h"
#include "../include/logger.h"
#include "logger.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <initializer_list>

Cartridge::Cartridge(Logger *logger) : Cartridge()
{
	this->logger = logger;
	this->rom = nullptr;
	this->rom_size = 0;
}

bool Cartridge::loadFromFile(const char *path)
{
	Delete(rom);
	rom_size = 0;
	FILE *romFile = fopen(path, "rb");
	
	if( romFile == nullptr) {
		return false;
	}
	
	fseek(romFile, 0, SEEK_END);
	Size sizeof_romFile = ftell(romFile);
	fseek(romFile, 0, SEEK_SET);

	if(sizeof_romFile & 0x3FFF) {
		logger->warning("Size of ROM is not multiple of 16KB [Its: %d]", sizeof_romFile);
		rom_size = sizeof_romFile - (sizeof_romFile & 0x3FFF) + 0x4000;
	} else rom_size = sizeof_romFile;
	if(not New(rom, rom_size)) {
		logger->fatal("There's a problem at memory allocation [Requested size: %d]", rom_size);
		return false;
	}
	
	if(rom_size != fread(rom, 1, rom_size, romFile))
		memset(rom + rom_size - 0x4000, 0 , 0x4000);
		
	for(auto headerLocation : {0x1FF0, 0x3FF0, 0x7FF0})
		if(not strcmp((char*)(rom + headerLocation), "TMR SEGA"))
			reinterpretCast(device_info.header, rom + headerLocation);
	if( (0x10000 - (rom[0x7fe6]| rom[0x7fe7] << 8)) == (rom[0x7fe8] | rom[0x7fe9] << 8) )
		reinterpretCast(device_info.codeMasters_header, rom + 0x7fe0);
	else device_info.codeMasters_header = nullptr;
	if(device_info.codeMasters_header != nullptr)
		logger->report("Loaded cartridge is Code Masters one");
	return true;
}

uint8 *Cartridge::page(uint8 index)
{
	if(index <= (rom_size / 0x4000))
		return rom + index * 0x4000;
	else return nullptr;
}

bool Cartridge::is_codeMasters() const
{
	return (device_info.codeMasters_header != nullptr);
}

#endif