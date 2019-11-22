#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/param.h>
#include <sys/locks.h>

/** Global vars declaration */
uint16_t expanded_timer = 0;
uint8_t preescale = 0;

/**	Increments expanded timer every time the microcontroller timer overflows */
void __attribute__((interrupt)) vi_tov(void){
    expanded_timer++; //incrementamos expanded timer
    _io_ports[M6812_TFLG2] = M6812B_TOF;//bajamos el flag que disparó la interrupción
}

/** Sets the preescaler to a microcontroller defined configuration */
uint8_t set_preescale(uint8_t preescale_r){
    if (preescale_r > 0 && preescale_r <= 7 ){
        _io_ports[M6812_TMSK2] |= preescale_r;
	preescale = preescale_r;
        return TRUE;
    }else{
        return FALSE;
    }
}

/** Concats two numbers in a string-like fashion */
uint32_t concat(uint16_t x, uint16_t y){
    uint32_t pow = 10;
    while (y >= pow) {
	pow *= 10;
    }
    return x * pow + y;
}

/** Calculates and returns the concatenation of the expanded timer 
 * with the microcontroller timer register */
uint32_t geticks(){
    uint16_t ticks = _IO_PORTS_W(M6812_TCNT);
    return concat(expanded_timer,ticks);
}

/** Returns ticks conversion in microseconds */
uint32_t getmicros(){
    //leemos la frecuencia de reloj
    //con resolución de microsegundos
    uint32_t frec = M6812_CPU_E_CLOCK/(1 << 0x4);//TODO: cambiar aquí el 4 por el valor del preescaler
    uint16_t tcnt = _IO_PORTS_W(M6812_TCNT); // valor del temporizador

    return (geticks());//TODO dividir esto por la frecuencia para obtener siempre microsegundos
} 

/** Returns ticks convertion in milliseconds */
uint32_t getmilis(){
    return getmicros()/1000;
}

/** Generates a time-based delay */
void delayms(uint32_t time){
	uint32_t wait_start;
	do {
		wait_start = getmilis();
	} while (wait_start <= time);
}

/** Waits for a time-based delay, then calls a function */
void future_f(void (*f), uint32_t time){
	// TODO
}

/** Calls a function periodically */
void periodic_f(void (*f), uint32_t time){
	// TODO
}

/** SimpleGEL library initialization */
void initialize(){
    //desabilitamos interrupciones
    lock();
	// habilitamos el timer
	_io_ports[M6812_TSCR] |= M6812B_TEN;
	//habilitamos interrupción de desbordamiento del timer
	_io_ports[M6812_TMSK2] |= M6812B_TOI;
	//habilitamos las interrupciones
	unlock();
}

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
}
