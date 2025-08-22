#include "stm32f4xx.h"
void delayMs(int n);
int main(void) {
__disable_irq(); /* global disable IRQs */
RCC->AHB1ENR |= 4; /* enable GPIOC clock */ RCC->AHB1ENR |= 1; /*
enable GPIOA clock */ RCC->APB2ENR |= 0x4000; /* enable SYSCFG clock
*/
/* configure PA5 for LED */
GPIOA->MODER &= ~0x00000C00; /* clear pin mode */ GPIOA->MODER |=
0x00000400; /* set pin to output mode */
/* configure PC13 for push button interrupt */
GPIOC->MODER &= ~0x0C000000; /* clear pin mode to input mode */
SYSCFG->EXTICR[3] &= ~0x00F0; /* clear port selection for EXTI13 */
SYSCFG->EXTICR[3] |= 0x0020; /* select port C for EXTI13 */
EXTI->IMR |= 0x2000; /* unmask EXTI13 */
EXTI->FTSR |= 0x2000; /* select falling edge trigger */
// NVIC->ISER[1] = 0x00000100; /* enable IRQ40 (bit 8 of ISER[1]) */
NVIC_EnableIRQ(EXTI15_10_IRQn);
__enable_irq(); /* global enable IRQs */
while(1) { }
}
void EXTI15_10_IRQHandler(void) {
GPIOA->BSRR = 0x00000020; /* turn
GPIOA->BSRR = 0x00200000; /* turn
GPIOA->BSRR = 0x00000020; /* turn
GPIOA->BSRR = 0x00200000; /* turn
on green LED */ delayMs(250);
off green LED */ delayMs(250);
on green LED */ delayMs(250);
off green LED */ delayMs(250);
EXTI->PR = 0x2000; /* clear interrupt pending flag */ }
/* 16 MHz SYSCLK */
void delayMs(int n) {
int i;for (; n > 0; n--)
for (i = 0; i < 3195; i++) ; }
