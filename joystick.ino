// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output

const int MIN_X = 0;
const int MAX_X = 895;
const int MIN_Y = 0;
const int MAX_Y = 895;

void setup_joystick() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
}

void print_joystick() {
  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(get_joy_x());
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(get_joy_y());
  Serial.print("\n\n");
}

/* [SCREEN RELATIVE] direction of joystick x on -1 to 1 scale*/
double joy_to_screen_x(){
  return -get_joy_x();
}

/* [SCREEN RELATIVE] direction of joystick y on -1 to 1 scale*/
double joy_to_screen_y(){
  return get_joy_y();
}

/* [JOYSTICK RELATIVE] direction of joystick x on -1 to 1 scale*/
double get_joy_x(){
  double center_x = (MAX_X + MIN_X)/2;
  double offset_x = ((double)analogRead(X_pin)) - center_x;
  return offset_x/center_x;
}

/* [JOYSTICK RELATIVE] direction of joystick y on -1 to 1 scale*/
double get_joy_y(){
  double center_y = (MAX_Y + MIN_Y)/2;
  double offset_y = ((double)analogRead(Y_pin)) - center_y;
  return offset_y/center_y;
}