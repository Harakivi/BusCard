#pragma once
#include <stdint.h>
#include "iLcd.hpp"
#include "Gui/Control.hpp"
#include "BoardButtons.hpp"
#include "Gui/FileManager.hpp"

class NES_EMU : public Gui::Control
{
private:
	Gui::FileManager fileManager = Gui::FileManager(*this);
	uint16_t Buffer_scanline1[8 + 256 + 8];
	uint16_t Buffer_scanline2[8 + 256 + 8];

	uint16_t *currentBuffer = Buffer_scanline1;
	uint8_t currentBufferNum = 0;

	Drivers::ActiveButtons _joyState;

	bool _romLoaded;

public:
	typedef struct
	{
		uint8_t Constant_NES[1];
		uint8_t romnum, vromnum, romfeature, rommappernum;
		uint8_t dontcare[8];
	} NesRomHeader;

	typedef struct
	{
		NesRomHeader Struct;
		uint8_t data[40960];
	} NesRom;

	NES_EMU(Gui::Control &parentControl);
	virtual void Draw(Utils::GFX &gfx);
	virtual void Processing(Utils::GFX &gfx, Control **activePtr)
	{
		if (!fileManager.GetSelectedName())
		{
			if (fileManager.GetOnNavigateButton() == Drivers::ButtonMask::B)
			{
				*activePtr = &_parentControl;
				return;
			}
			*activePtr = &fileManager;
			return;
		}
		else if (!_romLoaded)
		{
			Init();
		}

		Draw(gfx);
	}
	virtual Control &ButtonStateChanged(Drivers::ActiveButtons butt, Utils::GFX &gfx)
	{
		_joyState = butt;
		return *this;
	}
	virtual void ReDrawChild(Utils::GFX &gfx)
	{
	}

private:
	void Init();
	NesRom romStorage;
};