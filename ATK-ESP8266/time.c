#include <time.h>
#include "time.h"
#include "wifista.h"
#include "usart.h"
#include "malloc.h"
#include "usart3.h"
#include "delay.h"
#include "parsejson.h"


//ʱ��˿ں�
#define TIME_PORTNUM			"80"
//ʱ�������IP
#define TIME_SERVERIP			"api.m.taobao.com"

//��ȡ����ʱ��
u8 get_beijing_time(void)
{
	u8 *p;
	u8 res,jc = 0;
//	u8 ipbuf[16]; 	//IP����
	p=mymalloc(SRAMIN,40);							//����40�ֽ��ڴ�
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",TIME_SERVERIP,TIME_PORTNUM);    //����Ŀ��TCP������
	res = atk_8266_send_cmd(p,"OK",200);//���ӵ�Ŀ��TCP������
	if(res==1)
	{
		myfree(SRAMIN,p);
		return 1;
	}
	delay_ms(300);
	atk_8266_send_cmd("AT+CIPMODE=1","OK",100);      //����ģʽΪ��͸��	
//	atk_8266_get_wanip(ipbuf);//��ȡWAN IP

	USART3_RX_STA=0;
	atk_8266_send_cmd("AT+CIPSEND","OK",100);         //��ʼ͸��
	printf("start trans...\r\n");
	u3_printf("GET http://api.m.taobao.com/rest/api3.do?api=mtop.common.gettimestamp\n\n");	
	delay_ms(20);//��ʱ20ms���ص���ָ��ͳɹ���״̬
//	atk_8266_at_response(1);
	USART3_RX_STA=0;	//���㴮��3����
	delay_ms(1000);
//	atk_8266_at_response(0);
	if(USART3_RX_STA&0X8000)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊ����������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
	} 
	jc = parse_now_time();
	if(jc  == 1)
	{
//		WIFI_Set_Time();
//		WIFI_Set_Date();
	}
	atk_8266_quit_trans();//�˳�͸��
	atk_8266_send_cmd("AT+CIPCLOSE","OK",50);         //�ر�����
	myfree(SRAMIN,p);
	return 0;
}
