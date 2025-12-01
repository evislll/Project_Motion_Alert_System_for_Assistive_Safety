@function.s-Led control using Assembly
@Author: Ekaterina Vislova
@Date: 2025-11-30
@Assembly code for Motion Alert System for Assistive Safety Project

@Most of the code logic for register access is based on documentation for lab work for SEH600 course
@ Seneca Polytechnic, "SEH500 Lab 7," Seneca BSA GitHub Pages, 2025.
@[Online]. Available: https://seneca-bsa.github.io/bsa/seh500/lab7/

@Prof.: Jacky Lau
@Seneca Polytechnic, School of Software Design & Data Science
.syntax unified
.cpu cortex-m4
.thumb
.text
.global setup
.type setup, %function
setup:
@ Enable clocks for all three ports (Port A, C, E)
    ldr r1, =0x40048038     @ System Clock Gate Control Register 5 (SIM_SCGC5)
    ldr r0, [r1]
    orr r0, r0, #(1<<9)     @ Enable clock for Port A (bit 9)
    orr r0, r0, #(1<<11)    @ Enable clock for Port C (bit 11)
    orr r0, r0, #(1<<13)    @ Enable clock for Port E (bit 13)
    str r0, [r1]
    @ Configure Red LED (PTC9) as GPIO
    ldr r1, =0x4004B024     @ PORTC_PCR9 - Red LED
    mov r0, #0x00000100     @ Set pin to GPIO mode
    str r0, [r1]
    @ Configure Green LED (PTE6) as GPIO
    ldr r1, =0x4004D018     @ PORTE_PCR6 - Green LED
    mov r0, #0x00000100     @ Set pin to GPIO mode
    str r0, [r1]
    @ Configure Blue LED (PTA11) as GPIO
    ldr r1, =0x4004902C     @ PORTA_PCR11 - Blue LED
    mov r0, #0x00000100     @ Set pin to GPIO mode
    str r0, [r1]
    ldr r1, =0x400FF094     @ Set three pins as outputs,Red LED (PTC9) output, GPIOC_PDD
    mov r0, #(1<<9)         @ Set pin 9 to output mode
    str r0, [r1]
    @ Green LED (PTE6) output
    ldr r1, =0x400FF114     @ GPIOE_PDDR
    mov r0, #(1<<6)         @ Set pin 6 to output mode
    str r0, [r1]
    @ Blue LED (PTA11) output
    ldr r1, =0x400FF014     @ GPIOA_PDDR
    mov r0, #(1<<11)        @ Set pin 11 to output mode
    str r0, [r1]
    bx lr
.global func_led_on
.type func_led_on, %function
func_led_on:
    @ Turn ON all three LEDs (set outputs to LOW - active low)
    @ Red LED ON (PTC9 = LOW)
    ldr r1, =0x400FF080     @ GPIOC_PDOR
    mov r0, #0              @ Set all outputs to LOW
    str r0, [r1]
    @ Green LED ON (PTE6 = LOW)
    ldr r1, =0x400FF100     @ GPIOE_PDOR
    mov r0, #0              @ Set all outputs to LOW
    str r0, [r1]
    @ Blue LED ON (PTA11 = LOW)
    ldr r1, =0x400FF000     @ GPIOA_PDOR
    mov r0, #0              @ Set all outputs to LOW
    str r0, [r1]
    bx lr
.global func_led_off
.type func_led_off, %function
func_led_off:
    @ Turn OFF all three LEDs (set outputs to HIGH - active low)
    @ Red LED OFF (PTC9 = HIGH)
    ldr r1, =0x400FF080     @ GPIOC_PDOR
    mov r0, #(1<<9)         @ Set pin 9 to HIGH
    str r0, [r1]
    @ Green LED OFF (PTE6 = HIGH)
    ldr r1, =0x400FF100     @ GPIOE_PDOR
    mov r0, #(1<<6)         @ Set pin 6 to HIGH
    str r0, [r1]
    @ Blue LED OFF (PTA11 = HIGH)
    ldr r1, =0x400FF000     @ GPIOA_PDOR
    mov r0, #(1<<11)        @ Set pin 11 to HIGH
    str r0, [r1]
    bx lr
.global set_red_led
.type set_red_led, %function
set_red_led:
    @ Turn on ONLY Red LED
    ldr r1, =0x400FF080     @ GPIOC_PDOR - Red ON
    mov r0, #0
    str r0, [r1]
    ldr r1, =0x400FF100     @ GPIOE_PDOR - Green OFF
    mov r0, #0x40           @ (1<<6)
    str r0, [r1]
    ldr r1, =0x400FF000     @ GPIOA_PDOR - Blue OFF
    mov r0, #0x800          @ (1<<11)
    str r0, [r1]
    bx lr
.global set_blue_led
.type set_blue_led, %function
set_blue_led:
    @ Turn on ONLY Blue LED
    ldr r1, =0x400FF000     @ GPIOA_PDOR - Blue ON
    mov r0, #0
    str r0, [r1]
    ldr r1, =0x400FF080     @ GPIOC_PDOR - Red OFF
    mov r0, #0x200          @ (1<<9)
    str r0, [r1]
    ldr r1, =0x400FF100     @ GPIOE_PDOR - Green OFF
    mov r0, #0x40           @ (1<<6)
    str r0, [r1]
    bx lr
.global set_green_led
.type set_green_led, %function
set_green_led:
    @ Turn on ONLY Green LED
    @ Red LED OFF (PTC9 = HIGH)
    ldr r1, =0x400FF080     @ GPIOC_PDOR
    mov r0, #(1<<9)         @ Set pin 9 to HIGH
    str r0, [r1]
    @ Green LED ON (PTE6 = LOW)
    ldr r1, =0x400FF100     @ GPIOE_PDOR
    mov r0, #0              @ Set pin 6 to LOW
    str r0, [r1]
    @ Blue LED OFF (PTA11 = HIGH)
    ldr r1, =0x400FF000     @ GPIOA_PDOR
    mov r0, #(1<<11)        @ Set pin 11 to HIGH
    str r0, [r1]
    bx lr
