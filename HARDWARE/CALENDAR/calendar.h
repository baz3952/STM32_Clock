#ifndef _CALENDAR_H
#define _CALENDAR_H
	#include "sys.h"
	#include "math.h"
	typedef struct 
	{
		u8 hour;
		u8 min;
		u8 sec;			
		u8 ampm;
	}_calendar_obj;					 
	extern _calendar_obj calendar;

	static u16 TIME_TOPY;		//	120
	static u16 OTHER_TOPY;		//	200
	#define	app_pi	3.1415926535897932384626433832795 
	void calendar_play(void);
#endif

	
	
	
	
	

