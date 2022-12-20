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
//#include "sd_card_headers/sd_card_init.h"
#include "sd_card_headers/sd_card_make_file.h"

static const char *TAG = "app_main_make_file";
char *filename = "/test.txt";
#define MOUNT_POINT "/sdcard";

//void app_main(esp_ble_gattc_cb_param_t* p_data){
void app_main(void){
    //declare objects needed to interface with the SD_Card utilizing SDMMC on FAT filesystem
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    #ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
    #else
        .format_if_mount_failed = false,
    #endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    bool ready = false;
    esp_err_t ret;
    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT(); 
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    //printf("%s",slot_config.d0);
    ESP_LOGI(TAG, "Initializing SD card");

    #ifdef CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_1
        slot_config.width = 1;
    #else
        slot_config.width = 1;
    #endif
    #ifdef CONFIG_SOC_SDMMC_USE_GPIO_MATRIX
        slot_config.clk = CONFIG_EXAMPLE_PIN_CLK;
        slot_config.cmd = CONFIG_EXAMPLE_PIN_CMD;
        slot_config.d0 = CONFIG_EXAMPLE_PIN_D0;
    #ifdef CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
        slot_config.d1 = CONFIG_EXAMPLE_PIN_D1;
        slot_config.d2 = CONFIG_EXAMPLE_PIN_D2;
        slot_config.d3 = CONFIG_EXAMPLE_PIN_D3;
    #endif  // CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
    #endif  // CONFIG_SOC_SDMMC_USE_GPIO_MATRIX

    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
    //instantiate and configue sd_card properties
    //sd_card_init(TAG, ret, card, mount_config, mount_point, slot_config, host);
    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem."
                     "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
    ready = true;
    ESP_LOGI(TAG, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
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
    sd_card_write_file(ready, filename, data_array, bytes_to_write, TAG, ret, card, mount_point);
    if(ret == ESP_FAIL){
        ESP_LOGE(TAG, "There was an error in creating/writing to a file with error: (%s)", esp_err_to_name(ret));
        return;
    }

    sd_card_read_file(ready, filename, data_array, bytes_to_write, TAG, ret, card, mount_point);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "There was an error in creating/writing to a file with error: (%s)", esp_err_to_name(ret));
    }
    //cleanup sd connection after finishing
    //free(text_data);
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    //sdmmc_host_deinit();
    ESP_LOGI(TAG, "Card unmounted");
    //sd_card_deactivate(TAG, mount_point, card, host);
    // if(ret != ESP_OK){
    //     ESP_LOGE(TAG, "There was an error in creating/writing to a file with error: (%s)", esp_err_to_name(ret));
    // }
    return;
}