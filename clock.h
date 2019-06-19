/*
 * clock.h
 *
 *  Created on: 25/08/2017
 *      Author: Helio Rodrigues
 *				Gabriel Cacilho Zilio
*/

#ifndef CLOCK_H_
#define CLOCK_H_

#define XT1 0
#define VLO 1
#define REFO 2
#define DCO 3
#define DCOCLDIV 4
#define XT2 5

//=============================================================================
//=============================================================================
void iniciar_cristal(void);
//=============================================================================
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
//*****************************************************************************


//*****************************************************************************
//=============================================================================
void clock_setup_full (unsigned int, unsigned int, unsigned int);
//=============================================================================
//=============================================================================
//Toledo - PR, 19/02/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com 
//		 Gabriel Cacilho Zilio, email:gczilioo@gmail.com
//DEMIC - UNICAMP
//UTFPR 
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
// Esta rotina também ajusta o clock do cristal para o MCLK, ACLK e SMCLK
// simultaneamente, de acordo com o configurado pelo usuario.
// Os três parâmetros de entrada são destinado a configuracao desejada para os
// seguintes clocks aclk_source (Pino P11.0), smclk_source (Pino P11.2) e 
// mclk_source(Pino P11.1) nesta sequencia. O valor adequado para configuracao dos 
// pinos está definido no cabecalho do arquivo clock.h (linha 11 a 15).
// A estrutura inicial do programa é identica a funcao iniciar_cristal. Porém,
// após a configuraçao inicial dos registradores, o programa identifica e seleciona
// qual foi o source setado para determinado clock e configura cada um.
// Esta identificacao é feita através de um switch_case.
//
//=============================================================================
//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador
// MSP430F5438.
//=============================================================================
//*****************************************************************************

//*****************************************************************************
//=============================================================================
void clock_setup_aclk (unsigned int);
//=============================================================================
//=============================================================================
//Toledo - PR, 19/02/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com 
//		 Gabriel Cacilho Zilio, email:gczilioo@gmail.com
//DEMIC - UNICAMP
//UTFPR 
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
// Esta rotina também ajusta o clock do cristal para o ACLK. O programa identifica 
// e seleciona qual foi o source setado para determinado clock e configura.
// Esta identificacao é feita através de um switch_case.
//
//=============================================================================
//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador
// MSP430F5438.
//=============================================================================
//*****************************************************************************

//*****************************************************************************
//=============================================================================
void clock_setup_smclk (unsigned int);
//=============================================================================
//=============================================================================
//Toledo - PR, 19/02/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com 
//		 Gabriel Cacilho Zilio, email:gczilioo@gmail.com
//DEMIC - UNICAMP
//UTFPR 
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
// Esta rotina também ajusta o clock do cristal para o SMCLK. O programa identifica 
// e seleciona qual foi o source setado para determinado clock e configura.
// Esta identificacao é feita através de um switch_case.
//
//=============================================================================
//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador
// MSP430F5438.
//=============================================================================
//*****************************************************************************

//*****************************************************************************
//=============================================================================
void clock_setup_mclk (unsigned int);
//=============================================================================
//=============================================================================
//Toledo - PR, 19/02/2018
//Autor: Felipe W. D. Pfrimer, email:fpfirmer@gmail.com 
//		 Gabriel Cacilho Zilio, email:gczilioo@gmail.com
//DEMIC - UNICAMP
//UTFPR 
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
// Esta rotina também ajusta o clock do cristal para o MCLK. O programa identifica 
// e seleciona qual foi o source setado para determinado clock e configura.
// Esta identificacao é feita através de um switch_case.
//
//=============================================================================
//============================OBS==============================================
// Este arquivo foi criado para os projetos Gradscope e Braggsensor, mas pode
// ser utilizado como material didatico para aprendizado do microcontrolador
// MSP430F5438.
//=============================================================================
//*****************************************************************************
void SetVcoreUp (unsigned int);

#endif /* CLOCK_H_ */
