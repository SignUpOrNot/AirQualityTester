#include "rtc.h"
#include "stdio.h"

vu32 TimeDisplay = 0;
ErrorStatus HSEStartUpStatus;

void Clock_ini(void){
	  NVIC_InitTypeDef NVIC_InitStructure;	

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	  
 
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;					  //�����ⲿ�ж�Դ�����жϣ� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  

//     RTC_Configuration();							      //RTC��ʼ��	 
//     printf("\r\n RTC configured....");
//     Time_Adjust(22,52,10);										  //����RTC ʱ�Ӳ���
//     BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);    	      //RTC���ú󣬽������ñ�־д�뱸�����ݼĴ��� 

	
	
  if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)		     //�жϱ����ڱ��ݼĴ�����RTC��־�Ƿ��Ѿ������ù�
  {
     printf("\r\n\n RTC not yet configured....");
     RTC_Configuration();							      //RTC��ʼ��	 
     printf("\r\n RTC configured....");
     Time_Adjust(21,59,10);										  //����RTC ʱ�Ӳ���
     BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);    	      //RTC���ú󣬽������ñ�־д�뱸�����ݼĴ��� 
  }
  else
  {	     
     if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)	  //����Ƿ��������
     {
       printf("\r\n\n Power On Reset occurred....");
     }												     
     else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) //����Ƿ�reset��λ
     {
       printf("\r\n\n External Reset occurred....");
     }
     printf("\r\n No need to configure RTC....");  
     RTC_WaitForSynchro();								   //�ȴ�RTC�Ĵ�����ͬ�� 
     RTC_ITConfig(RTC_IT_SEC, ENABLE);					   //ʹ�����ж�
     RTC_WaitForLastTask();								   //�ȴ�д�����
  }
  RCC_ClearFlag();										   //�����λ��־
}

void RTC_Configuration(void)
{ 
  /* ʹ�� PWR �� BKP ��ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  
  /* �������BKP���� */
  PWR_BackupAccessCmd(ENABLE);

  /* ��λBKP */
  BKP_DeInit();

#ifdef RTCClockSource_LSI
  /* ʹ���ڲ�RTCʱ�� */ 
  RCC_LSICmd(ENABLE);
  /* �ȴ�RTC�ڲ�ʱ�Ӿ��� */
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }
  /* ѡ��RTC�ڲ�ʱ��ΪRTCʱ�� */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);  
#elif defined	RTCClockSource_LSE  
  /* ʹ��RTC�ⲿʱ�� */
  RCC_LSEConfig(RCC_LSE_ON);
  /* �ȴ�RTC�ⲿʱ�Ӿ��� */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {	    
  }

  /* ѡ��RTC�ⲿʱ��ΪRTCʱ�� */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
#endif
  /* ʹ��RTCʱ�� */
  RCC_RTCCLKCmd(ENABLE);


#ifdef RTCClockOutput_Enable  
  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                               functionality must be disabled */
                               
  /* ʹ����TAMPER�����RTCʱ�� */
  BKP_RTCCalibrationClockOutputCmd(ENABLE);
#endif 

  /* �ȴ�RTC�Ĵ���ͬ�� */
  RTC_WaitForSynchro();

  /* �ȴ�дRTC�Ĵ������ */
  RTC_WaitForLastTask();
  
  /* ʹ��RTC���ж� */  
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* �ȴ�дRTC�Ĵ������ */
  RTC_WaitForLastTask();
  
  /* ����RTCԤ��Ƶ */
#ifdef RTCClockSource_LSI
  RTC_SetPrescaler(31999);            /* RTC period = RTCCLK/RTC_PR = (32.000 KHz)/(31999+1) */
#elif defined	RTCClockSource_LSE
  RTC_SetPrescaler(32767);            /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#endif
  
  /* �ȴ�дRTC�Ĵ������ */
  RTC_WaitForLastTask();
}

/****************************************************************************
* ��    �ƣ�u32 Time_Regulate(void)
* ��    �ܣ�ʱ��У������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/
u32 Time_Regulate(int h,int m,int s)
{
//  u32 Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

//  printf("\r\n==============Time Settings=====================================");
//  printf("\r\n  Please Set Hours");
//  
//  while(Tmp_HH == 0xFF)				      
//  {
//    Tmp_HH = 12;//USART_Scanf(23);	         
//  }
//  printf(":  %d", Tmp_HH); 
//  printf("\r\n  Please Set Minutes");
//  while(Tmp_MM == 0xFF)
//  {
//    Tmp_MM = 12;//USART_Scanf(59);
//  }
//  printf(":  %d", Tmp_MM); 
//  printf("\r\n  Please Set Seconds");
//  while(Tmp_SS == 0xFF)
//  {
//    Tmp_SS = 12;//USART_Scanf(59);
//  }
//  printf(":  %d", Tmp_SS); 

  /* ���ر�����RTC�����Ĵ������ֵ */
  //return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
	return((h*3600 + m*60 + s));
}

/****************************************************************************
* ��    �ƣ�void Time_Adjust(void)
* ��    �ܣ�ʱ���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/
void Time_Adjust(int h,int m,int s)
{
  /* �ȴ�дRTC�Ĵ������ */
  RTC_WaitForLastTask(); 
  /* �ı䵱ǰʱ�� */
  RTC_SetCounter(h*3600 + m*60 + s);
  /* �ȴ�дRTC�Ĵ������ */
  RTC_WaitForLastTask();   
}

/****************************************************************************
* ��    �ƣ�void Time_Display(u32 TimeVar)
* ��    �ܣ���ʾ��ǰʱ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/
void Time_Display(u32 TimeVar)
{ 
  u32 THH = 0, TMM = 0, TSS = 0;

  /* ����Сʱ */
  THH = TimeVar/3600;
  /* ������� */
  TMM = (TimeVar % 3600)/60;
  /* ������ */
  TSS = (TimeVar % 3600)% 60;
  printf("Time: %0.2d:%0.2d:%0.2d\r\n",THH, TMM, TSS);
}

/****************************************************************************
* ��    �ƣ�void Time_Show(void)
* ��    �ܣ�ѭ����ʾ��ǰʱ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/
//void Time_Show(void)
//{
//  printf("\n\r");	   
//  while(1)
//  {
//    /* ����·��� */
//    if(TimeDisplay == 1)
//    {    
//      /* ��ʾ��ǰʱ�� */
//      Time_Display(RTC_GetCounter());
//      TimeDisplay = 0;
//    }
//  }
//}

