
/* ****************************************
 *  Programa de manejo del conjunto de 7 segmentos
 *  de la placa DISEN-EXP
 * Se suponene conectados los 7 segmentos al puerto H
 * Es necesario realizar un refresco del orden de 100MHz
 * para que aprezcan los números estables

   Copyright (C) Alberto F. Hamilton Castro
   Dpto. de Ingeniería Informática de Sistemas
   Universidad de La Laguna

  Licencia: GPLv3

  *************************************** */


#include <types.h>
#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

#define  TCM_FACTOR (3)  /*La potencia de 2 a aplicar al factor*/
#define  TCM_FREQ (M6812_CPU_E_CLOCK/(1 << TCM_FACTOR))
/*Pasa de microsegundos a ticks*/
#define  USG_2_TICKS(us)  ((us) * (TCM_FREQ/1000000L))
/*Pasa de milisegundos a ticks*/
#define  MSG_2_TICKS(ms)  ((ms) * (TCM_FREQ/1000L))

uint16_t Periodo;
uint16_t cuenta_irqs;

/*Byte que corresponde a cada 7 segmento */
uint8_t digi7s[4];
uint8_t da; /* digito actual */

int main () {

  /* Deshabilitamos interrupciones */
  lock ();

  /*Encendemos led*/
  _io_ports[M6812_DDRG] |= M6812B_PG7;
  _io_ports[M6812_PORTG] |= M6812B_PG7;


  serial_init();
  serial_print("\nSaca7Segmentos.c =============\n");

  /*Inicializamos las variables */
  cuenta_irqs = 0;
  Periodo = USG_2_TICKS(5000);
  serial_print("\n usg del periodo: ");
  serial_printdecword(Periodo/USG_2_TICKS(1));
  digi7s[0] = 1;
  digi7s[1] = 2;
  digi7s[2] = 3;
  digi7s[3] = 4;
  da = 0;

  /* Inicializamos los dispositivos */
  /*Inicializamos el puerto H*/
  _io_ports[M6812_DDRH] = 0xff; /*todos los pines como salida*/
  _io_ports[M6812_PORTH] = 0x91; /*valor inicial*/

  /*Inicialización del Temporizador*/
  _io_ports[M6812_TMSK2] = TCM_FACTOR;
  /* OC2 Invierte el pin en cada disparo */
  _io_ports[M6812_TCTL2] &= ~M6812B_OM2;
  _io_ports[M6812_TCTL2] |= M6812B_OL2;

  /*preparamos disparo*/
  _IO_PORTS_W(M6812_TC2) = _IO_PORTS_W(M6812_TCNT) + Periodo;


  /*configuramos canal 2 como comparador salida*/
  _io_ports[M6812_TIOS] |= M6812B_IOS2;


  _io_ports[M6812_TFLG1] = M6812B_IOS2; /*Bajamos el banderín de OC2 */
  _io_ports[M6812_TMSK1] |= M6812B_IOS2; /*habilitamos sus interrupciones*/
  _io_ports[M6812_TSCR] = M6812B_TEN; /*Habilitamos temporizador*/

  unlock(); /* habilitamos interrupciones */

  serial_print("\n\rTerminada inicialización\n");

  while(1) {
    uint16_t nv;

    serial_print("\n\n Cuenta irqs: ");
    serial_printdecword(cuenta_irqs);

    serial_print("\n\nNuevos valores:");
    nv = serial_gethexword();
    /*serparamos los dígitos */
    digi7s[0] = (uint8_t)(nv & 0x0f);
    digi7s[1] = (uint8_t)((nv >> 4) & 0x0f);
    digi7s[2] = (uint8_t)((nv >> 8) & 0x0f);
    digi7s[3] = (uint8_t)((nv >> 12) & 0x0f);

    /*Invertimos el led*/
    _io_ports[M6812_PORTG] ^= M6812B_PG7;
  }
}


/* Manejador interrupciones del OC2  */
void __attribute__((interrupt)) vi_ioc2 (void) {
  /*Bajamos el banderín de OC2 */
  _io_ports[M6812_TFLG1] = M6812B_IOS2;

  /*preparamos siguiente disparo*/
  _IO_PORTS_W(M6812_TC2) = _IO_PORTS_W(M6812_TC2) + Periodo;
  cuenta_irqs++;

  /*Refrescamos el display */
  _io_ports[M6812_PORTH] = (digi7s[da] & 0x0f) | (1 << (da + 4));
  da = (da + 1) % 4;
}

