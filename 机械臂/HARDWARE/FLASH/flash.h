#ifndef __FLASH_H
#define __FLASH_H			    
#include "sys.h" 
 
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
 

extern u16 SPI_FLASH_TYPE;		//��������ʹ�õ�flashоƬ�ͺ�		

#define	SPI_FLASH_CS PBout(12)  //ѡ��FLASH					 
////////////////////////////////////////////////////////////////////////////

extern u8 SPI_FLASH_BUF[4096];
//W25X64��д
#define FLASH_ID 0XEF16
//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void SpiFlashInit(void);
u16  SpiFlashReadID(void);  	    //��ȡFLASH ID
u8	 SpiFlashReadSR(void);        //��ȡ״̬�Ĵ��� 
void SpiFlashWriteSR(u8 sr);  	//д״̬�Ĵ���
void SpiFlashWriteEnable(void);  //дʹ�� 
void SpiFlashWriteDisable(void);	//д����
void SpiFlashRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void SpiFlashWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void SpiFlashEraseChip(void);    	  //��Ƭ����
void SpiFlashEraseSector(u32 Dst_Addr);//��������
void SpiFlashWaitBusy(void);           //�ȴ�����
void SpiFlashPowerDown(void);           //�������ģʽ
void SpiFlashWAKEUP(void);			  //����
#endif
















