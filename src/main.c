#include "timer.h"

int main(){
    //inicializamos
    initialize();
    //inicializamos el serial
    serial_init();
    serial_print("\nEjemplo de librería de temporización");
    //inicializamos preescaler
	serial_print("\npreescale: ");
    serial_printdecbyte(set_preescale(4));
	serial_print("\ngeticks: ");
    serial_printdeclong(geticks());
	serial_print("\ngetmicros: ");
    serial_printdeclong(getmicros());
	serial_print("\ngetmilis: ");
    serial_printdeclong(getmilis());
	serial_print("\ndelay: ");
    delayms(1000);
	serial_print("\ndelay: ");
    future_f(&dummyfunction,9911000);
    serial_print("FIN");
}
