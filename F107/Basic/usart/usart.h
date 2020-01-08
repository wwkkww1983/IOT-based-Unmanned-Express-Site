#ifndef __USART_H
#define __USART_H
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x.h"	 

#define USART_n		UART5  //����ʹ��printf�����Ĵ��ڣ���������Ҫʹ��USART_printfר�ú�������

#define USART1_REC_LEN  			200  	//����USART1�������ֽ���
#define USART2_REC_LEN  			200  	//����USART2�������ֽ���
#define USART3_REC_LEN  			200  	//����USART3�������ֽ���
#define USART4_REC_LEN  			200  	//����USART4�������ֽ���
#define USART5_REC_LEN  			200  	//����USART5�������ֽ���

//��ʹ��ĳ������ʱҪ��ֹ�˴��ڣ��Լ��ٱ�����
#define EN_USART1 			1		//ʹ�ܣ�1��/��ֹ��0������1
#define EN_USART2 			1		//ʹ�ܣ�1��/��ֹ��0������2
#define EN_USART3 			1		//ʹ�ܣ�1��/��ֹ��0������3
#define EN_UART4 			  1		//ʹ�ܣ�1��/��ֹ��0�����
#define EN_UART5 			  1		//ʹ�ܣ�1��/��ֹ��0������4
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8  USART4_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8  USART5_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
 
extern u16 USART1_RX_STA;         		//����״̬���	
extern u16 USART2_RX_STA;         		//����״̬���	
extern u16 USART3_RX_STA;         		//����״̬���
extern u16 USART4_RX_STA;         		//����״̬���
extern u16 USART5_RX_STA;         		//����״̬���

//��������
void USART1_Init(u32 bound);//����1��ʼ��������
void USART2_Init(u32 bound);//����2��ʼ��������
void USART3_Init(u32 bound);//����3��ʼ��������
void USART4_Init(u32 bound);//����4��ʼ��������
void USART5_Init(u32 bound);//����5��ʼ��������
void USART1_printf(char* fmt,...); //����1��ר��printf����
void USART2_printf(char* fmt,...); //����2��ר��printf����
void USART3_printf(char* fmt,...); //����3��ר��printf����
void USART4_printf(char* fmt,...); //����4��ר��printf����
void USART5_printf(char* fmt,...); //����5��ר��printf����

#endif
