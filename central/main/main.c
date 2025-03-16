#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "dht.h"                 // DHT component
#include "ldr.h"                 // LDR component
#include "wifi.h"                // Wi-Fi component
#include "mqtt_app.h"            // MQTT component

// DHT11 configuration
#define SENSOR_TYPE DHT_TYPE_DHT11
#define DHT_GPIO_PIN GPIO_NUM_15

// Tag for logging
static const char *TAG = "MQTT Example";

// Struct to hold sensor data
typedef struct {
    int16_t temperature;
    int16_t humidity;
    int ldr;
} sensor_data_t;

// Queue to hold sensor data
static QueueHandle_t sensor_data_queue;

bool wifi = false;

void wifi_mqtt_task(void *pvParameters) {
    ESP_ERROR_CHECK(wifi_init_sta());
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA iniciado...");

    mqtt_app_start();
    ESP_LOGI(TAG, "MQTT iniciado...");

    sensor_data_t sensor_data;
    while (1) {
        if (xQueueReceive(sensor_data_queue, &sensor_data, portMAX_DELAY)) {
            char temperature_str[10];
            char humidity_str[10];
            char ldr_str[10];

            sprintf(temperature_str, "%d", sensor_data.temperature);
            sprintf(humidity_str, "%d", sensor_data.humidity);
            sprintf(ldr_str, "%d", sensor_data.ldr);

            mqtt_app_publish("esp32-central/temperatura", temperature_str, 1, 1);
            mqtt_app_publish("esp32-central/umidade", humidity_str, 1, 1);
            mqtt_app_publish("esp32-central/ldr", ldr_str, 1, 1);
        }
    }
}

void dht_task(void *pvParameters) {
    int16_t temperature = 0;
    int16_t humidity = 0;

    while (1) {
        dht_read_data(SENSOR_TYPE, DHT_GPIO_PIN, &humidity, &temperature);
        ESP_LOGI(TAG, "Umidade: %d %%", humidity);
        ESP_LOGI(TAG, "Temperatura: %d C", temperature);

        sensor_data_t sensor_data;
        if (xQueueReceive(sensor_data_queue, &sensor_data, 0) == pdPASS) {  // Recebe a mensagem atual
            sensor_data.temperature = temperature;
            sensor_data.humidity = humidity;
            xQueueSend(sensor_data_queue, &sensor_data, portMAX_DELAY);
        } else {
            sensor_data.temperature = temperature;
            sensor_data.humidity = humidity;
            xQueueSend(sensor_data_queue, &sensor_data, portMAX_DELAY);
        }

        vTaskDelay(pdMS_TO_TICKS(5000));  // Leitura a cada 5 segundos
    }
}

void ldr_task(void *pvParameters) {
    int ldr = 0;
    ldr_sensor_init();

    while (1) {
        ldr = ldr_sensor_get_percentage();
        ESP_LOGI(TAG, "Percentagem iluminacao: %d %%", ldr);

        sensor_data_t sensor_data;
        if (xQueueReceive(sensor_data_queue, &sensor_data, 0) == pdPASS) {  // Recebe a mensagem atual
            sensor_data.ldr = ldr;
            xQueueSend(sensor_data_queue, &sensor_data, portMAX_DELAY);
        } else {
            sensor_data.ldr = ldr;
            xQueueSend(sensor_data_queue, &sensor_data, portMAX_DELAY);
        }

        vTaskDelay(pdMS_TO_TICKS(5000));  // Leitura a cada 5 segundos
    }
}

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    sensor_data_queue = xQueueCreate(10, sizeof(sensor_data_t));

    xTaskCreate(&wifi_mqtt_task, "wifi_mqtt_task", 4096, NULL, 5, NULL);
    xTaskCreate(&dht_task, "dht_task", 2048, NULL, 5, NULL);  
    xTaskCreate(&ldr_task, "ldr_task", 2048, NULL, 5, NULL);  
}
