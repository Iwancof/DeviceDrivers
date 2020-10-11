#include "i2c_data_struct.c"

// LCD Options
#define LCD_BACK_LIGHT  0b00001000

// L2C Options
#define I2C_READ_MODE 1
#define I2C_WRITE_MODE 0

u_int32_t I2CDeviceWrite(const I2CDevice device, const I2C8bitData data, const int mock_print_data) {
  if(device._is_mock & mock_print_data) {
    show_simple_i2c_data(data);
    return 0;
  } else {
    //return wiringPiI2CWrite(device.fd, data.raw_data | LCD_BACK_LIGHT);
    return 0;
  }
}

u_int32_t raw_I2C_data_write(const I2CDevice device, const I2C8bitData data, const int mock_print_data) {
  const u_int32_t ret = I2CDeviceWrite(device, data, mock_print_data);
  if(ret != 0) {
    printf("[ERROR] I2C write error. fd = %d\n");
    show_i2c_data(data);
    return ret;
  }
  return 0;
}

void I2C_data_write(const I2CDevice device, I2C8bitData data) {
  show_simple_i2c_data_when_enable(data);
  // show_i2c_data(data);
  data.field.enable = 0;
  raw_I2C_data_write(device, data, 0);
  //delayMicroseconds(10);
  data.field.enable = 1;
  raw_I2C_data_write(device, data, 0);
  //delayMicroseconds(10);
  data.field.enable = 0;
  raw_I2C_data_write(device, data, 0);
  //delayMicroseconds(100);
}

void I2C_send_value(const I2CDevice device, const u_int8_t value, const u_int8_t ctrl_option) {
  I2C8bitData data = {.raw_data = 0};

  // send high
  data.raw_data = (value & 0xf0) | ctrl_option;
  I2C_data_write(device, data);

  // send low
  data.raw_data = ((value & 0x0f) << 4) | ctrl_option;
  I2C_data_write(device, data);
}
