#include <types.h>
#include <sys/sio.h>
#include <sys/param.h>

//declaramos variable global, un contador para aumentar el tiempo que puede contar el timer
uint16_t expanded_timer = 0;
uint8_t preescale = 0;

//función a la que se llama con una interrupción
//cada vez que se desborda el timer
//En caso de overflow de expanded_timer, se vuelve a 0
void __attribute__((interrupt)) vi_tov(void){
    expanded_timer++;
}

//Establece el preescaler
uint8_t set_preescale(uint8_t preescale_r){
    if (preescale_r > 0 && preescale_r <= 7 ){
        _io_ports[M6812_TMSK2] = preescale_r - 1;
	preescale = preescale_r -1;
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

    return frec / tcnt;
} 

int main(){
    //ejemplo
    //inicializamos el serial
    serial_init();
    serial_print("\nEJemplo de librería de temporización");
    //inicializamos preescaler
    serial_printdecbyte(set_preescale(1));
    serial_printdeclong(geticks());
    serial_printdeclong(getmicros());
}
