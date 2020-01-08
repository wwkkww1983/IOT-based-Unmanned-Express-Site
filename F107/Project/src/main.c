#include "main.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "HX711.h"
#include <string.h>

__IO uint32_t LocalTime = 0;			///��ʼ��ϵͳ�˱������ڴ���ʱ�����ã�����Ϊ10����
uint32_t timingdelay;
u8 a;
extern char c[100][8];
extern MB member[100];

/***********LWIP������ú���***************/
/*��ʱ*/
void Delay(uint32_t nCount)
{
  timingdelay = LocalTime + nCount;  
  while(timingdelay > LocalTime)
  {     
  }
}
/*����ϵͳ����ʱ��*/
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}
/*����ϵͳ�Ķ�������*/
void System_Periodic_Handle(void)
{
  Display_Periodic_Handle(LocalTime);
  LwIP_Periodic_Handle(LocalTime);
}
/*****************************************/

/*************************������*******************************/
int main(void)
{
		int flag=0;			//���ձ�־λ
		int nm,p=0;			//�ж����ޱ�־λ
		unsigned char flag_uart2_rev=0;
		char str[100]={1};			//����
		unsigned char tcp_data[100];
		struct tcp_pcb *pcb;
  	System_Setup();			//��ʼ��ϵͳ    
    LwIP_Init();			//��ʼ��LwIP  satck ip��ַ���ã�mac����
	  TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP);			//tcp�ͻ��˳�ʼ��
		USART1_Init(115200);
		USART2_Init(115200);
	  USART4_Init(9600);     //SIM8000�涨��ͨ�Ų�����1
	  USART5_Init(115200);
		LED_Init1();
		LED_Init2();
		LED_Init3();
		NVIC_Configuration();			//����ģ������
		
	while(1)
	{  
		static unsigned int i=0;
		u8 Res;
    /*����OpenMV���������ݲ����͸����ݿ�*/
		if(USART_GetFlagStatus(UART5,USART_FLAG_RXNE) != RESET)
		{
			a =USART_ReceiveData(UART5);
		  if(a=='A')
			{
				tcp_data[0] = 'A';
				tcp_data[1] = '\0';
				printf("#1GC1\r\n");   
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
				pcb = Check_TCP_Connect();			//�������
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));			//���������������
				}
				Delay_s(0xfffff);			//��ʱ
			}
			if(a=='B')
			{
				tcp_data[0] = 'B';
				tcp_data[1] = '\0';
				printf("#2GC1\r\n");   
				GPIO_SetBits(GPIOC,GPIO_Pin_2);
				pcb = Check_TCP_Connect();			//�������
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));			//���������������
				}
				Delay_s(0xfffff);			//��ʱ
			}
			if(a=='C')
			{
				tcp_data[0] = 'C';
				tcp_data[1] = '\0';
				printf("#3GC1\r\n");   
			  GPIO_SetBits(GPIOC,GPIO_Pin_3);
				pcb = Check_TCP_Connect();			//�������
				if(pcb != 0)
				{	
					TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));			//���������������
				}
				Delay_s(0xfffff);			//��ʱ
			}
		}
		
		/*����A53���������ݲ��뻺������ݽ���ƥ��*/
		if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)  
		{ 
			Res =USART_ReceiveData(USART2);
			if(Res=='E')  //�жϽ�����־λ
			{
				flag_uart2_rev=1;
				flag=0;
				str[i]='\0';
				i=0;
			}
			if(flag==1)
			{
					str[i]=Res;
					i++;
			}
			if(Res=='S')			//�жϿ�ʼ��־λ
				flag=1;
		}	
		if(flag_uart2_rev==1)
		{
			flag_uart2_rev=0;
			for(nm=0;nm<3;nm++)
			{
				if(!strncmp(str,member[nm].number,6))
				{
					if(member[nm].name=='A')
					{
						printf("#4GC1\r\n");
						GPIO_ResetBits(GPIOC,GPIO_Pin_13); 
					} 
					if(member[nm].name=='B')
					{
						printf("#6GC1\r\n");
						GPIO_ResetBits(GPIOC,GPIO_Pin_2); 
					}
					if(member[nm].name=='C')
					{
						printf("#5GC1\r\n");
						GPIO_ResetBits(GPIOC,GPIO_Pin_3); 
					}
					p=1;
					break;
				}else{
					p=0;
				}
			}
			delay_ms(1000);
			if(p==1)USART1_printf("1Z");			//�û���֤������ݿ����ƥ�䣬����Ϊ1����֮Ϊ0
			if(p==0)USART1_printf("00");
		}
		System_Periodic_Handle();			//��Ϊ�������������¼�������Ҫ�Ǹ���TCP timers
	}
}

