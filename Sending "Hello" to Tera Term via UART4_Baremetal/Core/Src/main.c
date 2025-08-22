#include "stm32F4xx.h"
void UART4_init(void); void UART4_write(int c); void delayMs(int);
/*------------------------------------------------------------------
---------MAIN function*-------------------------------------------------------------------
---------*/ int main (void) {
char message[] = "Hello\r\n";
int i;
UART4_init(); while (1) { for (i = 0; i < 7; i++) {
UART4_write(message[i]); /* send a char */ } delayMs(10); /* leave a
gap between messages */ } }
/*------------------------------------------------------------------
---------Initialize UART pins, Baudrate
*-------------------------------------------------------------------
---------*/ void UART4_init (void) {
RCC->AHB1ENR |= 1; /* Enable GPIOA clock */
RCC->APB1ENR |= 0x80000; /* Enable UART4 clock */
/* Configure PA0 for UART4 TX */
GPIOA->AFR[0] &= ~0x000F;
GPIOA->AFR[0] |= 0x0008; /* alt8 for UART4 */
GPIOA->MODER &= ~0x0003;
GPIOA->MODER |= 0x0002; /* enable alternate function for PA0 */
UART4->BRR = 0x0683; /* 9600 baud @ 16 MHz */ UART4->CR1 = 0x0008;
/* enable Tx, 8-bit data */ UART4->CR2 = 0x0000; /* 1 stop bit */
UART4->CR3 = 0x0000; /* no flow control */ UART4->CR1 |= 0x2000; /*
enable UART4 */
}
/* Write a character to UART4 */
void UART4_write (int ch) {
while (!(UART4->SR & 0x0080)) {} // wait until Tx buffer empty
UART4->DR = (ch & 0xFF);
}
void delayMs(int n) {
int i;
for (; n > 0; n--)
for (i = 0; i < 2000; i++) ; }
