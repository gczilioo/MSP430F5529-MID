//=============================================================================
//	Toledo, 20/03/2019
//	Autor: Gabriel Cacilho Zilio, email: gczilioo@gmail.com
//	Universidade Tecnologica Federal do Parana - UTFPR - Campus Toledo
//	Arquivo: fms.c
//=============================================================================

//============================Dispositivo======================================
// uC: MSP430F5529
// Versão: 1
//=============================================================================
/*
#include "msp430f5529.h"       // Especificar dispositivo.
#include <intrinsics.h>     // Utilizar funções intrínsecas de baixo nível já definidas
#include "clock.h"          // Clock
#include "LEDs.h"           // Arquivo de protótipos de funções relacionadas aos LED´s.
#include "iso_rs_485.h"
#include "dac8586.h"
#include "ads1256.h"
#include <math.h>

//=============================================================================
//                           Definições
//=============================================================================

#define         true          0xff                // booleana 1
#define         false         0x00                // booleana 0

#define         F_CLK         20000000            // frequencia de clock do uC

#define			F_SMCLK		  F_CLK/2
#define         BAUD_RATE     500000              // baud rate da UCA1  (isolador)
#define         LM95071_BAUD  5000000             // baud rate da UCB3  (lm95071)
#define         DAC8568_BAUD  20000000             // baud rate da UCB0  (dac)
#define         ADS1256_BAUD  1000000             // baud rate da UCA0  (adc)
#define         VELOCIDADE    F_CLK/BAUD_RATE     // prescaler da uart (isolador)
#define         VELOCIDADE_T  F_CLK/LM95071_BAUD  // prescaler do SPI (lm95071)
#define         VELOCIDADE_D  F_CLK/DAC8568_BAUD  // prescaler do SPI (dac8568)
#define         VELOCIDADE_A  F_CLK/ADS1256_BAUD  // prescaler do SPI (ads1256)

#define			DAC_SIGNAL
#define			ADC_SIGNAL

//=============================================================================
//                           Variaveis Globais
//=============================================================================

unsigned long int 	i;

unsigned long int 	adc_dado;
unsigned char 		adc_canal;

unsigned long int 	dac_0a5_V;		// variavel para controle DAC de 0 a 5 Volts 
unsigned char 		dac_canal;
unsigned char		dac_ctrl = 0;			
unsigned long int 	dac_simetrico = 0x8000;	// variavel para controle DAC simétrico de -2,5 a 2,5 V
											// valor inicial setado para Vdac = 0 V

unsigned char dado_r;


//=============================================================================
//                           Estados Enumerado
//=============================================================================

enum estados_fsm { begin_sys, stanby_stat, config_sys, dac_zero, 
				adc_read, send_data, dac_set, dac_check, update_sys};
				
//=============================================================================
//                           
//=============================================================================

void main ()
{
	enum estados_fsm estados = begin_sys;
	
	while(1)
	{
		if( estados >= begin_sys && estados <= update )
		{	
			switch (estados)
			{
				case begin_sys:
						// estados = inicia_sistema(estados);
							WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT.
							iniciar_leds();                           // Inicia os leds
							clock_setup_full(DCO,DCOCLDIV,DCO);
							uart_init(VELOCIDADE);                    // inicia a UCA1
							dac_com_init(VELOCIDADE_D);               // inicia a porta do DAC
							adc_com_init(VELOCIDADE_A);               // Inicia a comunicação com o adc.
							acende_led(LED1);
							estados = stanby_stat;
							break;
		
				case stanby_stat:
						// stanby_stat
							dado_r = 0x00;						// seta valor inicial do dado
							dado_r = uart_rd();					// recebe valor da uart
							if(dado_r != 0x00)					// verifica se mudou valor originainal
								estados = config_sys;			// se sim - prox estado
							else
								estados = stanby_stat;			//se nao - mesmo estado
							break;
							
				case config_sys:
						// config recebido uart
							adc_canal = dado_r & 0xF0;			// zera LSB
							dac_canal = dado_r & 0x0F;			// zera MSB
							
							if(dac_canal < 0x01 && dac_canal > 0x08)	// verifica se eh um valor valido
								dac_canal = 0x01;					// caso nao seja seta para canal 1
							
							if(adc_canal < 0x0F && adc_canal > 0x70)	// verifica se eh um valor valido
								adc_canal = 0x00;					// caso nao seja seta para canal 1
							
							dac_channel_select_on(dac_canal);	// liga canal do DAC desejado
							dac_ref_on();						// liga referencia interna do dac
							
							adc_easy_setup(ACAL | BUFEN, adc_canal | N_AINCOM, CLK_0, R30KSPS); // liga adc com canal selecionado a 30 kSPS
							
							estados = dac_zero;					// prox estado
							break;

				
				case dac_zero:
						// Inicia DAC com um valor padrao
							dac_canal_mux(dac_canal, dac_simetrico);	// coloca valor de saida no canal desejado
							estados = adc_read;						// prox estado
							break;
					
				case adc_read:
						// realiza leitura
							adc_dado = adc_read_data();				// recebe valor lido do adc	
							
							if (adc_dado >= 0x00051EB9)				//verifica se valor for maior que 0.1 V
								estados = send_data;				// se sim - estado que envia dado
							else
								estados = dac_set;					// se nao - estado que seta dac
							break;
						
				case send_data:
						// send data
							desligar_receptor();
							uart_wr(ADC_SIGNAL);					// comando que avisa o PC que receberá o valor do ADC
							uart_wr(adc_dado>>16);					// envia MSB do dado do adc
							uart_wr(adc_dado>>8);					// envia byte do meio do dado do adc	
							uart_wr(adc_dado);						// envia LSB do dado do adc
							ligar_receptor();
							
							estados = dac_set;
							break;
						
				case dac_set:
						// dac set
							if(dac_ctrl == 0)						// verifica se esta incrementando
								dac_simetrico++;					// incrementa
								if(dac_simetrico >= 0xFF7C)			// verifica se chegou ao valor max
									dac_ctrl = 1;					// se sim - inverte sentido da rampa do DAC
							if(dac_ctrl == 1)						// verifica se esta decrementando
								dac_simetrico--;					// decrementa
								if(dac_simetrico <= 0x0082)			// verifica se chegou no valor min
									dac_ctrl = 0;					// se sim - inverte sentido da rampa
							
							estados = dac_check;
							break;

				case dac_check:
						// dac_check
							uart_wr(DAC_SIGNAL);							// comando que avisa o PC que receberá o valor do DAC
							uart_wr(dac_simetrico>>8);
							uart_wr(dac_simetrico);
							
							estados = update;
							break;
							
				case update_sys:
						// update
							dado_r = 0x00;						// seta valor inicial do dado
							dado_r = uart_rd();					// recebe valor da uart
							if(dado_r != 0x00)					// verifica se mudou valor originainal
								estados = stanby_stat;			// se sim - prox estado
							else
								estados = adc_read;			//se nao - mesmo estado
							break;
			}
		}
		else
		{
			estados = begin_sys;
		}
	}
}

*/









