#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "malloc.h"
#include "usart3.h"
#include "wifista.h"
#include "RTC.h"
#include "time.h"
#include "calendar.h"


int main(void)
{
	u8 key;
	delay_init(168);								//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);								//���ڳ�ʼ��Ϊ115200

	LED_Init(); //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init(); //��ʼ������
	LCD_Init(); //��ʼ��LCD
	My_RTC_Init();
	usart3_init(115200);	 //��ʼ������3
	key = KEY_Scan(0);
	delay_ms(200);
	LED0 = 0;
	atk_8266_wifista_config();
	printf("wifi init ok!\r\n");
	key = get_beijing_time();
	printf("%d\n",key);
	LCD_Clear(BLACK);
	while (1)
	{
		calendar_play();
		LED0 = ~LED0;
		delay_ms(250);
	}
}

