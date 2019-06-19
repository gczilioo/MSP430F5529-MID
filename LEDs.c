/*
 * LEDs.c
 *
 *  Created on: 16/01/2019
 *      Author: Gabriel Cacilho Zilio
 */
#include "msp430f5529.h"
#include "leds.h"
//=============================================================================
//Campinas, 14/02/2012
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//DEMIC - UNICAMP
//Arquivo: leds.h
//=============================================================================

//============================Dispositivo======================================
// uC: MSP430F5438
// Versão: 1
//=============================================================================

//============================Descrição========================================
// Algumas funções para os leds da placa de controle. Para ligar as portas dos
// leds basta colocar a função "iniciar_leds" no início do programa. Para mudar
// a velocidade altere o valor de DELAY.
//=============================================================================

//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador
// MSP430F5438.
//=============================================================================
#define             DELAY           300000		// Espera determinado tempo

//=============================================================================
//                          Rotina para iniciar a porta dos LED's
//=============================================================================
void iniciar_leds(void)
{
    P1DIR |= LED1;          //  LED 1 P1.0
    P1OUT &= !LED1;
    P4DIR |= LED2;          //  LED 2 P4.7
    P4OUT &= !LED2;
	
	P1DIR &= ~BIT1;             // P1.1 INPUT
    P1SEL &= ~BIT1;
    P1REN |= BIT1;              // P1.1 PULL-UP
    P1OUT |= BIT1;

	P2DIR &= ~BIT1;             // P2.1 INPUT
	P2SEL &= ~BIT1;
	P2REN |= BIT1;              // P2.1 PULL-UP
	P2OUT |= BIT1;

}


//=============================================================================
//                          Rotina para piscar os LED's n vezes
//=============================================================================
void pisca_led (unsigned int n)
{
	unsigned int x;
	long i;
  for (x = n; x > 0; x--)
  {
    P4OUT ^= LED2;                 // Pisca LEDs
	P1OUT ^= LED1;
    for (i=0; i<DELAY; i++);
    	P4OUT ^= LED2;                 // Pisca LEDs
		P1OUT ^= LED1;
    for (i=0; i<DELAY; i++);
  }
}

//=============================================================================
//                          Rotina para piscar o LED AZUL n vezes
//=============================================================================
void pisca_led_1 (unsigned int n)
{
	unsigned int x;
	long i;
  for (x = n; x > 0; x--)
  {
    P1OUT ^= LED1;                            // Pisca LED azul
    for (i=0; i<DELAY; i++);
    	P1OUT ^= LED1;                            // Pisca LED azul
    for (i=0; i<DELAY; i++);
  }
}

//=============================================================================
//                          Rotina para piscar o LED VERMELHO n vezes
//=============================================================================
void pisca_led_2 (unsigned int n)
{
	unsigned int x;
	long i;

  for (x = n; x > 0; x--)
  {
    P4OUT ^= LED2;              // Pisca LED vermelho
    for ( i=0; i<DELAY; i++);
		P4OUT ^= LED2;              // Pisca LED vermelho
    for ( i=0; i<DELAY; i++);
  }
}

//=============================================================================
//                          Rotina para acender os leds
//=============================================================================
void acende_led(unsigned char led)
{
	if(led == LED1)
		P1OUT |= led;
	else if(led == LED2)
		P4OUT |= led;
}

//=============================================================================
//                          Rotina para apagar os leds
//=============================================================================
void apaga_led(unsigned char led)
{
  	if(led == LED1)
		P1OUT &= ~led;
	else if(led == LED2)
		P4OUT &= ~led;
}

