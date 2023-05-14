
#include <Wire.h>  // Библиотека для работы с I2C
#include <LiquidCrystal_I2C.h> // Библиотека для работы с ЖК-дисплеем
#include <ACS712.h> // Библиотека для работы с датчиком ACS712


const int RESET_BUTTON_PIN = 6;
const int acs712_pin = A0; // Пин для подключения датчика тока ACS712
const int voltage_pin = A1; // Пин для измерения напряжения


const float voltage_divider_ratio = 6.686; //  5.730Коэффициент делителя напряжения
const float current_divider_ratio = 1.130;


int totalColumns = 16;
int totalRows = 2;
int buttonState = 0 ;

ACS712 sensor(ACS712_20A, acs712_pin);
LiquidCrystal_I2C lcd(0x23, totalColumns, totalRows);  


String staticMessage = "--VOLTMETER--";
String scrollingMessage = "VOLTMETER 0-30 VOLT 0-20 AMPER";


float voltage = 0;
float current = 0;
float power = 0;
float consumed_amp_hours = 0; 



void scrollMessage(int row, String message, int delayTime, int totalColumns) {
  for (int i=0; i < totalColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int position = 0; position < message.length(); position++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(position, position + totalColumns));
    delay(delayTime);
  }
}


void update_measurements() {
  
  voltage = analogRead(voltage_pin) * (4.30 / 1023.0) * voltage_divider_ratio ; // Расчет напряжения
  if (voltage <0.15){
  voltage = 0 ;
  }
  current = sensor.getCurrentDC() * current_divider_ratio ; // Расчет тока
  if (current <0.15){
  current = 0 ;
   }
  power = voltage * current; // Расчет мощности
  if (power <0.15){
  power = 0 ;
  }
  consumed_amp_hours += current / 3600.0; // Расчет накопленных Ампер-часов
}


void print_lcd() {
  lcd.setCursor(0,0);
  lcd.print("V:");
  lcd.print(voltage, 2);
  
  lcd.setCursor(0,1);
  lcd.print("I:");
  lcd.print(current, 2);
  
  lcd.setCursor(8,0);
  lcd.print("P:");
  lcd.print(power, 2);
  
  lcd.setCursor(8,1);
  lcd.print("Ah:");
  lcd.print(consumed_amp_hours, 2);
 
}



void setup() {
  
  
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  
  
  
  lcd.init(); // Инициализация ЖК-дисплея
  lcd.backlight(); // Включение подсветки ЖК-дисплея
  lcd.setCursor(1, 0);
  lcd.print(staticMessage);
  scrollMessage(1, scrollingMessage, 150, totalColumns);
  delay(500);
  lcd.clear();





}

void loop() {
 for(int i=0;i<200;i++)
 
 buttonState = digitalRead(RESET_BUTTON_PIN);
  if (buttonState == LOW) {
    consumed_amp_hours = 0;
  }
 
  
  update_measurements(); // Обновление измерений
  print_lcd(); // Вывод данных на ЖК-дисплей
  delay(1000);
}
