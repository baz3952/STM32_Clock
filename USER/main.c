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
	delay_init(168);								//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);								//串口初始化为115200

	LED_Init(); //初始化与LED连接的硬件接口
	KEY_Init(); //初始化按键
	LCD_Init(); //初始化LCD
	My_RTC_Init();
	usart3_init(115200);	 //初始化串口3
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

