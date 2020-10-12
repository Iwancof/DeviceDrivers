#include<stdlib.h>
#include<stdio.h>

#define I2C_ENABLE 1
#define I2C_DISABLE 0

#define I2C_READ 1
#define I2C_WRITE 0

#define I2C_RS_CMD 0
#define I2C_RS_DATA 1

#define I2C_DATA_8BIT_MODE 0x3c
#define I2C_DATA_4BIT_MODE 0x2c

#define LCD_BL_ON 1
#define LCD_BL_OFF 0

#define LCD_DISPLAY_OFF 0x08
#define LCD_DISPLAY_ON 0x0C
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_ENTRYMODE 0x06

#define LCD_CURSOR_SETTING 0x80



void assert_eq(int a, int b);

typedef struct {
  unsigned int register_selector: 1;
  unsigned int read_write: 1;
  unsigned int enable: 1;
  unsigned int back_light: 1;
  unsigned int bus4: 1;
  unsigned int bus5: 1;
  unsigned int bus6: 1;
  unsigned int bus7: 1;
} I2CDataUnitField;

typedef union {
  I2CDataUnitField field;
  u_int8_t raw;
} I2CDataUnit;


void show_data_unit(I2CDataUnit du) {
  printf("%d\t%d\t%d\t%s\t\t%d\t%d\t%d\t%d\n",
      du.field.register_selector,
      du.field.read_write,
      du.field.back_light,
      du.field.enable,
      du.field.bus7,
      du.field.bus6,
      du.field.bus5,
      du.field.bus4);
}
void show_simple_data_unit(I2CDataUnit du) {
  printf("%d\t%d\t%d\t%s\t\t%d\t%d\t%d\t%d\n",
      du.field.register_selector,
      du.field.read_write,
      du.field.back_light,
      "0->1->0",
      du.field.bus7,
      du.field.bus6,
      du.field.bus5,
      du.field.bus4);
}

typedef struct {
  const u_int32_t device_id; // Device ID
  unsigned int _is_mock;
  unsigned int _is_print;
  const u_int32_t fd; // Device file descriptor
  u_int8_t cursor_x;
  u_int8_t cursor_y;
  unsigned char state[16][2];
} LCDevice;


LCDevice get_i2c_device_with_id(u_int32_t id) {
  LCDevice ret = {.device_id = id, .fd = wiringPiI2CSetup(id) };
  ret._is_mock = 0;

  return ret;
}

int get_bit(u_int8_t orig, u_int8_t bit) {
  if((orig & (1 << bit)) == 0) {
    return 0;
  } else {
    return 1;
  }
}

void write_to_data_unit_upper(I2CDataUnit *data_unit, u_int8_t raw) {
  data_unit -> field .bus4 = get_bit(raw, 4);
  data_unit -> field .bus5 = get_bit(raw, 5);
  data_unit -> field .bus6 = get_bit(raw, 6);
  data_unit -> field .bus7 = get_bit(raw, 7);
}
void write_enable_data_unit(I2CDataUnit *data_unit, int enable) {
  data_unit -> field .enable = enable; 
}
void dump_lcd_device(LCDevice *device) {
  printf("[INFO] LCD Device at %d\n", device -> fd);
  printf("  ID = %d\n", device -> device_id);
  printf("  _is_mock = %d, _is_print = %d\n", device -> _is_mock, device -> _is_print);
  printf("  Cursor at (%d, %d)\n", device -> cursor_x, device -> cursor_y);

  for(int y = 0;y < 2;y++) {
    printf("  LINE %d ", y);
    for(int x = 0;x < 16;x++) {
      printf("%c", device -> state[x][y]);
    }
    puts("");
  }
}

void get_bit_test() {
  assert_eq(0, get_bit(0b1010, 0));
  assert_eq(1, get_bit(0b1010, 1));
  assert_eq(0, get_bit(0b1010, 2));
  assert_eq(1, get_bit(0b1010, 3));
}

void assert_eq(int a, int b) {
  if(a != b) {
    printf("Expected %d, got %d\n", a, b);
    exit(0);
  }
}
