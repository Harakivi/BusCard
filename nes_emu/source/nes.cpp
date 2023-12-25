#include "nes.hpp"
#include "cpu.h"
#include "ppu.h"
#include <stdbool.h>

Drivers::ActiveButtons *joypadState = 0;
Drivers::ActiveButtons currJoypadState;

NES_EMU::NES_EMU(const NesRom *game) : _joyState{0}
{
    romStorage = *game;
    cpu_initmem(romStorage.data, romStorage.Struct.romnum);
    cpu_reset();
    ppu_init(&romStorage.data[romStorage.Struct.romnum * 0x4000], (romStorage.Struct.romfeature & 0x01));
    joypadState = &_joyState;
    currJoypadState.data = 0;
}

void NES_EMU::Draw(Utils::GFX &gfx)
{
    uint8_t cycles = 0;
    while (cycles < 2)
    {
        SpriteHitFlag = false;
        for (PPU_scanline = 0; PPU_scanline < 21; PPU_scanline++)
        {
            cpu_exec(CLOCKS_PER_SCANLINE);
        }

        PPU_Reg.R2 &= ~R2_SPR0_HIT;

        /* scanline: 21~261*/
        for (; PPU_scanline < SCAN_LINE_DISPLAY_END_NUM; PPU_scanline++)
        {
            if ((SpriteHitFlag == true) && ((PPU_Reg.R2 & R2_SPR0_HIT) == 0))
            {
                int clocks = (sprite[0].x * CLOCKS_PER_SCANLINE) / NES_DISP_WIDTH;
                cpu_exec(clocks);

                PPU_Reg.R2 |= R2_SPR0_HIT;

                cpu_exec(CLOCKS_PER_SCANLINE - clocks);
            }
            else
            {
                cpu_exec(CLOCKS_PER_SCANLINE);
            }

            if (PPU_Reg.R1 & (R1_BG_VISIBLE | R1_SPR_VISIBLE))
            {
                if (SpriteHitFlag == false)
                    ppu_spr0_hit_flag(PPU_scanline - SCAN_LINE_DISPLAY_START_NUM);
            }

            if (cycles)
            {
                ppu_render_line(currentBuffer, PPU_scanline - SCAN_LINE_DISPLAY_START_NUM);

                gfx.DrawImage(0, PPU_scanline - SCAN_LINE_DISPLAY_START_NUM, 240, 1, (uint8_t *)&currentBuffer[16]);
                if (currentBufferNum == 0)
                {
                    currentBuffer = Buffer_scanline2;
                    currentBufferNum = 1;
                }
                else
                {
                    currentBuffer = Buffer_scanline1;
                    currentBufferNum = 0;
                }
            }
        }
        /* scanline: 262*/
        cpu_exec(CLOCKS_PER_SCANLINE);

        PPU_Reg.R2 |= R2_VBlank_Flag;
        if (PPU_Reg.R0 & R0_VB_NMI_EN)
        {
            NMI_Flag = SET1;
        }
        cycles++;
    }
}

enum JoypadButt
{
    A = 0,
    B,
    Select,
    Start,
    Up,
    Down,
    Left,
    Right,
    FirstButt = A,
    LastButt = Right,
};

uint8_t currentJoyReadPos[2] = {A};

extern "C" void joypad_write(uint8_t joy, uint8_t val)
{
    // if (joypad_strobe[n] && !v) {
    if (val)
    {
        currentJoyReadPos[joy] = FirstButt;
        currJoypadState = *joypadState;
    }
}

extern "C" uint8_t joypad_read(uint8_t joy)
{
    if (joy == 1)
        return 0;
    switch ((JoypadButt)currentJoyReadPos[joy]++)
    {
    case A:
        return currJoypadState.namedStruct.A;
        break;
    case B:
        return currJoypadState.namedStruct.B;
        break;
    case Select:
        return currJoypadState.namedStruct.Select;
        break;
    case Start:
        return currJoypadState.namedStruct.Start;
        break;
    case Up:
        return currJoypadState.namedStruct.Up;
        break;
    case Down:
        return currJoypadState.namedStruct.Down;
        break;
    case Left:
        return currJoypadState.namedStruct.Left;
        break;
    case Right:
        return currJoypadState.namedStruct.Right;
        break;
    default:
        break;
    }
    return 0;
}