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
    return M6812_CPU_E_CLOCK;
}

int main(){
    //ejemplo
    //inicializamos preescaler
    set_preescale(1);


}
