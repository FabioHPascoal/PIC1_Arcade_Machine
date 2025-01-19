#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 256

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const byte led_map[32][8] = {
  {  7,  6,  5,  4,  3,  2,  1,  0},
  {  8,  9, 10, 11, 12, 13, 14, 15},
  { 23, 22, 21, 20, 19, 18, 17, 16},
  { 24, 25, 26, 27, 28, 29, 30, 31},
  { 39, 38, 37, 36, 35, 34, 33, 32},
  { 40, 41, 42, 43, 44, 45, 46, 47},
  { 55, 54, 53, 52, 51, 50, 49, 48},
  { 56, 57, 58, 59, 60, 61, 62, 63},
  { 71, 70, 69, 68, 67, 66, 65, 64},
  { 72, 73, 74, 75, 76, 77, 78, 79},
  { 87, 86, 85, 84, 83, 82, 81, 80},
  { 88, 89, 90, 91, 92, 93, 94, 95},
  {103,102,101,100, 99, 98, 97, 96},
  {104,105,106,107,108,109,110,111},
  {119,118,117,116,115,114,113,112},
  {120,121,122,123,124,125,126,127},
  {135,134,133,132,131,130,129,128},
  {136,137,138,139,140,141,142,143},
  {151,150,149,148,147,146,145,144},
  {152,153,154,155,156,157,158,159},
  {167,166,165,164,163,162,161,160},
  {168,169,170,171,172,173,174,175},
  {183,182,181,180,179,178,177,176},
  {184,185,186,187,188,189,190,191},
  {199,198,197,196,195,194,193,192},
  {200,201,202,203,204,205,206,207},
  {215,214,213,212,211,210,209,208},
  {216,217,218,219,220,221,222,223},
  {231,230,229,228,227,226,225,224},
  {232,233,234,235,236,237,238,239},
  {247,246,245,244,243,242,241,240},
  {248,249,250,251,252,253,254,255}
};

// Definição de cores nomeadas em formato RGB
const uint32_t red    = pixels.Color(255, 0, 0);
const uint32_t green  = pixels.Color(0, 255, 0);
const uint32_t blue   = pixels.Color(0, 0, 255);
const uint32_t white  = pixels.Color(255, 255, 255);
const uint32_t black  = pixels.Color(0, 0, 0);
const uint32_t yellow = pixels.Color(255, 255, 0);
const uint32_t cyan   = pixels.Color(0, 255, 255);
const uint32_t magenta = pixels.Color(255, 0, 255);

const byte stick_left = 13;
const byte stick_right = 12;
const byte stick_up = 10;  
const byte stick_down = 11;

byte left_state = 0;
byte right_state = 0;
byte up_state = 0;
byte down_state = 0;

byte pos_x = 4;
byte pos_y = 15;

void move() {
  left_state = !digitalRead(stick_left);
  right_state = !digitalRead(stick_right);
  up_state = !digitalRead(stick_up);
  down_state = !digitalRead(stick_down);
  
  pixels.setPixelColor(led_map[pos_y][pos_x], black);
  
  pos_x = pos_x - left_state + right_state;
  pos_y = pos_y - up_state + down_state;

  pixels.setPixelColor(led_map[pos_y][pos_x], red);
  pixels.show();

  delay(100);
}

void setup() {
  pinMode(stick_left, INPUT_PULLUP);
  pinMode(stick_right, INPUT_PULLUP);
  pinMode(stick_up, INPUT_PULLUP);
  pinMode(stick_down, INPUT_PULLUP);

  pixels.begin();
  
  pixels.setPixelColor(led_map[pos_y][pos_x], red);
  pixels.show();
}

void loop() {
  move();
}
