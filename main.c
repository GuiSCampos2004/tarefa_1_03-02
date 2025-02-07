#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "pixel.h"

const uint botao_a = 5;
const uint botao_b = 6;
const uint pixel = 7;
const uint led_g = 11;
const uint led_b = 12;
const uint led_r = 13;

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint8_t endereco = 0x3C;
#define I2C_PORT i2c1

bool aceso_verde = false, aceso_azul = false; //Variáreis para ligar/desligar LED's
const char *init_message = "\r\nDigite alguma letra ou número: "; //Usei essa varável para digitar menos
ssd1306_t ssd; // Inicializa a estrutura do display

static volatile uint32_t ultimo_tempo = 0, tempo0=0, tempo_pixel0=0;
/*
ultimo_tempo é utilizado para Debounce dos botões
tempo0 é utilizado para apagar o display um tempo após a ultima mudança de texto
tempo_pixel0 é utilizado para apagar a matriz de LED's um tempo após a ultima mudança de número
*/

//-----------------------------------------------------------------------------------------------------------
int64_t turn_off_callback1(alarm_id_t id, void *user_data){ //Função que apaga display com base no
                                                            //tempo decorrido da última alteração de texto

    uint32_t tempo1 = to_us_since_boot(get_absolute_time());


    if (tempo1 - tempo0 >= 5000000){ //Caso o tempo entre a última chamada (tempo0) 
                                     //e a chamada da função for de 5s, entra no if

        ssd1306_fill(&ssd, false); // Limpa o display
        ssd1306_send_data(&ssd); // Atualiza o display
    }

    return 0;
}

int64_t turn_off_callback2(alarm_id_t id, void *user_data){ //Função que apaga matriz com base no
                                                            //tempo decorrido da última alteração de texto

    uint32_t tempo_pixel1 = to_us_since_boot(get_absolute_time());


    if (tempo_pixel1 - tempo_pixel0 >= 5000000){ //Caso o tempo entre a última chamada (tempo_pixel0) 
                                                 //e a chamada da função for de 5s, entra no if

        write(matriz[10]); //Apaga matriz de LED's
    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------
static void gpio_irq_handler(uint gpio, uint32_t events){ //Função para interrupção dos botões

    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());

    if (tempo_atual - ultimo_tempo > 200000){ //Debounce evitando que algo seja realizado caso a interrupção ocorra numa frequencia alta
        ultimo_tempo = tempo_atual;

        if(gpio==5){ //Caso o botão A seja apertado, entra nesse if

            aceso_verde = !aceso_verde; //Altera o estado do LED verde
            gpio_put(led_g, aceso_verde);


            ssd1306_fill(&ssd, false); // Limpa o display
            if(aceso_verde){
                ssd1306_draw_string(&ssd, "O LED VERDE", 19, 21); //Escreve no display
                ssd1306_draw_string(&ssd, "ESTA ACESO", 23, 33);  //Escreve no display
            }else{
                ssd1306_draw_string(&ssd, "O LED VERDE", 19, 21); //Escreve no display
                ssd1306_draw_string(&ssd, "ESTA APAGADO", 15, 33); //Escreve no display
            }
            ssd1306_send_data(&ssd); // Atualiza o display

            tempo0 = to_us_since_boot(get_absolute_time()); //Atualiza o tempo da última alteração de texto no display
            add_alarm_in_ms(5000, turn_off_callback1, NULL, false); //Alarme para apagar display futuramente
            printf("\r\nApertar o botão A altera o estado do LED verde\n");
            printf(init_message);

        }else{ //Caso o botão B seja apertado, entra nesse else

            aceso_azul = !aceso_azul; //Altera o estado do LED azul
            gpio_put(led_b, aceso_azul);


            ssd1306_fill(&ssd, false); // Limpa o display
            if(aceso_azul){
                ssd1306_draw_string(&ssd, "O LED AZUL", 23, 21); //Escreve no display
                ssd1306_draw_string(&ssd, "ESTA ACESO", 23, 33); //Escreve no display
            }else{
                ssd1306_draw_string(&ssd, "O LED AZUL", 23, 23); //Escreve no display
                ssd1306_draw_string(&ssd, "ESTA APAGADO", 15, 33); //Escreve no display
            }
            ssd1306_send_data(&ssd); // Atualiza o display

            tempo0 = to_us_since_boot(get_absolute_time()); //Atualiza o tempo da última alteração de texto no display
            add_alarm_in_ms(5000, turn_off_callback1, NULL, false); //Alarme para apagar display futuramente
            printf("\r\nApertar o botão B altera o estado do LED azul\n");
            printf(init_message);
        }
    }
}
//-----------------------------------------------------------------------------------------------------------
int main(){
    //------------------------------ Inicialização de portas -----------------------------
    stdio_init_all();
    
    pixel_init(pixel);
    

    gpio_init(botao_a);
    gpio_set_dir(botao_a, GPIO_IN);
    gpio_pull_up(botao_a);

    gpio_init(botao_b);
    gpio_set_dir(botao_a, GPIO_IN);
    gpio_pull_up(botao_b);
    
    
    gpio_init(led_g);
    gpio_set_dir(led_g, GPIO_OUT);
    gpio_put(led_g,0);
    gpio_init(led_b);
    gpio_set_dir(led_b, GPIO_OUT);
    gpio_put(led_b,0);

    write(matriz[10]); //Limpa matriz de LED's

    //------------------------------------------------------------------------------------
    // Inicialização do I2C. Utilizando a 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Configurando GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Configurando GPIO para I2C
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    //------------------------------------------------------------------------------------
    //Configuração de interrupção
    gpio_set_irq_enabled_with_callback(botao_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(botao_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true){
        printf(init_message);
        
        if (stdio_usb_connected()){ //Certifica-se de que o USB está conectado
            char c;
            scanf("%c", &c); //Grava o caractere que o usuário enviar
            printf("%c\n",c); //Imprime o caractere completando a frase "Digite alguma letra ou número: (caracere escolhido)"
            write(matriz[10]); //Limpa matriz de LED's

            if ((c>=65&&c<=90)||(c>=97&&c<=122)){ //Verifica se o caractere digitado faz parte do alfabeto
                ssd1306_fill(&ssd, false); // Limpa o display
                ssd1306_draw_char(&ssd, c, 59, 27); // Desenha uma string
                ssd1306_send_data(&ssd); // Atualiza o display
                
            }else{
                if(c>=48&&c<=57){ //Verifica se o caractere digitado está entre 0 e 9
                    ssd1306_fill(&ssd, false); // Limpa o display
                    ssd1306_draw_char(&ssd, c, 59, 27); // Desenha uma string
                    ssd1306_send_data(&ssd); // Atualiza o display

                    write(matriz[((int)c)-48]); //Coloca no matriz de LED's o número escolhido
                    tempo_pixel0 = to_us_since_boot(get_absolute_time()); //Atualiza o tempo da última alteração número na matriz
                    add_alarm_in_ms(5000, turn_off_callback2, NULL, false); //Alarme para apagar matriz futuramente
                }else{
                    printf("\rComando inválido\n"); //Caso o usuário digite algo fora do alfabeto
                                                    //ou dos números de 0-9, imprime a mensagem
                }
            }
        }
    }
}