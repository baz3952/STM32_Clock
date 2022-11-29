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
    int tm_sec;  /* �� �C ȡֵ����Ϊ[0,59] */
    int tm_min;  /* �� - ȡֵ����Ϊ[0,59] */
    int tm_hour; /* ʱ - ȡֵ����Ϊ[0,23] */
    int tm_mday; /* һ�����е����� - ȡֵ����Ϊ[1,31] */
    int tm_mon; /* �·ݣ���һ�¿�ʼ��0����һ�£� - ȡֵ����Ϊ[0,11] */
    int tm_year; /* ��ݣ���ֵ����ʵ����ݼ�ȥ1900 */
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

    //ȡ��ʱ��
    t->tm_sec = (int)(time % 60);
    time /= 60;
    //ȡ����ʱ��
    t->tm_min = (int)(time % 60);
    time /= 60;
    //ȡ��ȥ���ٸ����꣬ÿ������ 1461*24 Сʱ
    Pass4year = ((unsigned int)time / (1461L * 24L));
    //�������
    t->tm_year = (Pass4year << 2) + 1970;
    //������ʣ�µ�Сʱ��
    time %= 1461L * 24L;
    //У������Ӱ�����ݣ�����һ����ʣ�µ�Сʱ��
    for (;;) {
        //һ���Сʱ��
        hours_per_year = 365 * 24;
        //�ж�����
        if ((t->tm_year & 3) == 0) {
            //�����꣬һ�����24Сʱ����һ��
            hours_per_year += 24;
        }
        if (time < hours_per_year) {
            break;
        }
        t->tm_year++;
        time -= hours_per_year;
    }
    //Сʱ��
    t->tm_hour = (int)(time % 24);
    //һ����ʣ�µ�����
    time /= 24;
    //�ٶ�Ϊ����
    time++;
    //У��������������·ݣ�����
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
    //��������
    for (t->tm_mon = 0; Days[t->tm_mon] < time; t->tm_mon++) {
        time -= Days[t->tm_mon];
    }
    t->tm_mday = (int)(time);
    return;
}
