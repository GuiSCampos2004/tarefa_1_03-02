#include "ws2818b.pio.h"

static PIO pio;
static uint sm;

//-------------------------------------------------------------------
void pixel_init(uint8_t pin){ //Inicializa o PIO
    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    pio = pio0;

    // Toma posse de uma máquina PIO.
    sm = pio_claim_unused_sm(pio, false);
    if (sm < 0)
    {
        pio = pio1;
        sm = pio_claim_unused_sm(pio, true);
    }

    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(pio, sm, offset, pin, 800000.f);
}
//-------------------------------------------------------------------

void write(uint8_t matriz[5][5][3]){ // Escreve cada elemento do formato RGB como informação de 8-bit em sequência no buffer da máquina PIO.
    int i=0,j=0;                     // Faz ainda a conversão do formato da matriz para coincidir com a disposição da sequência dos LED's
    for(int i = 4; i>=0; i--){
      for(int j = 4; j>=0; j--){
        if(i%2==0){
          pio_sm_put_blocking(pio, sm, matriz[i][j][1]);
          pio_sm_put_blocking(pio, sm, matriz[i][j][0]);
          pio_sm_put_blocking(pio, sm, matriz[i][j][2]);
        }else{
          pio_sm_put_blocking(pio, sm, matriz[i][4-j][1]);
          pio_sm_put_blocking(pio, sm, matriz[i][4-j][0]);
          pio_sm_put_blocking(pio, sm, matriz[i][4-j][2]);

        }
      }
    }
}
//-------------------------------------------------------------------

uint8_t matriz[11][5][5][3] = { //Matriz que contém os sprites dos números
{
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}}
},{
{{0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 0, 0}} 
},{
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}}
},{
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}}
},{
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}}
},{
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}}   
},{
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}}
},{
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}}
},{
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}}
},{
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 64, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 64, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 64, 0}, {0, 64, 0}, {0, 64, 0}, {0, 0, 0}}
},{
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},  
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}} 
}};