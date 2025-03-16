#include "ldr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_11
#define EXAMPLE_ADC_UNIT ADC_UNIT_2
#define EXAMPLE_ADC_CHANNEL ADC_CHANNEL_1

static const char *TAG = "LDR_SENSOR";
int ldr_value;
int ldr_percentage = 0;
static adc_oneshot_unit_handle_t adc2_handle;

void ldr_sensor_init(void) {
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = EXAMPLE_ADC_UNIT,
    };
    adc_oneshot_new_unit(&init_config, &adc2_handle);

    adc_oneshot_chan_cfg_t config = {
        .atten = EXAMPLE_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(adc2_handle, EXAMPLE_ADC_CHANNEL, &config);

    ESP_LOGI(TAG, "LDR sensor initialized");
}

int ldr_sensor_get_percentage(void) {
    // Faz a leitura diretamente quando chamado
    adc_oneshot_read(adc2_handle, EXAMPLE_ADC_CHANNEL, &ldr_value);
    ESP_LOGI(TAG, "LDR sensor reading (raw): %d", ldr_value);
    ldr_percentage = 100 - ((ldr_value * 100) / 4095);
    ldr_percentage = ldr_percentage < 0 ? 0 : (ldr_percentage > 100 ? 100 : ldr_percentage);
    return ldr_percentage;
}
