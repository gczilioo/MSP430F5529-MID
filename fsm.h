/*
 * fsm.h
 *
 *  Created on: 20 de mar de 2019
 *      Author: Gabriel
 */

#ifndef FSM_H_
#define FSM_H_

estados_fsm inicia_sistema (estados_fsm estados)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT.
    iniciar_leds();                           // Inicia os leds
    clock_setup_full(DCO,DCOCLDIV,DCO);
    uart_init(VELOCIDADE);                    // inicia a UCA1
    dac_com_init(VELOCIDADE_D);               // inicia a porta do DAC
    adc_com_init(VELOCIDADE_A);               // Inicia a comunicação com o adc.
	acende_led(LED1);
	
	return stanby_stat;
}


#endif /* FSM_H_ */
