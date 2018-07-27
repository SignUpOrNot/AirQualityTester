#include "stm32f10x.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h" 

//#define RTCClockSource_LSI   /* �����õ�32K ʱ�Ӿ���Դ */
#define RTCClockSource_LSE   /* �����õ�32.768K ʱ�Ӿ���Դ */


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void Usart1_Init(void);
void RTC_Configuration(void);
void NVIC_Configuration(void);
u32 Time_Regulate(int h,int m,int s);
void Time_Adjust(int h,int m,int s);
void Time_Show(void);
void Time_Display(u32 TimeVar);
u8 USART_Scanf(u32 value);
void Clock_ini(void);
