#include <TFT_HX8357.h>

TFT_HX8357 tft = TFT_HX8357();

int SCREEN_WIDTH = 480;
int SCREEN_HEIGHT = 320;

#define background_color 0x000000

// Cursor
struct Cursor {
  int x; //center x
  int y; //center y
  int size;
  unsigned int color;
};

struct Cursor cursor1;
double max_cursor_speed = 20;

void setup(){
  tft.init();

  setup_joystick();
  
  setup_cursor(&cursor1, 5, 5, 10, 0xFFFFFF);

  print_joystick();
  paint_background();

  Serial.begin(9600);
}

void loop(){
  move_cursor(&cursor1, joy_to_screen_x()*max_cursor_speed, joy_to_screen_y()*max_cursor_speed);
  Serial.println("");
  Serial.println(joy_to_screen_x()*max_cursor_speed);
  Serial.println(joy_to_screen_y()*max_cursor_speed);
  Serial.println(cursor1.x);
  Serial.println(cursor1.y);
  Serial.println("");
  paint_cursor(&cursor1);
  delay(50);
}

void paint_background(){
  tft.fillScreen(background_color);
}