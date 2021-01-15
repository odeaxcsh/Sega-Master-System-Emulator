#ifndef __PSG_H__
#define __PSG_H__

#include "definitions.h"
#include "logger.h"

class PSG
{
  public:
    PSG() = default;
    PSG(Logger*);
    void update(byte);
    void writeOn(byte);
  
  private:
    const static uint16 volumeTable[16];
	
	Logger *logger = Logger::null;
	
    struct Channel {
    	int8 polarity = 1;
    	uint8 volume = 0xF;
    	uint16 tone = 0;
    	uint16 counter = 0;
    } channel[3], noise;
    
    uint8 latchedChannel = 0;
    uint8 regWhichLatched = 0;
    uint16 currentPosition = 0;
    int16 playbackBuffer[1024];
    uint16 LFSR = 0x8000;
    
    //Timing information
	long double precition = 0;
    long double clocksFromLastUpdateToBuffer = 0;
    const static long double clocksToFlushToBuffer;
};

#endif