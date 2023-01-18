// FREYA_EVO   v1      2022year
//    by EASYAMPER (https://www.youtube.com/channel/UCV4Uu5sQTTSmZkmL-U767qg)
// скетч создан на старой версии 4.1, за основу взяты некоторые наработки AlexGyver (https://alexgyver.ru/), 
// некоторые переменные оптимизированы мной, созданы свои режимы
// помидоры особо не кидать - я не программист, знаю что можно лучше сделать, мне просто лень)

#include <iarduino_RTC.h>
#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "FastLED.h"
#include "GyverButton.h"

#define LED_COUNT 30          // число светодиодов 
#define LED_DT 10             // пин подключения ленты
#define DHT11_PIN 8
#define BTN_PIN 3
dht11 DHT;
LiquidCrystal_I2C lcd(0x27, 20, 4);
iarduino_RTC watch(RTC_DS1307);
GButton butt1(BTN_PIN);
// переменные
byte mode = 1;              //  экраны
byte rez = 1;               // эффекты
int t;                       //  температура
int hum;                     //  влажность
byte brig = 150;             //яркость
int pot;                     // потенциометр
byte menu = 0;                // подпункты(на отдельном экране)
unsigned long b;              // чтобы че то считать
byte color;                   // для палитры цветов по HUE
byte r;                      // для своих режимов
unsigned long tim;            // для таймера самопальных эффектов
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ---------------------------------------------
byte ballColors[3][3] = {
  {0xff, 0, 0},
  {0xff, 0xff, 0xff},
  {0   , 0   , 0xff},
};
byte BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(LED_COUNT / 2);
int EVENODD = LED_COUNT % 2;
struct CRGB leds[LED_COUNT];
int ledsX[LED_COUNT][3];     //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)

int thisdelay = 20;          //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
byte thishue = 0;             //-FX LOOPS DELAY VAR
byte thissat = 255;           //-FX LOOPS DELAY VAR

int thisindex = 0;
byte thisRED = 0;
byte thisGRN = 0;
byte thisBLU = 0;

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
byte ihue = 0;                //-HUE (0-255)
byte ibright = 0;             //-BRIGHTNESS (0-255)
byte isat = 0;                //-SATURATION (0-255)
byte bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
int lcount = 0;              //-ANOTHER COUNTING VAR
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------------------------------------------
void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}

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
  lcd.print("    A P A R E L'    ");
  delay(500);
  lcd.setCursor(0, 2);
  lcd.print(" F R E Y A    E V O ");
  delay(1800);
  lcd.clear();                               // очищаем дисплей
  one_color_all(0, 0, 0);                    // ну и ленту тоже
  LEDS.show();
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
    one_color_all(0, 0, 0);                    // ленту очищаем
    LEDS.show();                                  //(она на данном "экране" гореть не должна)
    lcd.setCursor(0, 0);
    lcd.print(watch.gettime("      H : i      "));        // часы
    lcd.setCursor(0, 1);
    lcd.print(watch.gettime("d D      "));
    lcd.print(watch.gettime("M Y"));
    lcd.setCursor(0, 2);
    lcd.print("--------------------");
    lcd.setCursor(0, 3);
    DHT.read(DHT11_PIN);                         //узнаём у датчика занчения
    t = DHT.temperature;                         // температура
    hum = DHT.humidity;                          // влажность
    lcd.print(" t= ");                           // показываем
    lcd.print(t);
    lcd.print(" C   ");
    lcd.print(" h= ");
    lcd.print(hum);
    lcd.print(" % ");
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
      delay(100);
    }
    if (menu == 1) {
      rez = map(pot, 0, 1023, 1, 15);              // для смены эффектов поворотом ручки
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
      lcd.print("      cycle        ");
      rainbow_fade();
    }
    if (rez == 3) {
      lcd.print("   random burst    ");
      random_burst();
    }
    if (rez == 4) {
      lcd.print("  EMS lights ALL   ");
      ems_lightsALL();
    }
    if (rez == 5) {
      lcd.print("  random march     ");
      random_march();
    }
    if (rez == 6) {
      lcd.print(" rainbow  vertical ");
      rainbow_vertical();
    }
    if (rez == 7) {
      lcd.print("  police STROBE    ");
      ems_lightsSTROBE();
    }
    if (rez == 8) {
      lcd.print("      FLAME        ");
      flame();
    }
    if (rez == 9) {                           // самопальный эффект
      lcd.print("     carousel      ");       
    if(color>=250)color=0;                  // защита от "перебора" 
     for (int i = 0 ; i < LED_COUNT; i++ ) {     
    leds[i] = CHSV(color, 255, 255);
    LEDS.show();
    delay(20);
     }
    color=color+30;                 // отвечает за смену цвета
  }
  
    if (rez == 10) {
      lcd.print("      rainbow      ");
      new_rainbow_loop();
    }
    
    if (rez == 11) {                            // тоже самопальный
      lcd.print("       FATE        ");
     if(color>=250)color=0;                                       // fate (opt del 70 and k 20)
          for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[i] = CHSV(color, 255, 255);
        LEDS.show();
        delay(10);
      }
       for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[i] = CHSV(2, 255, 0);
        LEDS.show();
        delay(10);
      }
       color=color+20;
    }
    if (rez == 12) {
      lcd.print("     siesta       ");       //тоже самопальный эффект (оптимальный delay 5-15)
          for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[r] = CHSV(color, 255, 255);          // красим рандомные пиксели
        LEDS.show();
        r=random(LED_COUNT);                // делаем случайную позицию
        color=random(255);                      // и цвет (можно создать диапазон)
          delay(10);
      }
       for (int i = 0 ; i < LED_COUNT; i++ ) {
        leds[r] = CHSV(0, 255, 0);          // стираем рандомные пиксели
        LEDS.show();
        r=random(LED_COUNT);
          delay(10);
      }
    }
    if (rez == 13) {
      lcd.print("     estafet      ");
     for (int i = 0 ; i < LED_COUNT; i++ ) {        //   estafet   самопал
    if (millis() - tim > 30) {
      leds[r] = CHSV(color, 255, 255);
      LEDS.show();
      r = random(LED_COUNT);
      color = random(255);
      tim = millis();
    }
  }
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    if (millis() - tim > 30) {
      leds[r] = CHSV(0, 0, 100);
      LEDS.show();
      r = random(LED_COUNT);
      tim = millis();
    }
  }
    }
  
    if (rez == 14) {
      lcd.print("   siesta slow     ");         //  медленный 12й эффект
        for (int i = 0 ; i < LED_COUNT; i++ ) {
           if (millis() - tim > 100) {
        leds[r] = CHSV(color, 255, 255);
        LEDS.show();
        r=random(LED_COUNT);
        color=random(255);
            tim = millis();
      }
        }
       for (int i = 0 ; i < LED_COUNT; i++ ) {
         if (millis() - tim > 100) {
        leds[r] = CHSV(0, 255, 0);
        LEDS.show();
        r=random(LED_COUNT);
           tim = millis(); 
      }
    }
  }
  }
  if (mode == 3) {                                // экран 3 - настройка цвета по всей ленте
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
      //   leds[0].setHue(color);
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
