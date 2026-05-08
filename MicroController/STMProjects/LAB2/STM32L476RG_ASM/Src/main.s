.syntax unified
.cpu cortex-m4
.fpu fpv4-sp-d16
.thumb

.global main
.type main, %function

.equ RCC_AHB2ENR,  0x4002104C
.equ GPIOB_MODER,  0x48000400
.equ GPIOB_OTYPER, 0x48000404
.equ GPIOB_OSPEEDR,0x48000408
.equ GPIOB_PUPDR,  0x4800040C
.equ GPIOB_ODR,    0x48000414

.equ DELAY_VALUE, 500000

main:
    /* Enable GPIOB clock */
    ldr r0, =RCC_AHB2ENR
    ldr r1, [r0]
    movs r2, #0x02
    orrs r1, r1, r2
    str r1, [r0]

    /* Configure PB0-PB6 as output */
    ldr r0, =GPIOB_MODER
    ldr r1, [r0]
    ldr r2, =0x00003FFF
    bics r1, r1, r2
    ldr r2, =0x00001555
    orrs r1, r1, r2
    str r1, [r0]

    /* PB0-PB6 push-pull */
    ldr r0, =GPIOB_OTYPER
    ldr r1, [r0]
    movs r2, #0x7F
    bics r1, r1, r2
    str r1, [r0]

    /* PB0-PB6 low speed */
    ldr r0, =GPIOB_OSPEEDR
    ldr r1, [r0]
    ldr r2, =0x00003FFF
    bics r1, r1, r2
    str r1, [r0]

    /* PB0-PB6 no pull-up/pull-down */
    ldr r0, =GPIOB_PUPDR
    ldr r1, [r0]
    ldr r2, =0x00003FFF
    bics r1, r1, r2
    str r1, [r0]

    /* All segments OFF initially (common-anode: 1 = OFF) */
    ldr r0, =GPIOB_ODR
    ldr r1, [r0]
    movs r2, #0x7F
    bics r1, r1, r2
    orrs r1, r1, r2
    str r1, [r0]

main_loop:
    ldr r0, =digit_table
    movs r1, #10

display_digit:
    ldrb r2, [r0]
    adds r0, r0, #1

    ldr r3, =GPIOB_ODR
    ldr r5, [r3]
    movs r6, #0x7F
    bics r5, r5, r6
    orrs r5, r5, r2
    str r5, [r3]

    ldr r4, =DELAY_VALUE

delay:
    subs r4, r4, #1
    bne delay

    subs r1, r1, #1
    bne display_digit

    b main_loop

.align 2
digit_table:
    .byte 0x40   /* 0 */
    .byte 0x79   /* 1 */
    .byte 0x24   /* 2 */
    .byte 0x30   /* 3 */
    .byte 0x19   /* 4 */
    .byte 0x12   /* 5 */
    .byte 0x02   /* 6 */
    .byte 0x78   /* 7 */
    .byte 0x00   /* 8 */
    .byte 0x10   /* 9 */
