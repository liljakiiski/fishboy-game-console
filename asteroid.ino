//GENERAL------------------------------------>
#define ASTEROID_BG TFT_BLACK

long START_TIME;
long last_time;

int SCORE;

long last_time_interval_change;
long time_interval;

bool asteroid_game_over_ = false;
bool asteroid_game_started_ = false;

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
  set_asteroid_game_started(false);
  set_asteroid_game_over(false);
  SCORE = 0;
}

bool asteroid_game_started(){
  return asteroid_game_started_;
}

void set_asteroid_game_started(bool b){
  //Game is starting
  if(b){
    START_TIME = millis();
    last_time = START_TIME;
    time_interval = 1000;
    last_time_interval_change = START_TIME;
  }
  asteroid_game_started_ = b;
}

bool asteroid_game_over(){
  return asteroid_game_over_;
}

void set_asteroid_game_over(bool b){
  asteroid_game_over_ = b;  
}

/* Time since last asteroid shot
*/
long time_since_last_asteroid(){
  return millis() - last_time;
}

long get_time_interval_asteroid(){
  //Decrease time between asteroid throws every 8 seconds
  if(millis() - last_time_interval_change > 8000){ 
    last_time_interval_change = millis();
    time_interval = 0.7 * time_interval;
  }
  return time_interval;
}

/* Set the time when asteroid was shot
*/
void set_time_since_last_asteroid(long the_time){
  last_time = the_time;
}

int get_asteroid_score(){
  return SCORE;
}

/* Tells whether or not too circles collided
*/
bool collided(int x1, int y1, int r1, int x2, int y2, int r2){
  int dx = x2 - x1;
  int dy = y2 - y1;
  int distance = sqrt(pow(dx, 2) + pow(dy, 2));
  int radii_sum = r1 + r2;
  return distance <= radii_sum;
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

      asteroids[i].radius = random(8, 20);
      asteroids[i].speed = random(5, 10);
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

      //Check collisions - shooter
      if(collided(asteroids[i].x, asteroids[i].y, asteroids[i].radius, shooter_x1, shooter_y1, shooter_len)){
        asteroid_game_over_ = true;
        //Paint message before delay
        tft.setRotation(2);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE);
        tft.drawString("Game Over", tft.width() / 2, tft.height() / 2 - 50, 4);
        tft.drawString("Score:", tft.width()/2, tft.height()/2, 4);
        char cstr[16];
        itoa(get_asteroid_score(), cstr, 10);
        tft.drawString(cstr, tft.width()/2, tft.height()/2 + 50, 4);
        Serial.println(cstr);
        Serial.println(get_asteroid_score());
        tft.setRotation(0);
        delay(1000);
        break;
      }

      //Check collisions - bullets
      for(int x = 0; x < BULLET_CAPACITY; x++){
        //If bullet is active & the two collided
        if(bullets[x].active && collided(asteroids[i].x, asteroids[i].y, asteroids[i].radius, bullets[x].x, bullets[x].y, bullets[x].radius)){
          tft.drawCircle(asteroids[i].prev_x, asteroids[i].prev_y, asteroids[i].radius, ASTEROID_BG);
          tft.drawCircle(bullets[x].prev_x, bullets[x].prev_y, BULLET_RADIUS, ASTEROID_BG);
          asteroids[i].active = false;
          bullets[x].active = false;  
          SCORE += 1;
          break;
        }
      }
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