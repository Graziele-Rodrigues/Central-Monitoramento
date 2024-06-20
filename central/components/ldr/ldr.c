#include "ldr.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"


#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_11

static const char *TAG = "LDR_SENSOR";
static int ldr_value[1][0];
static bool example_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);
adc_oneshot_unit_handle_t adc2_handle;   
adc_cali_handle_t adc2_cali_handle = NULL;
bool do_calibration2 = false;

// Function to initialize ADC calibration
static bool example_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle) {
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }

    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}


void ldr_sensor_init(void) {
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc2_handle)); // ADC2 Init

    // ADC2 Channel Config
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ADC_CHANNEL_3, &config));

    // Initialize ADC2 Calibration
    do_calibration2 = example_adc_calibration_init(ADC_UNIT_2, ADC_CHANNEL_3, ADC_ATTEN_DB_11, &adc2_cali_handle);
}

int ldr_sensor_read_raw(void) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC_CHANNEL_3, &ldr_value[1][0]));
    ESP_LOGI(TAG, "LDR sensor reading (raw): %d", ldr_value[1][0]);
    return ldr_value[1][0];
}

int ldr_sensor_read_percentage(void) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc2_handle, ADC_CHANNEL_3, &ldr_value[1][0]));
    ESP_LOGI(TAG, "LDR sensor reading (raw): %d", ldr_value[1][0]);
    // Convert to percentage
    int percentage = 100 - ((ldr_value[1][0] * 100) / 4095);
    percentage = percentage < 0 ? 0 : (percentage > 100 ? 100 : percentage);
    return percentage;
}
 
