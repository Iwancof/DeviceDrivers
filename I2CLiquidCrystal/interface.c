#include "i2c_wrapper.c"

void LCD_initialize(LCDevice *device) {
  delay(15);
  if(device -> _is_print) {
    puts("RS\tRW\tBL\tEnable\t\tBUS7\tBUS6\tBUS5\tBUS4\n");
  }

  I2C_8bit_mode_write_upper(device, I2C_DATA_8BIT_MODE);
  I2C_8bit_mode_write_upper(device, I2C_DATA_8BIT_MODE);
  I2C_8bit_mode_write_upper(device, I2C_DATA_8BIT_MODE);

  I2C_8bit_mode_write_upper(device, I2C_DATA_4BIT_MODE);

  I2C_4bit_mode_write_command(device, I2C_DATA_4BIT_MODE);
  I2C_4bit_mode_write_command(device, LCD_DISPLAY_OFF);
  I2C_4bit_mode_write_command(device, LCD_CLEAR_DISPLAY);
  for(int y = 0;y < 2;y++) {
    for(int x = 0;x < 16;x++) {
      device -> state[x][y] = ' ';
    }
  }
  device -> cursor_x = 0;
  device -> cursor_y = 0;

  delay(2);

  I2C_4bit_mode_write_command(device, LCD_ENTRYMODE);
  I2C_4bit_mode_write_command(device, LCD_DISPLAY_ON);
}

void LCD_set_cursor(LCDevice *device, u_int8_t x, u_int8_t y) {
  // no check range.

  I2C_4bit_mode_write_command(device, LCD_CURSOR_SETTING | (y * 0x40 + x));
  device -> cursor_x = x;
  device -> cursor_y = y;
}
void LCD_write_char(LCDevice *device, unsigned char c) {
  I2C_4bit_mode_write_data(device, c);
  /*
  if(0 <= device->cursor_x && device->cursor_x < 16 &&
      0 <= device->cursor_y && device->cursor_y < 2) {
    device -> state[device->cursor_x][device->cursor_y] = c;
  }
  */
  //device -> state[device->cursor_x][device->cursor_y] = c;
  //printf("%d %d\n", device->cursor_x, device->cursor_y);

      
}
void LCD_write_string(LCDevice *device, unsigned char* str) {
  char c;
  while(c = *(str++)) {
    switch (c) {
      case '\n':
        LCD_set_cursor(device, 0, device -> cursor_y + 1);
        break;
      case '\t':
        LCD_set_cursor(device, device -> cursor_x + 2, device -> cursor_y);
        break;
      default: 
        LCD_write_char(device, c);
        break;
    }
  }
}
void LCD_write_char_at(LCDevice *device, u_int8_t x, u_int8_t y, unsigned char c) {
  LCD_set_cursor(device, x, y);
  LCD_write_char(device, c);
}
void LCD_clear_display(LCDevice *device) {
  I2C_4bit_mode_write_command(device, 1);
  delay(2);
  for(int y = 0;y < 2;y++) {
    for(int x = 0;x < 16;x++) {
      device -> state[y][x] = ' ';
    }
  }
  device -> cursor_x = 0;
  device -> cursor_y = 0;
}


int main() {
  LCDevice device = get_i2c_device_with_id(0x27);
  device._is_print = 1;

  LCD_initialize(&device);

  LCD_write_string(&device, "Hello??\n");
  LCD_write_string(&device, "Hmm...");
  dump_lcd_device(&device);
  delay(1000);
  LCD_clear_display(&device);

  LCD_write_string(&device, "HI!!");

  dump_lcd_device(&device);
}

