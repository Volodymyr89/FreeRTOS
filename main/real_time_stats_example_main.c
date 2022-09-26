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
typedef enum structDataID{
    I2C_ID,
    SPI_ID
}ID_t;

typedef struct{
    ID_t DataID;
    uint32_t Value;
}data_t;

TaskHandle_t xTaskhandle1 = NULL;
TaskHandle_t xTaskhandle2 = NULL;
QueueHandle_t xQueue = NULL;

static void Task1(void *arg)
{
    data_t *data = (data_t *) arg;
    data_t *receive = NULL;
    xQueueSend(xQueue, data, 100);
    xQueueReceive(xQueue, receive, 100);
    printf("Data received from %s and Value is %d", receive->DataID, receive->Value);
 
}

static void Task2(void *arg)
{
    data_t *data = (data_t *) arg;
    xQueueSend(xQueue, data, 100);
}


void app_main(void)
{
    const data_t dataID[2] = {
        { .DataID = I2C_ID, .Value = 10},
        { .DataID = SPI_ID, .Value = 20}
    };
    xQueue = xQueueCreate(2, sizeof(dataID));
    if(xQueue !=NULL){
        xTaskCreatePinnedToCore(Task1, "Task1", 4096, &(dataID[0]), 1, &xTaskhandle1, 0);
        xTaskCreatePinnedToCore(Task2, "Task2", 4096, &(dataID[1]), 2, &xTaskhandle2, 0);
    }
}
