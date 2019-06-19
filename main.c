//=============================================================================
//
//		UNIVERSIDADE TECNOLOGICA FEDERAL DO PARANA - UTFPR
//
//		Trabalho de Conclusao de Curso		
//
//		Sistema Eletronico para medida de temperatura em fibra optica 
//		utilizando grade de Bragg
//
//		Iniciacao Tecnologica
//
//	Toledo, 	26/05/2019
//
//	Autores: 	Gabriel Cacilho Zilio
//				Matheus Henrique Mayer Campos
//
//	Arquivo: main.c
//
//=============================================================================
//============================Dispositivo======================================
//	uC: MSP430F55529
//	Versão: 1
//=============================================================================
//============================Descrição========================================
//
//	Funcao main que acolhe a maquina de estados que faz a amostragem dos canais do ADC 
//	e realiza a leitura de V_FBG e V_TH para encontrar o pico da grade e os valores necessarios
//	Tambem controla o excursionamento do DAC_TEC, inicializa o DAC_TEC no limite minimo de temperatura
//
//=============================================================================
//============================OBS==============================================
// 	Este arquivo foi criado para os projetos de Trabalho de Conclusao de Curso, mas pode
// 	ser utilizado como material didatico para aprendizado do microcontrolador
// 	MSP430F55529.
//=============================================================================
#include "msp430f5529.h"    // 	Especificar dispositivo.
#include <intrinsics.h>     // 	Utilizar funcoes intrinsecas de baixo nivel ja definidas
#include "clock.h"          // 	Clock
#include "LEDs.h"           //	Arquivo de prototipos de funcoes relacionadas aos LEDs.
#include "iso_rs_485.h"		//	Header que tem as funcoes destinadas ao isolador
#include "dac8586.h"		//	Header com funcoes destinadas ao DAC
#include "ads1256.h"		//	Header com funcoes para o ADC
#include <math.h>
//=============================================================================
//                           Definicoes
//=============================================================================
//  defines de baud rate e frequencias
#define         F_CLK         20000000              // frequencia de clock do uC
#define         F_SMCLK       F_CLK/2
#define         BAUD_RATE     500000                // baud rate da UCA1  (isolador)
#define         DAC8568_BAUD  20000000              // baud rate da UCB0  (dac)
#define         ADS1256_BAUD  1000000               // baud rate da UCA0  (adc)
#define         VELOCIDADE    F_CLK/BAUD_RATE       // prescaler da uart (isolador)
#define         VELOCIDADE_D  F_CLK/DAC8568_BAUD    // prescaler do SPI (dac8568)
#define         VELOCIDADE_A  F_CLK/ADS1256_BAUD    // prescaler do SPI (ads1256)
// defines de controle dos perifericos
#define         TEMP_LASER_MIN      0x001A1CAC      // valor setado para temperatura minima. 0,51 V -> +/- 15 graus
#define         TEMP_LASER_MAX      0x00245A1C      // valor setado para temperatura maxima. 0,71 -> +/- 35 graus
#define         V_FBG_REF           0x00400000      // valor de tensão de 1,25V para referencia
#define         DAC_START_VALUE     0x00008000      // valor para DAC ir para 0V
#define         DAC_MAX_VALUE       0x0000FFFF      // valor maximo do DAC -> 2,5V
#define         DAC_MIN_VALUE       0x00000000      // valor minimo do DAC -> -2,5V
#define         ADC_CANAL_FBG       P_AIN1          // seta canal do ADC para registrar V_FBG
#define         ADC_CANAL_TH        P_AIN8          // seta canal do ADC para registrar V_TH
#define         DAC_CANAL           0x01            // seta canal do DAC para enviar o DAC_TEC
// defines de controle do software
#define         BOTAO_2_1           !(P2IN&BIT1)    // definicao para verificar se o botao 2.1 esta pressionado
#define         BOTAO_1_1           !(P1IN&BIT1)    // definicao para verificar se o botao 2.1 esta pressionado
#define			true				0xFF			// variavel true
#define			false				0x00			// variavel falso
//=============================================================================
//                           Variaveis Globais
//=============================================================================
unsigned char       flag_start = 0;                 //  flag que identifica se a UART recebeu o comando "I"
unsigned char       flag_stop = 0;                  //  flag que identifica se a UART recebeu o comando "P"
//unsigned long int   cont_timer = 0;                 //  variavel de controle da interrupcao, ira de 0 a 4, para estourar 4x e contar 209,72ms
unsigned long int   adc_FBG;                        //  variavel que envia o valor lido pelo ADC referente a tensao recebida da grade V_FBG
unsigned long int   adc_TH;                         //  variavel que envia o valor lido pelo ADC referente a temperatura do termistor acoplado ao laser V_TH
//=============================================================================
//                           Estados Enumerado
//=============================================================================
enum estados_fsm { begin_sys, standby_stat, config_sys, dac_zero,
                    adc_read, dac_set, update_sys} estados;
//=============================================================================
//=============================================================================
//=============================================================================
//                           Inicio do programa
//=============================================================================
//=============================================================================
//=============================================================================
void main ()		//	main principal
 {
    //=============================================================================
	// Declaracoes de variaveis
    //=============================================================================
    unsigned char       dac_rise;	                                // variavel para controle da rampa do DAC
	unsigned char		flag_1	= false;							// variavel para verificar se a V_FBG está crescendo
	unsigned char		flag_2 	= false;							// variavel para verificar se a V_FBG está decrescendo
    unsigned long int   dac_simetrico = DAC_START_VALUE;            // variavel para controle DAC simétrico de -2,5 a 2,5 V, valor inicial setado para Vdac = 0 V
	//=============================================================================
	// Inicio do while infinito onde está a maquina de estados
	//=============================================================================	
    while(1)				//	while infinito onde abriga a maquina de estado
    {
        if( estados >= begin_sys && estados <= update_sys ) 		// verifica se a variavel de estados esta dentro dos limites
        {
            switch (estados)		//	funcao switch que altera o estado da maquina
            {
				//=============================================================================
				//	estado que configura as portas de clock, portas de comunicacoes com os perifericos e interrupcoes do timer e da UART
				//=============================================================================	
                case begin_sys:
                             WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT.
                            iniciar_leds();                           // Inicia os leds
							//	Configura os pinos utilizados para comunicacoes com os perifericos
                            clock_setup_full(DCO,DCOCLDIV,DCO);       // inicia sistemas de clocks
                            uart_init(VELOCIDADE);                    // inicia os pinos de comunicacoes do isolador
                            dac_com_init(VELOCIDADE_D);               // inicia os pinos de comunicacoes do DAC
                            adc_com_init(VELOCIDADE_A);               // Inicia os pinos de comunicacoes do ADC
                            TA1CTL = TASSEL__SMCLK +
                                    MC__CONTINUOUS + TACLR + ID__8;              // aciona timer 1 SMCLK, continuo, /8
                            _BIS_SR (GIE);                                 // Interrupcao Geral habilitada
							pisca_led(2);
							// atualiza estado
                            estados = standby_stat;
                            break;
				//=============================================================================															
				//=============================================================================	
				// status em que trava o programa esperando instrucao para inicializar 
				//=============================================================================	
                case standby_stat:
                            if(flag_start == 1 || BOTAO_2_1)        // verifica se mudou valor originainal
                            {
                                pisca_led_2(1);
                                // atualiza estado
                                estados = config_sys;               // se sim - prox estado
                            }
                            else                                    //se nao - mesmo estado
                            {
                                pisca_led(1);
                                // atualiza estado
                                estados = standby_stat;
                            }
                            break;
				//=============================================================================					
				//=============================================================================
				// estados para configurar os perifericos e ativar interrupcao do TIMER
				//=============================================================================							
                case config_sys:
							pisca_led_1(1);
                            dac_channel_select_on(DAC_CANAL);   				// liga canal do DAC desejado
                            dac_ref_on();                       				// liga referencia interna do dac
                            adc_easy_setup(ACAL | BUFEN, ADC_CANAL_TH | N_AINCOM, CLK_0, R1KSPS); // liga adc com canal selecionado a 1 kSPS
                            adc_selfcal();                          			// funcao para autocalibracao do ADC
                            TA1CCTL1 = CCIE;                                  	// Ativa interrupcao de tempo para envio dos dados
							pisca_led_1(1);
							// atualiza estado
                            estados = dac_zero;                 // prox estado
                            break;
				//=============================================================================
				//=============================================================================									
				// Inicia DAC com um valor padrao
				//=============================================================================	
                case dac_zero:
                            acende_led(LED2);
							// atualiza variavel de controle com menor valor do DAC.
                            dac_simetrico = DAC_MIN_VALUE;              // valor do dac simetrico com o valor minimo para resfriar o laser
                            dac_canal_mux(DAC_CANAL, dac_simetrico);    // coloca valor de saida no canal desejado
							// algoritmo que altera o canal do ADC e ao final recebe ultimo valor lido do canal anterior							
							while(P8IN&ADC_DRDY);
                            sync_adc();
                            wakeup_adc();
                            adc_TH = adc_read_data();             // recebe valor lido do adc V_TH
							//  if que verifica se o valor ja chegou ao minimo valor para alterar estado
							//  enquanto o laser nao esfria ao minimo o V_FBG nao eh lido
                            if(adc_TH > TEMP_LASER_MIN)					// verifica se V_TH esta maior que valor minino
                            {											// este if garante que a primeira excursao comece no valor minimo
                                pisca_led_1(1);
								// atualiza estado
                                estados = dac_zero;
                            }
                            else 								// se nao - significa que chegou ao valor minimo então altera canal do
                            {
								// algoritmo que altera o canal do ADC e ao final recebe ultimo valor lido do canal anterior
                                while(P8IN&ADC_DRDY);
                                adc_wreg(MUX, ADC_CANAL_FBG | N_AINCOM);       // liga canal FBG
                                sync_adc();
                                wakeup_adc();
                                adc_TH = adc_read_data();             // recebe valor anterior de V_TH
								pisca_led_2(2);
                                dac_rise = true;						// seta dac para aquecer
								// atualiza estado
								estados = adc_read;                     // prox estado
                            }
							//	if que verifica se recebeu valor para colocar o sistema em standby
							//	possibilitando parar o sistema ainda no estado de resfriamento
                            if( flag_stop == 1 || BOTAO_1_1)                  // verifica se recebeu instrução para terminar
                            {
                                pisca_led_1(2);
                                TA1CCTL1 &= 0xFFEF;             //  Desliga interrupção de tempo
                                dac_canal_mux(DAC_CANAL, DAC_START_VALUE);  // seta valor do DAC para 0V
                                adc_reset();                     // reseta ADC e para aquisição de dados
								// atualiza estado
                                estados = standby_stat;           // se sim - prox estado
                            }
                            apaga_led(LED2);
                            break;
				//=============================================================================								
				//=============================================================================	
				// realiza leitura do ADC no canal 1 para ler V_FBG	e no canal 8 para ler V_TH		
				//=============================================================================
                case adc_read:
							pisca_led_1(1);
							// algoritmo que altera o canal do ADC e ao final recebe ultimo valor lido do canal anterior
                            while(P8IN&ADC_DRDY);
                            adc_wreg(MUX, ADC_CANAL_TH | N_AINCOM);         // liga canal TH
                            sync_adc();
                            wakeup_adc();
                            adc_FBG = adc_read_data();             // recebe valor anterior de V_FBG
							//pisca_led_1(1);
                            // algoritmo que altera o canal do ADC e ao final recebe ultimo valor lido do canal anterior
                            while(P8IN&ADC_DRDY);                           // aguarda finalizacao de conversao do ADC
                            adc_wreg(MUX, ADC_CANAL_FBG | N_AINCOM);        // altera canal do ADC
                            sync_adc();
                            wakeup_adc();
                            adc_TH = adc_read_data();                       // recebe valor lido do adc
/*
                            desligar_receptor();        // Envia valor  das vari�veis
                            // envia valor de tensao referente a temperatura do Peltier acoplado ao laser
                            uart_wr(adc_TH>>16);
                            uart_wr(adc_TH>>8);
                            uart_wr(adc_TH);
                            // envia valor de tensao gerado pelo fotodiodo acoplado ao sistema ou o sistema simulado
                            uart_wr(adc_FBG>>16);                  // envia MSB do dado do adc
                            uart_wr(adc_FBG>>8);                   // envia byte do meio do dado do adc
                            uart_wr(adc_FBG);                      // envia LSB do dado do adc
                            ligar_receptor();                       //  liga receptor
*/
							// atualiza estado
                            estados = dac_set;                  		// se nao - estado que seta e controla dac
                            break;
				//=============================================================================															
				//=============================================================================	
				// Funcao que controla a excursao do laser aquecendo ou esfriando, aumentando ou diminuindo a tensao no DAC 			
				//=============================================================================
                case dac_set:
                            pisca_led_2(1);
							/*	Este if avalia o excursionamento do laser
								O primeiro if verifica se a flag 1 esta ativada e se V_FBG eh menor que o V_REF, 
									No inicio o valor de V_FBG sera menor que o V_REF, porem a FLAG 1 INICIA COMO FALSA
								Como inicia a flag 1 como falsa, passa para o segundo if  e verifica se o V_FBG esta 
								em estado crescente e se passar por V_REF ativa flag 1
								Se a flag 1 estiver ativa significa que a excursao passou em ascendencia pela referencia
								Nas proximas iteracoes o primeiro if sera avaliado
								Se flag 1 e flag 2 forem ativas, significa que o laser passou o valor de referencia no sentido de subida
								e no valor de V_REF na descida, logo eh necessario inverter o sentido da excursao
								Caso a grade varie e o laser se perca, ele ira excursionar ate o limite do intervalo de interrogacao (15-35 graus Celsius) */
							if((adc_FBG <= V_FBG_REF) && flag_1)			// verifica se flag 1 acionada e tensão FBG passou para um valor menor do que 1.25V
								flag_2 = true;									// se sim - aciona flag 2							
							else if(adc_FBG >= V_FBG_REF)					// verifica se a tensao FBG já passou o primeiro valor de ref 1.25V e se esta subindo
								flag_1 = true;									// se sim - aciona flag 1
							// if que verifica se se o V_TH esta decrescente atraves da flag 2, e verifica qual o sentido do excursionamento
							if(flag_2 && dac_rise == true)						// se ja passou por 1.25V na subida e 1.25V na descida inverte o sentido da excursao
							{
								dac_rise = false;								// inverte sentido da excursao
								flag_1 = false;									// zera flags de controle de excursao
								flag_2 = false;
							}
							else if( flag_2 && dac_rise == false)				// se ja passou por 1.25V na subida e 1.25V na descida inverte o sentido da excursao
							{
								dac_rise = true;								// inverte sentido da excursao
								flag_1 = false;									// zera flags de controle de excursao
								flag_2 = false;
							}					
							// if que verifica se o laser esta fora dos limites de leitura
							if(adc_TH < TEMP_LASER_MIN)					// verifica se o laser esta esfriando por ter perdido a grade, e obriga aquecimento do TEC
								dac_rise = true;						//	atualiza se ter perdido
							else if(adc_TH > TEMP_LASER_MAX)					// verifica se o laser esta esquentando por ter perdido a grade, e obriga resfriamento do TEC
								dac_rise = false;						// 	atualiza
							// if que atualiza a variavel que controla a rampa do DAC atraves da flag dac_rise que altera se o o valor do DAC_TEC eh maximo ou minimo
                            if(dac_rise == true)      							// verifica sentido da excursao	
                            {
                                dac_simetrico = DAC_MAX_VALUE;          // se sim atualiza com o valor maximo
                            }
                            else if(dac_rise == false)              			// verifica sentido da excursao
                            {
                                dac_simetrico = DAC_MIN_VALUE;          // se sim atualiza com o valor minimo
                            }
							// atualiza o valor do DAC com o determinado valor setado
                            dac_canal_mux(DAC_CANAL, dac_simetrico);    // atualiza valor de saida no canal desejado
							// atualiza estado
                            estados = update_sys;
                            break;
				//=============================================================================								
				//=============================================================================	
				// estado que verifica se o programa deve continuar realizando leitura ou ir para o estado standby		
				//=============================================================================							
                case update_sys:
                            if( flag_stop == 1 || BOTAO_1_1)                // verifica se recebeu instrução para terminar
                            {
                                pisca_led_2(1);
                                TA1CCTL1 &= 0xFFEF;             			//  Desliga interrupção de tempo
                                dac_canal_mux(DAC_CANAL, DAC_START_VALUE);  // seta valor do DAC para 0V
                                adc_reset();                     			// reseta ADC e para aquisição de dados
								// atualiza estado
                                estados = standby_stat;           			// se sim - prox estado
                            }
                            else
                            {
                                pisca_led_1(1);
								// atualiza estado
                                estados = adc_read;          //se nao - verifica variação de V_FBG
                            }
                            break;
				//=============================================================================								
            } // fim do switch
        } // fim do if dos estados
        else // senao estados recebe primeiro estado
            estados = begin_sys;
    } // fechamento do while infinito
} // fim do main
//  Interrupcao UART
#pragma vector=USCI_A0_VECTOR						//	configura o vetor da UART para interrupcao
__interrupt void USCI_A0_ISR(void)					//	funcao destinada para interrupcao da UART
{
    unsigned char       dado_r;             		//	variavel criada para resgatar o valor do RXBUF
    switch(__even_in_range(UCA0IV,4)){
        case 0:break;                                // Vector 0 - no interrupt
        case 2:                                      // Vector 2 - RXIFG
            dado_r = uart_rd();						// armazena valor recebido chamando funcao que recebe dado do isolador
            if(dado_r == 'I')						// caso "I"
            {
                flag_start = 1;						// seta flag de inicio do sistema
                flag_stop = 0;						// seta fag de parada do sistema
            }
            else if(dado_r == 'P')					// se nao, caso "P"
            {
               flag_start = 0;						// seta flag de inicio do sistema
                flag_stop = 1;						// seta fag de parada do sistema
            }
            break;
        case 4:break;                                // Vector 4 - TXIFG
        default: break;
    }
}
// Interrupcao TIMER
#pragma vector = TIMER1_A1_VECTOR			//	configura o vetor do TIMER para interrupcao
__interrupt void Timer1_A1_ISR(void)		//	funcao destinada para interrupcao do TIMER
{
	TA1CCTL1 &= 0xFFFE;     		// limpa a flag da interrupcao
	//cont_timer++;
	//if(cont_timer == 2)            	// Se estourar 4x passou 209,72 ms
	//{
        desligar_receptor();    	// Envia valor  das vari�veis
        // envia valor de tensao referente a temperatura do Peltier acoplado ao laser
        uart_wr(adc_TH>>16);
        uart_wr(adc_TH>>8);
        uart_wr(adc_TH);
        // envia valor de tensao gerado pelo fotodiodo acoplado ao sistema ou o sistema simulado
        uart_wr(adc_FBG>>16);                  // envia MSB do dado do adc
        uart_wr(adc_FBG>>8);                   // envia byte do meio do dado do adc
        uart_wr(adc_FBG);                      // envia LSB do dado do adc
        ligar_receptor();						//	liga receptor
//        cont_timer = 0;
    //}
}
