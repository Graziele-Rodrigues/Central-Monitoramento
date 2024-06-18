/******************************************************************************
* This example shows how to read the temperature and humidity from the DHT11 sensor
* and public with mqtt
* Components Repositories: 
    dht11: https://github.com/UncleRus/esp-idf-lib
* This code was based on the lessons from the course "Programe o ESP32 com ESP-IDF 5" by Fábio Souza.
* The course is available on https://cursos.embarcados.com.br
*
* This example code Creative Commons Attribution 4.0 International License.
* When using the code, you must keep the above copyright notice,
* this list of conditions and the following disclaimer in the source code.
* (http://creativecommons.org/licenses/by/4.0/)

* Author: Graziele Rodrigues
* No warranty of any kind is provided.
*******************************************************************************/
//include the libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "dht.h"                 //dht component
#include "ldr.h"                 //ldr component
#include "wifi.h"                //wifi component
#include "mqtt_app.h"            //mqtt component

//DHT11 configuration
static const dht_sensor_type_t sensor_type = DHT_TYPE_DHT11;
static const gpio_num_t dht_gpio = 12;

//tag for logging
static const char *TAG = "MQTT Example";


void app_main(void)
{
    int16_t temperature = 0;        //temperature variable
    int16_t humidity = 0;           //humidity variable
    int ldr=0;
    // Inicializa o sensor LDR
    ldr_sensor_init();  

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {   //if nvs flash is not initialized
      ESP_ERROR_CHECK(nvs_flash_erase());                                             //erase nvs flash
      ret = nvs_flash_init();                                                         //initialize nvs flash
    }
    ESP_ERROR_CHECK(ret);                                                             //check error

    ESP_ERROR_CHECK(wifi_init_sta());                                 //initialize wifi station mode                                                           
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA iniciado...");                     //log wifi station mode started 

    mqtt_app_start();                                                   //start mqtt client 
    ESP_LOGI(TAG, "MQTT iniciado...");                                  //log mqtt started

    while(1)
    {
        char temperature_str[10];                                           //string to store temperature    
        char humidity_str[10];                                              //string to store humidity
        char ldr_str[10];                                                   //string to store ldr     
        //leitura dht 
        dht_read_data(sensor_type,dht_gpio,&humidity,&temperature);
        //LOG 
        ESP_LOGI(TAG, "humidity: %d %%" ,humidity);                          //print the humidity   
        ESP_LOGI(TAG, "Temperature: %d C", temperature);                     //print the temperature
        ldr = ldr_sensor_read_percentage();                                  //leitura ldr
        ESP_LOGI(TAG, "Percentage: %d C", ldr);
        sprintf(temperature_str,"%d",temperature);                           //convert to string
        sprintf(humidity_str,"%d",humidity);                                 //convert to string
        sprintf(ldr_str,"%d",ldr);                                            //convert to strinh
        mqtt_app_publish("esp32-central/temperatura",temperature_str,1,1);
        mqtt_app_publish("esp32-central/umidade",humidity_str,1,1);
        mqtt_app_publish("esp32-central/ldr",ldr_str,1,1);
        vTaskDelay(pdMS_TO_TICKS(5000));                                        //delay             
    }
}
