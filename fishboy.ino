#include <TFT_HX8357.h>

//Constants
//unsigned int colour = red << 11; // Colour order is RGB 5+6+5 bits each
#define BG TFT_CYAN
#define FISH TFT_BLUE
#define BAIT TFT_RED

struct Being {
  int width;
  int height;
  int x;
  int y;
  unsigned int color;
};

TFT_HX8357 tft = TFT_HX8357();

Being fish;

void setup(){
  fish = {100, 100, 20, 20, TFT_BLUE};
  tft.init();
  Serial.begin(9600);
  Serial.println(fish.width);
  paint_bg();
  draw_fish();
}

void loop(){

}