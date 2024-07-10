/*
 * Copyright (c) 2023 Beijing HuaQing YuanJian Education Technology Co., Ltd
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "hal_bsp_sht20.h"
#include "hal_bsp_pcf8574.h"
#include "hal_bsp_ssd1306.h"
#include "hal_bsp_ssd1306_bmps.h"


osThreadId_t Task1_ID; //  任务1 ID

typedef struct {
    int top;  // 上边距
    int left; // 左边距
    int hight; // 高
    int width; // 宽
} margin_t;   // 边距类型

/* 标题-温度 */
margin_t temp_title = {
    .top = 0,
    .left = 0,
};
/* 标题-湿度 */
margin_t humi_title = {
    .top = 0,
    .left = 0,
};
/* 数字-十位 */
margin_t number_1 = {
    .top = 16,
    .left = 8,
    .width = 16,
    .hight = 32
};
/* 数字-个位 */
margin_t number_2 = {
    .top = 16,
    .left = 24,
    .width = 16,
    .hight = 32
};
/* 小数点 */
margin_t dian = {
    .top = 32,
    .left = 40,
    .width = 16,
    .hight = 16
};
/* 数字-小数 */
margin_t number_3 = {
    .top = 32,
    .left = 56,
    .width = 8,
    .hight = 16
};
/* 单位 */
margin_t danwei = {
    .top = 16,
    .left = 52,
    .width = 16,
    .hight = 16
};
/* 图片 */
margin_t image = {
    .top = 16,
    .left = 80,
    .width = 48,
    .hight = 48
};

#define TASK_STACK_SIZE (1024 * 5)
#define TASK_DELAY_TIME2 30 // s
#define TASK_DELAY_TIME1 40 // s
#define COEFFICIENT_10 10
#define COEFFICIENT_100 100
#define COEFFICIENT_1000 1000
typedef enum {
    TEMP_RANG_0 = 0,
    TEMP_RANG_6 = 6,
    TEMP_RANG_12 = 12,
    TEMP_RANG_18 = 18,
    TEMP_RANG_24 = 24,
}te_temperature_range_t;

typedef enum {
    HUMI_RANG_0 = 0,
    HUMI_RANG_20 = 20,
    HUMI_RANG_40 = 40,
    HUMI_RANG_60 = 60,
    HUMI_RANG_80 = 80,
}te_humidity_range_t;

/**
 * @brief  显示温度页面
 * @note
 * @param  val:
 * @retval None
 */

typedef struct {  
    int hour;  
    int minute;  
    char medicineName[50];  
} Reminder;  
  
bool timerTriggered = false;  
  
void ReceiveReminderFromApp(Reminder *reminder) {  
    reminder->hour = 12;  
    reminder->minute = 30;  
    snprintf(reminder->medicineName, sizeof(reminder->medicineName), "A药");  
}  
  
void SetReminderTimer(Reminder reminder) {  
    printf("定时器设置为 %02d:%02d，药品名称：%s\n", reminder.hour, reminder.minute, reminder.medicineName);  
    sleep(5);  
    timerTriggered = true;  
}  
  
void TimerCallback(Reminder reminder) {  
    if (timerTriggered) {  
        printf("到吃药时间了！您该吃%s了。\n", reminder.medicineName);  
        sleep(300); // 模拟5分钟等待用户响应  
        if (!CheckButtonPress()) {  
            SendMissedReminderToApp();  
        } else {  
            printf("用户已确认吃药。\n");  
        }  
    }  
}  
  
bool CheckButtonPress() {  
    char input[10];  
    printf("请输入'y'确认吃药，或输入其他任意键继续等待：");  
    scanf("%s", input);  
    return input[0] == 'y' || input[0] == 'Y';  
}  
  
void SendMissedReminderToApp() {  
    printf("用户未按时吃药，已向小程序发送通知。\n");  
}  
  
int main() {  
    Reminder reminder;  
    ReceiveReminderFromApp(&reminder);  
    SetReminderTimer(reminder);  
    while (!timerTriggered) {  
        sleep(1);  
    }  
    TimerCallback(reminder);  
    return 0;  
}

/**
 * @description: 任务1为低优先级任务
 * @param {*}
 * @return {*}
 */
void Task1(void)
{
    uint8_t x = 0, y = 0;
    while (1) {
       
        show_temp_page(); // 显示温度页面
        
    }
}
sendMeasureCommand();      
uint16_t rawTemperature = readMLX90614Temperature();    
float amplifiedTemperature = amplifySignal(rawTemperature);    
float processedTemperature = dspProcessSignal(amplifiedTemperature);      
if (!isTemperatureNormal(processedTemperature)) {    
    uploadTemperatureInfo(processedTemperature);    
}    
return 0;    
}    
  
void sendMeasureCommand() {    
    printf("Sending measure command to sensor...\n");    
}    
  
uint16_t readMLX90614Temperature() {    
    printf("Reading temperature from MLX90614...\n");    
    return 0x1234;    
}    
  
float amplifySignal(uint16_t rawSignal) {    
    return (float)rawSignal * 1.5;    
}    
  
float dspProcessSignal(float amplifiedSignal) {    
    return amplifiedSignal * 0.95;    
}    
  
int isTemperatureNormal(float temperature) {    
    const float normalMin = 36.0;    
    const float normalMax = 37.5;    
    return temperature >= normalMin && temperature <= normalMax;    
}    
  
void uploadTemperatureInfo(float temperature) {    
    printf("Uploading temperature info: %.2f°C\n", temperature);    
}
tn_pcf8574_io_t pcf8574_io = {0}; // IO扩展芯片的引脚  
osThreadId_t mqtt_send_task_id;  
osThreadId_t mqtt_recv_task_id;  
  
int8_t mqtt_sub_payload_callback(unsigned char *topic, unsigned char *payload)  
{  
    printf("[info] topic:[%s] recv<== %s\r\n", topic, payload);  
}  
  
void mqtt_recv_task(void)  
{  
    while (1) {  
        MQTTClient_sub();  
        usleep(MQTT_RECV_TASK_TIME);  
    }  
}  
  
void mqtt_send_task(void)  
{  
    if (WiFi_connectHotspots("AI_DEV", "HQYJ12345678") != WIFI_SUCCESS) {  
        printf("[error] WiFi_connectHotspots\r\n");  
    }  
  
    if (MQTTClient_connectServer(SERVER_IP_ADDR, SERVER_IP_PORT) != 0) {  
        printf("[error] MQTTClient_connectServer\r\n");  
    } else {  
        printf("[success] MQTTClient_connectServer\r\n");  
    }  
    sleep(TASK_INIT_TIME);  
  
    if (MQTTClient_init("mqtt_client_123", "username", "password") != 0) {  
        printf("[error] MQTTClient_init\r\n");  
    } else {  
        printf("[success] MQTTClient_init\r\n");  
    }  
    sleep(TASK_INIT_TIME);  
  
    if (MQTTClient_subscribe(MQTT_TOPIC_SUB) != 0) {  
        printf("[error] MQTTClient_subscribe\r\n");  
    } else {  
        printf("[success] MQTTClient_subscribe\r\n");  
    }  
    sleep(TASK_INIT_TIME);  
}

static void smartTemp_demo(void)
{
    printf("Enter smartTemp_demo()!");

    PCF8574_Init();                     // 初始化IO扩展芯片
    SHT20_Init();   // SHT20初始化
    SSD1306_Init(); // 初始化OLED
    SSD1306_CLS();  // 清屏

    osThreadAttr_t options;
    options.name = "Task1";              // 任务的名字
    options.attr_bits = 0;               // 属性位
    options.cb_mem = NULL;               // 堆空间地址
    options.cb_size = 0;                 // 堆空间大小
    options.stack_mem = NULL;            // 栈空间地址
    options.stack_size = TASK_STACK_SIZE;       // 栈空间大小 单位:字节
    options.priority = osPriorityNormal; // 任务的优先级

    Task1_ID = osThreadNew((osThreadFunc_t)Task1, NULL, &options); // 创建任务1
    if (Task1_ID != NULL) {
        printf("ID = %d, Create Task1_ID is OK!\n", Task1_ID);
    }
}
SYS_RUN(smartTemp_demo);