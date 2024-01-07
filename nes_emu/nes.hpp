#pragma once
#include <stdint.h>
#include "iLcd.hpp"
#include "Gui/Control.hpp"
#include "BoardButtons.hpp"


class NES_EMU : public Gui::Control
{
private:
	uint16_t Buffer_scanline1[8 + 256 + 8];
	uint16_t Buffer_scanline2[8 + 256 + 8];

	uint16_t *currentBuffer = Buffer_scanline1;
	uint8_t currentBufferNum = 0;

	Drivers::ActiveButtons _joyState;

public:
	typedef struct
	{
		uint8_t Constant_NES[4];
		uint8_t romnum, vromnum, romfeature, rommappernum;
		uint8_t dontcare[8];
	} NesRomHeader;

	typedef struct
	{
		NesRomHeader Struct;
		uint8_t data[40960];
	} NesRom;

	NES_EMU(const NesRom *game);
	virtual void Draw(Utils::GFX &gfx);
	virtual void Processing(Utils::GFX &gfx, Control** activePtr)
	{
		Draw(gfx);
	}
	virtual Control &ButtonStateChanged(Drivers::ActiveButtons butt, Utils::GFX &gfx)
	{
		_joyState = butt;
		return *this;
	}

private:
	NesRom romStorage;
};