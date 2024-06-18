#ifndef LDR_H
#define LDR_H

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_system.h"
//#include "esp_timer.h"
#include "esp_log.h" // For ESP_LOGI macro


void ldr_sensor_init(void);
int ldr_sensor_read_raw(void);
int ldr_sensor_read_percentage(void);

#endif // LDR_H
