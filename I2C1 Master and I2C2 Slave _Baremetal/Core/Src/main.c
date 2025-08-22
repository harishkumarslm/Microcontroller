#include "stm32f4xx.h"

void i2c1_master_write(uint8_t addr, uint8_t data) {
    I2C1->CR1 |= (1 << 8); // Start
    while (!(I2C1->SR1 & (1 << 0))); // Wait SB
    I2C1->DR = addr << 1; // Address, write
    while (!(I2C1->SR1 & (1 << 1))); // Wait ADDR
    (void)I2C1->SR2; // Clear ADDR
    I2C1->DR = data;
    while (!(I2C1->SR1 & (1 << 7))); // Wait TXE
    I2C1->CR1 |= (1 << 9); // Stop
}

uint8_t i2c1_master_read(uint8_t addr) {
    I2C1->CR1 |= (1 << 8); // Start
    while (!(I2C1->SR1 & (1 << 0)));
    I2C1->DR = (addr << 1) | 1; // Address, read
    while (!(I2C1->SR1 & (1 << 1)));
    (void)I2C1->SR2;
    while (!(I2C1->SR1 & (1 << 6))); // Wait RXNE
    uint8_t data = I2C1->DR;
    I2C1->CR1 |= (1 << 9); // Stop
    return data;
}

uint8_t i2c2_slave_read_write(uint8_t tx_data) {
    while (!(I2C2->SR1 & (1 << 1))); // Wait ADDR
    (void)I2C2->SR2; // Clear ADDR
    while (!(I2C2->SR1 & (1 << 6))); // Wait RXNE
    uint8_t rx_data = I2C2->DR; // Read
    while (!(I2C2->SR1 & (1 << 1))); // Wait ADDR (read)
    (void)I2C2->SR2;
    I2C2->DR = tx_data; // Send
    while (!(I2C2->SR1 & (1 << 7))); // Wait TXE
    return rx_data;
}

int main(void) {
    // Clocks
    RCC->AHB1ENR |= (1 << 1); // GPIOB
    RCC->APB1ENR |= (1 << 21) | (1 << 22); // I2C1, I2C2

    // I2C1 Master: PB6(SCL), PB9(SDA)
    GPIOB->MODER |= (2 << 12) | (2 << 18); // AF
    GPIOB->OTYPER |= (1 << 6) | (1 << 9); // Open-drain
    GPIOB->AFR[0] |= (4 << 24); // AF4 PB6
    GPIOB->AFR[1] |= (4 << 4); // AF4 PB9
    I2C1->CR2 = 16; // 16 MHz APB1
    I2C1->CCR = 80; // 100 kHz
    I2C1->TRISE = 17;
    I2C1->CR1 = (1 << 0); // Enable

    // I2C2 Slave: PB10(SCL), PB11(SDA)
    GPIOB->MODER |= (2 << 20) | (2 << 22); // AF
    GPIOB->OTYPER |= (1 << 10) | (1 << 11); // Open-drain
    GPIOB->AFR[1] |= (4 << 8) | (4 << 12); // AF4
    I2C2->OAR1 = (0x50 << 1) | (1 << 14); // Address 0x50, 7-bit
    I2C2->CR1 = (1 << 0); // Enable

    uint8_t master_tx = 0xAA, master_rx, slave_rx;
    while (1) {
        i2c1_master_write(0x50, master_tx);
        slave_rx = i2c2_slave_read_write(slave_rx + 1);
        master_rx = i2c1_master_read(0x50);
        master_tx++; // Next byte
        for (volatile uint32_t i = 0; i < 1000000; i++); // Delay
    }
}
