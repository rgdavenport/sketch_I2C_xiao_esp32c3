# sketch_I2C_xiao_esp32c3
Version of I2C scan program designed to find the I2C address (0x19 in this example) of the MEMS Accelerometer in the STEVAL_MKI151V1 24 PIN DIP Evaluation module.  

Note that in order to function properly, CS (pin 19 of the eval module) and Vdd_IO (pin 2 of the eval module) must be wired to 3.3v (pin 3V3 of the xiao ESP32-C3 Risc V processor).

Currently the SDA IO (pin 21 of the eval module) is wired to D4, default SDA pin of the xiao ESP32-C3.  The SCL IO (pin 20 of the Eval module) is wired to D3, the default SDA pin of the xiao ESP32-C.
