/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��i2c.c
 * ����    ����ʼ��IIC��һЩ�����Ĳ��� 
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-11-11
 * Ӳ������: PB6-I2C1_SCL��PB7-I2C1_SDA
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/

//ͷ�ļ�
#include "i2c1.h"
#include "usart.h"

 /**
  * @file   I2C_Configuration
  * @brief  EEPROM�ܽ�����
  * @param  ��
  * @retval ��
  */
void I2C_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  /* Configure I2C2 pins: PB6->SCL and PB7->SDA */
  RCC_APB2PeriphClockCmd(EEPROM_I2C_SCL_GPIO_RCC|EEPROM_I2C_SDA_GPIO_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
	GPIO_InitStructure.GPIO_Pin =  EEPROM_I2C_SCL_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  EEPROM_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

 /**
  * @file   I2C_delay
  * @brief  �ӳ�ʱ��
  * @param  ��
  * @retval ��
  */
static void I2C_delay(uint16_t  time_us)
{	
   uint16_t i,j;
  for( i=0;i<time_us;i++ )
  {
		for( j=0;j<10;j++ );//��Լ1us
  }
}
  
 /**
  * @file   I2C_Start
  * @brief  ��ʼ�ź�
  * @param  ��
  * @retval ��
  */
static FunctionalState I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay(5);
	if(!SDA_read)return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
	SDA_L;
	I2C_delay(4);
	if(SDA_read) return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
	SDA_L;
	I2C_delay(4);
	return ENABLE;
}

 /**
  * @file   I2C_Stop
  * @brief  ֹͣ�ź�
  * @param  ��
  * @retval ��
  */
static void I2C_Stop(void)
{
	SCL_L;
	I2C_delay(1);
	SDA_L;
	I2C_delay(1);
	SCL_H;
	I2C_delay(5);
	SDA_H;
	I2C_delay(5);
}

 /**
  * @file   I2C_Ack
  * @brief  Ӧ���ź�
  * @param  ��
  * @retval ��
  */
static void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay(1);
	SDA_L;
	I2C_delay(1);
	SCL_H;
	I2C_delay(5);
	SCL_L;
	I2C_delay(5);
}

 /**
  * @file   I2C_NoAck
  * @brief  ��Ӧ���ź�
  * @param  ��
  * @retval ��
  */
static void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay(1);
	SDA_H;
	I2C_delay(1);
	SCL_H;
	I2C_delay(5);
	SCL_L;
	I2C_delay(5);
}

 /**
  * @file   I2C_WaitAck
  * @brief  �ȴ�Ack
  * @param  ��
  * @retval ����Ϊ:=1��ACK,=0��ACK
  */
static FunctionalState I2C_WaitAck(void) 	
{
	SCL_L;
	I2C_delay(1);
	SDA_H;			
	I2C_delay(1);
	SCL_H;
	I2C_delay(5);
	if(SDA_read)
	{
      SCL_L;
      return DISABLE;
	}
	SCL_L;
	return ENABLE;
}

 /**
  * @file   I2C_SendByte
  * @brief  ���ݴӸ�λ����λ
  * @param  - SendByte: ���͵�����
  * @retval ��
  */
static void I2C_SendByte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
			SCL_L;
			I2C_delay(5);
			if(SendByte&0x80)
			SDA_H;  
			else 
			SDA_L;   
			SendByte<<=1;
			I2C_delay(5);
			SCL_H;
			I2C_delay(10);
    }
    SCL_L;
}


 /**
  * @file   I2C_ReceiveByte
  * @brief  ���ݴӸ�λ����λ
  * @param  ��
  * @retval I2C���߷��ص�����
  */
static uint8_t I2C_ReceiveByte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
			ReceiveByte<<=1;      
			SCL_L;
			I2C_delay(5);
			SCL_H;
			I2C_delay(5);	
			if(SDA_read)
			{
				ReceiveByte|=0x01;
			}
    }
    SCL_L;
    return ReceiveByte;
}
   
 /**
  * @file   I2C_WriteByte
  * @brief  дһ�ֽ�����
  * @param  
	*          - SendByte: ��д������
	*          - WriteAddress: ��д���ַ
  * @retval ����Ϊ:=1�ɹ�д��,=0ʧ��
  */
FunctionalState I2C_WriteByte(uint8_t SendByte, uint16_t WriteAddress)
{
    if(!I2C_Start())
		{
			return DISABLE;
		}
    I2C_SendByte(EE_DEV_ADDR|I2C_WR); /*���ø���ʼ��ַ+������ַ */
    if(!I2C_WaitAck())
		{
			I2C_Stop(); 
			return DISABLE;
		}
    I2C_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
    I2C_WaitAck();	
    I2C_SendByte(SendByte);
    I2C_WaitAck();   
    I2C_Stop(); 
    return ENABLE;
}									 
/*
*********************************************************************************************************
*	�� �� ��: ee_WriteBytes
*	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*	��    �Σ�_usAddress : ��ʼ��ַ
*			      _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			      _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
FunctionalState I2C_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t Addr;
	uint8_t pages;
	uint8_t bytes;
	Addr = _usAddress;
	pages=_usSize/8;//��ҪЩeeprom��ҳ��
	bytes=_usSize%8;//ʣ�಻��һҳ���ֽ���
	for(m=0;m<pages;m++)
	{
		//printf("m=%d\r\n",m);
		
		if(!I2C_Start())
		{
			return DISABLE;
		}
		I2C_SendByte(EE_DEV_ADDR|I2C_WR); /*���ø���ʼ��ַ+������ַ */
		if(!I2C_WaitAck())
		{
			I2C_Stop(); 
			return DISABLE;
		}
		I2C_SendByte((uint8_t)(Addr & 0x00FF));   /* ���õ���ʼ��ַ */      
		I2C_WaitAck();
		for(i=0;i<8;i++)
		{
			I2C_SendByte(*_pWriteBuf++);
			//printf("_pWriteBuf2=%d\r\n",*_pWriteBuf++);
			I2C_WaitAck(); 
		}
		Addr=Addr+8;
		I2C_Stop(); 
		I2C_delay(10000);//�ȴ�����д��
	}
	if(bytes!=0)
	{
		if(!I2C_Start())
		{
			return DISABLE;
		}
		I2C_SendByte(EE_DEV_ADDR|I2C_WR); /*���ø���ʼ��ַ+������ַ */
		if(!I2C_WaitAck())
		{
			I2C_Stop(); 
			return DISABLE;
		}
		I2C_SendByte((uint8_t)(Addr & 0x00FF));   /* ���õ���ʼ��ַ */      
		I2C_WaitAck();
		for(i=0;i<bytes;i++)
		{
			I2C_SendByte(*_pWriteBuf++);
			I2C_WaitAck();	
		}
		I2C_Stop();
		I2C_delay(10000);//�ȴ�����д��
	}
		I2C_Stop();
   return ENABLE;
}
 /**
  * @file   I2C_ReadByte
  * @brief  ��ȡһ������
  * @param  
	*					- pBuffer: ��Ŷ�������
	*     	  - length: ����������
	*         - ReadAddress: ��������ַ
	*         - DeviceAddress: ��������(24c16��SD2403)
  * @retval ����Ϊ:=1�ɹ�����,=0ʧ��
  */
FunctionalState I2C_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint16_t ReadAddress)//,  uint8_t DeviceAddress)
{		
    if(!I2C_Start())return DISABLE;
    I2C_SendByte(EE_DEV_ADDR|I2C_WR); /* ���ø���ʼ��ַ+������ַ */ 
    if(!I2C_WaitAck()){I2C_Stop(); return DISABLE;}
    I2C_SendByte((uint8_t)(ReadAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(EE_DEV_ADDR|I2C_RD);
    I2C_WaitAck();
    while(length)
    {
      *pBuffer = I2C_ReceiveByte();
      if(length == 1)I2C_NoAck();
      else I2C_Ack(); 
      pBuffer++;
      length--;
    }
    I2C_Stop();
    return ENABLE;
}

FunctionalState I2C_Erase()
{
	uint16_t i;
	uint8_t buf[EE_SIZE];
	/* ��仺���� */
	for (i = 0; i < EE_SIZE; i++)
	{
		buf[i] = 0xFF;
	}
	if(I2C_WriteBytes(buf, 0, EE_SIZE)==DISABLE)
	{
			printf("����eeprom����\r\n");
			return  DISABLE;
	}
	else
	{
	    printf("����eeprom�ɹ���\r\n");
		  return  ENABLE;
	}

}

/**
  * @file   I2C_Test
  * @brief  I2C(AT24C08)��д���ԡ�
  * @param  ��
  * @retval ��
  */
void I2C_Test1(void)
{
		uint16_t Addr;
		uint8_t WriteBuffer[256],ReadBuffer[256];
	 //I2C_Erase();
		for(Addr=0; Addr<256; Addr++)
	  {
				WriteBuffer[Addr]=Addr;	 /* ���WriteBuffer */
				printf("0x%02x ",WriteBuffer[Addr]);
				if(Addr%16 == 15)    
				printf("\n\r");
    }
		/* ��ʼ��EEPROMд���� */
		printf("\r\n EEPROM 24C08 Write Test \r\n");
		I2C_WriteBytes(WriteBuffer, 0, 256);
		printf("\r\n EEPROM Write Test OK \r\n");

		/* EEPROM������ */
		printf("\r\n EEPROM 24C08 Read Test \r\n");
		I2C_ReadByte(ReadBuffer, sizeof(WriteBuffer),0);//, EE_DEV_ADDR);
    for(Addr=0; Addr<256; Addr++)
		{
			printf("0x%02x ",ReadBuffer[Addr]);
			if(Addr%16 == 15)    
			printf("\n\r");
		}
		if(  memcmp(WriteBuffer,ReadBuffer,sizeof(WriteBuffer)) == 0 ) /* ƥ������ */
		printf("\r\n EEPROM 24C08 Read Test OK\r\n");
		else
		printf("\r\n EEPROM 24C08 Read Test False\r\n");
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
