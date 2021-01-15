#ifndef __JOYPAD_CPP__
#define __JOYPAD_CPP__

#include "../include/definitions.h"
#include "../include/utilities.h"
#include "../include/joypad.h"
#include "../include/logger.h"
#include "../include/cpu.h"

Joypad::Joypad(Logger *logger, CPU *cpu)
{
	this->cpu = cpu;
	this->logger = logger;
}

byte Joypad::readPortA() const
{
	return (joypad_1 & 0x3F) | (joypad_2 << 6);
}

byte Joypad::readPortB() const
{
	return (joypad_2 >> 2) | 0xF0;
}

void Joypad::keyRelease(Key key)
{
	if(static_cast<uint8>(key) < 7)
		setBit(joypad_1, static_cast<uint8>(key));
	else setBit(joypad_2, static_cast<uint8>(key) - 7);
}

void Joypad::keyPress(Key key)
{
	if(static_cast<uint8>(key) > 6)
		clearBit(joypad_2, static_cast<uint8>(key) - 7);
	else {
		clearBit(joypad_1, static_cast<uint8>(key) - 7);
		if(key == Key::key_start_1)
			cpu->NMI_request();
	}
}

#endif