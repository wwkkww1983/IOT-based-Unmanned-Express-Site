#include "main.h"
#include <string.h>
#include "usart.h"
#include "HX711.h"
#include <string.h>
#include "delay.h"

char c[100][8];
char C[8];
MB member[100];

unsigned char connect_flag = 0;
/*��ʱ����*/
void Delay_s(unsigned long ulVal)			//����ѭ������һ������ʱ
{
	while ( --ulVal != 0 );
}

/*TCP�ͻ��˷������ݺ���*/
err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length)
{
	err_t err;
	err = tcp_write(cpcb,buff,length,TCP_WRITE_FLAG_COPY);			//��������
	tcp_output(cpcb);
	return err;					
}

/*�������*/
struct tcp_pcb *Check_TCP_Connect(void)
{
	struct tcp_pcb *cpcb = 0;
	connect_flag = 0;
	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)
	{
		if(cpcb -> state == ESTABLISHED) 
		{
			connect_flag = 1;			//���ӱ�־
			break;							   	
		}
	}
	if(connect_flag == 0)			// TCP_LOCAL_PORTָ���Ķ˿�δ���ӻ��ѶϿ�
	{
		TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP); //��������
		cpcb = 0;
	}
	return cpcb;	
}


/*����һ���ص���������TCP�ͻ�����������ӽ���ʱ������*/
err_t TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	return ERR_OK;
}

/*tcp�ͻ��˽������ݻص�����*/
err_t  TCP_Client_Recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
  static int i=0;
	int j,k,l=0,a;
	struct pbuf *q;
	if(p!= NULL)
	{
		tcp_recved(pcb,p->tot_len);			//��ȡ���ݳ��� tot_len��tcp���ݿ�ĳ���
		for(q=p;q!=NULL;q=q->next) 
		{
			if(q->tot_len>100)break;
			{
				for(k=0;k<100;k++)
				{
					if(member[k].name==*((char*) p->payload+6))			//���ǵ����
					{
						for(a=0;a<6;a++)
						{
						member[k].number[a]=*((char*) p->payload+a);
						}
						member[k].name=*((char*) p->payload+6);
						member[k].number[6]='\0';
						l=1;
						break;
					}
				}
				if(l==0)
				{
					for(k=0;k<100;k++)
					{
						if(member[k].name!=*((char*) p->payload+6)) //û�����ǵ����
						{                                      
							for(a=0;a<6;a++)
							{
							member[i].number[a]=*((char*) p->payload+a);
							}
							member[i].name=*((char*) p->payload+6);
							member[i].number[6]='\0';
							l=0;
							i++;
							break;
						}
					}
				}	
		}
			if(i==100)i=0;
			{
				strncpy(C,member[i-1].number,6);
				USART4_printf("AT\r\n");			//�����ķ��͸�����ģ��
				delay_ms(10);
				USART4_printf("AT+CMGF=1\r\n");
				delay_ms(10);
				USART4_printf("AT+CSCS=\"GSM\"\r\n");
				delay_ms(500);
				USART4_printf("AT+CMGS=\"13559725210\"\r\n");			//15994997557���� 13559725210ɵ��  13242353288��Ӫ 18988746231��
				delay_ms(500);
				USART4_printf("%s",C);
				delay_ms(10);
				USART4_printf("%c",0x1a);
				delay_ms(10);
			}
		}
	}
	else			//����������Ͽ����ӣ���ͻ���ҲӦ�Ͽ�
	{
		tcp_close(pcb); 						
	}
	pbuf_free(p); 				
	err = ERR_OK;
	return err;	
}

/*tcp�ͻ��˳�ʼ��*/
void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{

	struct ip_addr ipaddr;
	struct tcp_pcb *tcp_client_pcb;
	err_t err;
	IP4_ADDR(&ipaddr,a,b,c,d);			//������IP��ַ
	tcp_client_pcb = tcp_new();			//����ͨ�ŵ�TCP���ƿ�(Clipcb)
	if (!tcp_client_pcb)
	{
		return ;
	}	
	err = tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port);			//�󶨱���IP��ַ�Ͷ˿ں� ������ip��ַ��LwIP_Init()���Ѿ���ʼ��
    if(err != ERR_OK)
	{
		return ;
	}
	tcp_connect(tcp_client_pcb,&ipaddr,remote_port,TCP_Connected);			//ע��ص�����
	tcp_recv(tcp_client_pcb,TCP_Client_Recv);			//����tcp���ջص�����
}