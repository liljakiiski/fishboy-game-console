#include <TFT_HX8357.h>

TFT_HX8357 tft = TFT_HX8357();

int SCREEN_WIDTH = 480;
int SCREEN_HEIGHT = 320;

#define BASE_BG 0x000000

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
  
  //setup_cursor(&cursor1, 5, 5, 10, 0xFFFFFF);

  setup_asteroid_game();

  paint_base_background();

  Serial.begin(9600);
}

void loop(){
  //move_cursor(&cursor1, joy_to_screen_x()*max_cursor_speed, joy_to_screen_y()*max_cursor_speed);
  //paint_cursor(&cursor1);

  //Serial.println(get_joy_x());
  update_shooter(get_joy_x());
  draw_shooter();

  //button is pressed
  if(!get_button_state()){
    shoot_bullet(get_joy_x());
    delay(100);
  }

  handle_bullets();
  draw_bullets();

  //Serial.println(get_bullets_count());
  //Serial.println(get_button_state());
  delay(50);
}

void paint_base_background(){
  tft.fillScreen(BASE_BG);
}