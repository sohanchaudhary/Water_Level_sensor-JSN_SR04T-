#include <stdio.h>
#include "freertos/FREERTOS.h"
#include "freertos/task.h"
#include "jsn_sr04t.h"


#define TRIGGER_GPIO_PIN            16
#define ECHO_GPIO_PIN               14


void data_read(void *pvParameters)
{
    esp_err_t ret = ESP_OK;
    ESP_LOGI(TAG, "%s()", __FUNCTION__);
    jsn_sr04t_config_t jsn_sr04t_config = JSN_SR04T_CONFIG_DEFAULT();
    jsn_sr04t_config.trigger_gpio_num = TRIGGER_GPIO_PIN;
    jsn_sr04t_config.echo_gpio_num = ECHO_GPIO_PIN;
    jsn_sr04t_config.rmt_channel = 4;

    if(ESP_OK != init_JSN_SR04T(&jsn_sr04t_config))
    {
        ESP_LOGE(TAG, "JSN_SR04T not initialized");
    }
    else
    {
        ESP_LOGI(TAG, "JSN_SR04T initialized");
    }

    while(1)
    {
        jsn_sr04t_data_t jsn_sr04t_data = JSN_SR04T_DATA_DEFAULT();
        ret = measurement(&jsn_sr04t_config, &jsn_sr04t_data);
        if(ESP_OK == ret)
        {
            jsn_sr04t_print_data(jsn_sr04t_data);
        }
        else
        {
            ESP_LOGE(TAG,"ERROR in getting measurement");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    xTaskCreate(&data_read, "SENSOR_DATA_READING", 2048 * 4, NULL, 10, NULL);
}
