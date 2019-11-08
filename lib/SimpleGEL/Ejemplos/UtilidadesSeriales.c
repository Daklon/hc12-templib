/* ************************************************

  Ejemplo de uso de las distintas funciones de lectura y escritura de números
  que están disponibles en la librería <sys/sio.h>


   Copyright (C) Alberto F. Hamilton Castro
   Dpto. de Ingeniería Informática de Sistemas
   Universidad de La Laguna

  Licencia: GPLv3

  *************************************** */

#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>

int main() {
  uint8_t nb,nb2;
  uint16_t sa;

  serial_init();
  serial_print("\nUtilidadesSeriales.c ==========\n");
  while(1) {

    serial_print("\n\nEntra uint8_t decimal:");
    nb = serial_getdecbyte();
    serial_print("\nNumero entrado = 0b");
    serial_printbinbyte(nb);
    serial_print("\nNumero entrado = 0x");
    serial_printhexbyte(nb);
    serial_print("\nNumero entrado = ");
    serial_printdecbyte(nb);

    serial_print("\nEntra número hexa:");
    nb2 = serial_gethexbyte();
    serial_print("\nNumero entrado = 0x");
    serial_printhexbyte(nb2);
    serial_print("\nNumero entrado = 0b");
    serial_printbinbyte(nb2);
    serial_print("\nNumero entrado = ");
    serial_printdecbyte(nb2);

    serial_print("\n\nEntra uint16_t decimal:");
    sa = serial_getdecword();
    serial_print("\nNumero entrado = 0b");
    serial_printbinword(sa);
    serial_print("\nNumero entrado = 0x");
    serial_printhexword(sa);
    serial_print("\nNumero entrado = y");
    serial_printdecword(sa);

  }
}

