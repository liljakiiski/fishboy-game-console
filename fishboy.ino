#include <TFT_HX8357.h>

// Color order is RGB 5+6+5 bits each
#define BG TFT_CYAN

// Big Fish variables
#define BIG_FISH_W 50
#define BIG_FISH_H 35
#define BIG_FISH_COLOR TFT_RED
int big_fish_x = 20;
int big_fish_y = 20;

// Baby Fish (mean guys) variables
#define BABY_FISH_H 30
#define BABY_FISH_W 50

// Baby Fish struct
struct BabyFish {
  int x;
  int y;
  unsigned int color;
  int speed;
};

TFT_HX8357 tft = TFT_HX8357();

BabyFish baby_fish[10];

void setup(){
  tft.init();
  setupJoystick();
  Serial.begin(9600);
  //paint_bg();
}

void loop(){
  //move_fish(false);
  printJoystick();
  delay(1000);
}