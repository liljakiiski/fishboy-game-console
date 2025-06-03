#include <TFT_HX8357.h>

TFT_HX8357 tft = TFT_HX8357();
int SCREEN_WIDTH = 480;
int SCREEN_HEIGHT = 320;
#define BASE_BG TFT_BLACK

// Cursor
struct Cursor {
  int x; //center x
  int y; //center y
  int size;
  unsigned int color;
};

struct Cursor cursor1;
double max_cursor_speed = 20;

/*Screen setting (what is happening?);
0 = home screen;
1 = asteroid game
2 = snake
3 = ???
*/
int GAME_MODE = 1;

void setup(){
  tft.init();
  tft.begin();

  setup_joystick();
  setup_cursor(&cursor1, 5, 5, 10, 0xFFFFFF);
  setup_asteroid_game();
  paint_base_background();
  Serial.begin(9600);
}

void loop(){

  switch(GAME_MODE){
    //HOME SCREEN
    case 0:
    move_cursor(&cursor1, joy_to_screen_x()*max_cursor_speed, joy_to_screen_y()*max_cursor_speed);
    paint_cursor(&cursor1);      
      break;

    //ASTEROID GAME
    case 1:
      //Game not started
      if(!asteroid_game_started()){
        tft.setRotation(2);
        tft.setTextSize(1);
        tft.setTextColor(TFT_WHITE);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Honeycomb v1.0", tft.width() / 2, tft.height() / 2 - 25, 4);
        tft.drawString("Press Button to Start", tft.width() / 2, tft.height() / 2 + 25, 4);
        tft.setRotation(0);

      //Game is starting
      if(!get_button_state()){
        set_asteroid_game_started(true);
        shoot_asteroid();

        paint_base_background();
        tft.setRotation(0);
      }

      //Game ongoing
      } else if (asteroid_game_started() && !asteroid_game_over()){
        if(time_since_last_asteroid() > get_time_interval_asteroid()){
          shoot_asteroid();
          set_time_since_last_asteroid(millis());
        }
        update_shooter(joy_to_screen_x());
        draw_shooter();

        if(!get_button_state()){
          shoot_bullet(joy_to_screen_x());
          delay(100);
        }

        handle_bullets();
        handle_asteroids();
        draw_bullets();
        draw_asteroids();

      //Game over
      } else if (asteroid_game_over()){
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

        //Player wants to continue
        if(!get_button_state()){
          setup_asteroid_game();

          paint_base_background();
          tft.setRotation(0);
        }
      }

      break;

    //SNAKE
    case 2:

      break;
    case 3:

      break;
    default:
      break;      
  }
  delay(50);
}

void paint_base_background(){
  tft.fillScreen(BASE_BG);
}