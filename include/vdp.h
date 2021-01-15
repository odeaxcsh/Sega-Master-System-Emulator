#ifndef __VDP_H__
#define __VDP_H__

#include "definitions.h"
#include "logger.h"
#include "cpu.h"

class VDP
{
  public:
    VDP(Logger *logger, CPU*, bool);
    void writeOnData(byte);
    void writeOnControl(byte);
    byte getVCounter();
    byte getHCounter();
    byte getStatus();
    byte getData();
    
    void update(byte);
    uint8 *getScreen();
    
  private:
	void incControlWord();
    //void backgeoundRender();
    void spriteRender();
    void render();
    
    bool isPAL = false;
    CPU *cpu = nullptr;
   Logger *logger = Logger::null;
        
    uint8 screenPixels[256][240][3] = { };
    uint8 VRAM[0x4000] = { };
    uint8 CRAM[32] = { };

    uint8 HScroll = 0;
    uint8 VScroll = 0;
    uint16 VCounter = 0;
    uint16 HCounter = 0;
    uint8 lineInterruptCounter = 0xFF;
    
    struct {
    	uint8 registers[11];
 	   uint8 status;
    	uint8& operator[](const uint8 index) {
    		return registers[index];
    	}
    } registers;
	   
	struct { 
	    struct {
	    	uint8 width = 0;
	    	uint8 height = 0;
	    } setted, active;
	} resolution;
	
    byte VDPMode = 0;
    
    byte readBuffer = 0;
    word controlWord = 0;
    bool firstByteOfControl = true;
};

#endif