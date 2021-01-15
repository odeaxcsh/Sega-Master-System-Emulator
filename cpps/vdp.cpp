#ifndef __VDP_CPP__
#define __VDP_CPP__

#include "../include/definitions.h"
#include "../include/utilities.h"
#include "../include/logger.h"
#include "../include/cpu.h"
#include "../include/vdp.h"

VDP::VDP(Logger *logger, CPU *cpu, bool isPAL)
{
	this->logger = logger;
	this->isPAL = isPAL;
	this->cpu = cpu;
	
	registers[2] = 0xFF;
	registers[3] = 0xFF;
	registers[4] = 0x07;
	registers[5] = 0xFF;
	registers[10] = 0xFF;
	
	resolution.setted.width = 255;
	resolution.setted.height = 192;
	resolution.active = resolution.setted;
}

void VDP::update(byte cycles)
{
	HCounter += cycles;
	if(HCounter >= 684) {
		HCounter = HCounter % 684;
		++VCounter;
		
		if(VCounter == (isPAL ? 313 : 262)) {
			resolution.active = resolution.setted;
			VCounter = 0;
			render();
		}
		
		if(VCounter == resolution.active.height)
			setBit(registers.status, 7);
			
		if(VCounter > resolution.active.height) {
			VScroll = registers[9];
			lineInterruptCounter = registers[10];
		} else {
			--lineInterruptCounter;
			if(lineInterruptCounter < 0) {
				setBit(registers[0], 4);
				lineInterruptCounter = registers[10];
			}
		}
	}
	
	if(isBitSet(registers.status, 7) and isBitSet(registers[1], 5))
		cpu->MI_request(true);		
}

void VDP::render()
{
	spriteRender();
}

void VDP::spriteRender()
{
	uint8 spriteCount = 0;
	Address spriteTableAddress = (registers[5] & 0x7E) << 7;
	bool isZomed = isBitSet(registers[1], 0);
	bool isProtracted = isBitSet(registers[1], 1);
	bool isShiftingOn = isBitSet(registers[0], 3);
	bool useSecondPattern = isBitSet(registers[6], 2);
	uint8 spriteHeight = 8 + 8 * ((isZomed or isProtracted) ? 1 : 0);
	
	for(uint8 sprite = 0; sprite < 64; ++sprite) {
		uint8 yCoord = VRAM[spriteTableAddress + sprite];
		if((VCounter >= yCoord) and (VCounter < (yCoord + spriteHeight))) {
			if((++spriteCount) > 8) {
				//Sprite Overflow
				setBit(registers.status, 6);
				break;
			}
			uint8 xCoord = VRAM[spriteTableAddress + 128 + 2*sprite] - (isShiftingOn ? 8 : 0);
			uint8 tileNum = VRAM[spriteTableAddress + 128 + 2*sprite] + (useSecondPattern ? 255 : 0);
			Address startAddress = ((tileNum << 3) | (VCounter - yCoord)) << 2;
			uint8 *colours = VRAM + startAddress;
			
			for(int col = 0; col < 8; ++col) {
				if(screenPixels[yCoord][xCoord + col][0]) {
					setBit(registers.status, 5);
					continue;
				} 
				auto set = [](uint8& data, const uint8 pos, const bool value) ->uint8&
				{
					if(not value) return clearBit(data, pos);
					else return setBit(data, pos);
				};
				uint8 palette = 0;
				for(int i = 0; i < 4; ++i)
					set(palette, i, isBitSet(colours[i], col));
				if(palette == 0) continue;
				uint8 colour = CRAM[16 + palette];
				screenPixels[yCoord][xCoord + col][0] = (colour & 0x03) * 85;
				screenPixels[yCoord][xCoord + col][1] = ((colour >> 2) & 0x03) * 85;
				screenPixels[yCoord][xCoord + col][2] = ((colour >> 4) & 0x03) * 85;
			}
		}
	}
}

void VDP::writeOnData(byte data)
{
	switch(controlWord >> 14) {
		case 0x00: case 0x01: case 0x02:
			VRAM[controlWord & 0x3FFF] = data;
			break;
		case 0x03:
			CRAM[controlWord & 0x31] = data;
	}
	incControlWord();
	firstByteOfControl = true;
}

void VDP::writeOnControl(byte data)
{
	if(firstByteOfControl) {
		firstByteOfControl = false;
		controlWord = (controlWord & 0xFF00) | data;
	} else {
		firstByteOfControl = true;
		controlWord = (controlWord & 0x00FF) | (data << 8);
		switch(controlWord >> 14) {
			case 0x00:
				readBuffer = VRAM[controlWord & 0x3FFF];
				incControlWord();
				break;
			
			case 0x02:
				if(((controlWord >> 8) & 0x0F) < 11)
					registers[(controlWord >> 8) & 0xF] = (controlWord & 0xFF);
					
				if( (((controlWord >> 8) & 0x0F) == 1) and
				isBitSet(registers[1], 5) and isBitSet(registers.status, 7))
					cpu->MI_request(true);
				
				if(((controlWord >> 8) & 0x0F) < 2) {
					VDPMode = isBitSet(registers[0], 2) ? 4 : 2;
					if(isBitSet(registers[0], 1)) {
						if(isBitSet(registers[1], 3))
							resolution.setted.height = 240;
						else if(isBitSet(registers[1], 4))
							resolution.setted.height = 224;
						else resolution.setted.height = 192;
					}
				}
		}
	}
}

[[deprecated("I'm not sure how it must work")]]
byte VDP::getHCounter()
{
	return HCounter & 0x00FF;
}

//Because of difficulties of jumps implemetation in renderers, jumps are here
byte VDP::getVCounter()
{
	if(isPAL) //PAL Region [ Including Iran :)) ]
		if(resolution.active.height == 224)
			//Jumps: 00-FF -> 00-02 -> CA-FF 
			if(VCounter > 0x102)
				return VCounter - 0x39;
			else if(VCounter > 0xFF)
				return VCounter - 0x100;
			else return VCounter;
		else //Jumps: 00-F2 -> BA-FF
			if(VCounter > 0xF2)
				return VCounter - 0x39;
			else return VCounter;
	else //NTSC Region
		//Jumps: 00-EA -> EA-FF
		if(resolution.active.height == 224)
			if(VCounter > 0xEA)
				return VCounter - 0x06;
			else return VCounter;
		else //Jumps: 00-DA -> D5-FF
			if(VCounter > 0xDA)
				return VCounter - 0x06;
			else return VCounter;	
}

byte VDP::getStatus()
{
	firstByteOfControl = true;
	byte previousStatus = registers.status;
	cpu->MI_request(false);
	registers.status &= (VDPMode == 2) ? 0x2F : 0x1F;
	return previousStatus;
}

byte VDP::getData()
{
	firstByteOfControl = true;
	byte previousBuffer = readBuffer;
	readBuffer = VRAM[controlWord & 0x3FFF];
	incControlWord();
	return previousBuffer;
}

void VDP::incControlWord()
{
	if((controlWord & 0x3FFF) == 0x3FFF)
		controlWord &= 0xC000;
	else ++controlWord;
}

#endif