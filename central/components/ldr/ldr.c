#include "ldr.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

static const char *TAG = "LDR_SENSOR";
static int ldr_value;     //ADC Raw Data
adc_oneshot_unit_handle_t adc2_handle;   
adc_oneshot_unit_init_cfg_t init_config1 = {                        
    .unit_id = ADC_UNIT_2,                                    
}; 

void ldr_sensor_init(void) {
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc2_handle)); //ADC1 Init

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {                                                       //ADC1 Channel Config
        .bitwidth = ADC_BITWIDTH_DEFAULT,                                                   //ADC1 Bitwidth (Default)
        .atten = ADC_ATTEN_DB_11,                                                           //ADC1 Attenuation 
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC_CHANNEL_3, &config));       //ADC1 Channel Config
}

int ldr_sensor_read_raw(void) {
    adc_oneshot_read(adc2_handle, ADC_CHANNEL_3, &ldr_value);
    ESP_LOGI(TAG, "LDR sensor reading (raw): %d", ldr_value);
    return ldr_value;
}

int ldr_sensor_read_percentage(void) {
    adc_oneshot_read(adc2_handle, ADC_CHANNEL_3, &ldr_value);
    ESP_LOGI(TAG, "LDR sensor reading (raw): %d", ldr_value);
    // Convert to percentage
    int percentage = (ldr_value - 2500) * 100 / (4095 - 2500);
    percentage = percentage < 0 ? 0 : (percentage > 100 ? 100 : percentage);
    return percentage;
}
