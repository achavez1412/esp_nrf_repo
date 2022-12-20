/* SD card and FAT filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

// This example uses SDMMC peripheral to communicate with SD card.

#include <errno.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "sd_card_headers/sd_card_init.h"
#include "sd_card_headers/sd_card_make_file.h"

static const char *TAG = "app_main_make_file";
char *filename = "test.txt";
#define MOUNT_POINT "/sd_card";

//void app_main(esp_ble_gattc_cb_param_t* p_data){
void app_main(void){
    //declare objects needed to interface with the SD_Card utilizing SDMMC on FAT filesystem
    esp_err_t ret;
    sdmmc_card_t *card;
    esp_vfs_fat_sdmmc_mount_config_t mount_config;
    //mount_config_init(&mount_config);
    const char mount_point[] = MOUNT_POINT;
    sdmmc_host_t host;
    //sdmmc_host_init(&host); 
    sdmmc_slot_config_t slot_config;
    //sdmmc_slot_config_init(&config);
    bool ready = false;
    //instantiate and configue sd_card properties
    sd_card_init(TAG, ret, card, mount_config, mount_point, slot_config, host);
    ready = true;
    
    //makeshift sensor reading
    //struct data_packet send_data = {.temperature = 30, .humidity = 23, .id = 123456};
    //data_packet data_array[len(send_data)];
    //one set 16 bytes 32bit(4) 32(4) 64(8)
    uint8_t data_array[16] = {0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x0};
    //uint8_t data_array = p_data->notify.value;

    // if(text_data == NULL || errno == ENONMEM){
    //     ESP_LOGE(TAG, "There was an error in creating/writing to a file with error: (%s)", ESP_FAIL);
    //     return;
    // }
    // if(sprintf(text_data, "%f", f) == -1){
    //     ESP_LOGE(TAG, "There was an error in creating/writing to a file with error: (%s)", ESP_FAIL);
    //     return;
    // }

    //make file
    size_t bytes_to_write = 16;
    sd_card_write_file(ready, filename, &data_array, bytes_to_write, TAG, ret, card, mount_point);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "There was an error in creating/writing to a file with error: (%s)", esp_err_to_name(ret));
    }

    sd_card_read_file(ready, filename, &data_array, bytes_to_write, TAG, ret, card, mount_point);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "There was an error in creating/writing to a file with error: (%s)", esp_err_to_name(ret));
    }
    //cleanup sd connection after finishing
    //free(text_data);
    sd_card_deactivate(TAG, mount_point, card, host);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "There was an error in creating/writing to a file with error: (%s)", esp_err_to_name(ret));
    }
    return;
}