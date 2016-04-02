#include "gic.h"

/*
 *  CPU Interface 関連
 */

/*
 *  CPU Interface の初期化
 */
void
gicc_init(void)
{
	/* CPUインタフェースを無効に */
	*(volatile uint32_t *)ASP_GICC_ICCICR = 0;

	/* 最低優先度に設定 */
	gicc_set_priority(GIC_MIN_PRIORITY);

	/* 割込み優先度の全ビット有効に */
	gicc_set_bp(0);

	/* ペンディングしている可能性があるので，EOI によりクリア */
	*(volatile uint32_t *)ASP_GICC_ICCEOIR = *(volatile uint32_t *)ASP_GICC_ICCIAR;

	/* CPUインタフェースを有効に */
	*(volatile uint32_t *)ASP_GICC_ICCICR = (GICC_CTLR_ENABLEGRP1|GICC_CTLR_ENABLEGRP0);
}

/*
 *  CPU Interface の終了
 */
void
gicc_stop(void)
{
	*(volatile uint32_t *)(ASP_GICC_ICCICR) = 0;
}

/*
 *  Distoributor 関連
 */
static const uint32_t intc_icdicfrn_table[] =	 /* Initial value table of Interrupt Configuration Registers */
{						  /*		   Interrupt ID */
	0xAAAAAAAA,			/* ICDICFR0  :  15 to   0 */
	0x00000055,			/* ICDICFR1  :  19 to  16 */
	0xFFFD5555,			/* ICDICFR2  :  47 to  32 */
	0x555FFFFF,			/* ICDICFR3  :  63 to  48 */
	0x55555555,			/* ICDICFR4  :  79 to  64 */
	0x55555555,			/* ICDICFR5  :  95 to  80 */
	0x55555555,			/* ICDICFR6  : 111 to  96 */
	0x55555555,			/* ICDICFR7  : 127 to 112 */
	0x5555F555,			/* ICDICFR8  : 143 to 128 */
	0x55555555,			/* ICDICFR9  : 159 to 144 */
	0x55555555,			/* ICDICFR10 : 175 to 160 */
	0xF5555555,			/* ICDICFR11 : 191 to 176 */
	0xF555F555,			/* ICDICFR12 : 207 to 192 */
	0x5555F555,			/* ICDICFR13 : 223 to 208 */
	0x55555555,			/* ICDICFR14 : 239 to 224 */
	0x55555555,			/* ICDICFR15 : 255 to 240 */
	0x55555555,			/* ICDICFR16 : 271 to 256 */
	0xFD555555,			/* ICDICFR17 : 287 to 272 */
	0x55555557,			/* ICDICFR18 : 303 to 288 */
	0x55555555,			/* ICDICFR19 : 319 to 304 */
	0x55555555,			/* ICDICFR20 : 335 to 320 */
	0x5F555555,			/* ICDICFR21 : 351 to 336 */
	0xFD55555F,			/* ICDICFR22 : 367 to 352 */
	0x55555557,			/* ICDICFR23 : 383 to 368 */
	0x55555555,			/* ICDICFR24 : 399 to 384 */
	0x55555555,			/* ICDICFR25 : 415 to 400 */
	0x55555555,			/* ICDICFR26 : 431 to 416 */
	0x55555555,			/* ICDICFR27 : 447 to 432 */
	0x55555555,			/* ICDICFR28 : 463 to 448 */
	0x55555555,			/* ICDICFR29 : 479 to 464 */
	0x55555555,			/* ICDICFR30 : 495 to 480 */
	0x55555555,			/* ICDICFR31 : 511 to 496 */
	0x55555555,			/* ICDICFR32 : 527 to 512 */
	0x55555555,			/* ICDICFR33 : 543 to 528 */
	0x55555555,			/* ICDICFR34 : 559 to 544 */
	0x55555555,			/* ICDICFR35 : 575 to 560 */
	0x00155555			 /* ICDICFR36 : 586 to 576 */
};

/*
 *  割込み禁止
 */
void
gicd_disable_int(uint8_t id)
{
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	*(volatile uint32_t *)(ASP_GICD_ICDICER + offset_addr) = (1 << offset_bit);
}

/*
 *  割込み許可
 */
void
gicd_enable_int(uint8_t id)
{
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	*(volatile uint32_t *)(ASP_GICD_ICDISER + offset_addr)  = (1 << offset_bit);
}

/*
 *  割込みペンディングクリア
 */
void
gicd_clear_pending(uint8_t id)
{
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	*(volatile uint32_t *)(ASP_GICD_ICDICPR + offset_addr) = (1 << offset_bit);
}

/*
 *  割込みペンディングセット
 */
void
gicd_set_pending(uint8_t id)
{
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	*(volatile uint32_t *)(ASP_GICD_ICDISPR + offset_addr) = (1 << offset_bit);
}

/*
 *  割込み要求のチェック
 */
bool_t
gicd_probe_int(uint8_t id)
{
	uint32_t state;
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	state = *(volatile uint32_t *)(ASP_GICD_ICDISPR + offset_addr);

	if ((state & (1 << offset_bit)) == (1 << offset_bit)) {
		return(true);
	}
	else {
		return(false);
	}
}

/*
 *  割込みコンフィギュレーション設定
 */
void
gicd_config(uint8_t id,  bool_t is_edge, bool_t is_1_n)
{
	uint16_t offset_addr;
	uint16_t offset_bit;
	uint32_t cfgr_reg_val;
	uint8_t  config;

	if (id < 416) return;       /* ID 0～415は設定変更禁止。変更可能なのは端子割り込みのみ。 */

	if (is_edge) {
		config = GICD_ICFGRn_EDGE;
	}
	else {
		config = GICD_ICFGRn_LEVEL;
	}

	if (is_1_n) {
		config |= GICD_ICFGRn_1_N;
	}
	else {
		config |= GICD_ICFGRn_N_N;
	}
	
	
	offset_addr = (id / 16) * 4;
	offset_bit  = (id % 16) * 2;

	cfgr_reg_val  = *(volatile uint32_t *)(ASP_GICD_ICDICFR + offset_addr);
	cfgr_reg_val &= ~(0x03U << offset_bit);
	cfgr_reg_val |= (0x03U & config) << offset_bit;
	*(volatile uint32_t *)(ASP_GICD_ICDICFR + offset_addr) = cfgr_reg_val;
}

/*
 *  割込み優先度のセット
 *  内部表現で渡す．
 */
void
gicd_set_priority(uint8_t id, int pri)
{
	uint16_t offset_addr;
	uint16_t offset_bit;
	uint32_t pr_reg_val;

	offset_addr = (id / 4) * 4;
	offset_bit  = ((id % 4) * 8) + GICC_PMR_OFFSET;

	pr_reg_val  = *(volatile uint32_t *)(ASP_GICD_ICDIPR + offset_addr);
	pr_reg_val &= ~(GICC_PMR_MASK << offset_bit);
	pr_reg_val |= (GICC_PMR_MASK & pri) << (offset_bit);
	*(volatile uint32_t *)(ASP_GICD_ICDIPR + offset_addr) = pr_reg_val;
}

/*
 *  DIC割込みターゲットの設定
 *  cpusはターゲットとするCPUのビットパターンで指定
 *   CPU0 : 0x01
 *   CPU1 : 0x02
 *   CPU2 : 0x04
 *   CPU3 : 0x08
 */
void
gicd_set_target(uint8_t id, uint8_t cpus)
{
	uint32_t offset_addr;
	uint32_t offset_bit;
	uint32_t itr_reg_val;

	offset_addr = (id / 4) * 4;
	offset_bit  = (id % 4) * 8;

	itr_reg_val  = *(volatile uint32_t *)(ASP_GICD_ICDIPTR + offset_addr);
	itr_reg_val &= ~(0xf << offset_bit);
	itr_reg_val |= (cpus << offset_bit);
	*(volatile uint32_t *)(ASP_GICD_ICDIPTR + offset_addr) = itr_reg_val;
}

/*
 *  Distributor 初期化
 */
void
gicd_init(void)
{
	int i;

	/* Distributor を無効に */
	*(volatile uint32_t *)(ASP_GICD_ICDDCR) = 0;

	/* モード初期化(1-N Level) */
	for(i = 0; i < ASP_NUM_ICDICFR_ENTRY; i++){
		*(volatile uint32_t *)(ASP_GICD_ICDICFR + (4 * i)) = intc_icdicfrn_table[i];
	}

	/* 優先度最低に設定  */
	for(i = 0; i < ASP_NUM_ICDIPR_ENTRY; i++){
		*(volatile uint32_t *)(ASP_GICD_ICDIPR + (4 * i)) = 0xffffffff;
	}

	/* ターゲット初期化（全てCPU0へ） */
	for(i = ASP_NUM_ICDIPTR_ENTRY; i < GIC_TNUM_INT/4; i++){
		*(volatile uint32_t *)(ASP_GICD_ICDIPTR + (4 * i)) = 0x01010101;
	}

	/* 割込みを全て禁止 */
	for(i = 0; i < ASP_NUM_ICDICER_ENTRY; i++){
		*(volatile uint32_t *)(ASP_GICD_ICDICER + (4 * i)) = 0xffffffff;
	}

	/* ペンディングをクリア */
	for(i = 0; i < ASP_NUM_ICDICPR_ENTRY; i++){
		*(volatile uint32_t *)(ASP_GICD_ICDICPR + (4 * i)) = 0xffffffff;
	}

	/* Distibutor を有効に */
	*(volatile uint32_t *)(ASP_GICD_ICDDCR) = GICD_CTLR_ENABLE;
}

/*
 *  Distributor 終了
 */
void
gicd_stop(void)
{
	/* Distributor を無効に */
	*(volatile uint32_t *)(ASP_GICD_ICDDCR) = 0;
}
