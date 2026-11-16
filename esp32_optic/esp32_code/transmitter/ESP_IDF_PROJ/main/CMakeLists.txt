idf_component_register(SRCS "main.c" "wifiAndTcp.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_driver_gpio esp_adc nvs_flash lwip esp_event esp_wifi protocol_examples_common)
