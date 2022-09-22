/* FreeRTOS Real Time Stats Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"

char value1[50] = "I'm from Task1";
char value2[50] = "I'm from Task2";
char value3[50] = "I'm from Task3";

TaskHandle_t xTaskhandle1 = NULL;
TaskHandle_t xTaskhandle2 = NULL;
TaskHandle_t xTaskhandle3 = NULL;
QueueHandle_t xQueue = NULL;

static void Task1(void *arg)
{
    char *s = (char *) arg;
    //Print real time stats periodically
    while (1) {
        printf("Task1 is running %s\n", s);
        if(xQueue != pdFALSE){
          xQueueSendToBack(xQueue, (char*) s, 100);
        }
          else{
            printf("ERROR\n");
          }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void Task2(void *arg)
{
    char *s = (char*) arg;
    //Print real time stats periodically
    while (1) {
        printf("Task2 is running  %s\n", s);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void Task3(void *arg)
{
    char *s = (char*) arg;
    xQueueReceive(xQueue, s, 10);
    while (1) {
        printf("Task3 is running  %s\n", s);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    xQueue = xQueueCreate(1, (sizeof(char)*50));
    //Create and start stats task
    xTaskCreatePinnedToCore(Task1, "Task1", 4096, value1, 1, &xTaskhandle1, 0);
    xTaskCreatePinnedToCore(Task2, "Task2", 4096, value2, 1, &xTaskhandle2, 0);
    xTaskCreatePinnedToCore(Task3, "Task3", 4096, value3, 1, &xTaskhandle2, 0);
}
