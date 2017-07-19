/*
 * Gpmc_8148.c
 *
 *  Created on: Jul 21, 2011
 *      Author: sailesh.myneni
 */

#include "Gpmc_8148.h"

inline static void writel(unsigned int uiVal, unsigned int *uiRegAddr)
{
	*(volatile unsigned int*)uiRegAddr = uiVal;

}

inline static unsigned int readl(unsigned int *uiRegAddr)
{
	return (*(volatile unsigned int*)uiRegAddr);

}


void DC4_Gpmc_init(UNS32 uiChipsel_num)
{
	UNS32 uiConfig_Regs = 0;

		stGpmc_Regs *pstGpmc_RegsOvly = (stGpmc_Regs *) GPMCCONTROLLER_BASE;

		/* GPMC clock enable in PRCM module */
		*pCM_ALWON_GPMC_CLKCTRL = 0x2;
		*pCM_ALWON_GPIO_1_CLKCTRL = 0x2; // Enable GPI1 System and Functional Clock

		while(*pCM_ALWON_GPMC_CLKCTRL != 0x2);
    		while(*pCM_ALWON_GPIO_1_CLKCTRL != 0x2); // Poll till Module is functional	

	    /*GPMC reset*/
		pstGpmc_RegsOvly->GPMC_SYSCONFIG=(1<<1);


		/*polling whether reset is complete or not*/
		while(!(pstGpmc_RegsOvly->GPMC_SYSSTATUS & 0x01));

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6] = (0<<6);
		//pstGpmc_RegsOvly->GPMC_CONFIG_i[0][6] = (0<<6);

		pstGpmc_RegsOvly->GPMC_SYSCONFIG=(1<<3|0<<0);

		/*GPMC register configurations to be done after reset*/

		//pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] = 0x010F1001;
		/*NOR memory type configuration*/
		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] = (0<<11|0<<10);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<13|1<<12);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<9|0<<8);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (1<<24|1<<23);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<31);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<4);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<1|1<<0);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<26|0<<25);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<30);/*single read access*/

		//pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (1<<30);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<29);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<28);/*single write access*/

		//pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (1<<28);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<27);

		/*NOR chip select configuration*/
		/*Base Address of chip select */
		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6] |= (0<<5|0<<4|0<<3|1<<2|0<<1|0<<0);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6] |= (1<<11|1<<10|0<<9|0<<8);

		//printf("pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6] = 0x%x", pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6]);

		/*NOR timing parameters configuration*/


		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+1] = 0x001F1F8F;
		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+2] = 0x001F1F8F;
		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+3] = 0x1F8F1F8F;
		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+4] = 0x0F1F1F1F;
		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+5] = 0x1F000F8F;

		/*Wait pin configuration*/
		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (0<<22|0<<21);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (1<<19|1<<18);

		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs] |= (1<<17|1<<16);


		/*Enabling the chip select after doing the necessary configurations for GPMC registers*/
		pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6] |= (1<<6);
		//pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6] = 0x00000C44;
		//printf("\n pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6] = 0x%x", pstGpmc_RegsOvly->GPMC_CONFIG_i[uiChipsel_num][uiConfig_Regs+6]);

	//DC4_NORdevice_Info();

  }

UNS16 DC4_NORdevice_Info(UNS16 *ID)
{
	volatile UNS16 *pBase;

	/*pointing to the Gpmc base address which is mapped to NOR flash*/
	pBase = (volatile UNS16 *) GPMCMEMORY_BASE;

	/*write operations required to select the AUTOSELECT mode.*/

	/*The AUTOSELECT mode gives info like manufacturer code, the device code, the protection status of each
    block (Block Protection status) and the Extended Memory Block protection indicator. */

#if 1
	*(pBase+0x555)=0xAA; /*1st write operation */

	*(pBase+0x2AA)=0x55;/*2nd write operation*/

	*(pBase+0x555)=0x90;/*3rd write operation*/
#endif


	/*Read operations required to read the info after selecting the AUTOSELECT mode*/
	ID[0] = *(pBase);

	ID[1] = *(pBase+ 0x01); /*Cycle 1*/

	ID[2] = *(pBase+ 0x0E); /*Cycle 2*/

	ID[3] = *(pBase + 0x0F); /*Cycle 3*/


	/* Read/Reset command to bring device out of AUTOSELECT mode*/

	*(pBase) = 0xF0;


	printf("\n Manufacture_Id= 0x%x ", ID[0]);
	printf("\n Device_code1= 0x%x", ID[1]);
	printf("\n Device_code2= 0x%x", ID[2]);
	printf("\n Device_code3= 0x%x", ID[3]);

}
void DC4_Nor_Reset()
{
	volatile UNS16 *pBase;

	pBase = (volatile UNS16 *) GPMCMEMORY_BASE;

	/*Reset command for the NOR device reset*/
	*(pBase) = 0xF0;

}


void DC4_Nor_ChipErase()
{
	volatile UNS16 *pBase;

	pBase = (volatile UNS16 *) GPMCMEMORY_BASE;

	*(pBase+0x555)=0xAA; /*1st write operation */

	*(pBase+0x2AA)=0x55; /*2nd write operation*/

	*(pBase+0x555)=0x80; /*3rd write operation*/

	*(pBase+0x555)=0xAA; /*4th write operation */

	*(pBase+0x2AA)=0x55; /*5th write operation*/

	*(pBase+0x555)=0x10; /*6th write operation*/

}


void DC4_Nor_BlockErase(UNS32 uiBlock_num)
{
	volatile UNS16 *pBase;

	UNS32 uiBlock;
	UNS32 uiStatus;

	uiBlock = ((uiBlock_num)*(BLOCKSIZE));
	pBase = (volatile UNS16 *) (GPMCMEMORY_BASE+uiBlock);

	*(pBase+0x555)=0xAA; /*1st write operation */

	*(pBase+0x2AA)=0x55; /*2nd write operation*/

	*(pBase+0x555)=0x80; /*3rd write operation*/

	*(pBase+0x555)=0xAA; /*4th write operation */

	*(pBase+0x2AA)=0x55; /*5th write operation*/

	*(pBase+uiBlock)=0x30; /*6th write operation*/

	uiStatus = *(pBase);
}


void DC4_Nor_PageWrite(UNS32 uiBlock_num , UNS32 uiPage_num ,UNS16 *ucBuffer)
{

	volatile UNS16 *pBase;

	UNS32 uiBlock;
	UNS32 uiIndex;
	UNS32 uiPage_offset;
	UNS32 uiStatus;

	uiBlock = ((uiBlock_num)*(BLOCKSIZE));
	uiPage_offset= ((uiPage_num)*PAGESIZE) + uiBlock;
	pBase = (volatile UNS16 *) (GPMCMEMORY_BASE+uiPage_offset);

	for(uiIndex=0; uiIndex < PAGESIZE; uiIndex++ )
	{
		/*The write operations for the program command*/

			*(pBase+0x555)=0xAA; /*1st write operation */

			uiStatus = *(pBase);

			*(pBase+0x2AA)=0x55;/*2nd write operation*/

			uiStatus = *(pBase);

			*(pBase+0x555)=0xA0;/*3rd write operation*/

			uiStatus = *(pBase);

			pBase[uiIndex] = ucBuffer[uiIndex];/*4th write operation for wrting the program address and program data*/

			uiStatus = *(pBase);
	}

}


void DC4_Nor_Write(UNS32 uiBlock_num , UNS32 uiPage_num ,UNS32 uiOffset,UNS16 *ucBuffer)
{

	volatile UNS16 *pBase;

	UNS32 uiBlock = 0;
	UNS32 uiIndex;
	UNS32 uiPage_offset;
	UNS32 uiTotal_offset;
	UNS32 uiStatus;

	uiBlock = ((uiBlock_num)*(BLOCKSIZE));
	uiPage_offset = ((uiPage_num)*PAGESIZE) + uiBlock;
	uiTotal_offset = uiPage_offset + uiOffset;
	pBase = (volatile UNS16 *) (GPMCMEMORY_BASE+uiTotal_offset);

	for(uiIndex=0; uiIndex < PAGESIZE; uiIndex++ )
	{
		/*The write operations for the program command*/

			*(pBase+0x555)=0xAA; /*1st write operation */

			uiStatus = *(pBase);

			*(pBase+0x2AA)=0x55;/*2nd write operation*/

			uiStatus = *(pBase);

			*(pBase+0x555)=0xA0;/*3rd write operation*/

			uiStatus = *(pBase);

			pBase[uiIndex] = ucBuffer[uiIndex];/*4th write operation for wrting the program address and program data*/

			uiStatus = *(pBase);

	         /* Wait for programming to complete */
	         // Wait for operation to complete
	         while(1)
	             if (pBase[uiIndex] == ucBuffer[uiIndex])
	                 break;

	}

}


void DC4_Nor_PageRead(UNS32 uiBlock_num , UNS32 uiPage_num ,UNS16 *ucBuffer)
{
	volatile UNS16 *pBase;

	UNS32 uiBlock;
	UNS32 uiIndex;
	UNS32 uiPage_offset;

	uiBlock =((uiBlock_num)*(BLOCKSIZE));
	uiPage_offset= ((uiPage_num)*PAGESIZE) + uiBlock;
	pBase = (volatile UNS16 *) (GPMCMEMORY_BASE+uiPage_offset);

	for(uiIndex=0; uiIndex < PAGESIZE; uiIndex++ )
	{
		ucBuffer[uiIndex]=pBase[uiIndex];
	}

}


UNS16 DC4_Nor_Read(UNS32 uiBlock_num , UNS32 uiPage_num ,UNS32 uiOffset,UNS16 *ucBuffer)
{
	volatile UNS16 *pBase;

	UNS32 uiBlock;
	UNS32 uiIndex;
	UNS32 uiPage_offset;
	UNS32 uiTotal_offset;

	uiBlock = ((uiBlock_num)*(BLOCKSIZE));
	uiPage_offset= ((uiPage_num)*PAGESIZE) + uiBlock;
	uiTotal_offset = uiPage_offset + uiOffset;
	pBase = (volatile UNS16 *) (GPMCMEMORY_BASE+uiTotal_offset);

	for(uiIndex=0; uiIndex < PAGESIZE; uiIndex++ )
	{
		ucBuffer[uiIndex]=pBase[uiIndex];
	}

}

void DC4_Read()
{
	volatile UNS16 *pBase;
	volatile UNS32 val;
	UNS32 i;

	pBase = (volatile UNS16 *) GPMCMEMORY_BASE;

	for(i=0;i<20;i++ )
	{
	val=*(pBase+i);
	}

}

void NOR_Flash_PwrUp(void)
{

	UNS32 uiReadVal=0;

	printf("NOR Flash Reset Release\n");

	uiReadVal = readl((UNS32 *)(GPIO_BANK1_BASE_ADDR + 0x000));/*pRBR*/
	printf("GPIO Revision  Read byte 0x%x \n",uiReadVal);

	uiReadVal = readl((UNS32 *)(GPIO_BANK1_BASE_ADDR + 0x134));/*pRBR*/
	printf("GPIO Reg GPIO_OE  Read byte 0x%x \n",uiReadVal);

	uiReadVal&= 0xFFFFFDFF;

	writel(uiReadVal, (UNS32 *)(GPIO_BANK1_BASE_ADDR + 0x134)); 	//GPIO_OE

	uiReadVal = readl((UNS32 *)(GPIO_BANK1_BASE_ADDR + 0x134));/*pRBR*/
	printf("GPIO Reg GPIO_OE  Read byte 0x%x \n",uiReadVal);

	uiReadVal = readl( (UNS32 *)(GPIO_BANK1_BASE_ADDR + 0x13C));/*pRBR*/
	printf("GPIO Reg GPIO_DATAOUT Read byte 0x%x \n",uiReadVal);

	uiReadVal|= 0x00000200;

	//Set GP1[7] pin to high
	writel(uiReadVal, (UNS32 *)(GPIO_BANK1_BASE_ADDR + 0x13C)); 	//GPIO_OE

	uiReadVal = readl((UNS32 *)(GPIO_BANK1_BASE_ADDR + 0x13C));/*pRBR*/

	printf("GPIO Reg GPIO_DATAOUT Read byte 0x%x \n",uiReadVal);


}



