#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define ESP_INR_FLAG_DEFAULT 0
#define CONFIG_BUTTON_PIN 0
#define CONFIG_LED_PIN 2

TaskHandle_t ISR = NULL;

void IRAM_ATTR button_isr_handler(void *arg){
    xTaskResumeFromISR(ISR);
}


void button_task(){
    bool led_status = false;
    while (1)
    {
        vTaskSuspend(NULL);
        led_status = !led_status;
        gpio_set_level(CONFIG_LED_PIN, led_status);
        printf("Button Pressed\n");
    }
    
}


void app_main(void){

    esp_rom_gpio_pad_select_gpio(CONFIG_BUTTON_PIN);
    esp_rom_gpio_pad_select_gpio(CONFIG_LED_PIN);


    // set the correct direction

    gpio_set_direction(CONFIG_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(CONFIG_LED_PIN, GPIO_MODE_OUTPUT);

    // enable interrupt on falling edge
    gpio_set_intr_type(CONFIG_BUTTON_PIN, GPIO_INTR_NEGEDGE);

    //install the driver ISR handler service
    gpio_install_isr_service(ESP_INR_FLAG_DEFAULT);

    //attach the ISR
    gpio_isr_handler_add(CONFIG_BUTTON_PIN, button_isr_handler, NULL);

    //tasks

    xTaskCreate( button_task, "button_task", 4096, NULL, 10, &ISR);


}