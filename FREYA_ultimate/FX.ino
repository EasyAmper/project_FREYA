void rainbow_loop() {
  if (millis() - tim > del) {
    idex++;
    color = color + 20;       // цвет = цвет + шаг(можно подстроить под свою ленту)
    if (idex >= LED_COUNT)idex = 0;
    if (color > 255) color = 0;          // защита от перебора
    leds[idex] = CHSV(color, 255, 255);  // красим ленту
    LEDS.show();
    tim = millis();
  }
}
void cycle() {                
  if (millis() - tim > del) {       //  при большой паузе интерфейс останется жив, задаём паузу при вызове функции
    color++;                            // прибавляем цвет через указанную паузу
    tim = millis();
  }
  if (color > 255) color = 0;
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(color, 255, 255);
  }
  LEDS.show();
}
void color_flashes() {
  if (millis() - tim > del) {          // !есть "баг"(время горения), но это даж круто
    r = random(LED_COUNT);              // делаем случайную позицию
    color = random(255);                    // и цвет (можно создать диапазон)
    leds[r] = CHSV(color, 255, 255);
    LEDS.show();
    tim = millis();
  }
  if (millis() - tim2 > (del + 50)) {
    LEDS.clear();         // стираем
    LEDS.show();
    tim2 = millis();
  }
}
void color_dot_fill() {
  //  цветная точечная заливка
  if (millis() - tim > del) {
    r = random(LED_COUNT);              // делаем случайную позицию
    color = random(255);                 // и цвет (можно создать диапазон)
    leds[r] = CHSV(color, 255, 255);    // красим рандомные пиксели
    // leds[r+1] = CHSV(color, 255, 255);  // два сразу(тест)
    LEDS.show();
    tim = millis();
  }
  if (millis() - tim2 > (del + 6800)) {    // через время все стираем
    LEDS.clear();
    LEDS.show();
    tim2 = millis();
  }
}
void fire_dots() {
  // огненные точки(типо огонь)
  if (millis() - tim > (del + 30)) {
    r = random(LED_COUNT);              // делаем случайную позицию
    leds[r] = CRGB(253, 105, 6);    // оттенок 1
    LEDS.show();
    r = random(LED_COUNT);              // делаем случайную позицию
    leds[r] = CRGB(253, 47, 6);    // оттенок 2(можно больше)
    LEDS.show();
    tim = millis();
  }
  if (millis() - tim2 > del) {      // интервал удаления
    r = random(LED_COUNT);              // делаем случайную позицию
    leds[r] = CRGB(0, 0, 0);    // стираем рандомные пиксели
    LEDS.show();
    tim2 = millis();
  }
}
void carosel() {
  if (color >= 250)color = 0;             // защита от "перебора"
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i] = CHSV(color, 255, 255);
    LEDS.show();
    delay(20);
  }
  color = color + 30;             // отвечает за смену цвета
}

void fireVSwater() {      // огонь против воды версия 2     половина-огонь, другая-вода
  if (millis() - tim > (del + 120)) {     // вода более медленная
    // позицию определяем как половину ленты +- смешивание стихий(по вкусу, зависит от длины ленты)
    r = random(((LED_COUNT / 2) - 2), LED_COUNT);
    //(половина ленты минус смешивание 2 светодиода до конца ленты)
    leds[r] = CRGB(7, 123, 255);  // водичка
    LEDS.show();     //спавним(ну отправляем кароче)
    r = random(((LED_COUNT / 2) - 2), LED_COUNT);
    leds[r] = CRGB(26, 0, 255);   //иной оттенок
    LEDS.show();
    r = random(((LED_COUNT / 2) - 2), LED_COUNT);
    leds[r] = CRGB(0, 0, 255);  // тупо синий
    LEDS.show();
    r = random(((LED_COUNT / 2) - 2), LED_COUNT);
    leds[r] = CRGB(0, 0, 0);  // для подвижности воды(убрать-будет плавнее, но менее заметно)
    LEDS.show();
    tim = millis();
  }
  if (millis() - tim2 > del) {   // огонь сделаем побыстрее воды
    r = random(0, ((LED_COUNT / 2) + 2));
    // то же самое токо от начала до середины + смешивание
    leds[r] = CRGB(253, 105, 6);  // огонёк
    LEDS.show();
    r = random(0, ((LED_COUNT / 2) + 2));
    leds[r] = CRGB(0, 0, 0);    // то же самое токо для огня
    LEDS.show();
    r = random(0, ((LED_COUNT / 2) + 2));
    leds[r] = CRGB(253, 47, 6);
    LEDS.show();
    r = random(0, ((LED_COUNT / 2) + 2));
    leds[r] = CRGB(253, 0, 0);    // тупо красный
    LEDS.show();
    tim2 = millis();
  }
}
void siesta() {
  // siesta (оптимально del 5-10)
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[r] = CHSV(color, 255, 255);
    LEDS.show();
    r = random(LED_COUNT);
    color = random(255);
    delay(10);
  }
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[r] = CHSV(0, 255, 0);
    LEDS.show();
    r = random(LED_COUNT);
    delay(10);
  }
}
