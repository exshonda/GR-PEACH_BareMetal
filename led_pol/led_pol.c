#include "arm.h"
#include "rza1.h"

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
	*(volatile uint16_t*)(ASP_PORT_PMC(6))  &= ~PUSH_MASK;
	*(volatile uint16_t*)(ASP_PORT_PM(6))   |=  PUSH_MASK;
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
arm_main(void) {

	init_led();
	init_push();

	while(1){
		set_led_red(1);
		set_led_user(get_push());
		busy_loop();
		set_led_red(0);
		set_led_user(get_push());
		busy_loop();
	}
}

void
irq_handler_c(void){
	while(1);
}