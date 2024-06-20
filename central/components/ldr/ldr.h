#ifndef LDR_H
#define LDR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_11
//#include "esp_timer.h"
#include "esp_log.h" // For ESP_LOGI macro


void ldr_sensor_init(void);
int ldr_sensor_read_raw(void);
int ldr_sensor_read_percentage(void);
#endif // LDR_H
