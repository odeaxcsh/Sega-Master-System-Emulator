#ifndef __JOYPAD_H__
#define __JOYPAD_H__

#include "definitions.h"
#include "logger.h"
#include "cpu.h"

enum class Key: uint8
{
	key_up_1 = 0,
	key_down_1 = 1,
	key_left_1 = 2,
	key_right_1 = 3,
	key_1_1 = 4,
	key_2_1 = 5,
	key_start_1 = 6,
	key_up_2 = 7,
	key_down_2 = 8,
	key_left_2 = 9,
	key_right_2 = 10,
	key_1_2 = 11,
	key_2_2 = 12,
	key_start_2 = 13
};

class Joypad
{
  public:
    Joypad(Logger *logger, CPU*);
    byte readPortA() const;
    byte readPortB() const;
    void keyPress(Key);
    void keyRelease(Key);
  
  private:
    Logger *logger = Logger::null;
    CPU* cpu = nullptr;
    byte joypad_1 = 0xFF;
    byte joypad_2 = 0xFF;
};

#endif