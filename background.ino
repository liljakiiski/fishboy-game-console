void paint_bg(){
  tft.fillScreen(BG);
}

void draw_fish(){
  tft.drawPixel(50, 50, BAIT);
  tft.drawRect(50, 50, 100, 100, FISH);
}