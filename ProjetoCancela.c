// INCLUINDO BIBLIOTECAS
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ESTABELECENDO CLOCK 20 MHz
#define _XTAL_FREQ 20000000 
#define TOTAL_PASSOS 440    
                            
// PADRAO DE CONFIGURACOES  
#pragma config FOSC  = HS   // Oscillator Selection bits  (HS oscillator)
#pragma config WDTE  = OFF  // Watchdog Timer Enable bit  (WDT disabled)
#pragma config PWRTE = OFF  // Power-up Timer Enable bit  (PWRT disabled)
#pragma config BOREN = ON   // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP   = OFF  // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD   = OFF  // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT   = OFF  // FlasDatah Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP    = OFF  // Flash Program Memory Code Protection bit (Code protection off)


// LINEARIZANDO DUAS GRANDEZAS
int mapear(float VAL, float Xo, float X, float Yo, float Y){
  if(VAL < Xo)
    return (int) Yo;
  if(VAL > X)
    return (int) Y;

  return (int) ( (Y - Yo)/(X - Xo)*(VAL - Xo) + Yo );
}


// CONFIGURANDO OS PINOS E INICIANDO O PROGRAMA
void setup(){
    TRISB = (1 << TRISB5) || (0 << TRISB7);
    TRISC = (1 << TRISC4) || (0 << TRISC5);
    TRISD = (1 << TRISD2) || (0 << TRISD3);

    RB7 = 1;
    __delay_ms(2000);
    RB7 = 0;
    __delay_ms(2000);

    for(int x = 0; x < 15; x++){
        RB7 = 1;
      __delay_ms(100);
        RB7 = 0;
      __delay_ms(100);
    }
}


// ROTACIONANDO O MOTOR NO SENTIDO ESPECIFICADO
void passo(bool sentido){
    if(sentido){
        RC5 = 1;
        RC4 = 0;
        RD3 = 0;
        RD2 = 0;
        __delay_ms(5);

        RC5 = 0;
        RC4 = 1;
        RD3 = 0;
        RD2 = 0;
        __delay_ms(5);

        RC5 = 0;
        RC4 = 0;
        RD3 = 1;
        RD2 = 0;
        __delay_ms(5);

        RC5 = 0;
        RC4 = 0;
        RD3 = 0;
        RD2 = 1;
        __delay_ms(5);
    }
    else{
        RC5 = 0;
        RC4 = 0;
        RD3 = 0;
        RD2 = 1;
        __delay_ms(5);

        RC5 = 0;
        RC4 = 0;
        RD3 = 1;
        RD2 = 0;
        __delay_ms(5);

        RC5 = 0;
        RC4 = 1;
        RD3 = 0;
        RD2 = 0;
        __delay_ms(5);
        
        RC5 = 1;
        RC4 = 0;
        RD3 = 0;
        RD2 = 0;
        __delay_ms(5);
    }
}


// DESLIGANDO AS BOBINAS DO MOTOR
void desligarBobinas(void){
    __delay_ms(5);
    RC5 = 0;
    RC4 = 0;
    RD3 = 0;
    RD2 = 0;
    __delay_ms(5);
}


// ROTACIONANDO O MOTOR DE PASSOS EM UMA CERTA ANGULA��O
void rotacionarMotor(int angulo, bool sentido){
    int totalPassos = mapear(angulo, 0, 360, 0, TOTAL_PASSOS);
    
    for(int x=0; x<totalPassos; x++)
        passo(sentido);
        
    desligarBobinas();
}


// FUNCAOO QUE OCORRER EM LOOP INFINITO
void loop(){
    if(RB5){
        __delay_ms(500);
        
        RB7 = 1;
        rotacionarMotor(90, true);
        
        while(RB5){}
    }
    
    if(!RB5){
        __delay_ms(500);
        
        RB7 = 0;
        rotacionarMotor(90, false);
        
        while(!RB5){}
    }
}


// FUNCAO PRINCIPAL DO PROGRAMA
int main(void){
    setup();
    
    while(1)
        loop();
      
  return 0;
}
