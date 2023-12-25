#include "cpu.h"
#include "mem.h"
#include <stdint.h>

#define __STATIC_FORCEINLINE static inline __attribute__((always_inline))

// flags = NVRBDIZC
#define C_FLAG 0x01 // 1: Carry
#define Z_FLAG 0x02 // 1: Zero
#define I_FLAG 0x04 // 1: Irq disabled
#define D_FLAG 0x08 // 1: Decimal mode flag (NES unused)
#define B_FLAG 0x10 // 1: Break
#define R_FLAG 0x20 // 1: Reserved (Always 1)
#define V_FLAG 0x40 // 1: Overflow
#define N_FLAG 0x80 // 1: Negative

#define NMI_VECTOR (*(uint16_t *)&prg_rombank1[0x3FFA])
#define IRQ_VECTOR (*(uint16_t *)&prg_rombank1[0x3FFE])
#define RST_VECTOR (*(uint16_t *)&prg_rombank1[0x3FFC])

// /* flags = NVRBDIZC */

CPU_InitFlag NMI_Flag;
static CPU_InitFlag IRQ_Flag;

static uint8_t A;
static uint8_t X;
static uint8_t Y;
static uint8_t P;
static uint8_t S;
static uint16_t PC;

/* internal registers */
static uint8_t opcode;
static uint8_t operand;
static uint16_t operand16;
int cpu_clockticks;

#include "cpu_optable.h"

__STATIC_FORCEINLINE void cpu_loadinstruction()
{
	uint16_t addr = PC++;
	switch (addr & 0xF000)
	{
	default:
		return;
	case 0x0000:
	case 0x1000:
		opcode = cpu_ram[addr & 0x7FF];
		// operand = cpu_ram[(addr+1) & 0x7FF];
		// operand16 = *(uint16_t*)&cpu_ram[(addr+1) & 0x7FF];

#ifdef EROM
	case 0x5000: /* expansion rom*/
		opcode = EROM[addr - 0x5000];
		operand = EROM[(addr + 1) - 0x5000];
		operand16 = *(uint16_t *)&EROM[(addr + 1) - 0x5000];
#endif
	case 0x6000:

#ifdef SRAM
	case 0x7000:
		opcode = SRAM[addr - 0x6000];
		operand = SRAM[(addr + 1) - 0x6000];
		operand16 = *(uint16_t *)&SRAM[(addr + 1) - 0x6000];
#endif

	case 0x8000:
	case 0x9000:
	case 0xA000:
	case 0xB000:
		opcode = prg_rombank0[addr - 0x8000];
		operand = prg_rombank0[(addr + 1) - 0x8000];
		operand16 = *(uint16_t *)&prg_rombank0[(addr + 1) - 0x8000];
	case 0xC000:
	case 0xD000:
	case 0xE000:
	case 0xF000:
		opcode = prg_rombank1[addr - 0xC000];
		operand = prg_rombank1[(addr + 1) - 0xC000];
		operand16 = *(uint16_t *)&prg_rombank1[(addr + 1) - 0xC000];
	}
}
__STATIC_FORCEINLINE uint8_t cpu_getoperand()
{
	// return cpu_getmemory(PC++);
	PC++;
	return operand;
}
__STATIC_FORCEINLINE uint16_t cpu_getoperand16()
{
	// return cpu_getmemory16(PC);
	// PC += 2;
	return operand16;
}

__STATIC_FORCEINLINE void cpu_pushstack(uint8_t value)
{
	cpu_ram[0x0100 + S--] = value;
}
__STATIC_FORCEINLINE void cpu_push16stack(uint16_t value)
{
	S -= 2;
	*(uint16_t *)&cpu_ram[0x0101 + S] = value;
}
__STATIC_FORCEINLINE uint8_t cpu_popstack(void)
{
	return cpu_ram[++S + 0x100];
}
__STATIC_FORCEINLINE uint16_t cpu_pop16stack(void)
{
	uint16_t tmp = *(uint16_t *)&cpu_ram[S + 0x101];
	S += 2;
	return tmp;
}
__STATIC_FORCEINLINE uint8_t cpu_getzeropage(uint8_t addr)
{
	return cpu_ram[addr];
}
__STATIC_FORCEINLINE uint8_t cpu_getzeropage16(uint8_t addr)
{
	return *(uint16_t *)&cpu_ram[addr];
}
__STATIC_FORCEINLINE void cpu_putzeropage(uint8_t addr, uint8_t value)
{
	cpu_ram[addr] = value;
}

/* Adressing modes */
/* Implied */
inline uint16_t cpu_implied(void)
{
	return 0;
}

/* #Immediate */
inline uint16_t cpu_immediate(void)
{
	uint16_t savepc;
	savepc = PC++;
	return savepc;
}

/* ABS */
inline uint16_t cpu_abs(void)
{
	uint16_t savepc;
	savepc = cpu_getoperand16();

	PC++;
	PC++;

	return savepc;
}

/* Branch */
inline uint16_t cpu_relative(void)
{
	uint16_t savepc;
	savepc = cpu_getoperand();

	if (savepc & 0x80)
		savepc -= 0x100;
	if ((savepc >> 8) != (PC >> 8))
		cpu_clockticks++;

	return savepc;
}

/* (ABS) */
inline uint16_t cpu_indirect(void)
{
	uint16_t savepc;
	uint16_t help = cpu_getoperand16();
	savepc = cpu_getmemory16(help);

	PC++;
	PC++;

	return savepc;
}

/* ABS,X */
inline uint16_t cpu_absx(void)
{
	uint16_t savepc;
	savepc = cpu_getoperand16();

	PC++;
	PC++;
	if (opcodeticks[opcode] == 4)
		if ((savepc >> 8) != ((savepc + X) >> 8))
			cpu_clockticks++;
	savepc += X;
	return savepc;
}

/* ABS,Y */
inline uint16_t cpu_absy(void)
{
	uint16_t savepc;
	savepc = cpu_getoperand16();

	PC++;
	PC++;

	if (opcodeticks[opcode] == 4)
		if ((savepc >> 8) != ((savepc + Y) >> 8))
			cpu_clockticks++;
	savepc += Y;
	return savepc;
}

/* ZP */
inline uint16_t cpu_zp(void)
{
	uint16_t savepc;
	savepc = cpu_getoperand();
	return savepc;
}

/* ZP,X */
inline uint16_t cpu_zpx(void)
{
	uint16_t savepc;
	savepc = cpu_getoperand() + X;

	savepc &= 0x00ff;
	return savepc;
}

/* ZP,Y */
inline uint16_t cpu_zpy(void)
{
	uint16_t savepc;
	savepc = cpu_getoperand() + Y;

	savepc &= 0x00ff;
	return savepc;
}

/* (ZP,X) */
inline uint16_t cpu_indx(void)
{
	uint16_t savepc;
	uint8_t value = cpu_getoperand() + Y;
	// savepc = cpu_getmemory16(value);
	savepc = cpu_getzeropage16(value);
	return savepc;
}

/* (ZP),Y */
inline uint16_t cpu_indy(void)
{
	uint16_t savepc;
	uint8_t value = cpu_getoperand();
	savepc = cpu_getmemory16(value);
	// savepc = cpu_getzeropage16(value);

	if (opcodeticks[opcode] == 5)
		if ((savepc >> 8) != ((savepc + Y) >> 8))
			cpu_clockticks++;
	savepc += Y;
	return savepc;
}

/* (ABS,X) */
inline uint16_t cpu_indabsx(void)
{
	uint16_t savepc;
	uint16_t help = cpu_getoperand16();
	savepc = cpu_getmemory16(help);
	return savepc;
}

/* (ZP) */
inline uint16_t cpu_indzp(void)
{
	uint16_t savepc;
	uint8_t value = cpu_getoperand();
	savepc = cpu_getzeropage16(value);
	return savepc;
}

/* Instructions */
inline void cpu_adc(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);

	int saveflags = (P & 0x01);
	int sum = ((char)A) + ((char)value) + saveflags;
	if ((sum > 0x7f) || (sum < -0x80))
		P |= 0x40;
	else
		P &= 0xbf;
	sum = A + value + saveflags;
	if (sum > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	A = sum;
	if (P & 0x08)
	{
		P &= 0xfe;
		if ((A & 0x0f) > 0x09)
			A += 0x06;
		if ((A & 0xf0) > 0x90)
		{
			A += 0x60;
			P |= 0x01;
		}
	}
	else
	{
		cpu_clockticks++;
	}
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_and(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);

	A &= value;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_asl(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);
	P = (P & 0xfe) | ((value >> 7) & 0x01);
	value = value << 1;
	cpu_putmemory(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_asla(CpuAdrMode adrmode)
{
	P = (P & 0xfe) | ((A >> 7) & 0x01);
	A = A << 1;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_bcc(CpuAdrMode adrmode)
{
	if ((P & 0x01) == 0)
	{
		uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
		PC += savepc;
		cpu_clockticks++;
	}
	else
	{
		// uint8_t value = cpu_getmemory(PC);
		PC++;
	}
}

inline void cpu_bcs(CpuAdrMode adrmode)
{
	if (P & 0x01)
	{
		uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
		PC += savepc;
		cpu_clockticks++;
	}
	else
	{
		// uint8_t value = cpu_getmemory(PC);
		PC++;
	}
}

inline void cpu_beq(CpuAdrMode adrmode)
{
	if (P & 0x02)
	{
		uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
		PC += savepc;
		cpu_clockticks++;
	}
	else
	{
		// uint8_t value = cpu_getmemory(PC);
		PC++;
	}
}

inline void cpu_bit(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);

	/* non-destrucive logically And between value and the accumulator
	 * and set zero flag */
	if (value & A)
		P &= 0xfd;
	else
		P |= 0x02;

	/* set negative and overflow flags from value */
	P = (P & 0x3f) | (value & 0xc0);
}

inline void cpu_bmi(CpuAdrMode adrmode)
{
	if (P & 0x80)
	{
		uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
		PC += savepc;
		cpu_clockticks++;
	}
	else
	{
		// uint8_t value = cpu_getmemory(PC);
		PC++;
	}
}

inline void cpu_bne(CpuAdrMode adrmode)
{
	if ((P & 0x02) == 0)
	{
		uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
		PC += savepc;
		cpu_clockticks++;
	}
	else
	{
		// uint8_t value = cpu_getmemory(PC);
		PC++;
	}
}

inline void cpu_bpl(CpuAdrMode adrmode)
{
	if ((P & 0x80) == 0)
	{
		uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
		PC += savepc;
		cpu_clockticks++;
	}
	else
	{
		// uint8_t value = cpu_getmemory(PC);
		PC++;
	}
}

inline void cpu_brk(CpuAdrMode adrmode)
{
	PC++;

	// cpu_putmemory(0x0100+S--,(uint8_t)(PC>>8));
	// cpu_putmemory(0x0100+S--,(uint8_t)(PC & 0xff));
	// cpu_putmemory(0x0100+S--,P);
	cpu_push16stack(PC);
	cpu_pushstack(P);

	P |= 0x14;

	// PC = cpu_getmemory16(0xfffe);
	PC = IRQ_VECTOR;
	// PC=cpu_getmemory(0xfffe);
	// PC |= cpu_getmemory(0xffff) << 8;
}

inline void cpu_bvc(CpuAdrMode adrmode)
{
	if ((P & 0x40) == 0)
	{
		uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
		PC += savepc;
		cpu_clockticks++;
	}
	else
	{
		// uint8_t value = cpu_getmemory(PC);
		PC++;
	}
}

inline void cpu_bvs(CpuAdrMode adrmode)
{
	if (P & 0x40)
	{
		uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
		PC += savepc;
		cpu_clockticks++;
	}
	else
	{
		// uint8_t value = cpu_getmemory(PC);
		PC++;
	}
}

inline void cpu_clc(CpuAdrMode adrmode)
{
	P &= 0xfe;
}

inline void cpu_cld(CpuAdrMode adrmode)
{
	P &= 0xf7;
}

inline void cpu_cli(CpuAdrMode adrmode)
{
	P &= 0xfb;
}

inline void cpu_clv(CpuAdrMode adrmode)
{
	P &= 0xbf;
}

inline void cpu_cmp(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);
	if (A + 0x100 - value > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	value = A + 0x100 - value;
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_cpx(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);
	if (X + 0x100 - value > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	value = X + 0x100 - value;
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_cpy(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);
	if (Y + 0x100 - value > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	value = Y + 0x100 - value;
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_dec(CpuAdrMode adrmode)
{
	uint8_t temp;

	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	temp = cpu_getmemory(savepc);
	temp--;
	cpu_putmemory(savepc, temp);

	uint8_t value = cpu_getmemory(savepc);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_dex(CpuAdrMode adrmode)
{
	X--;
	if (X)
		P &= 0xfd;
	else
		P |= 0x02;
	if (X & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_dey(CpuAdrMode adrmode)
{
	Y--;
	if (Y)
		P &= 0xfd;
	else
		P |= 0x02;
	if (Y & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_eor(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	A ^= cpu_getmemory(savepc);
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_inc(CpuAdrMode adrmode)
{
	uint8_t temp;

	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	temp = cpu_getmemory(savepc);
	temp++;
	cpu_putmemory(savepc, temp);

	uint8_t value = cpu_getmemory(savepc);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_inx(CpuAdrMode adrmode)
{
	X++;
	if (X)
		P &= 0xfd;
	else
		P |= 0x02;
	if (X & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_iny(CpuAdrMode adrmode)
{
	Y++;
	if (Y)
		P &= 0xfd;
	else
		P |= 0x02;
	if (Y & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_jmp(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	PC = savepc;
}

inline void cpu_jsr(CpuAdrMode adrmode)
{
	PC++;
	// cpu_putmemory(0x0100+S--,(uint8_t)(PC >> 8));
	// cpu_putmemory(0x0100+S--,(uint8_t)(PC & 0xff));
	cpu_push16stack(PC);
	PC--;
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	PC = savepc;
}

inline void cpu_lda(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	A = cpu_getmemory(savepc);
	// set the zero flag
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	// set the negative flag
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_ldx(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	X = cpu_getmemory(savepc);
	if (X)
		P &= 0xfd;
	else
		P |= 0x02;
	if (X & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_ldy(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	Y = cpu_getmemory(savepc);
	if (Y)
		P &= 0xfd;
	else
		P |= 0x02;
	if (Y & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_lsr(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);

	/* set carry flag if shifting right causes a bit to be lost */
	P = (P & 0xfe) | (value & 0x01);

	value = value >> 1;
	cpu_putmemory(savepc, value);

	/* set zero flag if value is zero */
	if (value != 0)
		P &= 0xfd;
	else
		P |= 0x02;

	/* set negative flag if bit 8 set??? can this happen on an LSR? */
	if ((value & 0x80) == 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_lsra(CpuAdrMode adrmode)
{
	P = (P & 0xfe) | (A & 0x01);
	A = A >> 1;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_nop(CpuAdrMode adrmode)
{
}

inline void cpu_ora(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	A |= cpu_getmemory(savepc);

	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_pha(CpuAdrMode adrmode)
{
	cpu_pushstack(A);
}

inline void cpu_php(CpuAdrMode adrmode)
{
	cpu_pushstack(P);
}

inline void cpu_pla(CpuAdrMode adrmode)
{
	A = cpu_popstack();
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_plp(CpuAdrMode adrmode)
{
	P = cpu_popstack() | 0x20;
}

inline void cpu_rol(CpuAdrMode adrmode)
{
	int saveflags = (P & 0x01);
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);
	P = (P & 0xfe) | ((value >> 7) & 0x01);
	value = value << 1;
	value |= saveflags;
	cpu_putmemory(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_rola(CpuAdrMode adrmode)
{
	int saveflags = (P & 0x01);
	P = (P & 0xfe) | ((A >> 7) & 0x01);
	A = A << 1;
	A |= saveflags;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_ror(CpuAdrMode adrmode)
{
	int saveflags = (P & 0x01);
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc);

	P = (P & 0xfe) | (value & 0x01);
	value = value >> 1;
	if (saveflags)
		value |= 0x80;
	cpu_putmemory(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_rora(CpuAdrMode adrmode)
{
	int saveflags = (P & 0x01);
	P = (P & 0xfe) | (A & 0x01);
	A = A >> 1;
	if (saveflags)
		A |= 0x80;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_rti(CpuAdrMode adrmode)
{
	P = cpu_popstack();
	P |= 0x20;
	PC = cpu_pop16stack();
	// PC  = cpu_popstack();
	// PC |= (cpu_popstack() << 8);
}

inline void cpu_rts(CpuAdrMode adrmode)
{
	//      PC++;
	// PC  = cpu_popstack();
	// PC |= (cpu_popstack() << 8);
	PC = cpu_pop16stack();
	PC++;
}

inline void cpu_sbc(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	uint8_t value = cpu_getmemory(savepc) ^ 0xFF;

	int saveflags = (P & 0x01);
	int sum = ((char)A) + ((char)value) + (saveflags << 4);
	if ((sum > 0x7f) || (sum < -0x80))
		P |= 0x40;
	else
		P &= 0xbf;
	sum = A + value + saveflags;
	if (sum > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	A = sum;
	if (P & 0x08)
	{
		A -= 0x66;
		P &= 0xfe;
		if ((A & 0x0f) > 0x09)
			A += 0x06;
		if ((A & 0xf0) > 0x90)
		{
			A += 0x60;
			P |= 0x01;
		}
	}
	else
	{
		cpu_clockticks++;
	}
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_sec(CpuAdrMode adrmode)
{
	P |= 0x01;
}

inline void cpu_sed(CpuAdrMode adrmode)
{
	P |= 0x08;
}

inline void cpu_sei(CpuAdrMode adrmode)
{
	P |= 0x04;
}

inline void cpu_sta(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	cpu_putmemory(savepc, A);
}

inline void cpu_stx(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	cpu_putmemory(savepc, X);
}

inline void cpu_sty(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	cpu_putmemory(savepc, Y);
}

inline void cpu_tax(CpuAdrMode adrmode)
{
	X = A;
	if (X)
		P &= 0xfd;
	else
		P |= 0x02;
	if (X & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_tay(CpuAdrMode adrmode)
{
	Y = A;
	if (Y)
		P &= 0xfd;
	else
		P |= 0x02;
	if (Y & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_tsx(CpuAdrMode adrmode)
{
	X = S;
	if (X)
		P &= 0xfd;
	else
		P |= 0x02;
	if (X & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_txa(CpuAdrMode adrmode)
{
	A = X;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_txs(CpuAdrMode adrmode)
{
	S = X;
}

inline void cpu_tya(CpuAdrMode adrmode)
{
	A = Y;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_bra(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	PC += savepc;
	cpu_clockticks++;
}

inline void cpu_dea(CpuAdrMode adrmode)
{
	A--;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_ina(CpuAdrMode adrmode)
{
	A++;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_phx(CpuAdrMode adrmode)
{
	cpu_pushstack(X);
}

inline void cpu_plx(CpuAdrMode adrmode)
{
	X = cpu_popstack();

	if (X)
		P &= 0xfd;
	else
		P |= 0x02;
	if (X & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_phy(CpuAdrMode adrmode)
{
	cpu_pushstack(Y);
}

inline void cpu_ply(CpuAdrMode adrmode)
{
	Y = cpu_popstack();

	if (Y)
		P &= 0xfd;
	else
		P |= 0x02;
	if (Y & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_stz(CpuAdrMode adrmode)
{
	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	cpu_putmemory(savepc, 0);
}

inline void cpu_tsb(CpuAdrMode adrmode)
{
	uint8_t temp;

	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	temp = cpu_getmemory(savepc);
	temp |= A;
	cpu_putmemory(savepc, temp);

	if (cpu_getmemory(savepc))
		P &= 0xfd;
	else
		P |= 0x02;
}

inline void cpu_trb(CpuAdrMode adrmode)
{
	uint8_t temp;

	uint16_t savepc = /*opcodetable[opcode].*/ adrmode();
	temp = cpu_getmemory(savepc);
	temp &= (A ^ 0xFF);
	cpu_putmemory(savepc, temp);

	if (cpu_getmemory(savepc))
		P &= 0xfd;
	else
		P |= 0x02;
}

// Zero page indexing instructions
inline void cpu_tsbzp(CpuAdrMode x)
{
	uint8_t temp;

	uint16_t savepc = cpu_zp();
	temp = cpu_getzeropage(savepc);
	temp |= A;
	cpu_putzeropage(savepc, temp);

	if (cpu_getzeropage(savepc))
		P &= 0xfd;
	else
		P |= 0x02;
}
inline void cpu_orazp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	A |= cpu_getzeropage(savepc);

	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_aslzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);
	P = (P & 0xfe) | ((value >> 7) & 0x01);
	value = value << 1;
	cpu_putzeropage(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_trbzp(CpuAdrMode x)
{
	uint8_t temp;

	uint16_t savepc = cpu_zp();
	temp = cpu_getzeropage(savepc);
	temp &= (A ^ 0xFF);
	cpu_putzeropage(savepc, temp);

	if (cpu_getzeropage(savepc))
		P &= 0xfd;
	else
		P |= 0x02;
}
inline void cpu_orazpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	A |= cpu_getzeropage(savepc);

	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_aslzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc);
	P = (P & 0xfe) | ((value >> 7) & 0x01);
	value = value << 1;
	cpu_putzeropage(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_bitzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);

	/* non-destrucive logically And between value and the accumulator
	 * and set zero flag */
	if (value & A)
		P &= 0xfd;
	else
		P |= 0x02;

	/* set negative and overflow flags from value */
	P = (P & 0x3f) | (value & 0xc0);
}
inline void cpu_andzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);

	A &= value;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_rolzp(CpuAdrMode x)
{
	int saveflags = (P & 0x01);
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);
	P = (P & 0xfe) | ((value >> 7) & 0x01);
	value = value << 1;
	value |= saveflags;
	cpu_putzeropage(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_bitzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc);

	/* non-destrucive logically And between value and the accumulator
	 * and set zero flag */
	if (value & A)
		P &= 0xfd;
	else
		P |= 0x02;

	/* set negative and overflow flags from value */
	P = (P & 0x3f) | (value & 0xc0);
}
inline void cpu_andzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc);

	A &= value;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_rolzpx(CpuAdrMode x)
{
	int saveflags = (P & 0x01);
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc);
	P = (P & 0xfe) | ((value >> 7) & 0x01);
	value = value << 1;
	value |= saveflags;
	cpu_putzeropage(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_eorzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	A ^= cpu_getzeropage(savepc);
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_lsrzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);

	/* set carry flag if shifting right causes a bit to be lost */
	P = (P & 0xfe) | (value & 0x01);

	value = value >> 1;
	cpu_putzeropage(savepc, value);

	/* set zero flag if value is zero */
	if (value != 0)
		P &= 0xfd;
	else
		P |= 0x02;

	/* set negative flag if bit 8 set??? can this happen on an LSR? */
	if ((value & 0x80) == 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_eorzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	A ^= cpu_getzeropage(savepc);
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_lsrzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc);

	/* set carry flag if shifting right causes a bit to be lost */
	P = (P & 0xfe) | (value & 0x01);

	value = value >> 1;
	cpu_putzeropage(savepc, value);

	/* set zero flag if value is zero */
	if (value != 0)
		P &= 0xfd;
	else
		P |= 0x02;

	/* set negative flag if bit 8 set??? can this happen on an LSR? */
	if ((value & 0x80) == 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_stzzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	cpu_putzeropage(savepc, 0);
}
inline void cpu_adczp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);

	int saveflags = (P & 0x01);
	int sum = ((char)A) + ((char)value) + saveflags;
	if ((sum > 0x7f) || (sum < -0x80))
		P |= 0x40;
	else
		P &= 0xbf;
	sum = A + value + saveflags;
	if (sum > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	A = sum;
	if (P & 0x08)
	{
		P &= 0xfe;
		if ((A & 0x0f) > 0x09)
			A += 0x06;
		if ((A & 0xf0) > 0x90)
		{
			A += 0x60;
			P |= 0x01;
		}
	}
	else
	{
		cpu_clockticks++;
	}
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_rorzp(CpuAdrMode x)
{
	int saveflags = (P & 0x01);
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);

	P = (P & 0xfe) | (value & 0x01);
	value = value >> 1;
	if (saveflags)
		value |= 0x80;
	cpu_putzeropage(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_stzzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	cpu_putzeropage(savepc, 0);
}
inline void cpu_adczpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc);

	int saveflags = (P & 0x01);
	int sum = ((char)A) + ((char)value) + saveflags;
	if ((sum > 0x7f) || (sum < -0x80))
		P |= 0x40;
	else
		P &= 0xbf;
	sum = A + value + saveflags;
	if (sum > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	A = sum;
	if (P & 0x08)
	{
		P &= 0xfe;
		if ((A & 0x0f) > 0x09)
			A += 0x06;
		if ((A & 0xf0) > 0x90)
		{
			A += 0x60;
			P |= 0x01;
		}
	}
	else
	{
		cpu_clockticks++;
	}
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_rorzpx(CpuAdrMode x)
{
	int saveflags = (P & 0x01);
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc);

	P = (P & 0xfe) | (value & 0x01);
	value = value >> 1;
	if (saveflags)
		value |= 0x80;
	cpu_putzeropage(savepc, value);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_styzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	cpu_putzeropage(savepc, Y);
}
inline void cpu_stazp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	cpu_putzeropage(savepc, A);
}
inline void cpu_stxzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	cpu_putzeropage(savepc, X);
}
inline void cpu_styzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	cpu_putzeropage(savepc, Y);
}
inline void cpu_stazpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	cpu_putzeropage(savepc, A);
}
inline void cpu_stxzpy(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpy();
	cpu_putzeropage(savepc, X);
}
inline void cpu_ldyzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	Y = cpu_getzeropage(savepc);
	if (Y)
		P &= 0xfd;
	else
		P |= 0x02;
	if (Y & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_ldazp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	A = cpu_getzeropage(savepc);
	// set the zero flag
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	// set the negative flag
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_ldxzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	X = cpu_getzeropage(savepc);
	if (X)
		P &= 0xfd;
	else
		P |= 0x02;
	if (X & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_ldyzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	Y = cpu_getzeropage(savepc);
	if (Y)
		P &= 0xfd;
	else
		P |= 0x02;
	if (Y & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_ldazpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	A = cpu_getzeropage(savepc);
	// set the zero flag
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	// set the negative flag
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_ldxzpy(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpy();
	X = cpu_getzeropage(savepc);
	if (X)
		P &= 0xfd;
	else
		P |= 0x02;
	if (X & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_cpyzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);
	if (Y + 0x100 - value > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	value = Y + 0x100 - value;
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_cmpzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);
	if (A + 0x100 - value > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	value = A + 0x100 - value;
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_deczp(CpuAdrMode x)
{
	uint8_t temp;

	uint16_t savepc = cpu_zp();
	temp = cpu_getzeropage(savepc);
	temp--;
	cpu_putzeropage(savepc, temp);

	uint8_t value = cpu_getzeropage(savepc);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_cmpzpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc);
	if (A + 0x100 - value > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	value = A + 0x100 - value;
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_deczpx(CpuAdrMode x)
{
	uint8_t temp;

	uint16_t savepc = cpu_zpx();
	temp = cpu_getzeropage(savepc);
	temp--;
	cpu_putzeropage(savepc, temp);

	uint8_t value = cpu_getzeropage(savepc);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_cpxzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc);
	if (X + 0x100 - value > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	value = X + 0x100 - value;
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_sbczp(CpuAdrMode x)
{
	uint16_t savepc = cpu_zp();
	uint8_t value = cpu_getzeropage(savepc) ^ 0xFF;

	int saveflags = (P & 0x01);
	int sum = ((char)A) + ((char)value) + (saveflags << 4);
	if ((sum > 0x7f) || (sum < -0x80))
		P |= 0x40;
	else
		P &= 0xbf;
	sum = A + value + saveflags;
	if (sum > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	A = sum;
	if (P & 0x08)
	{
		A -= 0x66;
		P &= 0xfe;
		if ((A & 0x0f) > 0x09)
			A += 0x06;
		if ((A & 0xf0) > 0x90)
		{
			A += 0x60;
			P |= 0x01;
		}
	}
	else
	{
		cpu_clockticks++;
	}
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_inczp(CpuAdrMode x)
{
	uint8_t temp;

	uint16_t savepc = cpu_zp();
	temp = cpu_getzeropage(savepc);
	temp++;
	cpu_putzeropage(savepc, temp);

	uint8_t value = cpu_getzeropage(savepc);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_sbczpx(CpuAdrMode x)
{
	uint16_t savepc = cpu_zpx();
	uint8_t value = cpu_getzeropage(savepc) ^ 0xFF;

	int saveflags = (P & 0x01);
	int sum = ((char)A) + ((char)value) + (saveflags << 4);
	if ((sum > 0x7f) || (sum < -0x80))
		P |= 0x40;
	else
		P &= 0xbf;
	sum = A + value + saveflags;
	if (sum > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	A = sum;
	if (P & 0x08)
	{
		A -= 0x66;
		P &= 0xfe;
		if ((A & 0x0f) > 0x09)
			A += 0x06;
		if ((A & 0xf0) > 0x90)
		{
			A += 0x60;
			P |= 0x01;
		}
	}
	else
	{
		cpu_clockticks++;
	}
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}
inline void cpu_inczpx(CpuAdrMode x)
{
	uint8_t temp;

	uint16_t savepc = cpu_zpx();
	temp = cpu_getzeropage(savepc);
	temp++;
	cpu_putzeropage(savepc, temp);

	uint8_t value = cpu_getzeropage(savepc);
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_oraindzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_indzp();
	A |= cpu_getmemory(savepc);

	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_andindzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_indzp();
	uint8_t value = cpu_getmemory(savepc);

	A &= value;
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_eorindzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_indzp();
	A ^= cpu_getmemory(savepc);
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_adcindzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_indzp();
	uint8_t value = cpu_getmemory(savepc);

	int saveflags = (P & 0x01);
	int sum = ((char)A) + ((char)value) + saveflags;
	if ((sum > 0x7f) || (sum < -0x80))
		P |= 0x40;
	else
		P &= 0xbf;
	sum = A + value + saveflags;
	if (sum > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	A = sum;
	if (P & 0x08)
	{
		P &= 0xfe;
		if ((A & 0x0f) > 0x09)
			A += 0x06;
		if ((A & 0xf0) > 0x90)
		{
			A += 0x60;
			P |= 0x01;
		}
	}
	else
	{
		cpu_clockticks++;
	}
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_staindzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_indzp();
	cpu_putmemory(savepc, A);
}

inline void cpu_ldaindzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_indzp();
	A = cpu_getmemory(savepc);
	// set the zero flag
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	// set the negative flag
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_cmpindzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_indzp();
	uint8_t value = cpu_getmemory(savepc);
	if (A + 0x100 - value > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	value = A + 0x100 - value;
	if (value)
		P &= 0xfd;
	else
		P |= 0x02;
	if (value & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

inline void cpu_sbcindzp(CpuAdrMode x)
{
	uint16_t savepc = cpu_indzp();
	uint8_t value = cpu_getmemory(savepc) ^ 0xFF;

	int saveflags = (P & 0x01);
	int sum = ((char)A) + ((char)value) + (saveflags << 4);
	if ((sum > 0x7f) || (sum < -0x80))
		P |= 0x40;
	else
		P &= 0xbf;
	sum = A + value + saveflags;
	if (sum > 0xff)
		P |= 0x01;
	else
		P &= 0xfe;
	A = sum;
	if (P & 0x08)
	{
		A -= 0x66;
		P &= 0xfe;
		if ((A & 0x0f) > 0x09)
			A += 0x06;
		if ((A & 0xf0) > 0x90)
		{
			A += 0x60;
			P |= 0x01;
		}
	}
	else
	{
		cpu_clockticks++;
	}
	if (A)
		P &= 0xfd;
	else
		P |= 0x02;
	if (A & 0x80)
		P |= 0x80;
	else
		P &= 0x7f;
}

/* Reset CPU */
void cpu_reset(void)
{
	A = X = Y = 0;
	P = R_FLAG | Z_FLAG; // Z_FLAG | R_FLAG
	S = 0xff;

	PC = RST_VECTOR;

	NMI_Flag = CLR0;
	IRQ_Flag = CLR0;
}

/* Non maskerable interrupt */
void cpu_nmi(void)
{
	// cpu_putmemory(0x0100+S--,(uint8_t)(PC>>8));
	// cpu_putmemory(0x0100+S--,(uint8_t)(PC & 0xff));
	// cpu_putmemory(0x0100+S--,P);
	cpu_push16stack(PC);
	cpu_pushstack(P);

	P |= 0x04;

	// PC = cpu_getmemory16(0xfffa);
	PC = NMI_VECTOR;
	// PC=cpu_getmemory(0xfffa);
	// PC |= cpu_getmemory(0xfffb) << 8;
}

/* Maskerable Interrupt */
void cpu_irq(void)
{
	// cpu_putmemory(0x0100+S--,(uint8_t)(PC>>8));
	// cpu_putmemory(0x0100+S--,(uint8_t)(PC & 0xff));
	// cpu_putmemory(0x0100+S--,P);
	cpu_push16stack(PC);
	cpu_pushstack(P);

	P |= 0x04;

	// PC = cpu_getmemory16(0xfffe);
	PC = IRQ_VECTOR;
	// PC=cpu_getmemory(0xfffe);
	// PC |= cpu_getmemory(0xffff) << 8;
}

/* Execute Instruction */
void cpu_exec(int timerTicks)
{
	if (NMI_Flag == SET1)
	{
		cpu_nmi();
		NMI_Flag = CLR0;
	}
	/*else if(IRQ_Flag == SET1 && !(P & I_FLAG)){
		cpu_irq();
		IRQ_Flag = CLR0;
	}*/

	while (timerTicks > cpu_clockticks)
	{
		// opcode = cpu_getmemory(PC);
		// PC++;
		cpu_loadinstruction();

		// opcodetable[opcode].instruction();
		// cpu_clockticks += opcodetable[opcode].ticks;
		cpu_execinstruction(opcode);
	}
	cpu_clockticks -= timerTicks;
}

void cpu_initmem(const uint8_t *prg_rombank0ptr, uint8_t rom_num)
{
	prg_rombank0 = prg_rombank0ptr;							   /* prg-rom lower bank*/
	prg_rombank1 = prg_rombank0ptr + (0x4000 * (rom_num - 1)); /* prg-rom upper bank*/

	cpu_clockticks = 0;
}
