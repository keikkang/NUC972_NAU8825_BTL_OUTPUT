/**************************************************************************//**
 * @file    nau8822.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 15/05/07 4:46p $
 * @brief    Read/write NAU8822 Codec via I2S/I2S interface
 *
 * @note
 * Copyright (C) 2015 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "nuc970.h"
#include "sys.h"
#include "i2s.h"
#include "i2c.h"

#define SOUND_BUFFER_DEFAULT 1024

// use master or slave mode
#define MASTER_MODE

__attribute__((aligned(32))) uint32_t u32PlayBuf[] = {
	//#include "test_sinewave.dat"
	#include "here_mono.dat"
};





void Delay(int count)
{
	volatile uint32_t i;
	for (i = 0; i < count ; i++);
}

void play_callback(uint32_t u32Sn)
{
	#ifdef DEBUG_CALLBACK
		sysprintf("\n else sn value :%d",u32Sn);
	#endif
}



/*---------------------------------------------------------------------------------------------------------*/
/*  Write 9-bit data to 7-bit address register of NAU8822 with I2C0                                        */
/*---------------------------------------------------------------------------------------------------------*/
void I2C_WriteNAU8822(uint8_t u8addr, uint16_t u16data)
{
  uint8_t TxData[2];
	uint8_t debug_tmp;
    
retry:    
	  //sysprintf("\n try ok ...");
  TxData[0] = (uint8_t)((u8addr << 1) | (u16data >> 8));
  TxData[1] = (uint8_t)(u16data & 0x00FF);
  //sysprintf("\n TxData[0],TxData[1] %d %d ...",TxData[0],TxData[1]);
  i2cIoctl(0, I2C_IOC_SET_SUB_ADDRESS, TxData[0], 0);
	Delay(2);
	debug_tmp = i2cWrite(0, &TxData[0], 2);
  if(debug_tmp != 2)
  {
	  //sysprintf("\n debug_tmp %d ...",debug_tmp);
	  Delay(100);
    goto retry;
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/*  NAU8822 Settings with I2C interface                                                                    */
/*---------------------------------------------------------------------------------------------------------*/
void NAU8822_Setup()
{
    sysprintf("\nConfigure NAU8822 ...");
		
    I2C_WriteNAU8822(0,  0x000);   /* Reset all registers */
    Delay(0x200);

    I2C_WriteNAU8822(1,  0x00F);
    I2C_WriteNAU8822(2,  0x1BF);   /* Enable L/R Headphone, ADC Mix/Boost, ADC */
    I2C_WriteNAU8822(3,  0x06F);   /* Enable L/R main mixer, DAC */
    I2C_WriteNAU8822(4,  0x010);   /* 16-bit word length, I2S format, Stereo */
    I2C_WriteNAU8822(5,  0x000);   /* Companding control and loop back mode (all disable) */


    I2C_WriteNAU8822(6,  0x000);   /* Divide by 6, 16K */
    I2C_WriteNAU8822(7,  0x006);   /* 16K for internal filter coefficients */

    I2C_WriteNAU8822(10, 0x008);   /* DAC soft mute is disabled, DAC oversampling rate is 128x */
    I2C_WriteNAU8822(14, 0x108);   /* ADC HP filter is disabled, ADC oversampling rate is 128x */
    I2C_WriteNAU8822(15, 0x0FF);   /* ADC left digital volume control */
    I2C_WriteNAU8822(16, 0x1FF);   /* ADC right digital volume control */

	
	  I2C_WriteNAU8822(43, 0x010);   /* ADC right digital volume control */
		
		
    I2C_WriteNAU8822(44, 0x000);   /* LLIN/RLIN is not connected to PGA */
    //I2C_WriteNAU8822(45, 0x090);   /* LAUXIN connected, and its Gain value is 0dB */
    //I2C_WriteNAU8822(46, 0x190);   /* RAUXIN connected, and its Gain value is 0dB */
    //I2C_WriteNAU8822(47, 0x005);   /* LAUXIN connected, and its Gain value is 0dB */
    //I2C_WriteNAU8822(48, 0x005);   /* RAUXIN connected, and its Gain value is 0dB */
    I2C_WriteNAU8822(50, 0x001);   /* Left DAC connected to LMIX */
    I2C_WriteNAU8822(51, 0x001);   /* Right DAC connected to RMIX */

    //I2C_WriteNAU8822(52, 0x039);   /* HP Volume */
    //I2C_WriteNAU8822(53, 0x139);   /* HP Volume */

    I2C_WriteNAU8822(54, 0x03F);   /* LSPKOUT Volume */
    I2C_WriteNAU8822(55, 0x03F);   /* RSPKOUT Volume */
		
		sysprintf("[OK]\n");
}

int32_t main(void)
{	
	
	outpw(REG_CLK_HCLKEN, 0x0527);
	outpw(REG_CLK_PCLKEN0, 0);
	outpw(REG_CLK_PCLKEN1, 0);
	
	sysDisableCache();
  sysFlushCache(I_D_CACHE);
  sysEnableCache(CACHE_WRITE_BACK);
  sysInitializeUART();
	
	/* Configure multi function pins to I2S */
  outpw(REG_SYS_GPG_MFPH, (inpw(REG_SYS_GPG_MFPH) & ~0x0FFFFF00) | 0x08888800);
	
    /* Configure multi function pins to I2C0 */
	outpw(REG_SYS_GPG_MFPL, (inpw(REG_SYS_GPG_MFPL) & ~0xffff) | 0x88);
    
	// Initialize I2S interface
  i2sInit();
  if(i2sOpen() != 0)
    return 0;
         
  // Select I2S function
  i2sIoctl(I2S_SELECT_BLOCK, I2S_BLOCK_I2S, 0);
  // Select 16-bit data width
  i2sIoctl(I2S_SELECT_BIT, I2S_BIT_WIDTH_16, 0);
  // Set DMA interrupt selection to half of DMA buffer
  i2sIoctl(I2S_SET_PLAY_DMA_INT_SEL, I2S_DMA_INT_EIGHTH , 0);
  //i2sIoctl(I2S_SET_REC_DMA_INT_SEL, I2S_DMA_INT_HALF, 0); 
  // Set to stereo 
  i2sIoctl(I2S_SET_CHANNEL, I2S_PLAY, I2S_CHANNEL_P_I2S_TWO); //for mono
	//i2sIoctl(I2S_SET_CHANNEL, I2S_REC, I2S_CHANNEL_R_I2S_TWO); 
  // Set DMA buffer address
  i2sIoctl(I2S_SET_DMA_ADDRESS, I2S_PLAY, (uint32_t)u32PlayBuf);	
  // Set DMA buffer length
  i2sIoctl(I2S_SET_DMA_LENGTH, I2S_PLAY,(sizeof(u32PlayBuf)/SOUND_BUFFER_DEFAULT)*SOUND_BUFFER_DEFAULT );		 
  // Select I2S format
  i2sIoctl(I2S_SET_I2S_FORMAT, I2S_FORMAT_I2S, 0);


    //12.288MHz ==> APLL=98.4MHz / 8 = 12.3MHz
    
    //APLL is 98.4MHz
  outpw(REG_CLK_APLLCON, 0xC0008028);
	// Select APLL as I2S source and divider is (7+1)
  outpw(REG_CLK_DIVCTL1, (inpw(REG_CLK_DIVCTL1) & ~0x001f0000) | (0x2 << 19) | (0x7 << 24));
	// Set sampleing rate is 16k, data width is 16-bit, stereo
  i2sSetSampleRate(12300000, 16000, 16, 2);
  // Set as master
  i2sIoctl(I2S_SET_MODE, I2S_MODE_MASTER, 0);
  // Set play and record call-back functions
  i2sIoctl(I2S_SET_I2S_CALLBACKFUN, I2S_PLAY, (uint32_t)&play_callback);
  // Initialize I2C-0 interface
  i2cInit(0);
  if(i2cOpen(0) != 0)
    return 0;
  // Set slave address is 0x1a
  i2cIoctl(0, I2C_IOC_SET_DEV_ADDRESS, 0x1a, 0);
	// I2C interface speed is 100KHz
  i2cIoctl(0, I2C_IOC_SET_SPEED, 100, 0);
    
  // Configure NAU8822 audio codec
  NAU8822_Setup();
    
  // Start playing and recording
  sysprintf("Start playing & recording ...\n");
 // i2sIoctl(I2S_SET_RECORD, I2S_START_REC, 0);
  i2sIoctl(I2S_SET_PLAY, I2S_START_PLAY, 0);
    
    while(1);    
}	
