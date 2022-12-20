/*
    void sd_card_init(...) utilizes SDMMC at default configurations:{}, utilizing the FAT Filestystem at default configurations {}
    TODO: implement to take in configuation requirements

    This function takes in:

    - esp_err_t ret: return type that contains error codes, otherwise null
    - sdmmc_card_t *card: sd card object for utilizing (SDMMC)
    - esp_vfs_fat_sdmmc_mount_config_t *mount_config: fat object for filesystem config object
    - const char mount_point[]: specification of path for mounting sdcard (relative root)
    - sdmmc_host_t *host: host object that provides api functionality such as being able to utilize card object properly 
                        utilizes SDMMC peripheral, with 4-bit mode enabled, and max frequency set to 20MHz
    - sdmmc_slot_config_t *config: config object used to set up sdmmc communications with sdmmc_card object
*/

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"

void sd_card_init(const char *TAG, esp_err_t ret, sdmmc_card_t *card, esp_vfs_fat_sdmmc_mount_config_t mount_config, const char *mount_point, sdmmc_slot_config_t slot_config, sdmmc_host_t host){
        // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    // #ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
    //     mount_config->format_if_mount_failed = true;
    // #else
    // #endif
        // mount_config->format_if_mount_failed = false;
        // mount_config->max_files = 5;
        // mount_config->allocation_unit_size = 16 * 1024;
    // *mount_config = {
    //     #ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
    //             .format_if_mount_failed = true,
    //     #else
    //             .format_if_mount_failed = false,
    //     #endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
    //             .max_files = 5,
    //             .allocation_unit_size = 16 * 1024
    // };

    //mount_config = 
    //sdmmc_card_t *card;
    //const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");
    
    // ret = sdmmc_card_init(host,card);
    // if(ret != ESP_OK){
    //     ESP_LOGE(TAG,"Something went wrong in initializing the sdmmc card object");
    // }
    // Check source code and implement error recovery when developing
    // production applications.

    ESP_LOGI(TAG, "Using SDMMC peripheral");
    //sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host = SDMMC_HOST_DEFAULT();

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    //slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    // Set bus width to use:
#ifdef CONFIG_EXAMPLE_SDMMC_BUS_WIDTH_4
    slot_config.width = 4;
#else
    slot_config.width = 1;
#endif

    // On chips where the GPIOs used for SD card can be configured, set them in
    // the slot_config structure:
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

    // Enable internal pullups on enabled pins. The internal pullups
    // are insufficient however, please make sure 10k external pullups are
    // connected on the bus. This is for debug / example purpose only.
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
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
    ESP_LOGI(TAG, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
}

void sd_card_deactivate(const char *TAG, const char *mount_point, sdmmc_card_t *card, sdmmc_host_t host){
    // All done, unmount partition and disable SDMMC peripheral
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    //sdmmc_host_deinit();
    ESP_LOGI(TAG, "Card unmounted");
}

// void mount_config_init(esp_vfs_fat_sdmmc_mount_config_t *mount_config_loc){
//         mount_config->format_if_mount_failed = false;
//         mount_config->max_files = 5;
//         mount_config->allocation_unit_size = 16 * 1024;
// }

// void sdmmc_host_init(sdmmc_host_t *host){
//     *host = SDMMC_HOST_DEFAULT();
// }