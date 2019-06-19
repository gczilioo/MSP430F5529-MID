//=============================================================================
//	Campinas, 10/12/2018
//	Autor: Gabriel Cacilho Zilio, email: gczilioo@gmail.com
//	Universidade Tecnologica Federal do Parana - UTFPR - Campus Toledo
//	Arquivo: ads1256_5529.h
//=============================================================================

//============================Dispositivo======================================
// uC: MSP430F5529
// Versão: 1
//=============================================================================

//============================Descrição========================================
// Conjunto de fuunções que controla a comunicação entre o uC e o ADC de 24 bits.
// (asd1256 da texas instruments)
//=============================================================================

//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador 
// MSP430F5529.
//=============================================================================

#define ADC_CS    BIT1  //==> PORT8
#define ADC_DRDY  BIT2  //==> PORT8

// Registradores:
#define STATUS    0x00
#define MUX       0x01
#define ADCON     0x02
#define DRATE     0x03
#define IO        0x04
#define OFC0      0x05
#define OFC1      0x06
#define OFC2      0x07
#define FSC0      0x08
#define FSC1      0x09
#define FSC2      0x0A

// Bits de controle:

/*STATUS register bits:*/
#define ORDER     0x08  /*0 = MSB first (default), 1= LSB first*/
#define ACAL      0x04  /*Alto-Calibration on = 1/off = 0 (default)*/
#define BUFEN     0x02  /*Buffer enabled = 1/Disabled = 0 (default)*/
#define DRDY      0x01  /*Duplicates the state of the DRDY pin*/

/*MUX register bits:*/
#define PSEL0     0x10  /*Positive input Channel (AINp) select bit0*/
#define PSEL1     0x20  /*Positive input Channel (AINp) select bit1*/
#define PSEL2     0x40  /*Positive input Channel (AINp) select bit2*/
#define PSEL3     0x80  /*Positive input Channel (AINp) select bit3*/

#define NSEL0     0x01  /*Positive input Channel (AINp) select bit0*/
#define NSEL1     0x02  /*Positive input Channel (AINp) select bit1*/
#define NSEL2     0x04  /*Positive input Channel (AINp) select bit2*/
#define NSEL3     0x08  /*Positive input Channel (AINp) select bit3*/

#define P_AIN1    0x00  /*Canal 1 como entrada positiva (default)*/
#define P_AIN2    0x10  /*Canal 2 como entrada positiva*/
#define P_AIN3    0x20  /*Canal 3 como entrada positiva*/
#define P_AIN4    0x30  /*Canal 4 como entrada positiva*/
#define P_AIN5    0x40  /*Canal 5 como entrada positiva*/
#define P_AIN6    0x50  /*Canal 6 como entrada positiva*/
#define P_AIN7    0x60  /*Canal 7 como entrada positiva*/
#define P_AIN8    0x70  /*Canal 8 como entrada positiva*/
#define P_AINCOM  0x80  /*Pino AINCOM como entrada positiva*/

#define N_AIN1    0x00  /*Canal 1 como entrada negativa*/
#define N_AIN2    0x01  /*Canal 2 como entrada negativa (default)*/
#define N_AIN3    0x02  /*Canal 3 como entrada negativa*/
#define N_AIN4    0x03  /*Canal 4 como entrada negativa*/
#define N_AIN5    0x04  /*Canal 5 como entrada negativa*/
#define N_AIN6    0x05  /*Canal 6 como entrada negativa*/
#define N_AIN7    0x06  /*Canal 7 como entrada negativa*/
#define N_AIN8    0x07  /*Canal 8 como entrada negativa*/
#define N_AINCOM  0x08  /*Pino AINCOM como entrada negativa*/

/*ADCON register bits:*/
#define CLK1      0x40  /*Clock Out Rate Setting bit1*/
#define CLK0      0x20  /*Clock Out Rate Setting bit0*/

#define CLK_0     0x00  /*Clock Out OFF*/
#define CLK_1     0x20  /*Clock Out Fin (default)*/
#define CLK_2     0x40  /*Clock Out Fin/2*/
#define CLK_3     0x60  /*Clock Out Fin/4*/

#define SDCS1     0x10  /*Sensor Detect Current Sources bit1*/
#define SDCS0     0x08  /*Sensor Detect Current Sources bit1*/

#define SDCS_0    0x00  /*Sensor Detect OFF (Default)*/
#define SDCS_1    0x08  /*Sensor Detect Current = 0.5uA*/
#define SDCS_2    0x10  /*Sensor Detect Current = 2uA*/
#define SDCS_3    0x18  /*Sensor Detect Current = 10uA*/

#define PGA0      0x01  /*PGA bit0*/
#define PGA1      0x02  /*PGA bit1*/
#define PGA2      0x04  /*PGA bit2*/

#define PGA_1     0x00  /*Gain 1* (Default)*/
#define PGA_2     0x01  /*Gain 2*/
#define PGA_4     0x02  /*Gain 4*/
#define PGA_8     0x03  /*Gain 8*/
#define PGA_16    0x04  /*Gain 16*/
#define PGA_32    0x05  /*Gain 32*/
#define PGA_64    0x06  /*Gain 64*/

/*DRATE register bits:*/
#define R30KSPS   0xf0  /*30000 amostras por segundo*/
#define R15KSPS   0xe0  /*15000 amostras por segundo*/
#define R7d5KSPS  0xd0  /*7500 amostras por segundo*/
#define R3d75KSPS 0xc0  /*3750 amostras por segundo*/
#define R2KSPS    0xb0  /*2000 amostras por segundo*/
#define R1KSPS    0xa1  /*1000 amostras por segundo*/
#define R500SPS   0x92  /*500 amostras por segundo*/
#define R100SPS   0x82  /*100 amostras por segundo*/
#define R60SPS    0x72  /*60 amostras por segundo*/
#define R50SPS    0x63  /*50 amostras por segundo*/
#define R30SPS    0x53  /*30 amostras por segundo*/
#define R25SPS    0x43  /*25 amostras por segundo*/
#define R15SPS    0x33  /*15 amostras por segundo*/
#define R10SPS    0x23  /*10 amostras por segundo*/
#define R5SPS     0x13  /*5 amostras por segundo*/
#define R2d5SPS   0x03  /*2.5 amostras por segundo*/

/*GPIO register bits:*/
#define DIR0      0x10
#define DIR1      0x20
#define DIR2      0x40
#define DIR3      0x80

#define DIO0      0x01
#define DIO1      0x02
#define DIO2      0x04
#define DIO3      0x08

// Constantes:
#define CYCLES    10                        // Clock do uC dividido pelo SLK do adc
#define WREG_M0		0x50


//=============================================================================
//                          Rotina para iniciar a porta de comunicação
//=============================================================================
void adc_com_init(unsigned long int velocidade)
{
  P3SEL |= BIT0 + BIT1 + BIT2;              // P3.0,1,2 option select.
  
// Seta os chip select.
  P8DIR |= ADC_CS;                          //
  P8OUT |= ADC_CS;                          //
   
  
  UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCB0CTL0 |= UCMST+UCSYNC+UCMSB;           // 3-pin, 8-bit SPI master
                                            // Clock polarity low, MSB.
  UCB0CTL1 |= UCSSEL__ACLK;                // MCLK
  UCB0BR0 = velocidade;                     // fator de divisão do clock.
  UCB0BR1 = 0;                              //                                           
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//  UCA0IE |= UCRXIE;                       // Enable USCI_A0 RX interrupt .
}

//=============================================================================
//                          Rotina para enviar um comando no ADC1
//=============================================================================
void adc_wreg(unsigned char registrador, unsigned char valor)
{
  registrador |= 0x50;
  P8OUT &= ~ADC_CS;                         // Seleciona o ADC2
 
  while(UCB0STAT& 0x01);

  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = registrador;                  // Envia parte alta do comando
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x00;                        // Envia parte baixa do comando 
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = valor;                        // Envia parte baixa do comando  
  
  while(UCB0STAT&UCBUSY);                   // Aguarda ultimo sinal de SLK
  __delay_cycles(CYCLES*4);                 // Tempo necessário para levantar o CS
  P8OUT |= ADC_CS;                          // Levanta o CS
}


//=============================================================================
//                          Rotina para ler registrador do ADC1
//=============================================================================
unsigned char adc_rreg(unsigned char registrador)
{
  unsigned char valor;
  registrador |= 0x10;
  P8OUT &= ~ADC_CS;                         // Seleciona o ADC
 
  while(UCB0STAT& 0x01);

  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = registrador;                  // Envia parte alta do comando
  while(UCB0STAT&UCBUSY);
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x00;                         // Envia parte baixa do comando
  while(UCB0STAT&UCBUSY);
  
  valor = UCB0RXBUF;                        // elimina o primeiro dado
  
  __delay_cycles(CYCLES*50);                // Delay from last SCLK edge for DIN to first SCLK rising edge for DOUT
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x00;
  while (!(UCB0IFG&UCRXIFG));               // USCI_A0 RX buffer ready?
  valor = UCB0RXBUF;                      //==> primeiro byte MSB  
  
  while(UCB0STAT&UCBUSY);                   // Aguarda ultimo sinal de SLK
  __delay_cycles(CYCLES*4);                 // Tempo necessário para levantar o CS
  P8OUT |= ADC_CS;                          // Levanta o CS
  
  return valor;
}


//=============================================================================
//                          Rotina para resetar o ADC1
//=============================================================================
void adc_reset(void)
{
  P8OUT &= ~ADC_CS;                         // Seleciona o ADC1

  while(UCB0STAT& 0x01);

  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0xFE;                         // Envia parte alta do comando
  __delay_cycles(CYCLES*4);                 // Tempo necessário para levantar o CS
  while(UCB0STAT&UCBUSY); 
  P8OUT |= ADC_CS;
}


//=============================================================================
//                          Rotina para Calibração do ADC1
//=============================================================================
void adc_selfcal(void)
{
  P8OUT &= ~ADC_CS;                         // Seleciona o ADC1

  while(UCB0STAT& UCBUSY);

  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0xF0;                         // Envia parte alta do comando
  while(UCB0STAT&UCBUSY);
  __delay_cycles(CYCLES*4);                 // Tempo necessário para levantar o CS
  P8OUT |= ADC_CS;
  while(P8IN&ADC_DRDY);                         // Espera o sinal DRDY ==> fim de calibração
 
}


//=============================================================================
//                          Rotina para ler dado do ADC1
//=============================================================================
unsigned long int adc_read_data(void)
{
  unsigned long int leitura = 0;
  
  while(UCB0STAT& UCBUSY);
    
  P8OUT &= ~ADC_CS;                         // Seleciona o ADC1
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x01;                         // Envia comando
  while(UCB0STAT&UCBUSY);
  while(!(UCB0IFG&UCRXIFG));                // USCI_A0 RX buffer ready?
  leitura = UCB0RXBUF;                      //==> elimina primeiro dado
  
  __delay_cycles(CYCLES*50);                // Delay from last SCLK edge for DIN to first SCLK rising edge for DOUT
  
  while(UCB0STAT&UCBUSY);
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x00;
  while (!(UCB0IFG&UCRXIFG));               // USCI_A0 RX buffer ready?
  leitura = UCB0RXBUF;                      //==> primeiro byte MSB
  
  while(UCB0STAT&UCBUSY);
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x00; 
  while (!(UCB0IFG&UCRXIFG));               // USCI_A0 RX buffer ready?
  leitura = UCB0RXBUF + leitura*256;        //==> segundo byte
  
  while(UCB0STAT&UCBUSY);
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x00;    
  while (!(UCB0IFG&UCRXIFG));               // USCI_A0 RX buffer ready?
  leitura = UCB0RXBUF + leitura*256;        //==> terceiro byte LSB
  
  while(UCB0STAT&UCBUSY);
  __delay_cycles(CYCLES*10);                 // Tempo necessário para levantar o CS
  P8OUT |= ADC_CS;  
  
  return leitura;
}


//=============================================================================
//                          Easy setup do ADC1
//=============================================================================
void adc_easy_setup(unsigned char status, unsigned char mux, unsigned char adcon, unsigned char drate)
{
  
  P8OUT &= ~ADC_CS;                         // Seleciona o ADC2

  while(UCB0STAT& 0x01);
 
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = WREG_M0;                         // Envia comando de endereçar na memoria comecando do zero
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x03;                         // Envia quantidade de bytes que ira ser mandado N-1
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = status;                       // Envia parte baixa do comando 
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = mux;
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = adcon;
  
  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = drate;
  
  while(UCB0STAT&UCBUSY);                   // Aguarda ultimo sinal de SLK
  __delay_cycles(CYCLES*4);                 // Tempo necessário para levantar o CS
  P8OUT |= ADC_CS;                          // Levanta o CS
}


//=============================================================================
//                          SYNC ADC1
//=============================================================================

void sync_adc(void)
{
  P8OUT &= ~ADC_CS;                         // Seleciona o ADC2
  
  while(UCB0STAT& 0x01);

  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0xfc;                         // Envia parte alta do comando
  
  while(UCB0STAT&UCBUSY);                   // Aguarda ultimo sinal de SLK
  __delay_cycles(CYCLES*4);                 // Tempo necessário para levantar o CS
  P8OUT |= ADC_CS;                          // Levanta o CS
}


//=============================================================================
//                          WAKEUP ADC1
//=============================================================================

void wakeup_adc(void)
{
  P8OUT &= ~ADC_CS;                         // Seleciona o ADC2
  
  while(UCB0STAT& 0x01);

  while (!(UCB0IFG&UCTXIFG));               // USCI_A0 TX buffer ready?
  UCB0TXBUF = 0x00;                         // Envia parte alta do comando
  
  while(UCB0STAT&UCBUSY);                   // Aguarda ultimo sinal de SLK
  __delay_cycles(CYCLES*4);                 // Tempo necessário para levantar o CS
  P8OUT |= ADC_CS;                          // Levanta o CS
}
