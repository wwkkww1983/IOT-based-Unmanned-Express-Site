#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����
//V1.2 20120904
//1,����TIM5_Cap_Init����
//2,����TIM5_IRQHandler�жϷ�����	
//V1.3 20120908
//1,����TIM4_PWM_Init����	
//V1.4 20120914
//1,����TIM6_Int_Init������
//2,����TIM6_IRQHandler����
////////////////////////////////////////////////////////////////////////////////// 

//ͨ���ı�TIM3->CCR2��ֵ���ı�ռ�ձȣ��Ӷ�����LED0������
#define LED0_PWM_VAL TIM3->CCR2    
//TIM4 CH1��ΪPWM DAC�����ͨ�� 
#define PWM_DAC_VAL  TIM4->CCR1 

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM6_Int_Init(u16 arr,u16 psc);
void Tim3_Init(void);
void Tim3_PWM(u8 Value);
#endif























