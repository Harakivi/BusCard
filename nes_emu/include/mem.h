#pragma once
#include <stdint.h>

extern uint8_t cpu_ram[0x800];  /* RAM*/

extern const uint8_t* prg_rombank0;    /* prg-rom lower bank*/   
extern const uint8_t* prg_rombank1;    /* prg-rom upper bank*/   
 
uint8_t ppu_read_nametables(void);
void ppu_write_nametables(uint8_t value);
void ppu_putmemory(uint8_t value);
uint8_t ppu_getmemory(void);
void ppu_write_register(uint16_t RX, uint8_t value);
uint8_t ppu_read_register(uint16_t RX);
void sprite_dma(uint8_t scr_addr);
int cpu_getmemory(uint16_t addr);
int cpu_getmemory16(uint16_t addr);
void cpu_putmemory(uint16_t addr, uint8_t value);
