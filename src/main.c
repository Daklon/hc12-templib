#include <types.h>
#include <sys/sio.h>
#include <sys/param.h>

//declaramos variable global, un contador para aumentar el tiempo que puede contar el timer
uint16_t expanded_timer = 0;

//función a la que se llama con una interrupción
//cada vez que se desborda el timer
//En caso de overflow de expanded_timer, se vuelve a 0
void __attribute__((interrupt)) vi_tov(void){
    expanded_timer++;
}

uint8_t set_preescale(uint8_t preescale){
    if (preescale > 0 && preescale <= 7 ){
        _io_ports[M6812_TMSK2] = preescale - 1;
        return TRUE;
    }else{
        return FALSE;
    }
}

//concatenamos expanded_timer con el TCNT y lo devolvemos
uint32_t geticks(){
    uint32_t pow = 10;
    uint16_t ticks = _IO_PORTS_W(M6812_TCNT);
    while(ticks >= pow){
        pow *= 10;
    }
    return expanded_timer * pow + ticks;
}

uint32_t getmicros(){
    //leemos la frecuencia de reloj
    // con resolución de microsegundos
    uint32_t frec = M6812_CPU_E_CLOCK/(1 << 0x4); /* Frecuencia del temporizador*/
    uint16_t tcnt = _IO_PORTS_W(M6812_TCNT); // valor del temporizador
    
    return frec / tcnt;
} 

int main(){
    //ejemplo
    //inicializamos preescaler
    set_preescale(1);


}
