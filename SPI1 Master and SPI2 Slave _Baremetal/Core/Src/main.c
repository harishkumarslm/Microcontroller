#include "stm32f4xx.h"

void spi1_master_write_read(uint8_t data, uint8_t *rx_data) {
    GPIOA->BSRR = (1 << (4 + 16)); // NSS low
    while (!(SPI1->SR & (1 << 1))); // Wait TXE
    SPI1->DR = data;
    while (!(SPI1->SR & (1 << 0))); // Wait RXNE
    *rx_data = SPI1->DR;
    while (SPI1->SR & (1 << 7)); // Wait BSY
    GPIOA->BSRR = (1 << 4); // NSS high
}

uint8_t spi2_slave_read_write(uint8_t tx_data) {
    while (!(SPI2->SR & (1 << 0))); // Wait RXNE
    uint8_t rx_data = SPI2->DR; // Read received
    while (!(SPI2->SR & (1 << 1))); // Wait TXE
    SPI2->DR = rx_data + 1; // Send incremented
    return rx_data;
}

int main(void) {
    // Clocks
    RCC->AHB1ENR |= (1 << 0) | (1 << 1); // GPIOA, GPIOB
    RCC->APB2ENR |= (1 << 12); // SPI1
    RCC->APB1ENR |= (1 << 14); // SPI2

    // SPI1 Master: PA5(SCK), PA6(MISO), PA7(MOSI), PA4(NSS)
    GPIOA->MODER |= (2 << 10) | (2 << 12) | (2 << 14) | (1 << 8); // AF for PA5-7, output PA4
    GPIOA->AFR[0] |= (5 << 20) | (5 << 24) | (5 << 28); // AF5
    GPIOA->BSRR = (1 << 4); // NSS high
    SPI1->CR1 = (1 << 2) | (2 << 3) | (1 << 6); // Master, fck/8, enable

    // SPI2 Slave: PB13(SCK), PB14(MISO), PB15(MOSI), PB12(NSS)
    GPIOB->MODER |= (2 << 24) | (2 << 26) | (2 << 28) | (2 << 30); // AF for PB12-15
    GPIOB->AFR[1] |= (5 << 16) | (5 << 20) | (5 << 24) | (5 << 28); // AF5
    SPI2->CR1 = (1 << 6); // Slave, enable

    uint8_t master_tx = 0x55, master_rx, slave_rx;
    while (1) {
        spi1_master_write_read(master_tx, &master_rx);
        slave_rx = spi2_slave_read_write(master_tx + 1);
        master_tx++; // Next byte
        for (volatile uint32_t i = 0; i < 1000000; i++); // Delay
    }
}
