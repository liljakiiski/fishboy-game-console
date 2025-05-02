void move_fish(bool up){
  draw_fish(BG);

  if (up && big_fish_y > 0){
    big_fish_y--;

  } else if (!up && (big_fish_y + BIG_FISH_H) < tft.width()){
    big_fish_y++;
  }

  draw_fish(BIG_FISH_COLOR);
}

void move_baby_fish(){

}
