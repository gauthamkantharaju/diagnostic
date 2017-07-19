/************************************************************************************************************
 *
 * Title			: McASP_HLD.h
 *
 * Application		: Bighill project
 *
 * Author			: Mahantesh.Kumbar
 *
 * Creation Date    : 10/06/2011
 *
 * Description      : Functions prototype, Data Structure & Bit definition for McASP.c file
 *
 * Limitations      : Supported for Polling method only
 *
 *
 *----------------------------------------------------------------------------
 * Modification History
 *----------------------------------------------------------------------------
 * Ver 	Date       Initials       	Description
 *----------------------------------------------------------------------------
 * 0.1	10/06/2011 Mahantesh.Kumbar Initial Version
 *
 *
 *---------------------------------------------------------------------------
************************************************************************************************************/
#ifndef MCASP_HLL_H_
#define MCASP_HLL_H_

/*------------------------------------------- Include files -----------------------------------------------*/


/*-------------------------------- Symbolic Constants and Macros (defines) --------------------------------*/
#define FUNC_CLOCK (200 * 1000 * 1000)

#define INTERNALCLOCK 0x01
#define EXTERNALCLOCK 0x00

#define SERIALIZER_0 0x00
#define SERIALIZER_1 0x01
#define SERIALIZER_2 0x02
#define SERIALIZER_3 0x03
#define SERIALIZER_4 0x04
#define SERIALIZER_5 0x05

#define SLOTSIZE_8	0x03
#define SLOTSIZE_12	0x05
#define SLOTSIZE_16	0x07
#define SLOTSIZE_20	0x09
#define SLOTSIZE_24	0x0B
#define SLOTSIZE_28	0x0D
#define SLOTSIZE_32	0x0F

#define ROTATERIGHT_0B	 	0x00
#define ROTATERIGHT_4B 		0x01
#define ROTATERIGHT_8B 		0x02
#define ROTATERIGHT_12B 	0x03
#define ROTATERIGHT_16B 	0x04
#define ROTATERIGHT_20B 	0x05
#define ROTATERIGHT_24B		0x06
#define ROTATERIGHT_28B 	0x07

#define TXORD_MSB       BIT(15)
#define TXORD_LSB		0

/*------------------------------------------ Typedefs and structs -----------------------------------------*/
typedef struct  {
	UNS32 uiSpeed;
	UNS32 uiClockSource;
	UNS32 uiSerializer;
	UNS32 uiSlots;
	UNS32 uiSlotSize;
	UNS32 uiWordSize;
	UNS32 uiSlotDelay;
	UNS32 uiRotate;
	UNS32 uiBitorder;
}McASP_dev;

/*------------------------------------------ Function prototypes ------------------------------------------*/

/*------------------------------------ Declarations (statics and globals) ---------------------------------*/


/*---------------------------------------- Functions  Prototype ---------------------------------------------*/

/*********************************************************************************************
Function Name : DC4_Stop_TX
Description   : This function Stops the Transmitter of McASP controller.
Prototype     : void DC4_Stop_TX(UNS32 uiMcASP_Controller)
Parameters    : [I] uiMcASP_Controller - Base address of controller
Returns       : void
***********************************************************************************************/
void DC4_Stop_TX(UNS32 uiMcASP_Controller);

/*********************************************************************************************
Function Name : DC4_Start_TX
Description   : This function starts the Transmitter of McASP controller.
Prototype     : void DC4_Start_TX(UNS32 uiMcASP_Controller, UNS32 uiSerializer)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] uiSerializer - Serializer to be used transmit
Returns       : void
***********************************************************************************************/
void DC4_Start_TX(UNS32 uiMcASP_Controller, UNS32 uiSerializer);

/*********************************************************************************************
Function Name : DC4_Config_TX
Description   : This function configure the Transmitter of McASP controller.
Prototype     : void DC4_Start_TX(UNS32 uiMcASP_Controller, UNS32 uiSerializer)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] dev - Device structure which configuration data
Returns       : void
***********************************************************************************************/
UNS32 DC4_Config_TX(UNS32 uiMcASP_Controller, McASP_dev *dev);

/*********************************************************************************************
Function Name : DC4_McASP_Transmit
Description   : This function transmit the data on McASP controller.
Prototype     : void DC4_McASP_Transmit(UNS32 uiMcASP_Controller, McASP_dev *dev, void *buffer, UNS32 uiLength)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] dev - Device structure which holds configuration data
				[I] *buffer - Data buffer pointer which holds data
				[I] uiLength - Number of words to transmit
Returns       : void
***********************************************************************************************/
void DC4_McASP_Transmit(UNS32 uiMcASP_Controller, McASP_dev *dev, void *buffer, UNS32 uiLength);

/*********************************************************************************************
Function Name : DC4_Stop_RX
Description   : This function Stops the Receiver of McASP controller.
Prototype     : void DC4_Stop_RX(UNS32 uiMcASP_Controller)
Parameters    : [I] uiMcASP_Controller - Base address of controller
Returns       : void
***********************************************************************************************/
void DC4_Stop_RX(UNS32 uiMcASP_Controller);

/*********************************************************************************************
Function Name : DC4_Start_RX
Description   : This function starts the Receiver of McASP controller.
Prototype     : void DC4_Start_RX(UNS32 uiMcASP_Controller, UNS32 uiSerializer)
Parameters    : [I] uiMcASP_Controller - Base address of controller
Returns       : void
***********************************************************************************************/
void DC4_Start_RX(UNS32 uiMcASP_Controller, UNS32 uiSerializer);

/*********************************************************************************************
Function Name : DC4_Config_RX
Description   : This function configure the Receiver of McASP controller.
Prototype     : UNS32 DC4_Config_RX(UNS32 uiMcASP_Controller, McASP_dev *dev)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] dev - Device structure which configuration data
Returns       : void
***********************************************************************************************/
UNS32 DC4_Config_RX(UNS32 uiMcASP_Controller, McASP_dev *dev);

/*********************************************************************************************
Function Name : DC4_McASP_Receive
Description   : This function transmit the data on McASP controller.
Prototype     : UNS32 DC4_Config_RX(UNS32 uiMcASP_Controller, McASP_dev *dev)
Parameters    : [I] uiMcASP_Controller - Base address of controller
				[I] dev - Device structure which configuration data
				[I] *buffer - Data buffer pointer which holds data
				[I] uiLength - Number of words to transmit
Returns       : void
***********************************************************************************************/
void DC4_McASP_Receive(UNS32 uiMcASP_Controller, McASP_dev *dev, void *buffer, UNS32 uiLength);

void DC4_McASP_Loopback(UNS32 uiMcASP_Controller);

#endif /* MCASP_HLL_H_ */
