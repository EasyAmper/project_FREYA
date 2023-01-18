//      by EASYAMPER
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

byte md;                   // стиль главного экрана
bool BL = 1;                // подсветка
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
byte customChar[] = {      //волшебная дубина(палочка)
  B10001,
  B00100,
  B10000,
  B00011,
  B00011,
  B00100,
  B01000,
  B10000
};
byte customChar1[] = {     // солнце
  B00000,
  B00000,
  B10101,
  B01110,
  B11011,
  B01110,
  B10101,
  B00000
};
byte customChar2[] = { // типо градусник
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B10001,
  B10001,
  B01110
};
byte customChar3[] = { // типо сон (кровать и месяц)
  B00010,
  B00001,
  B00001,
  B00010,
  B00000,
  B11000,
  B11111,
  B10001
};
byte customChar4[] = {    // капля
  B00100,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110
};
byte customChar5[] = {   // дом
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B01010,
  B01110,
  B00000
};
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
  lcd.print("      revolution    ");
  delay(1800);
  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar1);
  lcd.createChar(2, customChar2);
  lcd.createChar(3, customChar3);
  lcd.createChar(4, customChar4);
  lcd.createChar(5, customChar5);// можно загрузить 8 символов, любой символ можно перезаписать
  lcd.clear();                               // очищаем дисплей
  LEDS.clear();                // ну и ленту тоже
  LEDS.show();
}

void loop() {
  pot = analogRead(A1);     // опрос потенциометра
  butt1.tick();             //      и кнопки
  if (butt1.isHold()) {            // обработка длительного касания(переключение экранов)
    if (millis() - b > 500) {
      b = millis();
      mode = mode + 1;             // смена экранов
      if (mode < 1) mode = 1;
      if (mode > 3) mode = 1;
      lcd.clear();               // очищаем дисплей при переходе на другой экран
    }
  }

  if (mode == 1) {       // главный экран( время, дата и прочие радости)
    LEDS.clear();       //(лента на данном "экране" гореть не должна)
    LEDS.show();
    md = map(pot, 0, 1023, 0, 3);
    DHT.read(DHT11_PIN);                         //узнаём у датчика занчения
    t = DHT.temperature;                         // температура
    hum = DHT.humidity;                          // влажность

    if (md == 0) {
      lcd.setCursor(0, 0);
      lcd.write(2);
      lcd.print(t);
      lcd.print(char(223));
      lcd.print("C     ");
      lcd.write(5);
      lcd.print("    ");
      lcd.write(4);
      lcd.print(hum);
      lcd.print(" %");
      lcd.setCursor(0, 1);
      lcd.print("                    ");
      lcd.setCursor(0, 2);
      lcd.print(watch.gettime("      H : i       "));        // часы
      lcd.setCursor(0, 3);
      lcd.print(watch.gettime("d D      "));
      lcd.print(watch.gettime("M Y"));
    }
    if (md == 1) {
      lcd.setCursor(0, 0);
      lcd.write(5);
      lcd.print("            |");
       lcd.write(2);
      lcd.print(" ");
      lcd.print(t);
      lcd.print(char(223));
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print(watch.gettime("   H:i:s  |"));
      lcd.write(4);
      lcd.print(" ");
      lcd.print(hum);
      lcd.print(" %");
      lcd.setCursor(0, 2);
      lcd.print("             |      ");
      lcd.setCursor(0, 3);
      lcd.print(watch.gettime("  d.m.Y |  D "));
    }
    if (md == 2) {
      lcd.setCursor(0, 3);
      lcd.print(watch.gettime("     d.m.Y     "));
      lcd.setCursor(0, 1);
      lcd.print(watch.gettime("    H : i : s    "));
      lcd.setCursor(0, 0);
      lcd.print(t);
      lcd.print(char(223));
      lcd.print("C            ");
      lcd.print(hum);
      lcd.print(" %");
      lcd.setCursor(0, 2);
      if ((watch.seconds) == 0)lcd.print ("|                   ");
      if ((watch.seconds) == 3)lcd.print ("||                  ");
      if ((watch.seconds) == 6)lcd.print ("|||                 ");
      if ((watch.seconds) == 9)lcd.print ("||||                ");
      if ((watch.seconds) == 12)lcd.print ("|||||               ");
      if ((watch.seconds) == 15)lcd.print ("||||||              ");
      if ((watch.seconds) == 18)lcd.print ("|||||||             ");
      if ((watch.seconds) == 21)lcd.print ("||||||||            ");
      if ((watch.seconds) == 24)lcd.print ("|||||||||           ");
      if ((watch.seconds) == 27)lcd.print ("||||||||||          ");
      if ((watch.seconds) == 30)lcd.print ("|||||||||||         ");
      if ((watch.seconds) == 33)lcd.print ("||||||||||||        ");
      if ((watch.seconds) == 36)lcd.print ("|||||||||||||       ");
      if ((watch.seconds) == 39)lcd.print ("||||||||||||||      ");
      if ((watch.seconds) == 42)lcd.print ("|||||||||||||||     ");
      if ((watch.seconds) == 45)lcd.print ("||||||||||||||||    ");
      if ((watch.seconds) == 48)lcd.print ("|||||||||||||||||   ");
      if ((watch.seconds) == 51)lcd.print ("||||||||||||||||||  ");
      if ((watch.seconds) == 54)lcd.print ("||||||||||||||||||||");
    }
    if (md == 3) {
      lcd.setCursor(0, 0);
      lcd.write(5);
      lcd.print(" + ");
      lcd.write(3);
      lcd.print("               ");
      lcd.setCursor(0, 3);
      if (BL == 0) {
        lcd.print("    ");
        lcd.write(3);
        lcd.print(" SLEEP         ");
      }
      if (BL == 1)lcd.print("                    ");
      lcd.setCursor(0, 2);
      lcd.print(watch.gettime("      H : i       "));
      if ((digitalRead(3)) == 1) {       // для управления подсветкой двойным касанием
        BL = !BL;                            // инвертируем переменную (было 1 станет 0 и т.д.)
        if (BL == 1)lcd.backlight();
        if (BL == 0)lcd.noBacklight();
        delay(100);
      }
      lcd.setCursor(0, 1);
      lcd.print("                    ");
    }
    delay(1);
  }

  if (mode == 2) {                                // экран 2 - выбор эффекта
    lcd.setCursor(0, 0);
    lcd.write(0);
    lcd.print("     LED  FX       ");            // выводим интерфейс экрана
    lcd.setCursor(1, 3);
    lcd.write(1);
    lcd.print(" Brightness: ");
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
    lcd.write(0);
    lcd.print("   LED HSV color   ");
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
