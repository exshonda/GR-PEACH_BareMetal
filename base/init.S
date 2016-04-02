#define MACRO_ONLY

#include "arm.h"
#include "rza1.h"

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリ初期化の前に呼び出される．
 */
	.text
	.align 2
	.global hardware_init_hook
hardware_init_hook:
	/*
	 *  SRAM書き込み許可
	 */
	ldr	r2, =(ASP_SYSCR3)
	ldr	r0, =0x0F
	strb	r0, [r2]		/* RRAM page0-3 write enable */
	ldrb	r0, [r2]		/* dummy read */

	/* 
	 * クロック関係の初期化
	 */
	/* L2$をスタンバイモードへ */
	ldr	r2, =(ASP_PL310_POWER_CTRL)
	ldr	r0, [r2]
	orr	r0, r0, #0x01     /* enable stand-by mode */
	str	r0, [r2]

	/* CPUクロック= *1 (400MHz) */
set_frqcr:
	ldr	r2, =(ASP_FRQCR)
	ldr	r0, =0x1035
	strh	r0, [r2]
	ldrh	r1, [r2]
	cmp	r0, r1
	bne	set_frqcr

	/* 
	 * 画像処理クロック= *2/3
	 * Clockin = 13.33MHz, CKIO = 66.67MHz
	 * I  Clock = 400.00MHz,                
	 * G  Clock = 266.67MHz, 
	 * B  Clock = 133.33MHz, 
	 * P1 Clock =  66.67MHz,      
	 *  P0 Clock =  33.33MHz  
	 */
set_frqcr2:
	ldr	r2, =(ASP_FRQCR2)
	ldr	r0, =0x0001
	strh	r0, [r2]
	ldrh	r1, [r2]
	cmp	r0, r1
	bne	set_frqcr2

wait_isbusy:
	ldr	r2, =(ASP_CPUSTS)
	ldrb	r0, [r2]
	tst	r0, #0x10
	bne	wait_isbusy

	ldr	r2, =(ASP_PL310_POWER_CTRL)
	ldr	r0, [r2]
	bfc	r0, #0, #1    /* disable stand-by mode */
	str	r0, [r2]

	bx	lr

#ifdef EXECUTE_ON_ROM  
    ldr     r0, sf_boot
loop:
    b       loop

sf_boot: 
    .word   boot_loader
#endif /* EXECUTE_ON_ROM   */
