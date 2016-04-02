#ifndef _ARM_H_
#define _ARM_H_

/*
 *  CPSR 割込み禁止ビット
 */
#define CPSR_INT_MASK 0xC0
#define CPSR_IRQ_BIT  0x80
#define CPSR_FIQ_BIT  0x40

/*
 *  CPSR のモードビット
 */
#define CPSR_MODE_MASK   0x1f
#define CPSR_USER        0x10
#define CPSR_FIQ         0x11
#define CPSR_IRQ         0x12
#define CPSR_SVC         0x13
#define CPSR_ABT         0x17
#define CPSR_UND         0x1B
#define CPSR_SYS         0x1F

#ifndef MACRO_ONLY
#define Asm		__asm__ volatile	/* インラインアセンブラ（最適化抑止）*/
#define Inline	static __inline__	/* インライン関数 */
typedef unsigned int		uint32_t;	/* 符号無し32ビット整数 */
typedef unsigned short		uint16_t;	/* 符号無し16ビット整数 */
typedef unsigned char		uint8_t;	/* 符号無し8ビット整数 */
typedef unsigned char		bool_t;		/* 論理値 */

#define false 0
#define true  1

/*
 *  コプロの操作
 */
#define CP15_SET_VBAR(x)         Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define CP15_CONTROL_READ(x)     Asm("mrc p15, 0, %0, c1, c0, 0":"=r"(x))
#define CP15_CONTROL_WRITE(x)    Asm("mcr p15, 0, %0, c1, c0, 0"::"r"(x))

/*
 *  コプロのビット定義
 */
#define CP15_CONTROL_XP_BIT   (1 << 23)
#define CP15_CONTROL_V_BIT    (1 << 13)
#define CP15_CONTROL_I_BIT    (1 << 12)
#define CP15_CONTROL_Z_BIT    (1 << 11)
#define CP15_CONTROL_C_BIT    (1 <<  2)
#define CP15_CONTROL_M_BIT    (1 <<  0)

Inline void
disable_irq(void)
{
	uint32_t cpsr;
	Asm("mrs  %0,CPSR" : "=r"(cpsr));
	cpsr |= (CPSR_IRQ_BIT);
	Asm("msr CPSR, %0" : : "r"(cpsr) :"memory", "cc");
}

Inline void
enable_irq(void)
{
	uint32_t cpsr;
	Asm("mrs  %0,CPSR" : "=r"(cpsr));
	cpsr &= ~(CPSR_IRQ_BIT);
	Asm("msr CPSR, %0" : : "r"(cpsr) :"memory", "cc");
}

/*
 *  High exception vector を使うかの設定
 */
Inline void
set_high_vector(bool_t enable)
{
	uint32_t control;

	CP15_CONTROL_READ(control);

	if (enable) {
		control |= CP15_CONTROL_V_BIT;
	}
	else {
		control &= ~CP15_CONTROL_V_BIT;
	}

	CP15_CONTROL_WRITE(control);
}
#endif /* MACRO_ONLY */

#endif /* _ARM_H_ */
