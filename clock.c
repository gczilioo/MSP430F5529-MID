/*
 * clock.c
 *
 *  Created on: 25/08/2017
 *      Author: Helio Rodrigues
 */

#include "msp430f5438a.h"
#include "clock.h"

//=============================================================================
//Campinas, 14/02/2012
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//DEMIC - UNICAMP
//Arquivo: clock.h
//=============================================================================

//============================Dispositivo======================================
// uC: MSP430F5438
// Versão: 1
//=============================================================================

//============================Descrição========================================
// Esta rotina inicia o cristal que deve estar conectado nos pinos P7.0 e P7.1,
// junto com os capacitores de carga. É importante ajustar o Vcore para a tensão
// apropriada da freqüência do cristal. Para ajustar o Vcore inclua os arquivos
// "hal_pmm.h" e "hal_pmm.c" e utilize a função "SetVCore(PMMCOREV_x)" onde "x"
// é o nível de tensão desejado.
// Esta rotina também ajusta o clock do cristal para o MCLK e para ACLK e
// direciona o DCO para o SMCLK.
// O pino P11.0 gera uma onda quadrada na freqüência do ACLK/4.
// O pino P11.1 gera uma onda quadrada na freqüência do MCLK.
// O pino P11.2 gera uma onda quadrada na freqüência do SMCLK.
//=============================================================================

//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador
// MSP430F5438.
//=============================================================================


void iniciar_cristal(void)
{
    //=============================================================================
    // Configuracao dos pinos e registradores
    //=============================================================================
      volatile unsigned int i;

      WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

      P2DIR |= BIT2;                            // SMCLK set out to pins
      P2SEL |= BIT2;
      P7DIR |= BIT7;                            // MCLK set out to pins
      P7SEL |= BIT7;

      // Increase Vcore setting to level3 to support fsystem=25MHz
      // NOTE: Change core voltage one level at a time..
      SetVcoreUp (0x01);
      SetVcoreUp (0x02);
      SetVcoreUp (0x03);

      UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
    //  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

      __bis_SR_register(SCG0);                  // Disable the FLL control loop
      UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
      UCSCTL1 = DCORSEL_7;                      // Select DCO range 50MHz operation
      UCSCTL2 = FLLD_0 + 762;                   // Set DCO Multiplier for 25MHz
                                                // (N + 1) * FLLRef = Fdco
                                                // (762 + 1) * 32768 = 25MHz
                                                // Set FLL Div = fDCOCLK/2
      __bic_SR_register(SCG0);                  // Enable the FLL control loop

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
      __delay_cycles(782000);

      // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
      do
      {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                                // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
      }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

}
//============================Clock_Setup======================================
// 
// Rotina para configurar o setup de todas as portas
// 
//=============================================================================
//
// Rotina destinada a configuracao simultanea de todas as fontes de clocks
// As macros que servirao para a entrada dos parametros estao definida no
// arquivo clock.h
//
void clock_setup_full(unsigned int aclk_source, unsigned int smclk_source, unsigned int mclk_source)
{
	unsigned int layer = UCSCTL4; // inicializa layer com o valor de UCSCTL4
	
//=============================================================================	
// Configuracao dos pinos e registradores
//=============================================================================	
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  P2DIR |= BIT2;                            // SMCLK set out to pins
  P2SEL |= BIT2;
  P7DIR |= BIT7;                            // MCLK set out to pins
  P7SEL |= BIT7;

  // Increase Vcore setting to level3 to support fsystem=25MHz
  // NOTE: Change core voltage one level at a time..
  SetVcoreUp (0x01);
  SetVcoreUp (0x02);
  SetVcoreUp (0x03);

	UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
//	UCSCTL4 |= SELA_3;                        // Set ACLK = DCO
//	UCSCTL4 = SELS__DCOCLKDIV;				// Seta o SubMaster Clock para
											// receber o dco divido
	UCSCTL5 = DIVS_1;						// Seleciona o divisor do submaster clock
											// para 1, ou seja, SMCLK = DCO/2
												
//=============================================================================
// Configuracao dos pinos de clocks
//=============================================================================
// Variavel layer para ser usada de máscara para ir configurando as determinadas
// entradas que serao adicionadas de uma vez no registrador UCSCTL4

	// Configurando layer na posicao destinada ao ACLK
	layer &= 0xF8FF;	//operacao AND com hexadecimal para zerar bits de interesse
	
	switch(aclk_source){
		
		case 0:
				layer |= SELA__XT1CLK;
				break;
		case 1:
				layer |= SELA__VLOCLK;
				break;
		case 2:
				layer |= SELA__REFOCLK;
				break;
		case 3:
				layer |= SELA__DCOCLK;
				break;
		case 4:
				layer |= SELA__DCOCLKDIV;
				break;
		case 5:
				layer |= SELA__XT2CLK;
				break;
	}	

// Variavel layer para ser usada de máscara para ir configurando as determinadas
// entradas que serao adicionadas de uma vez no registrador UCSCTL4


	// Configurando layer na posicao destinada ao SMCLK	
	layer &= 0xFF8F;	//operacao AND com hexadecimal para zerar bits de interesse
	
	switch(smclk_source){
		
		case 0:
				layer |= SELS__XT1CLK;
				break;
		case 1:
				layer |= SELS__VLOCLK;
				break;
		case 2:
				layer |= SELS__REFOCLK;
				break;
		case 3:
				layer |= SELS__DCOCLK;
				break;
		case 4:
				layer |= SELS__DCOCLKDIV;
				break;
		case 5:
				layer |= SELS__XT2CLK;
				break;
	} 
 
	// Configurando layer na posicao destinada ao MCLK 
	layer &= 0xFFF8;	//operacao AND com hexadecimal para zerar bits de interesse
	
	switch(mclk_source){
		
		case 0:
				layer |= SELM__XT1CLK;
				break;
		case 1:
				layer |= SELM__VLOCLK;
				break;
		case 2:
				layer |= SELM__REFOCLK;
				break;
		case 3:
				layer |= SELM__DCOCLK;
				break;
		case 4:
				layer |= SELM__DCOCLKDIV;
				break;
		case 5:
				layer |= SELM__XT2CLK;
				break;
	}
	
	// Coloca o valor com as configuracoes de clock no registrador responsavel
	UCSCTL4 = layer; // retorna o valor de layer com as atualizacoes do clock
//=============================================================================

 __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_6;                      // Select DCO range operation
  UCSCTL2 = FLLD_0 + 609;                   // Set DCO Multiplier for 762 para 25MHz (609 para 20 MHz) (304 para 10MHz)
                                            // (N + 1) * FLLRef = Fdco
                                            // (762 + 1) * 32768 = 25MHz
											// (609 + 1) * 32768 = 20MHz
                                            // (304 + 1) * 32768 = 10MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
  // 32 x 32 x 20 MHz / 32,768 Hz ~ 625k
  // 32 x 32 x 10 MHz / 32,768 Hz ~ 312,5k
  __delay_cycles(625000);
/*
  // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
*/
	
//=============================================================================
//=============================================================================

//=============================================================================
}

//============================Clock_Setup_Smclk======================================
// 
// Rotina para configurar o clock do SMCLK
// 
//=============================================================================

void clock_setup_smclk(unsigned int smclk_source)
{
	if(smclk_source>5)	// verifica se o valor recebido é um valor valido
		return;						 // caso nao seja encerra a funcao

	
	unsigned int layer = UCSCTL4; // inicializa layer com o valor de UCSCTL4
	
	layer &= 0xFF8F;	//operacao AND com hexadecimal para zerar bits de interesse
	
	switch(smclk_source){
		
		case 0:
				layer |= SELS__XT1CLK;
				break;
		case 1:
				layer |= SELS__VLOCLK;
				break;
		case 2:
				layer |= SELS__REFOCLK;
				break;
		case 3:
				layer |= SELS__DCOCLK;
				break;
		case 4:
				layer |= SELS__DCOCLKDIV;
				break;
		case 5:
				layer |= SELS__XT2CLK;
				break;
	}

	UCSCTL4 = layer; // retorna o valor de layer com as atualizacoes do clock
}

//============================Clock_Setup_Mclk======================================
// 
// Rotina para configurar o clock do MCLK
// 
//=============================================================================

void clock_setup_mclk(unsigned int mclk_source)
{
	if(mclk_source>5)		// verifica se o valor recebido é um valor valido
		return;						 // caso nao seja encerra a funcao
	
	unsigned int layer = UCSCTL4; // inicializa layer com o valor de UCSCTL4
	
	layer &= 0xFFF8;	//operacao AND com hexadecimal para zerar bits de interesse
		
	switch(mclk_source){
		
		case 0:
				layer |= SELM__XT1CLK;
				break;
		case 1:
				layer |= SELM__VLOCLK;
				break;
		case 2:
				layer |= SELM__REFOCLK;
				break;
		case 3:
				layer |= SELM__DCOCLK;
				break;
		case 4:
				layer |= SELM__DCOCLKDIV;
				break;
		case 5:
				layer |= SELM__XT2CLK;
				break;
	}

	UCSCTL4 = layer; // retorna o valor de layer com as atualizacoes do clock

}

void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}
