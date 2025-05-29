//GENERAL------------------------------------>
#define ASTEROID_BG TFT_BLACK

struct Moving_Object {
  double angle;
  uint16_t color;
  double prev_x;
  double prev_y;
  double x;
  double y;
  double radius;
  double speed;
  bool active;
};

//SHOOTER SETUP------------------------------>
int shooter_len = 50; //length of shooter
int shooter_x1 = tft.width()/2; //base x
int shooter_y1 = 0; //base y
int shooter_x2 = shooter_x1; //moving x
int shooter_y2 = shooter_y1 + shooter_len;
int shooter_x2_prev = shooter_x2;
int shooter_y2_prev = shooter_y2;

//ASTEROID SETUP---------------------------->
const int ASTEROID_CAPACITY = 10;
Moving_Object asteroids[ASTEROID_CAPACITY];

//BULLET SETUP------------------------------>
const int BULLET_CAPACITY = 10;
const int BULLET_SPEED = 5;
const int BULLET_RADIUS = 3;
Moving_Object bullets[BULLET_CAPACITY];

double joystick_to_angle(double joystick_x){
  return acos(joystick_x);
}

double setup_asteroid_game(){
  setup_bullets();   
  setup_asteroids(); 
}

//HANDLING ASTEROIDS------------------------>
void setup_asteroids(){
  for (int i = 0; i < ASTEROID_CAPACITY; i++) {
    asteroids[i].angle = 0;
    asteroids[i].color = TFT_WHITE;
    asteroids[i].prev_x = 0;
    asteroids[i].prev_y = 0;
    asteroids[i].x = 0;
    asteroids[i].y = 0;
    asteroids[i].radius = 0;
    asteroids[i].speed = 0;
    asteroids[i].active = false;
  }
}

/* "Shoot" aka create a new asteroid (if space)
*/
void shoot_asteroid(){
  for(int i = 0; i < ASTEROID_CAPACITY; i++){
    if(!asteroids[i].active){
      asteroids[i].color = TFT_PINK;
      asteroids[i].x = random(0, tft.width());
      asteroids[i].y = tft.height();
      asteroids[i].prev_x = asteroids[i].x;
      asteroids[i].prev_y = asteroids[i].y;
     
      // Calculate proper angle (radians) toward shooter tip
      double dx = shooter_x2 - asteroids[i].x;
      double dy = shooter_y2 - asteroids[i].y;
      asteroids[i].angle = atan2(dy, dx);

      asteroids[i].radius = random(3, 10);
      asteroids[i].speed = random(2, 4);
      asteroids[i].active = true;
      break;
    }
  }
}


/* Handle asteroids every loop iteration
*/
void handle_asteroids(){
  for(int i = 0; i < ASTEROID_CAPACITY; i++){
    if(asteroids[i].active){
      asteroids[i].prev_x = asteroids[i].x;
      asteroids[i].prev_y = asteroids[i].y;

      // Move along angle using cos/sin (just like bullets)
      asteroids[i].x += cos(asteroids[i].angle) * asteroids[i].speed;
      asteroids[i].y += sin(asteroids[i].angle) * asteroids[i].speed;

      //asteroids[i].x = new_x;
      //asteroids[i].y = new_y;
    }
  }
}

void draw_asteroids(){
  for(int i = 0; i < ASTEROID_CAPACITY; i++){
    if(asteroids[i].active){
      tft.drawCircle(asteroids[i].prev_x, asteroids[i].prev_y, asteroids[i].radius, ASTEROID_BG);
      tft.drawCircle(asteroids[i].x, asteroids[i].y, asteroids[i].radius, asteroids[i].color);
    }
  }
}

//HANDLING BULLETS-------------------------->
void setup_bullets(){
  for (int i = 0; i < BULLET_CAPACITY; i++) {
    bullets[i].angle = 0;
    bullets[i].color = TFT_WHITE;
    bullets[i].prev_x = 0;
    bullets[i].prev_y = 0;
    bullets[i].x = 0;
    bullets[i].y = 0;
    bullets[i].radius = BULLET_RADIUS;
    bullets[i].speed = 0;
    bullets[i].active = false;
  }
}

int get_bullets_count(){
  int count = 0;
  for(int i = 0; i < BULLET_CAPACITY; i++){
    if(bullets[i].active){
      count++;
    }
  }
  return count;
}

/* "Shoot" aka create a new bullet (if space)
*/
void shoot_bullet(double joystick_x){
  for(int i = 0; i < BULLET_CAPACITY; i++){
    if(!bullets[i].active){
      double angle = joystick_to_angle(joystick_x);
      bullets[i].angle = joystick_to_angle(joystick_x);
      bullets[i].prev_x = shooter_x2;
      bullets[i].prev_y = shooter_y2;
      bullets[i].x = shooter_x2;
      bullets[i].y = shooter_y2;
      bullets[i].radius = BULLET_RADIUS;
      bullets[i].speed = BULLET_SPEED;
      bullets[i].active = true;
      break;
    }
  }
}

/*
 Checks if a bullets possible new location is in screen 
 Index i of bullets list
*/
bool bullet_location_in_screen(int new_x, int new_y){
  return !(new_x - BULLET_RADIUS < 0 || new_x + BULLET_RADIUS > tft.width() ||
      new_y - BULLET_RADIUS < 0 || new_y + BULLET_RADIUS > tft.height());
}

/* Handle bullets every loop iteration
*/
void handle_bullets(){
  for(int i = 0; i < BULLET_CAPACITY; i++){
    if(bullets[i].active){
      bullets[i].prev_x = bullets[i].x;
      bullets[i].prev_y = bullets[i].y;

      int new_x = bullets[i].x + cos(bullets[i].angle)*bullets[i].speed;
      int new_y = bullets[i].y + sin(bullets[i].angle)*bullets[i].speed;

      //Bullet is in-screen
      if (bullet_location_in_screen(new_x, new_y)){
        bullets[i].x = new_x;
        bullets[i].y = new_y;
      
      //Bullet is NOT in-screen
      } else {
        bullets[i].active = false;
        tft.drawCircle(bullets[i].prev_x, bullets[i].prev_y, BULLET_RADIUS, ASTEROID_BG);
      }
    }
  }
}

void draw_bullets(){
  for(int i = 0; i < BULLET_CAPACITY; i++){
    if(bullets[i].active){
      tft.drawCircle(bullets[i].prev_x, bullets[i].prev_y, BULLET_RADIUS, ASTEROID_BG);
      tft.drawCircle(bullets[i].x, bullets[i].y, BULLET_RADIUS, bullets[i].color);
    }
  }
}

//HANDLING SHOOTER---------------------------->
void draw_shooter(){
  int buffer = 3;
  if(shooter_x2 != shooter_x2_prev || shooter_y2 != shooter_y2_prev){
    tft.drawLine(shooter_x1, shooter_y1, shooter_x2_prev, shooter_y2_prev, ASTEROID_BG);
  }
  tft.drawCircle(shooter_x1, shooter_y1, shooter_len+5, TFT_RED);
  tft.drawLine(shooter_x1, shooter_y1, shooter_x2, shooter_y2, TFT_YELLOW);
}


/* Update shooter based on joystick input
*/
void update_shooter(double joystick_x){
  double angle = joystick_to_angle(joystick_x);

  shooter_x2_prev = shooter_x2;
  shooter_y2_prev = shooter_y2;

  shooter_x2 = shooter_x1 + cos(angle)*shooter_len;
  shooter_y2 = shooter_y1 + sin(angle)*shooter_len;
}