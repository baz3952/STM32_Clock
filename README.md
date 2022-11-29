# STM32+ESP8266 获取时间
#### 介绍
基于STM32对淘宝事件服务器（api.m.taobao.com）时间进行解析，并显示在开发板的LCD上。

#### 硬件
正点原子STM32探索者开发板+ESP8266 WiFi模块+正点原子4.3寸TFTLCD。

#### 注意
在烧录之前需要修改wifista.c中的wifista_ssid，wifista_password为自己的WiFi名称及密码，