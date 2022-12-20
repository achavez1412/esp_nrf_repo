void sd_card_init(const char *TAG, esp_err_t ret, sdmmc_card_t *card, esp_vfs_fat_sdmmc_mount_config_t mount_config, const char *mount_point, sdmmc_slot_config_t slot_config, sdmmc_host_t host);
void sd_card_deactivate(const char *TAG, const char *mount_point, sdmmc_card_t *card, sdmmc_host_t host);
//void mount_config_init(esp_vfs_fat_sdmmc_mount_config_t *mount_config_loc);