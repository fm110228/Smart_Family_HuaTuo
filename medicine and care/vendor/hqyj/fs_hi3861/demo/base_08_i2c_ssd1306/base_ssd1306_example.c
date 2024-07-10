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
#include "hal_bsp_ssd1306.h"
#include "hal_bsp_pcf8574.h"
#include "hal_bsp_ssd1306_bmps.h"
osThreadId_t Task1_ID; // 任务1设置为低优先级任务
#define TASK_STACK_SIZE 1024
#define SEC_MAX 60
#define MIN_MAX 60
#define HOUR_MAX 24

typedef struct {
    int top;  // 上边距
    int left; // 左边距
    int hight; // 高
    int width; // 宽
} margin_t;   // 边距类型

margin_t image = {
    .top = 16,
    .left = 80,
    .width = 48,
    .hight = 48
};
void Task1(void)
{
    uint8_t displayBuff[20] = {0};
    uint8_t hour = 10, min = 30, sec = 0;
    SSD1306_CLS(); // 清屏
    SSD1306_ShowStr(OLED_TEXT16_COLUMN_0, OLED_TEXT16_LINE_0, "  I am ", TEXT_SIZE_16);
    SSD1306_ShowStr(OLED_TEXT16_COLUMN_0, OLED_TEXT16_LINE_1, "  listening  ", TEXT_SIZE_16);
    SSD1306_DrawBMP(image.left, image.top, image.width, image.hight, bmp_48X48_1_mi_yan_xiao);
    while (1) {
        sec++;
        if (sec > (SEC_MAX - 1)) {
            sec = 0;
            min++;
        }
        if (min > (MIN_MAX - 1)) {
            min = 0;
            hour++;
        }
        if (hour > (HOUR_MAX - 1)) {
            hour = 0;
        }
        memset_s(displayBuff, sizeof(displayBuff), 0, sizeof(displayBuff));
      
        sleep(1); // 1 s
    }
}
static void base_sdd1306_demo(void)
{
    printf("Enter base_sdd1306_demo()!\r\n");

    PCF8574_Init();
    SSD1306_Init(); // OLED 显示屏初始化

    osThreadAttr_t options;
    options.name = "thread_1";
    options.attr_bits = 0;
    options.cb_mem = NULL;
    options.cb_size = 0;
    options.stack_mem = NULL;
    options.stack_size = TASK_STACK_SIZE;
    options.priority = osPriorityNormal;

    Task1_ID = osThreadNew((osThreadFunc_t)Task1, NULL, &options);
    if (Task1_ID != NULL) {
        printf("ID = %d, Create Task1_ID is OK!\r\n", Task1_ID);
    }
}
SYS_RUN(base_sdd1306_demo);