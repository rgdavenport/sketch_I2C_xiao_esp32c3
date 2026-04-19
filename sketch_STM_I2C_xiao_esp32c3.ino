#include <stdio.h>
#include "driver/i2c.h"

// Sensor Config
#define LIS2DH12_ADDR        0x19  // Or 0x18
#define REG_WHO_AM_I         0x0F
#define I2C_MASTER_NUM     I2C_NUM_0

// Hardware Config
#define I2C_MASTER_SCL_IO    D5    
#define I2C_MASTER_SDA_IO    D4    
#define I2C_MASTER_NUM       I2C_NUM_0
#define I2C_MASTER_FREQ_HZ   100000



void setup() {
    Serial.begin(115200);
    while(!Serial);

    // 1. Initialize Driver
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = { .clk_speed = I2C_MASTER_FREQ_HZ }
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    // 2. Perform one-time check
     check_sensor();
}


void loop() {
    // You can call your scanner here or move it to a specific task
    scan_bus();
    delay(5000);
}

 //--- Helper Functions ---

void check_sensor() {
    uint8_t reg = REG_WHO_AM_I;
    uint8_t id = 0;
    esp_err_t ret = i2c_master_write_read_device(I2C_MASTER_NUM, LIS2DH12_ADDR, &reg, 1, &id, 1, 1000 / portTICK_PERIOD_MS);
    
    if (ret == ESP_OK && id == 0x33) {
        Serial.printf("LIS2DH12 found! ID: 0x%02X\n", id);
    } else {
        Serial.println("Sensor check failed. Check address and wiring.");
    }
}

void scan_bus() {
    Serial.println("\nScanning I2C Bus...");
    int devices_found = 0;

    for (uint8_t i = 1; i < 127; i++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        // We only need to check if the device ACKs its address
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        
        esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 50 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret == ESP_OK) {
            Serial.printf("Found device at 0x%02X\n", i);
            devices_found++;
        }
    }
    if (devices_found == 0) Serial.println("No devices detected.");    // ... [Insert the scan_bus code from earlier] ...
}


