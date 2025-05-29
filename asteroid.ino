#define ASTEROID_BACKGROUND TFT_BLACK

int shooter_len = 50; //length of shooter

int shooter_x1 = 320/2; //base x
int shooter_y1 = 0; //base y
int shooter_x2 = 320/2; //moving x
int shooter_y2 = shooter_y1 + shooter_len;

int shooter_x2_prev = shooter_x2;
int shooter_y2_prev = shooter_y2;

float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = 120, osy = 120; // Saved x & y coords

/* Update shooter based on joystick input
*/
void update_shooter(double joystick_x){
  double angle = acos(joystick_x);

  shooter_x2_prev = shooter_x2;
  shooter_y2_prev = shooter_y2;

  shooter_x2 = shooter_x1 + cos(angle)*shooter_len;
  shooter_y2 = shooter_y1 + sin(angle)*shooter_len;
}

void draw_shooter(){
  int buffer = 3;
  tft.drawCircle(shooter_x1-shooter_len-buffer, shooter_y1-buffer, 5, TFT_GREEN);
  tft.fillRect(shooter_x1-shooter_len-buffer, shooter_y1-buffer, (shooter_len+buffer)*2, shooter_len+buffer*2, TFT_BLUE);
  delay(10);
  //tft.drawCircle(shooter_x1, shooter_y1, shooter_len+5, TFT_RED);
  //tft.drawLine(shooter_x1, shooter_y1, shooter_x2, shooter_y2, TFT_WHITE);
}