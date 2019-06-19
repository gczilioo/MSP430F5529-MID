//=============================================================================
//Toledo, 10/05/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//DEMIC - UNICAMP
//Arquivo: iso_rs_485.h
//=============================================================================

//============================Dispositivo======================================
// uC: MSP430F5438
// Versão: 1
//=============================================================================

//============================Descrição========================================
// Conjunto de funções que controla a comunicação serial entre a placa de 
// controle e o PC.
//=============================================================================

//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador 
// MSP430F5438.
//=============================================================================
#include "iso_rs_485.h"
#include "msp430f5529.h"
//=============================================================================
//                          Rotina para iniciar UART
//=============================================================================

void uart_init(unsigned int velocidade)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P3SEL |= BIT3 + BIT4;                     // Configura pinos P3.3 e P3.4 como periféricos: USCIA0TXD e USCIA0RXD, respectivamente.
  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL__ACLK;                // Configura clock do módulo USCI igual clock auxiliar (CLK = SMCLK).
  UCA0BR0 = velocidade;                     // Determinação do BAUD RATE: fMCLOCK/BR = numero inteiro (Ex: P/ 25MHz 0x32 --> BAUD RATE de 500 kbps)
  UCA0BR1 = 0;
  UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0 (???)
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
  
//  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
//  __no_operation();                         // For debugger
// ===========================================================================
//  DE em nível alto --> habilita a transmissao de um caractere pelo isolador 
//  RD em nível baixo --> habilita a recepcao de um caractere pelo isolador   
// ===========================================================================
  P2DIR |= BIT7;                            // Configura o pino P6.0 como saída (DE e RD do isolador)
  P2OUT &=~ BIT7;                           // Reseta o pino P6.0 (habilita o isolador para receber dados)
} 

//=============================================================================
//                          Rotina para escrever na UART
//=============================================================================
void uart_wr(unsigned char uart_dado)
{
  P2OUT |= BIT7;		// Realiza operacao logica: P2OUT OR BIT7 (1000_0000) para ligar a transmissao
  while (!(UCA0IFG&UCTXIFG));    // O buffer USCI_A0 TX esta pronto para poder receber o dado?
  while(UCA0STAT&UCBUSY);       // Esta transmitindo algo?
  UCA0TXBUF = uart_dado;        // Escreve no buffer de envio o valor a ser enviado
  while(UCA0STAT&UCBUSY);       // Aguarda para terminar o envio
  P2OUT &=~ BIT7;               // Zera o P2.7 para habilitar novamente a recepcao
}

//=============================================================================
//                          Rotina para ler da UART
//=============================================================================
unsigned char uart_rd()
{
  while(UCA0STAT&UCBUSY);
  unsigned char uart_dado = UCA0RXBUF;
  return uart_dado;
}

void desligar_receptor(void)
{
  P2OUT |= BIT7;
}

void ligar_receptor(void)
{
  P2OUT &=~ BIT7;
}
