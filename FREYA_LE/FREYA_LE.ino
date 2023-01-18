// облегчённая версия скетча "FREYA EVO"
//      by EASYAMPER
// большая часть эффектов написаны мной
#include <iarduino_RTC.h>
#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "FastLED.h"
#include "GyverButton.h"

#define LED_COUNT 30          // число светодиодов 
#define LED_DT 10             // пин подключения ленты
#define DHT11_PIN 8          //пин датчика температуры
#define BTN_PIN 3            // кнопка
#define del 20               // задержка для эффектов(по умолчанию)
#define sat 255              // насыщенность для HSV(по умолчанию)
dht11 DHT;
LiquidCrystal_I2C lcd(0x27, 20, 4);
iarduino_RTC watch(RTC_DS1307);
GButton butt1(BTN_PIN);

byte mode = 1;              //  экраны
byte rez = 1;               // эффекты
byte menu = 0;                // подпункты(на отдельном экране)
int t;                       //  температура
int hum;                     //  влажность
byte brig = 150;             //яркость
int pot;                     // потенциометр
byte color;                   // для палитры цветов по HUE
byte r;                      // для своих режимов
byte idex;                   // до 255 светодиодов(если больше смените на int)
unsigned long b;              // таймер кнопки
unsigned long tim;            // для таймеров в эффектах (без 2х никак, зато delay меньше)
unsigned long tim2;
struct CRGB leds[LED_COUNT];

void setup() {
  pinMode(A1, INPUT);
  LEDS.setBrightness(brig);                             //установить яркость
  LEDS.addLeds<WS2812B, LED_DT, GRB>(leds, LED_COUNT);  //для ленты ( WS2811, WS2812, WS2812B)
  LEDS.show();                                          // отослать команду на ленту
  butt1.setTimeout(300);                                // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setClickTimeout(200);
  butt1.setType(LOW_PULL);
  butt1.setDirection(NORM_OPEN);
  watch.begin();
  lcd.init();
  lcd.backlight();
  //    заставка      -------------------------------------------------
  for (int i = 0; i < LED_COUNT; i++) {         // подрубаем статичную радугу
    leds[i].setHue(i * 255 / LED_COUNT);
  }
  LEDS.show();
  lcd.setCursor(0, 1);
  lcd.print("     F R E Y A      ");
  delay(500);
  lcd.setCursor(0, 2);
  lcd.print("   light edition    ");
  delay(1800);
  lcd.clear();                               // очищаем дисплей
  LEDS.clear();                // ну и ленту тоже
  LEDS.show();
  //lcd.noBacklight();
}

void loop() {
  pot = analogRead(A1);
  butt1.tick();
  if (butt1.isHold()) {            //обработка длительного касания(переключение экранов)
    if (millis() - b > 500) {
      b = millis();
      mode = mode + 1;
      if (mode < 1) mode = 1;
      if (mode > 3) mode = 1;
      lcd.clear();
    }
  }

  if (mode == 1) {       // главный экран( время, дата и прочие радости)
    LEDS.clear();       //(лента на данном "экране" гореть не должна)
    LEDS.show();
    DHT.read(DHT11_PIN);                         //узнаём у датчика занчения
    t = DHT.temperature;                         // температура
    hum = DHT.humidity;                          // влажность
    lcd.setCursor(0, 0);
    lcd.print("t= ");                           // показываем
    lcd.print(t);
    lcd.print(" C");
    lcd.setCursor(13, 0);
    lcd.print("h= ");
    lcd.print(hum);
    lcd.print(" %");
    lcd.setCursor(0, 2);
    lcd.print(watch.gettime("      H : i      "));        // часы
    lcd.setCursor(0, 3);
    lcd.print(watch.gettime("d D      "));
    lcd.print(watch.gettime("M Y"));
    //альтернативный вид часов
    /*
      lcd.setCursor(0, 1);
      lcd.print("Hours:  ");
      lcd.print(watch.gettime("H"));
      lcd.setCursor(0, 2);
      lcd.print("Minutes: ");
      lcd.print(watch.gettime("i"));
    */
    delay(1);
  }

  if (mode == 2) {                                // экран 2 - выбор эффекта
    lcd.setCursor(0, 0);
    lcd.print("      LED  FX       ");            // выводим интерфейс экрана
    lcd.setCursor(1, 3);
    lcd.print("  Brightness: ");
    lcd.print(brig);
    lcd.print("  ");
    if ((digitalRead(3)) == 1) {                   // обработка короткого нажатия
      menu = menu + 1;                                 // для работы с меню
      if (menu > 2 || menu < 1) menu = 1;
      delay(50);
    }
    if (menu == 1) {
      rez = map(pot, 0, 1023, 1, 11);              // для смены эффектов поворотом ручки
      lcd.setCursor(0, 1);
      lcd.print(">");                              //один курсор пишем
      lcd.setCursor(0, 3);
      lcd.print(" ");                              // второй "закрываем" пробелом
      delay(20);                                   // техническая задержка(наверно можно и убрать)
    }
    if (menu == 2) {
      brig = map(pot, 0, 1023, 0, 250);            // для настройки яркости тем же потенциометром
      lcd.setCursor(0, 3);
      lcd.print(">");
      LEDS.setBrightness(brig);
      lcd.setCursor(0, 1);
      lcd.print(" ");
      delay(20);
    }
    lcd.setCursor(1, 1);
    if (rez == 1) {
      lcd.print("   rainbow loop    ");
      rainbow_loop();
    }
    if (rez == 2) {
      lcd.print("    slow cycle     ");
      cycle(250);       // вызываем, указывая задержку(до 255 при byte, см вкладку эффекты)
    }
    if (rez == 3) {
      lcd.print("      cycle        ");
      cycle(10);
    }
    if (rez == 4) {
      lcd.print("  color flashes    ");
      color_flashes();
    }
    if (rez == 5) {
      lcd.print("  color dot fill   ");
      color_dot_fill();
    }
    if (rez == 6) {
      lcd.print("     fire dots     ");
      fire_dots();
    }
    if (rez == 7) {
      lcd.print("     carousel      ");
      carosel();
    }
    if (rez == 8) {
      lcd.print("     rainbow       ");
      color -= 1;
      fill_rainbow( leds, LED_COUNT, color );
      LEDS.show();
      delay(del);
    }
    if (rez == 9) {
      lcd.print("  fire vs water    ");
      fireVSwater();
    }
    if (rez == 10) {
      lcd.print("     siesta        ");   // может лагать на этом режиме
      siesta();
    }
  }
  if (mode == 3) {
    lcd.setCursor(0, 0);
    lcd.print("    LED HSV color   ");
    lcd.setCursor(1, 3);
    lcd.print(" Brightness: ");
    lcd.print(brig);
    lcd.print("  ");
    lcd.setCursor(1, 1);
    lcd.print(" HSV: ");
    lcd.print(color);
    lcd.print("  ");
    lcd.setCursor(0, 2);
    lcd.print("--------------------");
    if ((digitalRead(3)) == 1) {
      menu = menu + 1;
      if (menu > 2 || menu < 1) menu = 1;
      delay(100);
    }
    if (menu == 1) {
      color = map(pot, 0, 1023, 0, 255);
      lcd.setCursor(0, 1);
      lcd.print(">");
      lcd.setCursor(0, 3);
      lcd.print(" ");
      delay(5);
    }
    if (menu == 2) {
      brig = map(pot, 0, 1023, 0, 255);
      lcd.setCursor(0, 3);
      lcd.print(">");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      delay(5);
    }
    if (color <= 249) {                          // белый цвет не врубается по HSV(может я тупанул)
      for (int i = 0 ; i < LED_COUNT; i++ ) {      // пишу его включение на повторяющийся красный цвет HSV(примерно)
        leds[i] = CHSV(color, 255, 255);
      }
      lcd.setCursor(12, 1);
      lcd.print("     ");
      LEDS.show();
    }
    if (color > 249) {
      for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[i] = CHSV(255, 0, 255);
      }
      lcd.setCursor(12, 1);
      lcd.print("WHITE");                 // помечаем что включился белый
      LEDS.show();
    }
    LEDS.setBrightness(brig);
    delay(5);
  }
}
