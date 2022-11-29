#include "parsejson.h"
#include "cJSON.h"
#include "delay.h"
#include "rtc.h"
#include "usart3.h"
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int time_t;
const char Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct my_time {
    int tm_sec;  /* 秒 – 取值区间为[0,59] */
    int tm_min;  /* 分 - 取值区间为[0,59] */
    int tm_hour; /* 时 - 取值区间为[0,23] */
    int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */
    int tm_mon; /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
    int tm_year; /* 年份，其值等于实际年份减去1900 */
};

void Mylocaltime(time_t time, struct my_time *t);

u8 parse_now_time(void) {
    cJSON *root = NULL;
    cJSON *item = NULL;
    struct my_time t;
    char *i = 0;
    u32 time = 0;
    u8 hour = 0, ampm = 0;

    root = cJSON_Parse((const char *)USART3_RX_BUF);

    if (!root) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return 0;
    } else {
        item = cJSON_GetObjectItem(root, "data");
        item = cJSON_GetObjectItem(item, "t");
    }
    i = item->valuestring;
    time = atoll(i) / 1000;
    printf("TimeStamp:%d\r\n", time);
    Mylocaltime(time, &t);

    if (t.tm_hour + 8 > 12) {
        hour = t.tm_hour + 8 - 12;
        ampm = RTC_H12_PM;
    } else {
        hour = t.tm_hour;
        ampm = RTC_H12_AM;
    }
    RTC_Set_Time(hour, (u8)t.tm_min, (u8)t.tm_sec, ampm);
    RTC_Set_Date((u8)(t.tm_year + 1900), (u8)t.tm_mon + 1, (u8)t.tm_mday, 0);

    cJSON_Delete(root);
    cJSON_Delete(item);
    return 1;
}

void Mylocaltime(time_t time, struct my_time *t) {
    u32 Pass4year;
    int hours_per_year;

    //取秒时间
    t->tm_sec = (int)(time % 60);
    time /= 60;
    //取分钟时间
    t->tm_min = (int)(time % 60);
    time /= 60;
    //取过去多少个四年，每四年有 1461*24 小时
    Pass4year = ((unsigned int)time / (1461L * 24L));
    //计算年份
    t->tm_year = (Pass4year << 2) + 1970;
    //四年中剩下的小时数
    time %= 1461L * 24L;
    //校正闰年影响的年份，计算一年中剩下的小时数
    for (;;) {
        //一年的小时数
        hours_per_year = 365 * 24;
        //判断闰年
        if ((t->tm_year & 3) == 0) {
            //是闰年，一年则多24小时，即一天
            hours_per_year += 24;
        }
        if (time < hours_per_year) {
            break;
        }
        t->tm_year++;
        time -= hours_per_year;
    }
    //小时数
    t->tm_hour = (int)(time % 24);
    //一年中剩下的天数
    time /= 24;
    //假定为闰年
    time++;
    //校正闰年的误差，计算月份，日期
    if ((t->tm_year & 3) == 0) {
        if (time > 60)
            time--;
        else {
            if (time == 60) {
                t->tm_mon = 1;
                t->tm_mday = 29;
                return;
            }
        }
    }
    //计算月日
    for (t->tm_mon = 0; Days[t->tm_mon] < time; t->tm_mon++) {
        time -= Days[t->tm_mon];
    }
    t->tm_mday = (int)(time);
    return;
}
