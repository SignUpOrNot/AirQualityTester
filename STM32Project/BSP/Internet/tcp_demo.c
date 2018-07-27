/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP ��ʾ����
* @attention  
************************************************************************************************
**/

#include <stdio.h>
#include <string.h>
#include "tcp_demo.h"
#include "W5500_conf.h"
#include "w5500.h"
#include "socket.h"

#define tick_second 1
uint8 buff[2048];				                              	         /*����һ��2KB�Ļ���*/

/**
*@brief		TCP Server�ػ���ʾ������
*@param		��
*@return	��
*/
void loopback_tcps(SOCKET s, uint16 port)
{
	uint16 len=0;  
	switch(getSn_SR(s))											            	/*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:													                  /*socket���ڹر�״̬*/
			socket(s ,Sn_MR_TCP,port,Sn_MR_ND);	        /*��socket*/
		  break;     
    
		case SOCK_INIT:														                  /*socket�ѳ�ʼ��״̬*/
			listen(s);												                /*socket��������*/
		  break;
		
		case SOCK_ESTABLISHED:												              /*socket�������ӽ���״̬*/
		
			if(getSn_IR(s) & Sn_IR_CON)
			{
				setSn_IR(s, Sn_IR_CON);								          /*��������жϱ�־λ*/
			}
			len=getSn_RX_RSR(s);									            /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(s,buff,len);								              	/*��������Client������*/
				buff[len]=0x00; 											                  /*����ַ���������*/
				printf("%s\r\n",buff);
				send(s,buff,len);									              /*��Client��������*/
		  }
		  break;
		
		case SOCK_CLOSE_WAIT:												                /*socket���ڵȴ��ر�״̬*/
			close(s);
		  break;
	}
}


/**
*@brief		TCP client�ػ���ʾ������
*@param		��
*@return	��
*/

u8 sockflag;
u8 conndisp;
void loopback_tcpc(SOCKET s, uint16 port)
{

   uint16 len=0;	

	switch(getSn_SR(s))						   		  				         /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:											        		         /*socket���ڹر�״̬*/
			printf("Sock close,recreat\r\n");
			socket(s,Sn_MR_TCP,local_port++,Sn_MR_ND);
			sockflag=0;
		  break;
		
		case SOCK_INIT:													        	         /*socket���ڳ�ʼ��״̬*/
			printf("Sock conn\r\n");
			connect(s,remote_ip,port);                /*socket���ӷ�����*/ 
			conndisp=1;
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket�������ӽ���״̬*/
			sockflag=1;
			if(sockflag && conndisp){
					conndisp=0;
					printf("Sock conn success\r\n");
			}
			if(getSn_IR(s) & Sn_IR_CON)
			{
				setSn_IR(s, Sn_IR_CON); 							         /*��������жϱ�־λ*/
			}
		
			len=getSn_RX_RSR(s); 								  	         /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(s,buff,len); 							   		         /*��������Server������*/
				buff[len]=0x00;  											                 /*����ַ���������*/
				printf("Sock REC-%s\r\n",buff);
				send(s,buff,len);								     	         /*��Server��������*/
			}		  
		  break;
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket���ڵȴ��ر�״̬*/
			printf("Sock colse wat\r\n");
			close(s);
		  break;
	}	

}





