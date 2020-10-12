#include<wiringPiI2C.h>
#include<wiringPi.h>
#include "data_struct.c"

// API
void I2C_4bit_mode_write_command(LCDevice*, u_int8_t);
void I2C_4bit_mode_write_data(LCDevice*, u_int8_t);
void I2C_8bit_mode_write_upper(LCDevice*, u_int8_t); // evaluate only upper 4bit
// void I2C_8bit_mode_write_raw(LCDevice, u_int8_t); // unsafe

// Low Level Wrapper
void I2C_write_enabled_data_unit(LCDevice*, I2CDataUnit);
void I2C_raw_write_data_unit(LCDevice*, I2CDataUnit);


void I2C_write_enabled_data_unit(LCDevice *device, I2CDataUnit du) {
  if(device -> _is_print) {
    show_simple_data_unit(du);
  }

  write_enable_data_unit(&du, I2C_DISABLE);
  I2C_raw_write_data_unit(device, du);
  delayMicroseconds(1);

  write_enable_data_unit(&du, I2C_ENABLE); // enable.
  I2C_raw_write_data_unit(device, du);
  delayMicroseconds(1);

  write_enable_data_unit(&du, I2C_DISABLE); // disable
  I2C_raw_write_data_unit(device, du);
  delayMicroseconds(50);
}

void I2C_raw_write_data_unit(LCDevice *device, I2CDataUnit du) {
  if(device -> _is_mock == 1) { // this is mock.
    return;
  }
  du.field.back_light = LCD_BL_ON;
  wiringPiI2CWrite(device -> fd, du.raw);
}


void I2C_4bit_mode_write_command(LCDevice *device, u_int8_t data) {
  // Write, Upper -> Buttom
  
  // Upper
  I2CDataUnit du = {.raw = 0};
  write_to_data_unit_upper(&du, data);
  I2C_write_enabled_data_unit(device, du);

  // Buttom
  data <<= 4;
  write_to_data_unit_upper(&du, data);
  I2C_write_enabled_data_unit(device, du);
}

void I2C_4bit_mode_write_data(LCDevice *device, u_int8_t data) {
  device -> state[device->cursor_x][device->cursor_y] = (unsigned char)data;
  device -> cursor_x++;

  I2CDataUnit du = {.raw = 0, .field.register_selector = I2C_RS_DATA};
  write_to_data_unit_upper(&du, data);
  I2C_write_enabled_data_unit(device, du);

  data <<= 4;
  write_to_data_unit_upper(&du, data);
  I2C_write_enabled_data_unit(device, du);
}

void I2C_8bit_mode_write_upper(LCDevice *device, u_int8_t data) {
  I2CDataUnit du = {.raw = 0};
  write_to_data_unit_upper(&du, data);
  I2C_write_enabled_data_unit(device, du);
}

/*
void I2C_8bit_mode_write_raw(LCDevice device, u_int8_t data) {
  I2CDataUnit du = {.raw = 0};
  write_to_data_unit_upper(&du, data);
  wiringPiI2CWrite(device.fd, du.raw);
}
*/
