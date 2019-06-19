/*
 * dac8586.h
 *
 *  Created on: 19 de mai de 2018
 *      Author: Gabriel
 */

 #ifndef DAC8586_H_
#define DAC8586_H_

 
//=============================================================================
//Campinas, 14/02/2012
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//DEMIC - UNICAMP
//Arquivo: dac8568.h
//=============================================================================

//============================Dispositivo======================================
// uC: MSP430F5438
// Vers�o: 1
//=============================================================================

//============================Descri��o========================================
// Conjunto de fuun��es que controla a comunica��o entre o uC e o DAC de 16 bits
// dac8568 atrav�s do protocolo SPI.
//=============================================================================

//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador
// MSP430F5438.
//=============================================================================
#define SYNC  BIT2 // PORT 1
#define LDAC  BIT3 // PORT 1
#define CLR   BIT4 // PORT 1

//=============================================================================
//                          Rotina para iniciar a porta de comunica��o
//=============================================================================
void dac_com_init(unsigned long int velocidade)
{
  P4SEL |= BIT1 + BIT2 + BIT3;              // P4.1,3 option select.

  P1DIR |= BIT2 + BIT3 + BIT4;              // Bits de controle.
  P1OUT |= CLR;                             // CLR sempe nivel alto.
  P1OUT &= ~(LDAC + SYNC);                  // LDAC sempre n�vel baixo e zera.
                                            // SYNC.

  UCB1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCB1CTL0 |= UCMST+UCSYNC+UCMSB;           // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB.
  UCB1CTL1 |= UCSSEL__ACLK;                // MCLK
  UCB1BR0 = velocidade;                     // fator de divis�o do clock.
  UCB1BR1 = 0;                              //
  UCB1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//  UCB1IE |= UCRXIE;                       // Enable USCI_A0 RX interrupt .
}

//=============================================================================
//                          Sinal de SYNC
//=============================================================================
void sync(void)
{
  P1OUT |= SYNC;
  __delay_cycles(2);
  P1OUT &= ~SYNC;
}


//=============================================================================
//                          Rotina para ligar a refer�ncia interna do DAC
//=============================================================================
void dac_ref_on(void)
{
  while(UCB1STAT& 0x01);
  
  sync();
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x09;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x0A;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;
}

//=============================================================================
//                          Rotina para ligar todos os canais do DAC
//=============================================================================
void dac_channels_on(void)
{
	
  while(UCB1STAT& 0x01);
  sync();
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x04;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0xFF;

}

//=============================================================================
//                          Rotina para desligar todos os canais do DAC
//=============================================================================
void dac_channels_off(void)
{
  sync();
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x04;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

}

//=============================================================================
//                          Rotina para ligar  o canal A do DAC
//=============================================================================
void dac_channel_select_on(unsigned char canal)
{
	unsigned char layer;
  sync();
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x04;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

	switch (canal) 
	{  
		case 1:
			layer = 0x01;
			break;
		
		case 2:
			layer = 0x40;
			break;
		
		case 3:
			layer = 0x10;
			break;
		
		case 4:
			layer = 0x04;
			break;
		
		case 5:
			layer = 0x02;
			break;
			
		case 6:
			layer = 0x08;
			break;
		
		case 7:
			layer = 0x20;
			break;
			
		case 8:
			layer = 0x80;
			break;
		
		default: // para qualquer outro valor seta como padrao o canal 1
			layer = 0x01;
			break;
	}
	
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = layer;

}

//=============================================================================
//                          Rotina para ligar  o canal A do DAC
//=============================================================================
void dac_channel_select_off(unsigned char canal)
{
	unsigned char layer;
  sync();
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x04;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x00;

	switch (canal) 
	{  
		case 1:
			layer = 0x01;
			break;
		
		case 2:
			layer = 0x40;
			break;
		
		case 3:
			layer = 0x10;
			break;
		
		case 4:
			layer = 0x04;
			break;
		
		case 5:
			layer = 0x02;
			break;
			
		case 6:
			layer = 0x08;
			break;
		
		case 7:
			layer = 0x20;
			break;
			
		case 8:
			layer = 0x80;
			break;
	}
	
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF ^= layer;

}

//=============================================================================
//                          Rotina para multiplexar canais do DAC
//=============================================================================

void dac_canal_mux (unsigned char canal, unsigned long int dado)
{
	dado = dado << 4;
	
	if ( canal > 0 && canal < 9)
	{
		switch (canal)
		{   
			case 1:
				break;
					
			case 2:
				dado |= 0x00600000;
				break;
			
			case 3:
				dado |= 0x00400000;
				break;
				
			case 4:
				dado |= 0x00200000;
				break;
				
			case 5:
				dado |= 0x00100000;
				break;
				
			case 6:
				dado |= 0x00300000;
				break;
				
			case 7:
				dado |= 0x00500000;
				break;
				
			case 8:
				dado |= 0x00700000;
				break;
		}

	}
	else
	{
		dado &= 0xFF8FFFFF; // realiza AND zerando para o canal 1 como padr�o
	}
		
	while(UCB1STAT& 0x01);
	sync();
  
	while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
	UCB1TXBUF = 0x03;
	
	while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
	UCB1TXBUF = dado >> 16;

	while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
	UCB1TXBUF = dado >> 8;

	while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
	UCB1TXBUF = dado;

}


//=============================================================================
//                          Rotina para atualizar o canal A (CANAL 1)
//=============================================================================
void dac_canal_1(unsigned long int dado)
{
  dado = dado << 4;
  while(UCB1STAT& 0x01);
  sync();
  
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x03; 

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 16;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 8;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado;

}

//=============================================================================
//                          Rotina para atualizar o canal G (CANAL 2)
//=============================================================================
void dac_canal_2(unsigned long int dado)
{
  dado = dado << 4;
  dado = dado|0x00600000;
  sync();
  
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x03; 

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 16;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 8;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado;

}

//=============================================================================
//                          Rotina para atualizar o canal E (CANAL 3)
//=============================================================================
void dac_canal_3(unsigned long int dado)
{
  dado = dado << 4;
  dado = dado|0x00400000;
  sync();
  
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x03; 

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 16;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 8;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado;

}

//=============================================================================
//                          Rotina para atualizar o canal C (CANAL 4)
//=============================================================================
void dac_canal_4(unsigned long int dado)
{
  dado = dado << 4;
  dado = dado|0x00200000;
  sync();
  
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x03; 

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 16;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 8;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado;

}

//=============================================================================
//                          Rotina para atualizar o canal B (CANAL 5)
//=============================================================================
void dac_canal_5(unsigned long int dado)
{
  dado = dado << 4;
  dado = dado|0x00100000;

  while(UCB1STAT & 0x01);
  sync();
  
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x03; 

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 16;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 8;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado;

}

//=============================================================================
//                          Rotina para atualizar o canal D (CANAL 6)
//=============================================================================
void dac_canal_6(unsigned long int dado)
{
  dado = dado << 4;
  dado = dado|0x00300000;
  while(UCB1STAT & 0x01);
  sync();

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x03;  
  
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 16;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 8;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado;

}

//=============================================================================
//                          Rotina para atualizar o canal F (CANAL 7)
//=============================================================================
void dac_canal_7(unsigned long int dado)
{
  dado = dado << 4;
  dado = dado|0x00500000;
  while(UCB1STAT & 0x01);
  sync();

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x03; 
 
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 16;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 8;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado;

}

//=============================================================================
//                          Rotina para atualizar o canal H (CANAL 8)
//=============================================================================
void dac_canal_8(unsigned long int dado)
{
  dado = dado << 4;
  dado = dado|0x00700000;
  while(UCB1STAT & 0x01);
  sync();
  
  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = 0x03;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 16;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado >> 8;

  while (!(UCB1IFG&UCTXIFG));           // USCI_A0 TX buffer ready?
  UCB1TXBUF = dado;

}

#endif /* DAC8586_H_ */










































