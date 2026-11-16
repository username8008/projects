#include "transmitter.h"

#define LED_PIN 25
#define PHOTO_RESISTOR_PIN 35
#define ONE_VAL 2000
#define POL 0x07

adc_oneshot_unit_handle_t adc1_handle = NULL;
QueueHandle_t tcpToMainQueue = NULL;

//TRANSMITTER


void setup()
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);


    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12,
        //.bitwidth = ADC_BITWIDTH_DEFAULT,
        //.atten = ADC_ATTEN_DB_6, with this attenuation, the reading got to the maximum value of 4095, which is not what i want, so i changed it to 12db attenuation
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_7, &config);

    //wifi and tcp connection

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());
}

void app_main(void)
{
    setup();

    tcpToMainQueue = xQueueCreate(10, sizeof(tcp_msg_t));
    if (tcpToMainQueue == NULL)
    {
        ESP_LOGE(TAG1, "Failed to create queue!");
        return;
    }

    char word[256]="Hello\n";

    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void*)AF_INET, 5, NULL);

    int adc_read=0;
    while(1)
    {
        //printf("ADC Read: %d\n", adc_read); //debug

        tcp_msg_t received_msg;
        if (xQueueReceive(tcpToMainQueue, &received_msg, 0) == pdTRUE)
        {
            ESP_LOGI(TAG1, "MAIN got message (%d bytes): %s", received_msg.length, received_msg.data);
            memset(word, 0, sizeof(word));
            strncpy(word, received_msg.data, sizeof(word) - 1);
            strncat(word, "\n", sizeof(word) - strlen(word) - 1);
        }

        for(int i = 0; word[i] != '\0'; i++)
        {
            uint16_t currentChar = word[i];
            int decData = (int)currentChar;

            adc_oneshot_read(adc1_handle, ADC_CHANNEL_7, &adc_read);
            while(adc_read<ONE_VAL) //wait for the reciver to be ready!
            {
                //printf("ADC Read: %d\n", adc_read); //debug
                vTaskDelay(pdMS_TO_TICKS(80));
                adc_oneshot_read(adc1_handle, ADC_CHANNEL_7, &adc_read);
            }

            printf("curr char: %c, curr dec: %d\n", currentChar, decData);



            currentChar = crc_stuff(currentChar);


            
            printf("char with crc in binary: ");
            for (int bit = 15; bit >= 0; bit--)
            {
                if ((currentChar & (1<<bit)) !=0)
                {
                    gpio_set_level(LED_PIN, 1);
                    printf("1");
                }
                else
                {
                    gpio_set_level(LED_PIN, 0);
                    printf("0");
                }
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            printf("\n\n");
        }
    }
}



uint16_t crc_stuff(uint16_t currChar)
{
    uint8_t crc = 0x00;
    crc ^= currChar;

    for (uint8_t j = 0; j < 8; j++)
    {
        if (crc & 0x80) //MSB = 1?
        {
            crc = (crc << 1) ^ POL;
        }
        else
        {
            crc <<= 1;
        }
    }

    uint16_t cahrWithCRC = (currChar<<8);
    cahrWithCRC |= crc;
    return cahrWithCRC;
}

