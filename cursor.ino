/*
(x, y) indicates top left corner of cursor
*/

// Function to set up the cursor
void setup_cursor(struct Cursor* cursor, int x, int y, int size, unsigned int color) {
    cursor->x = x;
    cursor->y = y;
    cursor->size = size;
    cursor->color = color;
}

void paint_cursor(struct Cursor* c){
  tft.drawRect(c->x, c->y, c->size, c->size, c->color);
}

/* Moves cursor desired shift (if inside screen bounds) */
void move_cursor(struct Cursor* c, int shift_x, int shift_y){
  //Only moves cursor if inside screen bounds
  if(cursor_in_bounds(c, shift_x, shift_y)){
    int x = c->x;
    int y = c->y;

    c->x = x + shift_x;
    c->y = y + shift_y;
  }
}

/* Checks if cursors */
bool cursor_in_bounds(struct Cursor* c, int shift_x, int shift_y){
  return (c->x + shift_x < SCREEN_WIDTH && c->x > 0 
      && c->y + shift_y < SCREEN_HEIGHT && c->y > 0);
}