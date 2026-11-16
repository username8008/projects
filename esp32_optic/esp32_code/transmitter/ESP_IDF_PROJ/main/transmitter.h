#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"
#include "freertos/queue.h"


static const char *TAG1 = "DATA_TRANSFER";
extern adc_oneshot_unit_handle_t adc1_handle;
extern QueueHandle_t tcpToMainQueue;
static char data[20];
static int dataIndex = 0;

typedef struct {
    int length;
    char data[128];
} tcp_msg_t;

uint16_t crc_stuff(uint16_t currChar);
void tcp_server_task(void *pvParameters);

#endif