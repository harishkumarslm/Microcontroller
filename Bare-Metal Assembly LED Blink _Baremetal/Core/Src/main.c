; STM32F446RE LED Blink on PA5 (Nucleo-F446RE)
    ; Bare-metal assembly, 16 MHz HSI clock
    ; Keil uVision, ARM Cortex-M4

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
    EXPORT  Reset_Handler

; Register addresses (from RM0090)
RCC_AHB1ENR     EQU     0x40023830  ; RCC AHB1 enable register
GPIOA_MODER     EQU     0x40020000  ; GPIOA mode register
GPIOA_BSRR      EQU     0x40020018  ; GPIOA bit set/reset register

; Constants
GPIOA_EN        EQU     0x00000001  ; GPIOA clock enable (bit 0)
PA5_OUTPUT      EQU     0x00000400  ; PA5 output (bit 10=1, bit 11=0)
PA5_SET         EQU     0x00000020  ; PA5 set (bit 5)
PA5_RESET       EQU     0x00200000  ; PA5 reset (bit 21)
DELAY_COUNT     EQU     0x000F4240  ; ~500ms delay at 16 MHz (approximate)

Reset_Handler
    ; Enable GPIOA clock
    LDR     R0, =RCC_AHB1ENR
    LDR     R1, [R0]
    ORR     R1, R1, #GPIOA_EN
    STR     R1, [R0]

    ; Configure PA5 as output
    LDR     R0, =GPIOA_MODER
    LDR     R1, [R0]
    ORR     R1, R1, #PA5_OUTPUT
    STR     R1, [R0]

main_loop
    ; Set PA5 (LED on)
    LDR     R0, =GPIOA_BSRR
    MOV     R1, #PA5_SET
    STR     R1, [R0]

    ; Delay
    BL      delay

    ; Reset PA5 (LED off)
    LDR     R0, =GPIOA_BSRR
    MOV     R1, #PA5_RESET
    STR     R1, [R0]

    ; Delay
    BL      delay

    B       main_loop

delay
    ; Simple delay loop (~500ms at 16 MHz)
    LDR     R2, =DELAY_COUNT
delay_loop
    SUBS    R2, R2, #1
    BNE     delay_loop
    BX      LR

    ALIGN
    END
