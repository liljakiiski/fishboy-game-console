#include <TFT_HX8357.h>

TFT_HX8357 tft = TFT_HX8357();

int SCREEN_WIDTH = 480;
int SCREEN_HEIGHT = 320;

// Cursor
struct Cursor {
  int x; //center x
  int y; //center y
  int size;
  unsigned int color;
};

struct Cursor cursor1;

void setup(){
  tft.init();

  setup_joystick();
  
  setup_cursor(&cursor1, 20, 500, 20, 0xFFFFFF);

  paint_background();

  Serial.begin(9600);
}

void loop(){s
  print_joystick();
  delay(1000);
  paint_cursor(&cursor1);
}

void paint_background(){
  tft.fillScreen(0x000000);
}