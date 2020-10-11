//#include<wiringPi.h>
//#include<wiringPiI2C.h>
#include<stdio.h>
#include<stdlib.h>

#include "i2c_data_control.c"

#define I2CReadMode 1
#define I2CWriteMode 0

void initialize_lcd(I2CDevice device) {
  I2C8bitData data = {.raw_data = 0};

  raw_I2C_data_write(device, data, 1);

  data.field.bus4 = 1;
  data.field.bus5 = 1;

  I2C_data_write(device, data);
  I2C_data_write(device, data);
  I2C_data_write(device, data);

  data.field.bus4 = 0;

  I2C_data_write(device, data);
  
  // Transmission mode change
  I2C_data_write(device, data);
  data.field.bus7 = 1;
  data.field.bus5 = 0;
  I2C_data_write(device, data);

  // Display off
  data.raw_data = 0;
  I2C_data_write(device, data);
  data.field.bus7 = 1;
  I2C_data_write(device, data);

  // Display clear
  data.raw_data = 0;
  I2C_data_write(device, data);
  data.field.bus4 = 1;
  I2C_data_write(device, data);

  // Entry mode
  data.raw_data = 0;
  I2C_data_write(device, data);
  data.field.bus6 = 1;
  data.field.bus5 = 1;
  I2C_data_write(device, data);

  data.raw_data = 0;
  I2C_data_write(device, data);
  data.field.bus5 = 1;
  I2C_data_write(device, data);

  show_i2c_data(data);

}

int main() {
  // u_int32_t fd = wiringPiI2CSetup(0x27);
  u_int32_t fd = 3;
  printf("[INFO] Start I2C Connection. fd = %d\n", fd);
  I2CDevice device = make_mock_from_fd(fd);

  initialize_lcd(device);

  I2C_send_value(device, 'A', 1);
}


