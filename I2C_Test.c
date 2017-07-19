#include "C_Types.h"
#include "I2C.h"
#include"GPIO_8148.h"
#include"TIMER_8148.h"

#define IMG_EXPOSE_F 	GPP18	//GPIO_1
#define IMG_OE_F 		GPP28	//GPIO_1
#define IMG_STANDBY_F 	GPP29	//GPIO_1
#define IMG_RESET_F 	GPP30	//GPIO_1
#define IMG_FLASH_F 	GPP17	//GPIO_2

#define IMG_EXPOSE_I 	GPP25	//GPIO_0
#define IMG_OE_I 		GPP26	//GPIO_0
#define IMG_STANDBY_I 	GPP27	//GPIO_0
#define IMG_RESET_I 	GPP28	//GPIO_0
#define IMG_FLASH_I 	GPP11	//GPIO_3

#define CODEC_RELEASE	GPP18

void USTIMER2_delay(UNS32 uiDelay)
{
	//DC4_Timer_Rate_Sel(TIMER1_BASE_ADDR, uiDelay, ONE_SHOT_MODE);
	uiDelay = uiDelay * 2000;
	while(uiDelay--);
}

UNS16 I2Cslavetest(UNS8 *ucdatabuffer,UNS16 usWR_CNT,UNS16 usRD_CNT,UNS32 ucCntrlSelect, UNS8 ucSlaveAddr)
{
	UNS8 ucStatus = 1; //Set to Err
	//UNS8 ucSlaveAddr[4] = {0x21,0x10,0x18,0xC};
	//UNS8 ucSlaveSelect;

	/*WRITE OPERATION*/
	if( !DC4_I2C_ReadrWrite(ucCntrlSelect, ucSlaveAddr,ucdatabuffer,&usWR_CNT,(I2C_WRITE | I2C_START | I2C_STOP)) )
	{
		/*READ OPERATION */
		/*write the register address that we want to read*/
		if( !DC4_I2C_ReadrWrite(ucCntrlSelect, ucSlaveAddr, ucdatabuffer, &usRD_CNT, (I2C_WRITE | I2C_START)) )
		{	
			/* clock out the register data*/
			if( !DC4_I2C_ReadrWrite(ucCntrlSelect, ucSlaveAddr, ucdatabuffer, &usRD_CNT, (I2C_START | I2C_READ | I2C_STOP)) )
			ucStatus = 0; //Communicated properly without error	
		}
		//printf("\n I2C Data %x", (ucdatabuffer[1] | (ucdatabuffer[0] << 8)));
	}
//	return (ucdatabuffer[1] | (ucdatabuffer[0] << 8));
	return ucStatus;

}

void Config_ForwardImager()
{
	//Configure DM8148 Pins as GPIO which connects to Forward imager control signals
#if 1
	*((VUNS32 *)0x48140958) = 0x80; //PINCNTL87 GPIO_1[18], IMG_EXPOSE_F
	*((VUNS32 *)0x481409FC) = 0x80; //PINCNTL128 GPIO_1[28], IMG_OE_F
	*((VUNS32 *)0x48140A08) = 0x80; //PINCNTL131 GPIO_1[29], IMG_STANDBY_F
	*((VUNS32 *)0x48140A0C) = 0x80; //PINCNTL132 GPIO_1[30], IMG_RESET_F
	*((VUNS32 *)0x48140A5C) = 0x80; //PINCNTL152 GPIO_2[17], IMG_FLASH_F
#endif
	DC4_GPIO_Init(GPIO1_BASE);
	DC4_GPIO_Init(GPIO2_BASE);

	DC4_GPIO_ConfigDir(GPIO1_BASE, (IMG_EXPOSE_F), OUTPUT);
	DC4_GPIO_ConfigDir(GPIO1_BASE, (IMG_OE_F ), OUTPUT);
	DC4_GPIO_ConfigDir(GPIO1_BASE, (IMG_STANDBY_F ), OUTPUT);
	DC4_GPIO_ConfigDir(GPIO1_BASE, (IMG_RESET_F), OUTPUT);
	DC4_GPIO_ConfigDir(GPIO2_BASE, IMG_FLASH_F, OUTPUT);

	//DC4_GPIO_Set(GPIO1_BASE, IMG_RESET_F); //Release Reset of Imager

	DC4_GPIO_Set(GPIO1_BASE, IMG_OE_F);//Set OutputEnable to HIGH

	DC4_GPIO_Set(GPIO1_BASE, IMG_EXPOSE_F); //Off Image Expose
	DC4_GPIO_Clear(GPIO1_BASE, IMG_STANDBY_F); //Force out of standby
	USTIMER2_delay(30);//delay for 30ms

	DC4_GPIO_Clear(GPIO1_BASE, IMG_RESET_F);//Reset the Imager
	USTIMER2_delay(60);//Add delay
	DC4_GPIO_Set(GPIO1_BASE, IMG_RESET_F); //Release Reset of Imager

	USTIMER2_delay(100);//Add delay
}

void Config_InteriorImager()
{
	//Configure DM8148 Pins as GPIO which connects to Interior imager control signals
#if 1
	*((VUNS32 *)0x481408D8) = 0x80; //PINCNTL55 GPIO_0[25], IMG_EXPOSE_I
	*((VUNS32 *)0x481408DC) = 0x80; //PINCNTL56 GPIO_0[26], IMG_OE_I
	*((VUNS32 *)0x481408E0) = 0x80; //PINCNTL57 GPIO_0[27], IMG_STANDBY_I
	*((VUNS32 *)0x481408E4) = 0x80; //PINCNTL58 GPIO_0[28], IMG_RESET_I
	*((VUNS32 *)0x48140B68) = 0x80; //PINCNTL219 GPIO_3[11], IMG_FLASH_I
#endif
	DC4_GPIO_Init(GPIO0_BASE);
	DC4_GPIO_Init(GPIO3_BASE);

	DC4_GPIO_ConfigDir(GPIO0_BASE, IMG_EXPOSE_I , OUTPUT);
	DC4_GPIO_ConfigDir(GPIO0_BASE,  IMG_OE_I , OUTPUT);
	DC4_GPIO_ConfigDir(GPIO0_BASE, IMG_STANDBY_I, OUTPUT);
	DC4_GPIO_ConfigDir(GPIO0_BASE,  IMG_RESET_I, OUTPUT);
	DC4_GPIO_ConfigDir(GPIO3_BASE, IMG_FLASH_I, OUTPUT);

	DC4_GPIO_Set(GPIO0_BASE, IMG_OE_I);//Set OutputEnable to HIGH

	DC4_GPIO_Set(GPIO0_BASE, IMG_EXPOSE_I);//Off Image Expose
	DC4_GPIO_Clear(GPIO0_BASE, IMG_STANDBY_I);//Force out of standby
	USTIMER2_delay(30);//Add delay
	DC4_GPIO_Clear(GPIO0_BASE, IMG_RESET_I);//Reset the Imager
	USTIMER2_delay(60);//Add delay
	DC4_GPIO_Set(GPIO0_BASE, IMG_RESET_I);//Release Reset of Imager

	USTIMER2_delay(100);//Add delay
}

void AudioCodec_release()
{
	//*((VUNS32 *)0x48140900) = 0x80;//PINCNTL55 GPIO_1[10]
	*((VUNS32 *)0x48140A60) =  0x80;//PINCNTL153
	DC4_GPIO_Init(GPIO2_BASE);
	DC4_GPIO_ConfigDir(GPIO2_BASE, CODEC_RELEASE , OUTPUT);
	DC4_GPIO_Set(GPIO2_BASE, CODEC_RELEASE);

}

void I2C_Test_Terminal()
{
	UNS32 uiControllerAddr[4] ={I2C_CONTROLLER_0, I2C_CONTROLLER_1, I2C_CONTROLLER_2, I2C_CONTROLLER_3};
	UNS16 usSpeed[2] = {100, 400};
	UNS16 usDatacount1;
	UNS16 usDatacount2;
	/*NTSC[0x42 => 0x21], Imager_Int[0x24],Imager_FWD[0x30 => 0x18], Audio[0x18 => 0xC]*/
	/*UNS8 ucSlaveAddr[4] = {0x21,0x24,0x18,0xC};
	UNS8 ucSlaveSelect = 0;*/
	UNS8 ucCntrlSelect;
	UNS8 ucSpeedSelect;
	UNS8 I2Cdata[10];
	/*UNS8 *ucDataBuffer = 0;*/
	UNS8 IsExit = 0;

	//DC4_I2C_EnableController();

	while(!IsExit)
	{
		Display_Str(" ");
		Display_Str(" 0: I2C Configure ");
		Display_Str(" 1: Test I2C Slave Devices");
		/*Display_Str(" 2: Configure I2C Slave Devices");*/
		Display_Str(" 2: Exit");
		Display_Str(" Enter your option: ");

		switch((ReadC() - 0x30))
		{
		case 0x00:

			Display_Str(" Select Controller[0,1,2,3]:");
			ucCntrlSelect = (ReadC() - 0x30);
			Display_Str(" Select Frequency[0-100Mhz, 1-400Mhz]:");
			ucSpeedSelect = (ReadC() - 0x30);
			DC4_I2C_init(uiControllerAddr[ucCntrlSelect], usSpeed[ucSpeedSelect], 0x25);
			break;

		case 0x01:

			/********NTSC Decoder alive test********/
			DC4_I2C_init(I2C_CONTROLLER_0, 100, 0x25);
			I2Cdata[0]= 0x04;
			I2Cdata[1]= 0x10;

			//if( (I2Cslavetest(I2Cdata, 2, 1, I2C_CONTROLLER_0, 0x21) & 0xFF00) == 0x1000)
			if(I2Cslavetest(I2Cdata, 2, 1, I2C_CONTROLLER_0, 0x21))
				Display_Str(" NTSC Decoder is not alive ");
			else
				Display_Str(" NTSC Decoder is alive ");


			/********Interior IMAGER alive test********/			
			//USTIMER2_delay(60);
			//Config_InteriorImager();
			DC4_I2C_init(I2C_CONTROLLER_1, 100, 0x25);

			I2Cdata[0]= ((0x301A & 0xFF00) >> 8);
			I2Cdata[1]= ((0x301A & 0x00FF));
			I2Cdata[2]= ((0x10D8 & 0xFF00) >> 8);
			I2Cdata[3]= (0x10D8 & 0x00FF);

			//if( (I2Cslavetest(I2Cdata, 4, 2, I2C_CONTROLLER_1, 0x10) & 0xFFFF) == 0x10D8)
			if(I2Cslavetest(I2Cdata, 4, 2, I2C_CONTROLLER_1, 0x10))
				Display_Str(" Interior Imager is not alive ");
			else
				Display_Str(" Interior Imager is alive ");

			/********Forward IMAGER alive test********/
			//Config_ForwardImager();
			DC4_I2C_init(I2C_CONTROLLER_2, 100, 0x25);

			I2Cdata[0]= ((0x301A & 0xFF00) >> 8);
			I2Cdata[1]= ((0x301A & 0x00FF));
			I2Cdata[2]= ((0x10D8 & 0xFF00) >> 8);
			I2Cdata[3]= (0x10D8 & 0x00FF);

			//if( (I2Cslavetest(I2Cdata, 4, 2, I2C_CONTROLLER_2, 0x18) & 0xFFFF) == 0x10D8)			
			if(I2Cslavetest(I2Cdata, 4, 2, I2C_CONTROLLER_2, 0x18))
				Display_Str(" Forward Imager is not alive ");
			else
				Display_Str(" Forward Imager is alive ");


			/********AUDIO-CODEC alive test********/
			AudioCodec_release();
			DC4_I2C_init(I2C_CONTROLLER_3, 100, 0x25);

			I2Cdata[0]= 0x01;
			I2Cdata[1]= 0x01;
			usDatacount1 = 2;
			DC4_I2C_ReadrWrite( I2C_CONTROLLER_3, 0x18,I2Cdata, &usDatacount1,(I2C_WRITE | I2C_START | I2C_STOP));

			I2Cdata[0]= 0x01;
			I2Cdata[1]= 0x00;
			usDatacount1 = 2;
			DC4_I2C_ReadrWrite(I2C_CONTROLLER_3, 0x18,I2Cdata, &usDatacount1,(I2C_WRITE | I2C_START | I2C_STOP));

			I2Cdata[0]= 0x00;
			I2Cdata[1]= 0x00;
			//if((I2Cslavetest(I2Cdata, 2, 1, I2C_CONTROLLER_3, 0x18) & 0xFF00) == 0x0000)
			if(I2Cslavetest(I2Cdata, 2, 1, I2C_CONTROLLER_3, 0x18))
				Display_Str(" Audio-Codec is not alive ");
			else
				Display_Str(" Audio-Codec is alive ");

			break;

			/*Display_Str(" Select Slave to configure[0 - NTSC, 1-GPS Module, 2-Imager, 3-Audio Codec]:");
			ucCntrlSelect = ucSlaveSelect = (ReadC() - 0x30);
			Display_Str(" Enter Data Length:");
			usDataLen = GetHexData();
			Display_Str(" Enter Data Pattern:");
			ucDataPattern = GetHexData();

			ucDataBuffer = (UNS8*)malloc(sizeof(UNS8) * usDataLen);
			memset(ucDataBuffer, ucDataPattern, usDataLen);
			DC4_I2C_ReadrWrite(uiControllerAddr[ucCntrlSelect], ucSlaveAddr[ucSlaveSelect], ucDataBuffer, &usDataLen, (I2C_WRITE | I2C_START | I2C_STOP));
			free(ucDataBuffer);
			break;*/

		/*case 0x02:

			Display_Str(" Select Frequency[0-100Mhz, 1-400Mhz]:");
			ucSpeedSelect = (ReadC() - 0x30);
			Display_Str(" Select Slave[0 - NTSC, 1-Interior Imager/GPS Module, 2- Forward Imager, 3-Audio Codec]:");
			ucCntrlSelect = ucSlaveSelect = (ReadC() - 0x30);

			if (ucCntrlSelect == 1)
			{
				Display_Str(" Select [1-Interior Imager,2-GPS Module]:");
				ucSelect = (ReadC() - 0x30);
					if(ucSelect == 1)
						ucSlaveSelect == 0x00;slave address of interior imager
					else
						ucSlaveSelect == 0x00;slave address of GPS module
			}


			ucRegAddr = GetHexData();

			ucRegData = GetHexData();

			DC4_I2C_init(uiControllerAddr[ucCntrlSelect], usSpeed[ucSpeedSelect], 0x25);
			WRITE OPERATION
			DC4_I2C_ReadrWrite(I2C_CONTROLLER_0, 0x21,ucI2Cdata,&usDataLen,(I2C_WRITE | I2C_START | I2C_STOP));

			READ OPERATION
			write the register address that we want to read.
			DC4_I2C_ReadrWrite(uiControllerAddr[ucCntrlSelect], ucSlaveAddr[ucSlaveSelect], &ucRegAddr, &usDataLen, (I2C_WRITE | I2C_START));
			// clock out the register data.
			DC4_I2C_ReadrWrite(uiControllerAddr[ucCntrlSelect], ucSlaveAddr[ucSlaveSelect], &ucRegData, &usDataLen, ((I2C_START | I2C_READ | I2C_STOP)));
			break;
*/
		case 0x02:
			IsExit = 1;
			break;

		default:
			Display_Str(" select proper option");
			break;
		}
	}

}



