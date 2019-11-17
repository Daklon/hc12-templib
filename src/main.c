#include <types.h>
#include <sys/sio.h>
#include <sys/param.h>

uint8_t set_preescale(uint8_t preescale){
    if (preescale > 0 && preescale <= 7 ){
        _io_ports[M6812_TMSK2] = preescale - 1;
        return TRUE;
    }else{
        return FALSE;
    }
}

uint32_t geticks(){
    //devolvemos el número de ticks
    return _IO_PORTS_W(M6812_TCNT);
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
