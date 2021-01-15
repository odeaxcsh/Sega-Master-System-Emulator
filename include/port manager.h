#ifndef __PORT_MANAGER_H__
#define __PORT_MANAGER_H__

#include "definitions.h"
#include "joypad.h"
#include "psg.h"
#include "vdp.h"

class PortManager
{
  public:
    using Port = uint8;
    PortManager(Logger*, VDP*, PSG*, Joypad*);
    void write(Port, byte);
    byte read(Port);
  
  private:
    Logger *logger = Logger::null;
    Joypad *joypad = nullptr;
    PSG *psg = nullptr;
    VDP *vdp = nullptr;
};

#endif