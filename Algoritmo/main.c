#include "config.h"
#include "bits.h"
#include "ssd.h"
#include "keypad.h"
#include "lcd.h"
#include "i2c.h"
#include "rtc.h"
#include "atraso.h"
#include "serial.h"
#include "io.h"
#include "pwm.h"
#include "bits.h"
#include "pic18f4520.h"
#define DATA  PORTD
#define DISP0 PIN_A2
#define DISP1 PIN_A3
#define DISP2 PIN_A4
#define DISP3 PIN_A5

void main(void) {


    unsigned char tempo, tmp;
    unsigned int tecla, g = 2000, o = 0;
    int conte, cont, con, x, y, j, i, contp, z, k, passo;
    char p, batimentos;
    char msg[15] = {"seja bem vindo"};
    char msg1[12] = {"1- Batimento"};
    char batimento[3] = {"bpm"};
    char msg2[8] = {"2- Passos"};
    char msg3[24] = {"3- Cronometro "};
    char msg4[8] = {"4- horas"};


    //  da linha 36 at� a linha 46 s�o feito as inicia��es que ser�o usadas.
    TRISC = 0x01;

    TRISD = 0x00;

    lcdInit();
    kpInit();
    i2c_init();
    for (j = 0; j < 15; j++) {
        lcdData(msg[j]);
    }
    rtc_r();





    for (;;) {
        // novamente da linha 54 at� 57 � feita mais algumas inicia��es para que seja impresso de forma correta
        // as mensagens  no LCD e para a incializa��o de alguns dispositivos da forma adequada.
        lcdCommand(0xC0);

        PORTA = 0x00;
        PORTD = 0X00;



        if (((contp / 10000) % 6) == 0) {

            batimentos++;

            if (batimentos > 9) {

                batimentos = 0;

            }
        }


        kpDebounce(); // da linha 74 at� 76 � feito o processo para que a tecla apertada seja lida de forma correta
        if (tecla != kpRead()) {
            tecla = kpRead();

            lcdCommand(0x80);

            if (bitTst(tecla, 3)) { // da linha 79 at� 94 � acionado a tecla "1" que vai emitir uma mensagem no LCD e em 
                // pegar as horas do computador printando no LCD tamb�m.
                lcdCommand(0x01);

                for (j = 0; j < 8; j++) {
                    lcdData(msg4[j]);
                }



                lcdCommand(0x80);
                lcdCommand(0xC0);
                for (i = 0; i < 9; i++) {
                    rtc_r();
                    lcdData((const char *) time[i]);
                }




            }
       
            if (bitTst(tecla, 6)) { // da linha 109 at� 123 desliga o display de 7 segmentos e printa o tempo que 
                // mostrado nele no painel LCD
                pinMode(DISP0, INPUT);
                pinMode(DISP1, INPUT);
                pinMode(DISP2, INPUT);
                pinMode(DISP3, INPUT);
                lcdCommand(0x01);
                lcdData(con + 48);
                lcdData(cont + 48);
                lcdData(':');
                lcdData(y + 48);
                lcdData(x + 48);
                PORTB = 0X00;
                PORTA = 0X00;
                PORTD = 0X00;
            }

            if (bitTst(tecla, 7)) { /// da linha 126 at� 147 as vari�veis que mostram o tempo no display de 7 segmentos s�o zeradas
                // aparece uma mensagem  os leds do portb s�o ligados  um som � emitido e o display come�a a rodar
                lcdCommand(0x01); // limpa o lcd
                conte = 0;
                con = 0;
                cont = 0;

                PORTB = 0xFF; // liga os leds do portB
                atraso_ms(1000);
                for (j = 0; j < 24; j++) {
                    lcdData(msg3[j]);
                }
                ssdInit(); // Inicia o display de 7 segmentos 
                pwmInit();
                for (k = 0; k < 3; k++) {
                    for (i = 1; i > 0; i = i * 2) {
                        bitSet(TRISC, 1);
                        PORTD = i;
                        atraso_ms(100);
                    }
                    bitClr(TRISC, 1);
                }
            }



            if (bitTst(tecla, 2)) { // da linha 151 at� 167 � feito os ajustes para contabilizar o batmento  card�aco de uma pessoa 
                // e em seguida ele printa na tela 

                lcdCommand(0x01);

                for (j = 0; j < 12; j++) {
                    lcdData(msg1[j]);
                }
                lcdCommand(0xC0);
                lcdData('1');
                lcdData((batimentos / 3) % 10 + 48);
                lcdData((batimentos + 15) % 10 + 48);
                for (i = 0; i < 3; i++) {
                    lcdData(batimento[i]);
                }
            }
            if (bitTst(tecla, 11)) { // da linha 168 at� 180  ocorre o processo para mostrar o n�mero de passos no display LCD
                lcdCommand(0x01);

                for (j = 0; j < 8; j++) {
                    lcdData(msg2[j]);
                }
                lcdCommand(0xC0);
                lcdData((passo / 1000 % 10) + 48);
                lcdData((passo / 100 % 10) + 48);
                lcdData((passo / 10 % 10) + 48);
                lcdData(passo % 10 + 48);
                passo++;
            }

        }


        
  if (x == 9 && y == 5) { // da linha 185 at� 211 � feito todo processo de contas para o funcionamento do display de 7 segmentos
            // al�m de alterar a vari�vel passo de acordo com a pssagem do display de 7 segmentos, aqui tamb�m ocorre 
            //uma altera��o no tempo de execuc�o e a constante atualiza��o do display de 7 segmentos.

            cont++;
            passo = passo + 92;
            conte = 0;

            if (cont > 9) {
                cont = 0;

                con++;
          }
            if (con > 9) {
                con = 0;
            }

        }
        conte++;
        x = ((conte / 100) % 10);
        y = ((conte / 1000) % 10);
        ssdDigit(x, 3);
        ssdDigit(y, 2);
        ssdDigit(cont, 1);
        ssdDigit(con, 0);

        ssdUpdate();
        for (tempo = 0; tempo < 255; tempo++);

    }

}
