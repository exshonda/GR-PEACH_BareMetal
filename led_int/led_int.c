#include "arm.h"
#include "rza1.h"
#include "gic.h"

#define LED_RED_BIT    (13)
#define LED_RED_MASK   (1 << LED_RED_BIT)

#define LED_USER_BIT    (12)
#define LED_USER_MASK   (1 << LED_USER_BIT)

#define PUSH_BIT    (0)
#define PUSH_MASK   (1 << PUSH_BIT)

void
init_led(void){
	/* RED */
	*(volatile uint16_t*)(ASP_PORT_PIBC(6)) &= ~LED_RED_MASK;
	*(volatile uint16_t*)(ASP_PORT_PBDC(6)) &= ~LED_RED_MASK;
	*(volatile uint16_t*)(ASP_PORT_PMC(6))  &= ~LED_RED_MASK;
	*(volatile uint16_t*)(ASP_PORT_PM(6))   &= ~LED_RED_MASK;
	*(volatile uint16_t*)(ASP_PORT_P(6))    &= ~LED_RED_MASK;

	/* USER */
	*(volatile uint16_t*)(ASP_PORT_PIBC(6)) &= ~LED_USER_MASK;
	*(volatile uint16_t*)(ASP_PORT_PBDC(6)) &= ~LED_USER_MASK;
	*(volatile uint16_t*)(ASP_PORT_PMC(6))  &= ~LED_USER_MASK;
	*(volatile uint16_t*)(ASP_PORT_PM(6))   &= ~LED_USER_MASK;
	*(volatile uint16_t*)(ASP_PORT_P(6))    &= ~LED_USER_MASK;
}

void
set_led_red(uint8_t on){
	if(on == 1){
		*(volatile uint16_t*)(ASP_PORT_P(6)) |= LED_RED_MASK;
	}else {
		*(volatile uint16_t*)(ASP_PORT_P(6)) &= ~LED_RED_MASK;
	}
}

void
set_led_user(uint8_t on){
	if(on == 1){
		*(volatile uint16_t*)(ASP_PORT_P(6)) |= LED_USER_MASK;
	}else {
		*(volatile uint16_t*)(ASP_PORT_P(6)) &= ~LED_USER_MASK;
	}
}

void
init_push(void){
	*(volatile uint16_t*)(ASP_PORT_PIBC(6)) |=  PUSH_MASK;
	*(volatile uint16_t*)(ASP_PORT_PBDC(6)) &= ~PUSH_MASK;

	*(volatile uint16_t*)(ASP_PORT_PMC(6))   |=  PUSH_MASK;
	*(volatile uint16_t*)(ASP_PORT_PFCAE(6)) |=  PUSH_MASK;
	*(volatile uint16_t*)(ASP_PORT_PFCE(6))  &= ~PUSH_MASK;
	*(volatile uint16_t*)(ASP_PORT_PFC(6))   |=  PUSH_MASK;

	*(volatile uint16_t*)(ASP_PORT_PMC(6))  |=  PUSH_MASK;
	*(volatile uint16_t*)(ASP_PORT_PM(6))   |=  PUSH_MASK;
}

void
init_int(void){
	set_high_vector(false);

	gicd_init();
	gicc_init();
	gicc_set_priority(TMIN_INTPRI);
	enable_irq();

	extern void *vector_table;
	CP15_SET_VBAR((uint32_t) &vector_table);
}

void
init_int_push(void){
	gicd_set_priority(TOPPERS_INTID_IRQ5, 10);
	gicd_set_target(TOPPERS_INTID_IRQ5, 1);
	gicd_config(TOPPERS_INTID_IRQ5, false, true);
	gicd_clear_pending(TOPPERS_INTID_IRQ5);
	gicd_enable_int(TOPPERS_INTID_IRQ5);
	*(volatile uint16_t*)(ICR1) = (0x03 << 10);
}

uint8_t
get_push(void){
	if((*(volatile uint16_t*)(ASP_PORT_PPR(6))  & PUSH_MASK) == 0){
		return 1;
	}
	else {
		return 0;
	}
}

#define LOOP_CNT 2000000

void
busy_loop(void){
	volatile int i;
	for(i = 0; i < LOOP_CNT; i++);
}

void
irq_handler_c(void) {
	uint32_t icciar;
	icciar = gicc_get_icciar();
	set_led_user(get_push());
	gicd_clear_pending(TOPPERS_INTID_IRQ5);
	gicc_end_int(icciar);
	*(volatile uint16_t*)IRQRR =  0x00;
}

void
arm_main(void) {

	init_led();
	init_push();
	init_int();
	init_int_push();

	while(1){
		set_led_red(1);
		busy_loop();
		set_led_red(0);
		busy_loop();
	}
}
