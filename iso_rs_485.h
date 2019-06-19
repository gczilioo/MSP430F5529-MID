//=============================================================================
//Toledo, 10/05/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//		Gabriel Cacilho Zilio
//DEMIC - UNICAMP
//UTFPR
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


//*****************************************************************************
//=============================================================================
//                          Rotina para iniciar UART
//=============================================================================
//=============================================================================
void uart_init(unsigned int velocidade);
//=============================================================================
//=============================================================================
//Toledo, 10/05/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//		Gabriel Cacilho Zilio
//DEMIC - UNICAMP
//UTFPR
//Arquivo: iso_rs_485.h
//=============================================================================
//============================Dispositivo======================================
// uC: MSP430F5438
// Versão: 1
//=============================================================================
//============================Descrição========================================
// Esta rotina tem por finalidade configurar o BAUD RATE da UART
// Ela Configura pinos P5.4 e P5.5 como periféricos: USCIA0TXD e USCIA0RXD,
// e direciona o clock de referencial para o ACLK. A determinaçao do baud
// rate segue a seguinte fórmula: (fMCLK/BR), sendo o fMCLK o clock do ACLK.
// Lembrando que o registrador BR é de 16 bits, porém dividido em 2 bytes.
// Nesta rotina utiliza-se como parâmetro de entrada apenas o byte menos
// significativo, logo o valor de entrada inteiro deve ser de no máximo 255.
// O valor arbitrado para o projeto foi de 500kbps, como fMCLK = 25MHz,
// o valor de BR deve ser de 50 (0x32). 25M/50=500kbps
// A função também configura o pino P6.0 que está ligado no isolador nas
// portas DE e RD, que funcionam para habilitar a transmissão ou a recepção.
// E inicializa o isolador para receber dados incialmente.
//  DE em nível alto --> habilita a transmissão de um caractere pelo isolador 
//  RD em nível baixo --> habilita a recepção de um caractere pelo isolador 
//=============================================================================
//*****************************************************************************


//*****************************************************************************
//=============================================================================
//                          Rotina para escrever na UART
//=============================================================================
//=============================================================================
void uart_wr(unsigned char uart_dado);
//=============================================================================
//=============================================================================
//Toledo, 10/05/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//		Gabriel Cacilho Zilio
//DEMIC - UNICAMP
//UTFPR
//Arquivo: iso_rs_485.h
//=============================================================================
//============================Dispositivo======================================
// uC: MSP430F5438
// Versão: 1
//=============================================================================
//============================Descrição========================================
// Rotina destinada ao envio de um caractere pela UART
// Inicialmente habilita o isolador a transmitir, e aguarda para que o registrador
// de buffer de transmissao seja liberado, quando envia aguarda a confirmacao do 
// envio para novamente desabilitar a transmissao do isolador.
//=============================================================================
//*****************************************************************************

//*****************************************************************************
//=============================================================================
//                          Rotina para ler da UART
//=============================================================================
unsigned char uart_rd();
//=============================================================================
//=============================================================================
//Toledo, 10/05/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//		Gabriel Cacilho Zilio
//DEMIC - UNICAMP
//UTFPR
//Arquivo: iso_rs_485.h
//=============================================================================
//============================Dispositivo======================================
// uC: MSP430F5438
// Versão: 1
//=============================================================================
//============================Descrição========================================
// Rotina destinada ao recebimento de um caractere pela UART
// Aguarda para que o registrador de buffer de recebimento esteja com algum valor
// e passa esse valor para a variavel interna uart_dado. A funcao tem como entrada
// o valor anterior recebido pela UART, para caso o registrador retorna com um valor
// nulo, o valor de saida da funcao seja o ultimo recebido.
//=============================================================================
//*****************************************************************************

//*****************************************************************************
//=============================================================================
//                          Rotinas para desligar e ligar o receptor
//=============================================================================
void desligar_receptor(void);
void ligar_receptor(void);
//=============================================================================
//=============================================================================
//Toledo, 10/05/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com
//		Gabriel Cacilho Zilio
//DEMIC - UNICAMP
//UTFPR
//Arquivo: iso_rs_485.h
//=============================================================================
//============================Dispositivo======================================
// uC: MSP430F5438
// Versão: 1
//=============================================================================
//============================Descrição========================================
// Simples instrucoes para apenas habilitar ou desabilitar o pino que controla
// o sentido de dados do isolador
//  DE em nível alto --> habilita a transmissão de um caractere pelo isolador 
//  RD em nível baixo --> habilita a recepção de um caractere pelo isolador 
// Para utilizar UART_RD deve utilizar a funcao ligar_receptor. 
// Para utilizar UART_WR deve utilizar a funcao desligar_receptor.
//=============================================================================
//*****************************************************************************

