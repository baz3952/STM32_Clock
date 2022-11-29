#include "calendar.h"
#include "rtc.h"
#include "lcd.h"



void calendar_get_time(_calendar_obj *calendarx)//得到时间
{
	RTC_Get_Time(&calendarx->hour,&calendarx->min,&calendarx->sec,&calendarx->ampm);	
}

//画表盘
void calendar_circle_clock_drawpanel(u16 x,u16 y,u16 size,u16 d)
{
	u16 r=size/2;//半径 
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1; 
	u16 i; 
	LCD_Draw_Circle(x,y,r,WHITE);		//外圈
	LCD_Draw_Circle(x,y,r-4,BLACK);		//内圈
	for(i=0;i<60;i++)//画秒钟格
	{ 
		px0=sx+r+(r-4)*sin((app_pi/30)*i); 
		py0=sy+r-(r-4)*cos((app_pi/30)*i); 
		px1=sx+r+(r-d)*sin((app_pi/30)*i); 
		py1=sy+r-(r-d)*cos((app_pi/30)*i);  
		gui_draw_bline1(px0,py0,px1,py1,0,WHITE);		
	}
	for(i=0;i<12;i++)//画小时格
	{ 
		px0=sx+r+(r-5)*sin((app_pi/6)*i); 
		py0=sy+r-(r-5)*cos((app_pi/6)*i); 
		px1=sx+r+(r-d)*sin((app_pi/6)*i); 
		py1=sy+r-(r-d)*cos((app_pi/6)*i);  
		gui_draw_bline1(px0,py0,px1,py1,2,YELLOW);		
	}
	for(i=0;i<4;i++)//画3小时格
	{ 
		px0=sx+r+(r-5)*sin((app_pi/2)*i); 
		py0=sy+r-(r-5)*cos((app_pi/2)*i); 
		px1=sx+r+(r-d-3)*sin((app_pi/2)*i); 
		py1=sy+r-(r-d-3)*cos((app_pi/2)*i);  
		gui_draw_bline1(px0,py0,px1,py1,2,YELLOW);		
	}
	LCD_Draw_Circle(x,y,d/2,WHITE);		//画中心圈
}

//显示时间
void calendar_circle_clock_showtime(u16 x,u16 y,u16 size,u16 d,u8 hour,u8 min,u8 sec)
{
	static u8 oldhour=0;
	static u8 oldmin=0;
	static u8 oldsec=0;
	float temp;
	u16 r=size/2;//半径 
	u16 sx=x-r;
	u16 sy=y-r;
	u16 px0,px1;
	u16 py0,py1;  
	u8 r1; 
	if(hour>11)hour-=12;
	r1=d/2+4;
	temp=(float)oldmin/60;
	temp+=oldhour;
	px0=sx+r+(r-3*d-7)*sin((app_pi/6)*temp); 
	py0=sy+r-(r-3*d-7)*cos((app_pi/6)*temp); 
	px1=sx+r+r1*sin((app_pi/6)*temp); 
	py1=sy+r-r1*cos((app_pi/6)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,2,BLACK);
	r1=d/2+3;
	temp=(float)oldsec/60;
	temp+=oldmin;
	px0=sx+r+(r-2*d-7)*sin((app_pi/30)*temp); 
	py0=sy+r-(r-2*d-7)*cos((app_pi/30)*temp); 
	px1=sx+r+r1*sin((app_pi/30)*temp); 
	py1=sy+r-r1*cos((app_pi/30)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,1,BLACK); 
	r1=d/2+3;
	px0=sx+r+(r-d-7)*sin((app_pi/30)*oldsec); 
	py0=sy+r-(r-d-7)*cos((app_pi/30)*oldsec); 
	px1=sx+r+r1*sin((app_pi/30)*oldsec); 
	py1=sy+r-r1*cos((app_pi/30)*oldsec); 
	gui_draw_bline1(px0,py0,px1,py1,0,BLACK); 

	//显示新的时钟
	//显示小时 
	r1=d/2+4; 
	temp=(float)min/60;
	temp+=hour;
	px0=sx+r+(r-3*d-7)*sin((app_pi/6)*temp); 
	py0=sy+r-(r-3*d-7)*cos((app_pi/6)*temp); 
	px1=sx+r+r1*sin((app_pi/6)*temp); 
	py1=sy+r-r1*cos((app_pi/6)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,2,YELLOW); 
	//显示分钟 
	r1=d/2+3; 
	temp=(float)sec/60;
	temp+=min;
	//显示新的分钟
	px0=sx+r+(r-2*d-7)*sin((app_pi/30)*temp); 
	py0=sy+r-(r-2*d-7)*cos((app_pi/30)*temp); 
	px1=sx+r+r1*sin((app_pi/30)*temp); 
	py1=sy+r-r1*cos((app_pi/30)*temp); 
	gui_draw_bline1(px0,py0,px1,py1,1,GREEN); 	
	//显示秒钟  
	r1=d/2+3;
	//显示新的秒钟
	px0=sx+r+(r-d-7)*sin((app_pi/30)*sec); 
	py0=sy+r-(r-d-7)*cos((app_pi/30)*sec);  
	px1=sx+r+r1*sin((app_pi/30)*sec); 
	py1=sy+r-r1*cos((app_pi/30)*sec); 
	gui_draw_bline1(px0,py0,px1,py1,0,RED); 
	oldhour=hour;
	oldmin=min;
	oldsec=sec;
}

void calendar_play(void)
{
	_calendar_obj calendar;
	u8 second = 0;
	u16 r=0;
	u8 d=0;
	u16 yoff=0;
	if(lcddev.width==240)
	{
		r=80;
		d=7; 
	}
	else if(lcddev.width==320)
	{
		r=120;
		d=9; 
	}
	else if(lcddev.width==480)
	{
		r=160;
		d=12; 
	}
	yoff=(lcddev.height-r*2-140)/2;
	TIME_TOPY=yoff+r*2+10;
	OTHER_TOPY=TIME_TOPY+60+10;
	//xoff=(lcddev.width-240)/2;   
	second = calendar.sec;
	calendar_circle_clock_drawpanel(lcddev.width/2,yoff+r,r*2,d);//显示指针时钟表盘
	calendar_get_time(&calendar);	//更新时间  
	if(second!=calendar.sec)//秒钟改变了
	{ 	
		second=calendar.sec;  
		calendar_circle_clock_showtime(lcddev.width/2,yoff+r,r*2,d,calendar.hour,calendar.min,calendar.sec);//指针时钟显示时间
		
		LCD_ShowNum(180,130,calendar.hour,2,24,GBLUE,BLACK);
		LCD_ShowChar(205,130,':',24,0,GBLUE,BLACK);
		
		LCD_ShowNum(220,130,calendar.min,2,24,GBLUE,BLACK);
		if(calendar.min < 10) LCD_ShowNum(219,130,0,1,24,GBLUE,BLACK);;
		LCD_ShowChar(245,130,':',24,0,GBLUE,BLACK);
		
		LCD_ShowNum(260,130,calendar.sec,2,24,GBLUE,BLACK);
		if(calendar.sec < 10) LCD_ShowNum(259,130,0,1,24,GBLUE,BLACK);
	}
}







