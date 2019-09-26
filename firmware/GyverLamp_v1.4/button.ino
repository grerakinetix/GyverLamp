#define CLICK_BRIGHTNESS_STEP (4U)                          // шаг изменения яркости по нажатию на кнопку (для конфигурации от 3 кнопок)
#define HOLD_BRIGHTNESS_STEP  (2U)                          // шаг изменения яркости при зажатии кнопки
#define HOLD_SCALE_STEP       (3U)                          // шаг изменения масштаба при зажатии кнопки

bool brightDirection;

void buttonTick()
{
  touch.tick();
  if (touch.isSingle())
  {
    if (dawnFlag)
    {
      manualOff = true;
      dawnFlag = false;
      FastLED.setBrightness(getCurrentBrightness());
      changePower();
    }
    else
    {
      ONflag = !ONflag;
      changePower();
    }
    loadingFlag = true;
  }

  if (ONflag)
  {
    #if (BUTTONS_COUNT < 5)
      if (touch.isDouble())
      {
        currentMode = (currentMode + 1) % MODE_AMOUNT;
        FastLED.setBrightness(getCurrentBrightness());
        loadingFlag = true;
        settChanged = true;
        eepromTimeout = millis();
        #ifdef GENERAL_DEBUG
          Serial.printf_P(PSTR("Mode changed: %d (Brightness: %d, Speed: %d, Scale: %d)\n"), currentMode, modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale);
        #endif
        FastLED.clear();
        delay(1);
      }
      
      if (touch.isTriple())
      {
        currentMode = (currentMode - 1 + MODE_AMOUNT) % MODE_AMOUNT;
        FastLED.setBrightness(getCurrentBrightness());
        loadingFlag = true;
        settChanged = true;
        eepromTimeout = millis();
        #ifdef GENERAL_DEBUG
          Serial.printf_P(PSTR("Mode changed: %d (Brightness: %d, Speed: %d, Scale: %d)\n"), currentMode, modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale);
        #endif
        FastLED.clear();
        delay(1);
      }
    #endif
    
    #if (BUTTONS_COUNT < 3)
      if (touch.isHolded())
      {
        brightDirection = !brightDirection;
      }

      if (touch.isStep())
      {
        if (brightDirection && modes[currentMode].Brightness < 101 - HOLD_BRIGHTNESS_STEP)
        {
          modes[currentMode].Brightness += HOLD_BRIGHTNESS_STEP;
          setCurrentBrightness();
        }
        else if (!brightDirection && modes[currentMode].Brightness > HOLD_BRIGHTNESS_STEP)
        {
          modes[currentMode].Brightness -= HOLD_BRIGHTNESS_STEP;
          setCurrentBrightness();
        }
      }
    #endif

    #ifdef OTA
      if (touch.hasClicks() && touch.getClicks() >= 4)
      {
        if (otaManager.RequestOtaUpdate())
        {
          currentMode = EFF_MATRIX;                            // принудительное включение режима "Матрица" для индикации перехода в режим обновления по воздуху
          FastLED.clear();
          delay(1);
        }
      }
    #endif
  }
}

#if (BUTTONS_COUNT >= 3)
void verticalButtonsTick()
{
  touchUp.tick();
  touchDown.tick();

  if (touchUp.isClick() && modes[currentMode].Brightness < 101 - CLICK_BRIGHTNESS_STEP)
  {
    modes[currentMode].Brightness += CLICK_BRIGHTNESS_STEP;
    setCurrentBrightness();
  }
  
  if (touchUp.isStep() && modes[currentMode].Brightness < 101 - HOLD_BRIGHTNESS_STEP)
  {
    modes[currentMode].Brightness += HOLD_BRIGHTNESS_STEP;
    setCurrentBrightness();
  }
  
  if (touchDown.isClick() && modes[currentMode].Brightness > CLICK_BRIGHTNESS_STEP)
  {
    modes[currentMode].Brightness -= CLICK_BRIGHTNESS_STEP;
    setCurrentBrightness();
  }
  
  if (touchDown.isStep() && modes[currentMode].Brightness > HOLD_BRIGHTNESS_STEP)
  {
    modes[currentMode].Brightness -= HOLD_BRIGHTNESS_STEP;
    setCurrentBrightness();
  }
}
#endif

#if (BUTTONS_COUNT >= 5)
void horizontalButtonsTick()
{
  touchLeft.tick();
  touchRight.tick();

  if (touchRight.isClick())
  {
    currentMode = (currentMode + 1) % MODE_AMOUNT;
    FastLED.setBrightness(getCurrentBrightness());
    loadingFlag = true;
    settChanged = true;
    eepromTimeout = millis();
    #ifdef GENERAL_DEBUG
      Serial.printf_P(PSTR("Mode changed: %d (Brightness: %d, Speed: %d, Scale: %d)\n"), currentMode, modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale);
    #endif
    FastLED.clear();
    delay(1);
  }

  if (touchLeft.isClick())
  {
    currentMode = (currentMode - 1 + MODE_AMOUNT) % MODE_AMOUNT;
    FastLED.setBrightness(getCurrentBrightness());
    loadingFlag = true;
    settChanged = true;
    eepromTimeout = millis();
    #ifdef GENERAL_DEBUG
      Serial.printf_P(PSTR("Mode changed: %d (Brightness: %d, Speed: %d, Scale: %d)\n"), currentMode, modes[currentMode].Brightness, modes[currentMode].Speed, modes[currentMode].Scale);
    #endif
    FastLED.clear();
    delay(1);
  }

  if (touchRight.isStep())
  {
    if (isCyclic(currentMode))
    {
      modes[currentMode].Scale = (modes[currentMode].Scale + 99 + HOLD_SCALE_STEP) % 100 + 1;
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      #ifdef GENERAL_DEBUG
        Serial.printf_P(PSTR("New scale value: %d\n"), modes[currentMode].Scale);
      #endif
    }
    else if (modes[currentMode].Scale < 101 - HOLD_SCALE_STEP)
    {
      modes[currentMode].Scale += HOLD_SCALE_STEP;
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      #ifdef GENERAL_DEBUG
        Serial.printf_P(PSTR("New scale value: %d\n"), modes[currentMode].Scale);
      #endif
    }
  }
  
  if (touchLeft.isStep())
  {
    if (isCyclic(currentMode))
    {
      modes[currentMode].Scale = (modes[currentMode].Scale + 99 - HOLD_SCALE_STEP) % 100 + 1;
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      #ifdef GENERAL_DEBUG
        Serial.printf_P(PSTR("New scale value: %d\n"), modes[currentMode].Scale);
      #endif
    }
    else if (modes[currentMode].Scale > HOLD_SCALE_STEP)
    {
      modes[currentMode].Scale -= HOLD_SCALE_STEP;
      loadingFlag = true;
      settChanged = true;
      eepromTimeout = millis();
      #ifdef GENERAL_DEBUG
        Serial.printf_P(PSTR("New scale value: %d\n"), modes[currentMode].Scale);
      #endif
    }
  }
}
#endif
