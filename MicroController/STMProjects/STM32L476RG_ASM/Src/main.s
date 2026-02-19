.syntax unified
.cpu cortex-m4
.thumb

.equ DELAY_OFF,           1000000
.equ DELAY_ON,             100000

/* --- STM32L476 --- */
.equ RCC_BASE,        0x40021000
.equ RCC_AHB2ENR,     (RCC_BASE + 0x4C)     /* AHB2 peripheral clock enable */

.equ GPIOA_BASE,      0x48000000
.equ GPIOA_MODER,     (GPIOA_BASE + 0x00)
.equ GPIOA_BSRR,      (GPIOA_BASE + 0x18)
.equ GPIOA_ODR,       (GPIOA_BASE + 0x14)

/* SWO pin is PB3 on Nucleo-L476RG */
.equ GPIOB_BASE,      0x48000400
.equ GPIOB_MODER,     (GPIOB_BASE + 0x00)
.equ GPIOB_OSPEEDR,   (GPIOB_BASE + 0x08)
.equ GPIOB_AFRL,      (GPIOB_BASE + 0x20)

/* Debug MCU: enable trace pins */
.equ DBGMCU_BASE,       0xE0042000
.equ DBGMCU_CR,         (DBGMCU_BASE + 0x04)
.equ DBGMCU_TRACE_IOEN, (1 << 5)

/* Core debug: enable trace */
.equ DEMCR,           0xE000EDFC
.equ DEMCR_TRCENA,    (1 << 24)

.equ GPIOAEN_BIT,     (1 << 0)             /* RCC_AHB2ENR.GPIOAEN */
.equ GPIOBEN_BIT,     (1 << 1)             /* RCC_AHB2ENR.GPIOBEN */

.equ LED_PIN,         5                     /* PA5 */
.equ LED_SET,         (1 << LED_PIN)        /* BSRR set */
.equ LED_RESET,       (1 << (LED_PIN+16))   /* BSRR reset */

.global main
.thumb_func
main:
    /* Enable trace in the core */
    ldr   r0, =DEMCR
    ldr   r1, [r0]
    orr   r1, r1, #DEMCR_TRCENA
    str   r1, [r0]

    /* Enable trace IO pins (lets output SWO) */
    ldr   r0, =DBGMCU_CR
    ldr   r1, [r0]
    orr   r1, r1, #DBGMCU_TRACE_IOEN
    str   r1, [r0]

    /* Enable GPIOA + GPIOB clocks */
    ldr   r0, =RCC_AHB2ENR
    ldr   r1, [r0]
    orr   r1, r1, #GPIOAEN_BIT
    orr   r1, r1, #GPIOBEN_BIT
    str   r1, [r0]

    /* Set PA5 as output: MODER5 = 01 */
    ldr   r0, =GPIOA_MODER
    ldr   r1, [r0]
    ldr   r2, =0x00000C00          /* mask (3 << 10) */
    bic   r1, r1, r2
    ldr   r2, =0x00000400          /* (1 << 10) */
    orr   r1, r1, r2
    str   r1, [r0]

    /* Configure PB3 for SWO: AF mode (MODER3=10), AF0, very high speed */
    ldr   r0, =GPIOB_MODER
    ldr   r1, [r0]
    ldr   r2, =0x000000C0          /* mask PB3 bits 7:6 */
    bic   r1, r1, r2
    ldr   r2, =0x00000080          /* set 10b at bits 7:6 */
    orr   r1, r1, r2
    str   r1, [r0]

    ldr   r0, =GPIOB_AFRL
    ldr   r1, [r0]
    ldr   r2, =0x0000F000          /* AFRL3 bits 15:12 */
    bic   r1, r1, r2               /* AF0 = 0 */
    str   r1, [r0]

    ldr   r0, =GPIOB_OSPEEDR
    ldr   r1, [r0]
    ldr   r2, =0x000000C0          /* speed bits for PB3 */
    bic   r1, r1, r2
    ldr   r2, =0x000000C0          /* 11b (very high speed) */
    orr   r1, r1, r2
    str   r1, [r0]

blink_loop:
    /* LED ON */
    ldr   r0, =GPIOA_BSRR
    ldr   r1, =LED_SET
    str   r1, [r0]

    /* READ ODR so DWT can trace it, then copy to RAM for an easy graph */
    ldr   r2, =GPIOA_ODR
    ldr   r3, [r2]
    ldr   r4, =odr_sample
    str   r3, [r4]

    bl    delay_on

    /* LED OFF */
    ldr   r1, =LED_RESET
    str   r1, [r0]

    /* READ ODR + copy to RAM */
    ldr   r2, =GPIOA_ODR
    ldr   r3, [r2]
    ldr   r4, =odr_sample
    str   r3, [r4]

    bl    delay_off
    b     blink_loop

.thumb_func
delay_off:
    ldr   r2, =DELAY_OFF
1:
    subs  r2, r2, #1
    bne   1b
    bx    lr
.thumb_func
delay_on:
    ldr   r2, =DELAY_ON
1:
    subs  r2, r2, #1
    bne   1b
    bx    lr

.ltorg

/* RAM variable you can trace (4 bytes) */
.section .data
.align 4
.global odr_sample
odr_sample:
    .word 0
