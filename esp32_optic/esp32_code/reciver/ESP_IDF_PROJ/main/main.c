#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PHOTO_RESISTOR_PIN 35
#define LED_PIN 26
#define ONE_VAL 2400
#define POL 0x07
#define HASH_SYMBOL 35 //hash symbol in ascii table
#define END_SYMBOL 10 //end of line symbol in ascii table

uint16_t crc_decode(uint16_t currChar);

//RECIVER

adc_oneshot_unit_handle_t adc1_handle;
static char data[25] = {0};
static int dataIndex = 0;

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
}

void app_main(void)
{
    setup();

    int adc_read=0;
    while(1)
    {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(LED_PIN, 0);
        int decData=0;

        for(int i=0; i<16; i++)
        {
            adc_oneshot_read(adc1_handle, ADC_CHANNEL_7, &adc_read);
            if(adc_read>ONE_VAL)
            {
                decData |= (1 << (15-i));
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        decData = crc_decode(decData);

        if (dataIndex < 25 && decData != END_SYMBOL)
        {
            data[dataIndex] = (char)decData;
            dataIndex++;
            data[dataIndex] = '\0';
        }
        else
        {
            dataIndex = 0;
            for(int i=0; i<25; i++)
            {
            data[i]='\0';
            }
        }
        printf("%d\n", decData);
        printf("%s\n", data);
    }
}


uint16_t crc_decode(uint16_t fullMessage)
{
    uint8_t highByte = (fullMessage >> 8);
    uint8_t lowByte = fullMessage;

    uint8_t crc = 0x00;

    crc ^= highByte;
    for (uint8_t j = 0; j < 8; j++)
    {
        if (crc & 0x80) {
            crc = (crc << 1) ^ POL;
        } else {
            crc <<= 1;
        }
    }

    crc ^= lowByte;
    for (uint8_t j = 0; j < 8; j++)
    {
        if (crc & 0x80) {
            crc = (crc << 1) ^ POL;
        } else {
            crc <<= 1;
        }
    }

    if (crc == 0)
    {
        return highByte;
    }
    
    return HASH_SYMBOL; //hash symbol in ascii table, to indicate that the message is corrupted
}