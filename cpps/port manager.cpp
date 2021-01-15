#ifndef __PORT_MANAGER_CPP__
#define __PORT_MANAGER_CPP__


#include "../include/definitions.h"
#include "../include/port manager.h"
#include "../include/joypad.h"
#include "../include/psg.h"
#include "../include/vdp.h"

PortManager::PortManager(Logger *logger, VDP *vdp, PSG *psg, Joypad *joypad)
{
	this->vdp = vdp;
	this->psg = psg;
	this->joypad = joypad;
	this->logger = logger;
}

byte PortManager::read(byte port)
{
	switch(port) {
		case 0x7E: return vdp->getVCounter();
		case 0x7F: return vdp->getHCounter();
		case 0xBE: return vdp->getData();
		case 0xBD: case 0xBF: return vdp->getStatus();
		case 0xC0: case 0xDC: return joypad->readPortA();
		case 0xC1: case 0xDD: return joypad->readPortB();
		default: 
			logger->warning("Unsupported port requested[%02X]", port);
			return 0xFF;
	}
}

void PortManager::write(byte port, byte value)
{
	switch(port) {
		case 0x7E: case 0x7F: psg->writeOn(value); break;
		case 0xBE: vdp->writeOnData(value); break;
		case 0xBD: case 0xBF: vdp->writeOnControl(value); break;
		default: logger->warning("Unsupported port requestes[%02X]", port);
	}
}

#endif