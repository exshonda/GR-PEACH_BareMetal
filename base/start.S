#define MACRO_ONLY

#include "arm.h"

/*
 *  例外ベクタ
 */
	.section vector
	.global vector_table
vector_table:
	ldr pc, reset_vector       /* リセット             */
	ldr pc, undef_vector       /* 未定義命令           */
	ldr pc, svc_vector         /* ソフトウェア割込み   */
	ldr pc, prefetch_vector    /* プリフェッチアボート */
	ldr pc, data_abort_vector  /* データアボート       */
	ldr pc, reset_vector
	ldr pc, irq_vector         /* IRQ                  */
	ldr pc, fiq_vector         /* FIQ                  */

/*
 *  例外ベクタの命令から参照される
 *  ジャンプ先アドレス
 */
	.global vector_ref_tbl
vector_ref_tbl:
reset_vector:
	.long  start
undef_vector:
	.long  undef_handler
svc_vector:
	.long  svc_handler
prefetch_vector:
	.long  prefetch_handler
data_abort_vector:
	.long  data_abort_handler
irq_vector:
	.long  irq_handler
fiq_vector:
	.long  fiq_handler

	.text
	.align 2
	.global start
start:
	/*
	 * プロセッサモードの初期化
	 * スーパーバイザーモード，FIQ/IRQ禁止
	 */
	msr  cpsr_cxsf, #(CPSR_SVC | CPSR_FIQ_BIT | CPSR_IRQ_BIT) 

	/*
	 * スタックポインタと，フレームポインタの初期化
	 * sl(スタック上限レジスタr10)は設定する必要があるか?
	 * gccはディフォルトでは-mapcsなので必要ない
	 */
	ldr  sp, =_stack_init
	mov  fp, #0        /* ARMモード用フレームポインタ(r11) */         
	mov  r7, #0        /* Thumbモード用フレームポインタ    */

	/*
	 * IRQモード用のスタックの初期化
	 */
	msr  cpsr_cxsf, #(CPSR_IRQ | CPSR_FIQ_BIT | CPSR_IRQ_BIT) 
	ldr  sp, =_irq_stack_init
	msr  cpsr_cxsf, #(CPSR_SVC | CPSR_FIQ_BIT | CPSR_IRQ_BIT) 

	/*
	 * hardware_init_hookの呼び出し （0でない場合）
	 *
	 * ターゲットハードウェアに依存して必要な初期化処理がある場合
	 * は，hardware_init_hook という関数を用意する
	 */
	ldr   r0, =hardware_init_hook
	cmp   r0, #0x00
	movne lr, pc
	bxne  r0

start_1:

	/*
	 *  bssセクションのクリア
	 */
	ldr  r1, =__bss_start
	ldr  r2, =__bss_end
	cmp  r1,r2
	bhs  start_3
	mov  r0,#0
start_2:
	str  r0,[r1],#4
	cmp  r1, r2
	blo  start_2

start_3:

	/*
	 *  dataセクションの初期化(ROM化対応)
	 *  __idata_start から __idata_end までを，__data_start 以降に
	 *  コピーする
	 */
	ldr  r1, =__idata_start
	ldr  r3, =__idata_end
	cmp  r1, r3
	bhs  start_5
	ldr  r2, =__data_start
start_4:
	ldr  r0, [r1],#4
	str  r0, [r2],#4
	cmp  r1, r3
	blo  start_4


	/*
	 *  software_init_hook を呼出し（0 でない場合）
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *  理がある場合は，software_init_hook という関数を用意すれば
	 *  よい．
	 */
start_5:
	ldr   r0, =software_init_hook
	cmp   r0, #0x00
	movne lr, pc
	bxne  r0

	/*
	 * main関数を実行
	 */
start_6:
	bl   arm_main

	.global undef_handler
undef_handler:
	b undef_handler

	.global svc_handler
svc_handler:
	b svc_handler

	.global prefetch_handler
prefetch_handler:
	b prefetch_handler

	.global data_abort_handler
data_abort_handler:
	b data_abort_handler

	.global fiq_handler
fiq_handler:
	b fiq_handler

	.global irq_handler
irq_handler:
	sub   lr, lr, #4 /* 戻り先アドレス  */

	/* 
	 * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
	 * 保存．
	 */
	srsdb #CPSR_SVC! 

	stmfd sp!, {r0-r3,ip,lr}
	bl    irq_handler_c
	ldmfd sp!, {r0-r3,ip,lr}

irq_handler1:
	msr  cpsr_cxsf, #(CPSR_SVC | CPSR_FIQ_BIT | CPSR_IRQ_BIT) 
	rfeia sp!
