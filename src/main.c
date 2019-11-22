#include <types.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/param.h>
#include <sys/locks.h>

//declaramos variable global, un contador para aumentar el tiempo que puede contar el timer
uint16_t expanded_timer = 0;
uint16_t expanded_programmed_timer = 0;
uint16_t iteration_triger = 0;
uint8_t preescale = 0;

void (*future_function)(void);

//función a la que se llama con una interrupción
//cada vez que se desborda el timer
//En caso de overflow de expanded_timer, se vuelve a 0
void __attribute__((interrupt)) vi_tov(void){
    expanded_timer++; //incrementamos expanded timer
    _io_ports[M6812_TFLG2] = M6812B_TOF;//bajamos el flag que disparó la interrupción
}

//función a la que se llama con una interrupción
//cuando el timer alcanza el valor programado
void __attribute__((interrupt)) vi_ioc0(void){
    _io_ports[M6812_TFLG1] = M6812B_C1F; //bajamos el flag que disparó la interrupción
    //compruebo si es la última iteración
    if (expanded_programmed_timer == 0){
        //desactivar interrupción definitivamente
        _io_ports[M6812_TMSK1] &= ~M6812B_C1I;
        //ejecutamos función
        future_function();
    } else{//se ha disparado porque se tiene que ejecutar la func
        expanded_programmed_timer--;
    }
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
    uint32_t frec = M6812_CPU_E_CLOCK/(1 << 0x4);//TODO: cambiar aquí el 4 por el valor del preescaler
    uint16_t tcnt = _IO_PORTS_W(M6812_TCNT); // valor del temporizador

    return (geticks());//TODO dividir esto por la frecuencia para obtener siempre microsegundos
} 

//Devuelve el tiempo desde que se encendió el microcontrolador en
//milisegundos
uint32_t getmilis(){
    return getmicros()/1000;
}

//Función que espera x milisegundos
void delayms(uint32_t time){
	uint32_t wait_start;
	do {
		wait_start = getmilis();
	} while (wait_start <= time);
}

//Ejecuta la función dentro de x microsegundos TODO
void future_f(void (*f)(void), uint32_t time){
    expanded_programmed_timer = time >> 16;
    future_function = f;
    //Establecemos el modo de comparador a salida(OC)
    _io_ports[M6812_TIOS] |= M6812B_IOS1;
    //Machacamos el valor
	_io_ports[M6812_TFLG1] = M6812B_C1F;
	//Establecemos el valor del tcnt
    _IO_PORTS_W(M6812_TC1) = _IO_PORTS_W(M6812_TCNT) + (uint16_t) time; //al hacer cast debería descartar la parte mas significativa
	//habilitamos interrupciones para el OC
	_io_ports[M6812_TMSK1] |= M6812B_C1I;
}


//Ejecuta la función cada x milisegundos, de forma periódica TODO
void periodic_f(void (*f), uint32_t time){
}

//inicializa la librería
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

void dummyfunction(){
    serial_print("FuncionaAA!");
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
    future_f(dummyfunction(),1000);
    serial_print("FIN");
}
