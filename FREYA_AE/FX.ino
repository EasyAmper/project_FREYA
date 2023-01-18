void rainbow_loop() {
  idex++;
  color = color + 20;       // цвет = цвет + шаг(можно подстроить под свою ленту)
  if (idex >= LED_COUNT)idex = 0;
  if (color > 255) color = 0;          // защита от перебора
  leds[idex] = CHSV(color, sat, 255);  // красим ленту
  LEDS.show();
  delay(del);
}
void cycle(byte mil) {                //  используем одну функцию для разных эффектов
  if (millis() - tim > mil) {       //  при большой паузе интерфейс останется жив, задаём паузу при вызове функции
    color++;                            // прибавляем цвет через указанную паузу
    tim = millis();
  }
  if (color > 255) color = 0;
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(color, sat, 255);
  }
  LEDS.show();
}
void color_flashes() {
  if (millis() - tim > 200) {          // !есть "баг"(время горения), но это даж круто
    r = random(LED_COUNT);              // делаем случайную позицию
    color = random(255);                    // и цвет (можно создать диапазон)
    leds[r] = CHSV(color, 255, 255);
    LEDS.show();
    tim = millis();
  }
  if (millis() - tim2 > 240) {
    LEDS.clear();         // стираем
    LEDS.show();
    tim2 = millis();
  }
}
void color_dot_fill() {
//  цветная точечная заливка
    if (millis() - tim > 200) {
      r = random(LED_COUNT);              // делаем случайную позицию
      color = random(255);                 // и цвет (можно создать диапазон)
      leds[r] = CHSV(color, 255, 255);    // красим рандомные пиксели
      // leds[r+1] = CHSV(color, 255, 255);  // два сразу(тест)
      LEDS.show();
      tim = millis();
    }
    if (millis() - tim2 > 7000) {      // через время все стираем
      LEDS.clear();
      LEDS.show();
      tim2 = millis();
    }
}
void fire_dots() {
// огненные точки(типо огонь)
  if (millis() - tim > 80) {          // поиграйтесь с задержкой
    r = random(LED_COUNT);              // делаем случайную позицию
    leds[r] = CRGB(253, 105, 6);    // оттенок 1
    LEDS.show();
     r = random(LED_COUNT);              // делаем случайную позицию
    leds[r] = CRGB(253, 47, 6);    // оттенок 2(можно больше)
    LEDS.show();
    tim = millis();
  }
  if (millis() - tim2 > 50) {      // интервал удаления(тоже можно по вкусу)
     r = random(LED_COUNT);              // делаем случайную позицию
    leds[r] = CRGB(0, 0, 0);    // стираем рандомные пиксели
    LEDS.show();
    tim2 = millis();
  }
}
  void carosel(){
 if(color>=250)color=0;                  // защита от "перебора" 
     for (int i = 0 ; i < LED_COUNT; i++ ) {     
    leds[i] = CHSV(color, 255, 255);
    LEDS.show();
    delay(20);
     }
    color=color+30;                 // отвечает за смену цвета
  }
  
