#ifndef __PSG_CPP__
#define __PSG_CPP__

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include "../include/definitions.h"
#include "../include/utilities.h"
#include "../include/logger.h"
#include "../include/psg.h"

const long double PSG::clocksToFlushToBuffer = 
(223720.4166 / //Clock Speed
(((44100 /*Frequency*/) / (1024 /*Buffer Size*/)) + 1)) /  //sdl callback frequency
(1024 /*Buffer size*/);

const uint16 PSG::volumeTable[16] = 
{ 8000, 5047, 3184, 2008, 1266, 798, 503, 317, 200, 126, 79, 49, 30, 18, 11, 0 };

struct wrapper
{
	uint16 *cp = nullptr;
	int16 *buffer = nullptr;
};

PSG::PSG(Logger *logger) : PSG()
{
	this->logger = logger;
	SDL_CloseAudioDevice(1);
	SDL_AudioSpec as;
	as.freq = 44100;
	as.format = AUDIO_S16SYS;
	as.channels = 1;
	as.silence = 0;
	as.samples = 1024;
	as.size = 0;
	as.callback = [](void *userData, Uint8 *buffer, int len) {
		wrapper *w = static_cast<wrapper*>(userData);
		memcpy(buffer, w->buffer, len);
		w->cp = 0;
	};
	
	as.userdata = new wrapper{&currentPosition, playbackBuffer};
		
	SDL_OpenAudio( &as, 0 ) ;
	SDL_PauseAudio( 0 );
}

void PSG::writeOn(byte value)
{
	if(isBitSet(value, 7)) {
		latchedChannel = (value & 0x60) >> 5;
		regWhichLatched = isBitSet(value, 4);
		Channel &channelToUpdate = (latchedChannel == 3) ? noise : channel[latchedChannel];
		if(regWhichLatched == 1)
			channelToUpdate.volume = (value & 0x0F);
		else {
			channelToUpdate.tone &= 0xFFF0;
			channelToUpdate.tone |= (value & 0x0F);
		}
	} else {
		if(latchedChannel == 3)
			if(not regWhichLatched)
				noise.tone = (value & 0x0F);
			else noise.volume = (value & 0x0F);
		else if(regWhichLatched)
			channel[latchedChannel].volume = (value & 0x0F);
		else {
			channel[latchedChannel].tone &= 0x000F;
			channel[latchedChannel].tone |= value << 4;
		}
	}
	
	if(latchedChannel == 3 and not regWhichLatched)
		LFSR = 0x8000;
	
	logger->report("%s of channel[%d] changed to %d", regWhichLatched ? "Volume" : "Tone", latchedChannel, ((latchedChannel == 3) ? noise : channel[latchedChannel]).*(regWhichLatched ? reinterpret_cast<uint16 PSG::Channel::*>(&Channel::volume) : &Channel::tone)); 
}

uint8 parity(uint16 val) { //Returns parity of number of seted bits
	val ^= val >> 8;
	val ^= val >> 4;
	val ^= val >> 2;
	val ^= val >> 1;
	return val;
};

void PSG::update(byte clocks)
{
	precition += ((long double) clocks / 16.0) - (clocks / 16);
	byte floor = clocks / 16;
	
	for(; precition >= 1; precition -= 1)
		++floor;
	
	clocksFromLastUpdateToBuffer += floor;
	
	int16 tone = 0;
	
	for(int i = 0; i < 3; ++i) {
		if(channel[i].tone) {
			channel[i].counter -= floor;
		
			if(channel[i].counter <= 0) {
				channel[i].counter = channel[i].tone;
				channel[i].polarity *= -1;
			}
		
			tone += volumeTable[channel[i].volume] * channel[i].polarity;
		}
	}
	
	if(noise.tone) {
		noise.counter -= floor;
		if(noise.counter <= 0) {
			switch(noise.tone & 0x03) {
				case 0x00: noise.counter = 0x10; break;
				case 0x01: noise.counter = 0x20; break;
				case 0x10: noise.counter = 0x40; break;
				case 0x11: noise.counter = channel[2].tone; break;
			}
			noise.polarity *= -1;
			if(noise.polarity == 1) {
				bool isWhiteNoise = isBitSet(noise.tone, 2);
				byte tapped = (isBitSet(noise.tone, 0) ? 1 : 0) | (isBitSet(noise.tone, 3) ? 8 : 0);
				LFSR = (LFSR >> 1) | ((isWhiteNoise ? parity(LFSR & tapped) : LFSR & 1) << 15);
			}
		}
		tone += volumeTable[noise.volume] * (LFSR & 1);
	}
	if(clocksFromLastUpdateToBuffer >= clocksToFlushToBuffer) {
		if(currentPosition < 1024)
			playbackBuffer[currentPosition++] = tone;
		
		clocksFromLastUpdateToBuffer -= clocksToFlushToBuffer;
	}
}

#endif			