/*
 *  RZ-A1 のハードウェア資源の定義
 */

#ifndef TOPPERS_RZA1_H
#define TOPPERS_RZA1_H

/*
 *  割込みの個数
 */
#define GIC_TMAX_INTNO    586U
#define GIC_TNUM_INT      587U

/*
 *  割込みコントローラのベースアドレス
 */
#define GICC_BASE           0xE8202000
#define GICD_BASE           0xE8201000

/*
 *	UARTのベースアドレス
 */
#define UART1_BASE      0xE8007000
#define UART2_BASE      0xE8007800
#define UART3_BASE      0xE8008000
#define UART4_BASE      0xE8008800
#define UART5_BASE      0xE8009000
#define UART6_BASE      0xE8009800
#define UART7_BASE      0xE800A000
#define UART8_BASE      0xE800A800

/*
 *	割込み番号
 */
#define TOPPERS_INTID_IRQ5               37     /* 割り込みID(IRQ5) */
#define TOPPERS_INTID_OSTM0             134     /* 割り込みID(OSTM0) */
#define TOPPERS_INTID_OSTM1             135     /* 割り込みID(OSTM1) */
#define TOPPERS_INTID_SCIF_BRI_1        221     /* 割り込みID(BRI0) */
#define TOPPERS_INTID_SCIF_ERI_1        222     /* 割り込みID(ERI0) */
#define TOPPERS_INTID_SCIF_RXI_1        223     /* 割り込みID(RXI0) */
#define TOPPERS_INTID_SCIF_TXI_1        224     /* 割り込みID(TXI0) */
#define TOPPERS_INTID_SCIF_BRI_2        225     /* 割り込みID(BRI1) */
#define TOPPERS_INTID_SCIF_ERI_2        226     /* 割り込みID(ERI1) */
#define TOPPERS_INTID_SCIF_RXI_2        227     /* 割り込みID(RXI1) */
#define TOPPERS_INTID_SCIF_TXI_2        228     /* 割り込みID(TXI1) */
#define TOPPERS_INTID_SCIF_BRI_3        229     /* 割り込みID(BRI2) */
#define TOPPERS_INTID_SCIF_ERI_3        230     /* 割り込みID(ERI2) */
#define TOPPERS_INTID_SCIF_RXI_3        231     /* 割り込みID(RXI2) */
#define TOPPERS_INTID_SCIF_TXI_3        232     /* 割り込みID(TXI2) */
#define TOPPERS_INTID_SCIF_BRI_4        233     /* 割り込みID(BRI3) */
#define TOPPERS_INTID_SCIF_ERI_4        234     /* 割り込みID(ERI3) */
#define TOPPERS_INTID_SCIF_RXI_4        235     /* 割り込みID(RXI3) */
#define TOPPERS_INTID_SCIF_TXI_4        236     /* 割り込みID(TXI3) */
#define TOPPERS_INTID_SCIF_BRI_5        237     /* 割り込みID(BRI4) */
#define TOPPERS_INTID_SCIF_ERI_5        238     /* 割り込みID(ERI4) */
#define TOPPERS_INTID_SCIF_RXI_5        239     /* 割り込みID(RXI4) */
#define TOPPERS_INTID_SCIF_TXI_5        240     /* 割り込みID(TXI4) */
#define TOPPERS_INTID_SCIF_BRI_6        241     /* 割り込みID(BRI5) */
#define TOPPERS_INTID_SCIF_ERI_6        242     /* 割り込みID(ERI5) */
#define TOPPERS_INTID_SCIF_RXI_6        243     /* 割り込みID(RXI5) */
#define TOPPERS_INTID_SCIF_TXI_6        244     /* 割り込みID(TXI5) */
#define TOPPERS_INTID_SCIF_BRI_7        245     /* 割り込みID(BRI6) */
#define TOPPERS_INTID_SCIF_ERI_7        246     /* 割り込みID(ERI6) */
#define TOPPERS_INTID_SCIF_RXI_7        247     /* 割り込みID(RXI6) */
#define TOPPERS_INTID_SCIF_TXI_7        248     /* 割り込みID(TXI6) */
#define TOPPERS_INTID_SCIF_BRI_8        249     /* 割り込みID(BRI7) */
#define TOPPERS_INTID_SCIF_ERI_8        250     /* 割り込みID(ERI7) */
#define TOPPERS_INTID_SCIF_RXI_8        251     /* 割り込みID(RXI7) */
#define TOPPERS_INTID_SCIF_TXI_8        252     /* 割り込みID(TXI7) */

/*
 *  割込み関連のレジスタ
 */
#define ICR0       0xFCFEF800
#define ICR1       0xFCFEF802
#define IRQRR      0xFCFEF804

/*
 *	GICに関する設定
 */
#define GIC_PRI_LEVEL		  32 //優先度の個数,256,128,64,32,16

/*
 *  割込み優先度に関する設定
 */
#define TMAX_INTPRI   0                    /* 割込み優先度の最小値（最低値）*/
#define TMIN_INTPRI   (GIC_PRI_LEVEL-1)    /* 割込み優先度の最大値（最高値）*/

/* Clock Pulse Generator */
#define ASP_CPG_BASE            0xFCFE0000
#define ASP_FRQCR               (ASP_CPG_BASE + 0x0010)
#define ASP_FRQCR2              (ASP_CPG_BASE + 0x0014)

/* 汎用入出力ポート */
#define ASP_PORT_BASE           0xFCFE3000
#define ASP_PORT_P_OFFSET       0x0000
#define ASP_PORT_PSR_OFFSET     0x0200
#define ASP_PORT_PPR_OFFSET     0x0200
#define ASP_PORT_PM_OFFSET      0x0300
#define ASP_PORT_PMC_OFFSET     0x0400
#define ASP_PORT_PFC_OFFSET     0x0500
#define ASP_PORT_PFCE_OFFSET    0x0600
#define ASP_PORT_PFCAE_OFFSET   0x0A00
#define ASP_PORT_PIBC_OFFSET    0x4000
#define ASP_PORT_PBDC_OFFSET    0x4100
#define ASP_PORT_PIPC_OFFSET    0x4200
#define ASP_PORT_P(n)           (ASP_PORT_BASE + ASP_PORT_P_OFFSET     + (n) * 4)
#define ASP_PORT_PSR(n)         (ASP_PORT_BASE + ASP_PORT_PSR_OFFSET   + (n) * 4)
#define ASP_PORT_PPR(n)         (ASP_PORT_BASE + ASP_PORT_PPR_OFFSET   + (n) * 4)
#define ASP_PORT_PM(n)          (ASP_PORT_BASE + ASP_PORT_PM_OFFSET    + (n) * 4)
#define ASP_PORT_PMC(n)         (ASP_PORT_BASE + ASP_PORT_PMC_OFFSET   + (n) * 4)
#define ASP_PORT_PFC(n)         (ASP_PORT_BASE + ASP_PORT_PFC_OFFSET   + (n) * 4)
#define ASP_PORT_PFCE(n)        (ASP_PORT_BASE + ASP_PORT_PFCE_OFFSET  + (n) * 4)
#define ASP_PORT_PFCAE(n)       (ASP_PORT_BASE + ASP_PORT_PFCAE_OFFSET + (n) * 4)
#define ASP_PORT_PIBC(n)        (ASP_PORT_BASE + ASP_PORT_PIBC_OFFSET  + (n) * 4)
#define ASP_PORT_PBDC(n)        (ASP_PORT_BASE + ASP_PORT_PBDC_OFFSET  + (n) * 4)
#define ASP_PORT_PIPC(n)        (ASP_PORT_BASE + ASP_PORT_PIPC_OFFSET  + (n) * 4)

/* 低消費電力モード */
#define ASP_LOWPWR_BASE         0xFCFE0000
#define ASP_STBCR1              (ASP_LOWPWR_BASE + 0x0020)
#define ASP_STBCR2              (ASP_LOWPWR_BASE + 0x0024)
#define ASP_STBCR3              (ASP_LOWPWR_BASE + 0x0420)
#define ASP_STBCR4              (ASP_LOWPWR_BASE + 0x0424)
#define ASP_STBCR5              (ASP_LOWPWR_BASE + 0x0428)
#define ASP_STBCR6              (ASP_LOWPWR_BASE + 0x042C)
#define ASP_STBCR7              (ASP_LOWPWR_BASE + 0x0430)
#define ASP_STBCR8              (ASP_LOWPWR_BASE + 0x0434)
#define ASP_STBCR9              (ASP_LOWPWR_BASE + 0x0438)
#define ASP_STBCR10             (ASP_LOWPWR_BASE + 0x043C)
#define ASP_STBCR11             (ASP_LOWPWR_BASE + 0x0440)
#define ASP_STBCR12             (ASP_LOWPWR_BASE + 0x0444)
#define ASP_STBCR13             (ASP_LOWPWR_BASE + 0x0470)
#define ASP_SYSCR3              (ASP_LOWPWR_BASE + 0x0408)
#define ASP_CPUSTS              (ASP_LOWPWR_BASE + 0x0018)

/* PL310 L2 cache controller */
#define ASP_PL310_BASE          0x3FFFF000
#define ASP_PL310_POWER_CTRL    (ASP_PL310_BASE + 0xF80)

#endif /* TOPPERS_RZA1_H */
