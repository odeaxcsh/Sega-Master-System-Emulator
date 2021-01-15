#include "main.h"
#include <SDL2/SDL_audio.h>

class Emulator
{
  public:
    Emulator();
    void run();
  
  private:
    CPU cpu;
    VDP vdp;
    PSG psg;
    Joypad pad;
    MMU mmu;
    Cartridge cart;
    PortManager pm;
};

Logger *null = Logger::null;
Logger *print = &((new Logger([](const char *str){ printf("%s\n", str); }))->setFlags(true, true, true));

int main()
{
	Emulator *e = new Emulator();
	e->run();
	
}

Emulator::Emulator() :
cart(null),
mmu(),
psg(null),
cpu(),
vdp(null, &cpu, false),
pad(null, &cpu),
pm(null, &vdp, &psg, &pad)
{
	cart.loadFromFile("ROMS/XXX.sms");
	mmu.Initialize(null, &cart);
	cpu.Initialize(null, &mmu, &pm);
}

void Emulator::run()
{
	static const long double MAX = 3.7e6 / 60.0;
	long double cycles = 0;
	while(true) {
		byte c = cpu.tick();
		vdp.update(c);
		psg.update(c);
		cycles += c;
		if(cycles >= MAX) {
			cycles -= MAX;
		}
	}
}
		
