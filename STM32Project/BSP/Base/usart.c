/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��usart.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ�������ݴ�
 *           ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�     
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-11-11
 * Ӳ������: TX->PA9;RX->PA10
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "usart.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


 /**
  * @file   USART_Config
  * @brief  USART1 GPIO ����,����ģʽ���á�9600-8-N-1
  * @param  ��
  * @retval ��
  */
void USART_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
    RCC_APB2PeriphClockCmd( USART1_RCC | USART1_GPIO_RCC,ENABLE);

    /*USART1_TX ->PA9*/			
    GPIO_InitStructure.GPIO_Pin = USART1_TX;	       //ѡ�д���Ĭ������ܽ�         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
    GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
    /*USART1_RX ->PA10*/
    GPIO_InitStructure.GPIO_Pin = USART1_RX;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);
    /*����ͨѶ��������*/
    USART_InitStructure.USART_BaudRate = BaudRate; //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

    USART_Init(USART1, &USART_InitStructure);
		USART_ClearFlag(USART1,USART_FLAG_TC);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);		
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);		
    USART_Cmd(USART1, ENABLE);
		
    /*USART2_TX ->PA2*/			
    RCC_APB1PeriphClockCmd( USART2_RCC ,ENABLE);
		GPIO_InitStructure.GPIO_Pin = USART2_TX;	       //ѡ�д���Ĭ������ܽ�         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
    GPIO_Init(USART2_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
    /*USART2_RX ->PA3*/
    GPIO_InitStructure.GPIO_Pin = USART2_RX;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART2_GPIO_PORT, &GPIO_InitStructure);
    /*����ͨѶ��������*/
    USART_InitStructure.USART_BaudRate = BaudRate; //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

    USART_Init(USART2, &USART_InitStructure);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART2, ENABLE);

    /*USART3_TX ->PB10*/			
    RCC_APB1PeriphClockCmd( USART3_RCC ,ENABLE);
    RCC_APB2PeriphClockCmd( USART3_GPIO_RCC,ENABLE);
		GPIO_InitStructure.GPIO_Pin = USART3_TX;	       //ѡ�д���Ĭ������ܽ�         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
    GPIO_Init(USART3_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
    /*USART3_RX ->PB11*/
    GPIO_InitStructure.GPIO_Pin = USART3_RX;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(USART3_GPIO_PORT, &GPIO_InitStructure);
    /*����ͨѶ��������*/
    USART_InitStructure.USART_BaudRate = 9600; //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

    USART_Init(USART3, &USART_InitStructure);
		USART_ClearFlag(USART3,USART_FLAG_TC);
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART3, ENABLE);
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
  return ch;
}
