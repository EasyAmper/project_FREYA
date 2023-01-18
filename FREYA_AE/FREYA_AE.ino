#include <Wire.h>
#define _LCD_TYPE 1                               //для работы с I2C дисплеями
#include <LCD_1602_RUS_ALL.h>
#include <iarduino_RTC.h>
#include <DHT.h>
#include "GyverButton.h"
#define COLOR_DEBTH 3
#include "FastLED.h"
#define LED_COUNT 30         // число светодиодов 
#define LED_DT 10            // пин подключения ленты
#define DHTPIN 8          //пин датчика температуры
#define PIN_POT A1                 //потенциометр
#define PIN_ONOFF 3            // кнопка
#define butdel 50            // задержка для кнопки
#define del 20               // задержка для эффектов(по умолчанию)
#define sat 255              // насыщенность для HSV(по умолчанию)

LCD_1602_RUS lcd(0x27, 20, 4);                    //подключение дисплея
//iarduino_RTC time(RTC_DS1302, 4, 3, 2);           //верни сам у мя конфликтует с кнопкой
DHT dht(DHTPIN, DHT11); //подключение датчика температуры и влажности
GButton butt1(PIN_ONOFF);

byte flag = 1;               // переключатель управления
byte flag2 = 0;
byte mode = 1;              //  экраны
byte rez = 0;               // эффекты
byte menu = 1;              // подпункты(на отдельном экране)
int t;                       //  температура
int h;                     //  влажность
byte brig = 150;             //яркость
int pot;                     // потенциометр
byte color = 200;                 // для палитры цветов по HUE
byte r;                      // для своих режимов
byte idex;                   // до 255 светодиодов(если больше смените на int)
unsigned long b;              // таймер кнопки
unsigned long tim;            // для таймеров в эффектах (без 2х никак, зато delay меньше)
unsigned long tim2;
struct CRGB leds[LED_COUNT];
void setup() {
  pinMode(PIN_POT, INPUT);                        //потенциометр как вход
  pinMode(PIN_ONOFF, INPUT);                      //кнопка как вход
  LEDS.setBrightness(brig);
  LEDS.addLeds<WS2812B, LED_DT, GRB>(leds, LED_COUNT);
  LEDS.show();
  butt1.setTimeout(300);                                // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setClickTimeout(200);
  butt1.setType(LOW_PULL);
  butt1.setDirection(NORM_OPEN);
  // time.begin();
  //  time.settime(i[0],i[1],i[2],i[3],i[4],i[5]);
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  butt1.tick();
  pot = analogRead(PIN_POT);
  if (flag == 1) {  // режим управления главными экранами
    mode = map(pot, 0, 1023, 1, 3);
  }
  if (mode == 1) {    // главный экран
    menu = 1;
    rez = 0;
    LEDS.clear();
    lcd.setCursor(0, 2);
    lcd.print("------------");
    lcd.setCursor(0, 1);
    lcd.print("-------------");
    lcd.setCursor(0, 0);
    lcd.print("alternative  edition");
    lcd.setCursor(0, 3);                          //вывод температуры и влажности
    lcd.print((String)"t=" + t + char(223) + "C    ");
    lcd.setCursor(15, 3);
    lcd.print((String)"h=" + h + "%");
    delay(1);
  }
  if (mode == 2) {     // режимы и цвет
    if ((digitalRead(3)) == 1) {
      flag2 = flag2 + 1;                                 // для работы с меню
      if (flag2 > 1) flag2 = 0;
      delay(butdel);
    }
    if (flag == 0 && flag2 == 0)menu = map(pot, 0, 1023, 1, 5);

    lcd.setCursor(2, 0);
    lcd.print((String) " Реж. " + rez + " / 9       ");
    lcd.setCursor(2, 1);
    lcd.print((String) " Ярк. " + brig + " / 255   ");
    lcd.setCursor(2, 2);
    lcd.print((String) " Цвет " + color + " / 255   ");
    lcd.setCursor(2, 3);
    lcd.print((String) " Выход            ");
    if (menu == 1) {
      flag = 0;
      lcd.setCursor(0, 0);
      if (flag2 == 0)lcd.print("> ");
      if (flag2 == 1)lcd.print(">>");
      lcd.setCursor(0, 1);
      lcd.print("  ");
      lcd.setCursor(0, 2);
      lcd.print("  ");
      lcd.setCursor(0, 3);
      lcd.print("  ");

      if (flag2 == 1) {
        rez = map(pot, 0, 1023, 1, 9);
      }
    }
    if (menu == 2) {
      lcd.setCursor(0, 0);
      lcd.print("  ");
      lcd.setCursor(0, 1);
      if (flag2 == 0)lcd.print("> ");
      if (flag2 == 1)lcd.print(">>");
      lcd.setCursor(0, 2);
      lcd.print("  ");
      lcd.setCursor(0, 3);
      lcd.print("  ");

      if (flag2 == 1) {
        brig = map(pot, 0, 1023, 0, 255);
        LEDS.setBrightness(brig);
      }
    }
    if (menu == 3) {
      lcd.setCursor(0, 0);
      lcd.print("  ");
      lcd.setCursor(0, 1);
      lcd.print("  ");
      lcd.setCursor(0, 2);
      if (flag2 == 0)lcd.print("> ");
      if (flag2 == 1)lcd.print(">>");
      lcd.setCursor(0, 3);
      lcd.print("  ");

      if (flag2 == 1) {
        rez = 0;
        color = map(pot, 0, 1023, 0, 255);
        for (int i = 0 ; i < LED_COUNT; i++ ) {      // пишу его включение на повторяющийся красный цвет HSV(примерно)
          leds[i] = CHSV(color, 255, 255);
        }
        LEDS.show();
        delay(1);
      }
    }
    if (menu == 4) {
      if (butt1.isHold()) {            //обработка длительного касания
        if (millis() - b > 300) {
          b = millis();                   // обработка короткого нажатия
          flag = 1;
        }
      }
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(0, 2);
      lcd.print(" ");
      lcd.setCursor(0, 3);
      if (flag == 0)lcd.print("> ");
      if (flag == 1)lcd.print("<<");
    }
    // режимы
    if (rez == 1)rainbow_loop();
    if (rez == 2)cycle(250);       // вызываем, указывая задержку(до 255 при byte, см вкладку эффекты)
    if (rez == 3)cycle(10);
    if (rez == 4)color_flashes();
    if (rez == 5)color_dot_fill();
    if (rez == 6)fire_dots();
    if (rez == 7)carosel();
    if (rez == 8) {
      color -= 1;
      fill_rainbow( leds, LED_COUNT, color );
      LEDS.show();
      delay(del);
    }
  }
}
