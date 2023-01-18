// by EASYAMPER
// эффекты для адресной ленты
// пользуйтесь,  меняйте параметры, пробуйте!
// переменные могут повторяться(использовал для отладки других скетчей)
// в ваш скетч советую заносить только необходимые для используемых эффектов

#include "FastLED.h"          // библиотека для работы с лентой
#define LED_COUNT 30          // число светодиодов в кольце/ленте
#define LED_DT 10             // пин, куда подключен DIN ленты
#define del 100               // скорость прохода / плавность заполнения
// меняя скорость можно получить совсем другой эффект
#define k 20                  // коэфицент смены цвета
struct CRGB leds[LED_COUNT];
byte c = 0;
byte color; // взаимозаменяем на byte c, просто мне потребовалось для отладки другой программы
byte r;
byte s;
byte v;
byte n;
int i;
unsigned long tim;            // для таймера
unsigned long tim2;            // для таймера
int max_bright = 200;         // максимальная яркость (0 - 255)
void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}
void setup() {
  // put your setup code here, to run once:
  LEDS.setBrightness(max_bright);  // ограничить максимальную яркость

  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);          // погасить все светодиоды(очистить)
  // можно использовать LEDS.clear для очистки
  LEDS.show();                     // отосл
}

void loop() {
  // нужный режим раскомментировать, остальные закомментировать
  /*
    if(c>=250)c=0;                                       // fate (opt del 70 and k 20)
          for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[i] = CHSV(c, 255, 255);
        LEDS.show();
        delay(del);
      }
       for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[i] = CHSV(2, 255, 0);
        LEDS.show();
        delay(del);
      }
       c=c+k;
  */


  /*
    if(c>=250)c=0;                                // один из лучших
    for (int i = 0 ; i < LED_COUNT; i++ ) {      // carousel
    leds[i] = CHSV(c, 255, 255);
    LEDS.show();
    delay(del);
    }
    c=c+k;
  */



  /*
    if (c >= 250)c = 0;
    for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i] = CHSV(200, 255, 255);
    LEDS.show();
    delay(del);
    }
    delay(del + 20);
    for ( int i = LED_COUNT ; i >= 0; i-- ) {
    leds[i] = CHSV(100, 255, 255);
    LEDS.show();
    delay(del);
    }
    delay(del + 20);
    for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i] = CHSV(150, 255,255);
    LEDS.show();
    delay(del);
    }
    delay(del + 20);
    for ( int i = LED_COUNT ; i >= 0; i-- ) {
    leds[i] = CHSV(50, 255, 255);
    LEDS.show();
    delay(del);
    }
  */

  /*
    if(c>=250)c=0;                                       // confeti (оптимально del 5-10)
          for (int i = 0 ; i < LED_COUNT; i=i+r ) {
        leds[i] = CHSV(c, 255, 255);
        LEDS.show();
        delay(del);
        r=r+1;
      }
    r=random(LED_COUNT);
    delay(del);
    //c=random(255);
    c=c+20;    // замените на строчку выше, получится более беспорядочно
    for (int i = 0 ; i < LED_COUNT; i=i+r ) {
        leds[i] = CHSV(c, 255, 255);
        LEDS.show();
        delay(del);
        r=r+1;
      }
    r=random(LED_COUNT);
    delay(del);
    //c=random(255);
    c=c+20;    // так более чётко меняется цвет
  */
  /*
                 // siesta (оптимально del 5-10)
          for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[r] = CHSV(c, 255, 255);
        LEDS.show();
        r=random(LED_COUNT);
        c=random(255);
          delay(10);
      }
       for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[r] = CHSV(0, 255, 0);
        LEDS.show();
        r=random(LED_COUNT);
          delay(10);
      }
  */

  /*
            // siesta RD (random del 0-20 )
          for (int i = 0 ; i < LED_COUNT; i=i+2 ) {
           r=random(LED_COUNT);
                c=random(255);
        leds[r] = CHSV(c, 255, 255);
        LEDS.show();
         r=random(20);             // используем одну переменную для
          delay(r);                  // нескольких действий(типо экономия памяти)
      }
       for (int i = 0 ; i < LED_COUNT; i++ ) {
        r=random(LED_COUNT);
        c=random(255);
        leds[r] = CHSV(c, 180, 170);
        LEDS.show();
           r=random(20);
          delay(r);
      }
  */

  
  /*
    // ALL RANDOM (полный рандом)
    for (int i = 0 ; i < LED_COUNT; i = i++ ) {
    r = random(LED_COUNT);
    leds[r] = CHSV(random(20,255), random(100,250), random(110,255) );   // оказывается можно запихнуть рандом прямо сюда
    LEDS.show();
    delay(10);
    }

  */


  /*
    for (int i = 0 ; i < LED_COUNT; i++ ) {        //   estafet
      if (millis() - tim > 300) {
        leds[r] = CHSV(c, 255, 255);
        LEDS.show();
        r = random(LED_COUNT);
        c = random(255);
        tim = millis();
      }
    }

    for (int i = 0 ; i < LED_COUNT; i++ ) {
      if (millis() - tim > 300) {
        leds[r] = CHSV(0, 0, 100);
        LEDS.show();
        r = random(LED_COUNT);
        tim = millis();
      }
    }

  */


  /*
    // color dot fill          цветная точечная заливка
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

  */


  /*
    // кароче огоньки (похож на следующий)
    if (millis() - tim > 500) {
      r = random(LED_COUNT);              // делаем случайную позицию
      color = random(255);                    // и цвет (можно создать диапазон)
      leds[r] = CHSV(color, 255, 255);
      r=r+1;      // красим по два светодиода одним цветом
      leds[r] = CHSV(color, 255, 255);
        LEDS.show();
        tim = millis();
    }
    if (millis() - tim2 > 700) {  // на одном таймере не успевает гореть
    LEDS.clear();
    LEDS.show();
    tim2 = millis();
    }
  */



  /*
    // color flashes    цветовые вспышки
    if (millis() - tim > 200) {
      r = random(LED_COUNT);              // делаем случайную позицию
      color = random(255);                    // и цвет (можно создать диапазон)
      leds[r] = CHSV(color, 255, 255);
      LEDS.show();
      tim = millis();
    }
    if (millis() - tim2 > 210) {  // на одном таймере не успевает гореть
      LEDS.clear();
      LEDS.show();
      tim2 = millis();
    }
  */



  /*
    //  fantasy     фантазия - получился случайно, при отладке другого режима
    //      возможно уже есть похожий, ну пускай и этот останется
    for (int i = 0 ; i < LED_COUNT; i+2 ) {
      r = random(LED_COUNT);              // делаем случайную позицию
      color = random(255);                    // и цвет (можно создать диапазон)
      leds[r] = CHSV(color, 255, 255);          // красим рандомные пиксели
      LEDS.show();
      delay(5);
    }
    for (int i = 0 ; i < LED_COUNT; i++ ) {
      leds[i] = CHSV(0, 255, 0);          // стираем
      LEDS.show();
    }
  */

  /*
    // fire dots          // огненные точки(типо огонь)
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
  */




  /*
    // огонь против воды версия 1      fire vs water v1
    // противоборство по всей ленте
    if (millis() - tim > 80) {          // поиграйтесь с задержкой
      r = random(LED_COUNT);              // делаем случайную позицию
      leds[r] = CRGB(253, 105, 6);    // оттенок 1
      LEDS.show();
      r = random(LED_COUNT);              // делаем случайную позицию
      leds[r] = CRGB(253, 47, 6);    // оттенок 2(можно больше)
      LEDS.show();
      r = random(LED_COUNT);              // делаем случайную позицию
      leds[r] = CRGB(7, 123, 255);    // оттенок 1
      LEDS.show();
      r = random(LED_COUNT);              // делаем случайную позицию
      leds[r] = CRGB(7, 255, 239);    // оттенок 2(можно больше)
      LEDS.show();
      tim = millis();
    }
  */

/*
 // огонь против воды версия 2      fire vs water v2
 // половина-огонь, другая-вода
  if (millis() - tim > 200) {       // вода более медленная
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
  if (millis() - tim2 > 80) {   // огонь сделаем побыстрее воды
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
*/

/*
// carousel 2
if (millis() - tim2 > 20) { 
  r=r+1;
if(r>LED_COUNT)r=0;
 leds[r] = CHSV(color, 255, 255);
         LEDS.show();   
tim2 = millis();
  }

  if (millis() - tim > 500) { 
   color=color+30;                 // отвечает за смену цвета

if(color>=250)color=0;                  // защита от "перебора" 
tim = millis();
  }
 */




  
  // огонь против воды версия 3      fire vs water v3
}
