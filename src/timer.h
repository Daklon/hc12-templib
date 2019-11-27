#ifndef TIMER_H
#define TIMER_H
#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/param.h>
#include <sys/locks.h>

/** Global vars declaration */
uint16_t expanded_timer = 0;
uint16_t expanded_programmed_timer = 0;
uint16_t iteration_triger = 0;
uint8_t preescale = 0;
uint32_t periodic_timer = 0;

/** Pointer to function */
void (*future_function)(void);

/**	Increments expanded timer every time the microcontroller timer overflows */
void __attribute__((interrupt)) vi_tov(void);

/** Calls function after an interruption if the countdown timer equals zero */
void __attribute__((interrupt)) vi_ioc1(void);

/** Sets the preescaler to a microcontroller defined configuration */
uint8_t set_preescale(uint8_t preescale_r);

/** Concats two numbers in a string-like fashion */
uint32_t concat(uint16_t x, uint16_t y);

/** Calculates and returns the concatenation of the expanded timer 
 * with the microcontroller timer register */
uint32_t geticks();

/** Returns ticks conversion in microseconds */
uint32_t getmicros();

/** Returns ticks convertion in milliseconds */
uint32_t getmilis();

/** Generates a time-based delay, time must be milliseconds*/
void delayms(uint32_t time);

/** Waits for a time-based delay, then calls a function , time must be in milliseconds*/
void future_f(void (*f)(void), uint32_t time);

/** Calls a function periodically, to stop it call it again with 0 as time parameter */
void periodic_f(void (*f), uint32_t time);

/** Timer library initialization */
void initialize();

/** Serves as test for the futures */
void dummyfunction();

#endif
