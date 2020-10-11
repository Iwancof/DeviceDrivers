

typedef struct {
  unsigned int register_selector: 1;
  unsigned int read_write: 1;
  unsigned int enable: 1;
  unsigned int back_light: 1;
  unsigned int bus4: 1;
  unsigned int bus5: 1;
  unsigned int bus6: 1;
  unsigned int bus7: 1;
} I2C8bitDataField;

typedef union {
  I2C8bitDataField field;
  u_int8_t raw_data;
} I2C8bitData;

typedef struct {
  u_int32_t fd;
  int _is_mock;
} I2CDevice;

I2CDevice make_mock_from_fd(u_int32_t fd) {
  I2CDevice ret;
  ret.fd = fd;
  ret._is_mock = 1;

  puts("RS\tRW\tBL\tEnable\t\tBUS7\tBUS6\tBUS5\tBUS4");

  return ret;
}
void show_simple_i2c_data(I2C8bitData data) {
  printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t%d\t\n",
      data.field.register_selector,
      data.field.read_write,
      data.field.back_light,
      data.field.enable,
      data.field.bus7,
      data.field.bus6,
      data.field.bus5,
      data.field.bus4
      );
}
void show_simple_i2c_data_when_enable(I2C8bitData data) {
  printf("%d\t%d\t%d\t%s\t\t%d\t%d\t%d\t%d\t\n",
      data.field.register_selector,
      data.field.read_write,
      data.field.back_light,
      "0->1->0",
      data.field.bus7,
      data.field.bus6,
      data.field.bus5,
      data.field.bus4
      );
}
void show_i2c_data(I2C8bitData data) {
  printf("[DEBUG] I2C Data Details. %d\n", data.raw_data);
  if (data.field.register_selector == 1) {
    puts("  RS: Command Mode");
  } else {
    puts("  RS: Data Mode");
  }
  
  if (data.field.read_write == 1) {
    puts("  RW: Read Mode");
  } else {
    puts("  RW: Write Mode");
  }

  if (data.field.enable == 1) {
    puts("  Enable Bit: Enable");
  } else {
    puts("  Enable Bit: Disable");
  }

  if (data.field.back_light == 1) {
    puts("  Back Light: On");
  } else {
    puts("  Back Light: Off");
  }

  const u_int8_t transmission_data = (data.raw_data & 0xF0) >> 4;
  printf("  Transmission Data: %d, %d%d%d%d\n", transmission_data, (transmission_data & 0b1000) >> 3, (transmission_data & 0b0100) >> 2, (transmission_data & 0b0010) >> 1, transmission_data & 0b0001);
}

// Low level API
I2C8bitData make_data_from_raw(u_int8_t data) {
  I2C8bitData ret = {.raw_data = data };
  return ret;
}



