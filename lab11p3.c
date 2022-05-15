/* 
 * File:   lab11p3.c
 * Author: dellG3
 *
 * Created on 14 de mayo de 2022, 05:47 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <stdint.h>
/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ 1000000
#define FLAG_SPI 0xFF
/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/

char cont_slave = 0xFF;
char val_temporal = 0;
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);
/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if (PIR1bits.SSPIF){
        
        SSPBUF = cont_slave;
        PIR1bits.SSPIF = 0;             // Limpiamos bandera de interrupci n?
       
    }
    if(INTCONbits.RBIF){
        if (!PORTBbits.RB0){             // REVISAR SI RB0 FUE PRESIONADO
            cont_slave++;            // INCREMENTAR PORTA
           
        }
        else if(!PORTBbits.RB1){         // REVISAR SI RB1 FUE PRESIONADO
            cont_slave--;            // DECREMENTAR PORTA
            
        }
        INTCONbits.RBIF = 0;    // LIMPIAR BANDERA DE INTERRUPCION EN PORTB
    }
    return;
}
/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    setup();
    while(1){        
        // Envio y recepcion de datos en maestro
    }
    return;
}
/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    PORTB = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    TRISA = 0b00100000;
    PORTA = 0;
    
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Reloj interno
    
    // Configuracion de SPI
    // Configs del esclavo
    TRISC = 0b00011000; // -> SDI y SCK entradas, SD0 como salida
    PORTC = 0;

    // SSPCON <5:0>
    SSPCONbits.SSPM = 0b0100;   // -> SPI Esclavo, SS hablitado
    SSPCONbits.CKP = 0;         // -> Reloj inactivo en 0
    SSPCONbits.SSPEN = 1;       // -> Habilitamos pines de SPI
    // SSPSTAT<7:6>
    SSPSTATbits.CKE = 1;        // -> Dato enviado cada flanco de subida
    SSPSTATbits.SMP = 0;        // -> Dato al final del pulso de reloj

   
    //CONFIG DE INTERRUPCIONES
    PIR1bits.SSPIF = 0;         // LIMPIAR BANDERA DE INTERRUPCIONES DE SP1
    PIE1bits.SSPIE = 1;         // ACTIVAR INTERRUPCIONES DE SPI
    INTCONbits.GIE = 1;         // ACTIVAR INTERRUPCIONES GLOBALES
    INTCONbits.PEIE = 1;        // ACTIVAR INTERRUPCIONES DE PERIFERICOS
    INTCONbits.RBIE = 1;        // HABILITAR INTERRUPCIONES EN PORTB
    IOCBbits.IOCB0 = 1;         // HABILITAR INTERRUPCION EN CAMBIO PARA RB0
    IOCBbits.IOCB1 = 1;         // HABILITAR INTERRUPCION EN CAMBIO PARA RB1
    INTCONbits.RBIF = 0;        // LIMPIAR BANDERA DE INTERRUPCION EN PORTB
    //CONFIG PUSHBUTTONS EN PORTB
    TRISBbits.TRISB0 = 1;       // RB0 COMO INPUT
    TRISBbits.TRISB1 = 1;       // RB1 COMO INPUT
    OPTION_REGbits.nRBPU = 0;   // HABILITAR WEAK PULLUP EN PUERTO B
    WPUBbits.WPUB0 = 1;         // HABILITAR RESISTENCIA EN RB0
    WPUBbits.WPUB1 = 1;         // HABILITAR RESISTENCIA EN RB
}
