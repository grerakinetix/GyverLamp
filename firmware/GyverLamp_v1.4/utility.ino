// служебные функции

// залить все
void fillAll(CRGB color)
{
  for (int32_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

// функция отрисовки точки по координатам X Y
void drawPixelXY(int8_t x, int8_t y, CRGB color)
{
  if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1) return;
  int32_t thisPixel = getPixelNumber(x, y) * SEGMENTS;
  for (uint8_t i = 0; i < SEGMENTS; i++)
  {
    leds[thisPixel + i] = color;
  }
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(uint32_t thisSegm)
{
  uint32_t thisPixel = thisSegm * SEGMENTS;
  if (thisPixel < 0 || thisPixel > NUM_LEDS - 1) return 0;
  return (((uint32_t)leds[thisPixel].r << 16) | ((uint32_t)leds[thisPixel].g << 8 ) | (uint32_t)leds[thisPixel].b);
}

// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(int8_t x, int8_t y)
{
  return getPixColor(getPixelNumber(x, y));
}

// ************* НАСТРОЙКА МАТРИЦЫ *****
#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y x

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y x

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y (WIDTH - x - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y y

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y (WIDTH - x - 1)

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif

// получить номер пикселя в ленте по координатам
uint16_t getPixelNumber(int8_t x, int8_t y)
{
  if ((THIS_Y % 2 == 0) || MATRIX_TYPE)                     // если чётная строка
  {
    return (THIS_Y * _WIDTH + THIS_X);
  }
  else                                                      // если нечётная строка
  {
    return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
  }
}

uint8_t brightnessLevels[100] = 
{ 
  1  , 1  , 1  , 1  , 1  , 1  , 1  , 2  , 2  , 2  ,
  2  , 2  , 2  , 2  , 2  , 2  , 3  , 3  , 3  , 3  ,
  3  , 3  , 4  , 4  , 4  , 4  , 4  , 5  , 5  , 5  ,
  6  , 6  , 6  , 7  , 7  , 7  , 8  , 8  , 9  , 9  ,
  10 , 10 , 11 , 11 , 12 , 13 , 14 , 14 , 15 , 16 ,
  17 , 18 , 19 , 20 , 21 , 22 , 24 , 25 , 26 , 28 ,
  29 , 31 , 33 , 35 , 37 , 39 , 41 , 43 , 46 , 49 ,
  51 , 54 , 57 , 61 , 64 , 68 , 72 , 76 , 80 , 84 ,
  89 , 94 , 100, 105, 111, 118, 124, 132, 139, 147,
  155, 164, 174, 184, 194, 205, 217, 229, 242, 255
};

uint8_t getCurrentBrightness() 
{
  return brightnessLevels[constrain(modes[currentMode].Brightness, 1, 100) - 1];
}

void setCurrentBrightness()
{
  FastLED.setBrightness(getCurrentBrightness());
  settChanged = true;
  eepromTimeout = millis();

  #ifdef GENERAL_DEBUG
    Serial.printf_P(PSTR("New brightness value: %d (%d)\n"), modes[currentMode].Brightness, getCurrentBrightness());
  #endif
}

boolean isCyclic(int8_t mode) {
  switch (mode)
  {
  case EFF_FIRE:
  case EFF_COLOR:
  case EFF_LIGHTER_TRACES:
  case EFF_PAINTBALL:
    return true;
  
  default:
    return false;
  }
}