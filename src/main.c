#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/param.h>
#include <sys/locks.h>

//declaramos variable global, un contador para aumentar el tiempo que puede contar el timer
uint16_t expanded_timer = 0;
uint8_t preescale = 0;

//función a la que se llama con una interrupción
//cada vez que se desborda el timer
//En caso de overflow de expanded_timer, se vuelve a 0
void __attribute__((interrupt)) vi_tov(void){
    expanded_timer++; //incrementamos expanded timer
    _io_ports[M6812_TFLG2] = M6812B_TOF;//bajamos el flag que disparó la interrupción
}

//Establece el preescaler
uint8_t set_preescale(uint8_t preescale_r){
    if (preescale_r > 0 && preescale_r <= 7 ){
        _io_ports[M6812_TMSK2] |= preescale_r;
	preescale = preescale_r;
        return TRUE;
    }else{
        return FALSE;
    }
}

//Función que concatena dos uint16_t
uint32_t concat(uint16_t x, uint16_t y){
    uint32_t pow = 10;
    while (y >= pow) {
	pow *= 10;
    }
    return x * pow + y;
}

//Concatenamos expanded_timer con el TCNT y lo devolvemos
uint32_t geticks(){
    uint16_t ticks = _IO_PORTS_W(M6812_TCNT);
    return concat(expanded_timer,ticks);
}

//Devuelve el tiempo desde que se encendió el microcontrolador en
//microsegundos
uint32_t getmicros(){
    //leemos la frecuencia de reloj
    //con resolución de microsegundos
    uint32_t frec = M6812_CPU_E_CLOCK/(1 << 0x4);
    uint16_t tcnt = _IO_PORTS_W(M6812_TCNT); // valor del temporizador

    return (geticks()/100);
} 

//Devuelve el tiempo desde que se encendió el microcontrolador en
//milisegundos TODO
uint32_t getmilis(){
    return getmicros()/1000;
}

//Función que espera x milisegundos TODO
void delayms(uint32_t time){
	uint32_t wait_start;
	do {
		wait_start = getmilis();
	} while (wait_start <= time);
}

//Ejecuta la función dentro de x milisegundos TODO
void future_f(void (*f), uint32_t time){
}

//Ejecuta la función cada x milisegundos, de forma periódica TODO
void periodic_f(void (*f), uint32_t time){
}

int main(){
    //desabilitamos interrupciones
    lock();
    //inicializamos el serial
    serial_init();
	// habilitamos el timer
	_io_ports[M6812_TSCR] |= M6812B_TEN;
	//habilitamos interrupción de desbordamiento del timer
	_io_ports[M6812_TMSK2] |= M6812B_TOI;
	//habilitamos las interrupciones
	unlock();
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
