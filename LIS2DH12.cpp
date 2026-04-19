#include <Wire.h> // Or your MCU I2C library

#define LIS2DH12_ADDR    0x19 // STEVAL-MKI151v1 default I2C address
#define CTRL_REG1        0x20
#define CTRL_REG4        0x23
#define OUT_X_L          0x28

void lis2dh12_init() {
  Wire.begin();
  
  // 1. Enable all axes, set 100Hz Normal Mode (ODR 0101)
  Wire.beginTransmission(LIS2DH12_ADDR);
  Wire.write(CTRL_REG1);
  Wire.write(0x57); // 0101 0111 (100Hz, normal, XYZ enabled)
  Wire.endTransmission();
  
  // 2. Set +/- 2g range, High-Resolution mode enabled
  Wire.beginTransmission(LIS2DH12_ADDR);
  Wire.write(CTRL_REG4);
  Wire.write(0x08); // 0000 1000
  Wire.endTransmission();
}

void read_accelerometer(int16_t *x) {
  Wire.beginTransmission(LIS2DH12_ADDR);
  Wire.write(OUT_X_L | 0x80); // Auto-increment address
  Wire.endTransmission();
  Wire.requestFrom(LIS2DH12_ADDR, 6); // Read 6 bytes (X, Y, Z)

  if (Wire.available() >= 6) {
    uint8_t x_l = Wire.read();
    uint8_t x_h = Wire.read();
    *x = (int16_t)((x_h << 8) | x_l); // Combine for 16-bit value
  }
}

