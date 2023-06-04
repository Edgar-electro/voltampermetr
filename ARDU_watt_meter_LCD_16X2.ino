
#include <Wire.h>  // Библиотека для работы с I2C
#include <LiquidCrystal_I2C.h> // Библиотека для работы с ЖК-дисплеем
int a,b,c;
long volt,current,power,ah;

unsigned long msec = 0;
float time = 0.0;
int sample = 0;
float totalCharge = 0.0;
float averageAmps = 0.0;
float ampSeconds = 0.0;
long ampHours = 0;


int totalColumns = 16;
int totalRows = 2;

LiquidCrystal_I2C lcd(0x23, totalColumns, totalRows);  
void setup()
{
  lcd.init(); // Инициализация ЖК-дисплея
  lcd.backlight(); // Включение подсветки ЖК-дисплея
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
  Serial.begin(9600);
}


void loop()

{
   for(int i=0;i<200;i++){
   c = analogRead(A0);
   a = analogRead(A1);
   volt=volt+a;
   current=current+c;
   delay(1);
 }
   current=(current/200-514);
   if(current<1)current=0;
   current=current*5.65;
   volt=volt/65.2;
   power=(volt*current)/1000;
 
   sample = sample + 1;
  
   msec = millis();
  
  
  
 time = (float) msec / 1000.0;
  
 totalCharge = totalCharge + (current);
  
 averageAmps = totalCharge / sample;
  
 ampSeconds = averageAmps*time;

 ampHours = ampSeconds/3600;

 //--------------
  //  Serial.print(volt);
   //   Serial.print("  ");
     //   Serial.print(current);
      //Serial.print("  ");
        //Serial.print(power);

        //Serial.print("  ");
        //Serial.print(ampHours);
        // Serial.print("  ");
       // Serial.println(time);
        
   lcd.setCursor(0,0);
   lcd.print("V:");
   b=volt%10;
   lcd.setCursor(6,0);
   lcd.print(b);
   b=(volt/10)%10;
   lcd.setCursor(5,0);
   lcd.print(b);
   lcd.setCursor(4,0);
   lcd.print(".");
   b=(volt/100)%10;
   lcd.setCursor(3,0);
   lcd.print(b);
   b=(volt/1000)%10;
   lcd.setCursor(2,0);
   if(volt>999)lcd.print(b);
   else lcd.print(" ");
  
   lcd.setCursor(9,0);
   lcd.print("A:");
   b=current%10;
   lcd.setCursor(15,0);
   lcd.print(b);
   b=(current/10)%10;
   lcd.setCursor(14,0);
   lcd.print(b);
   lcd.setCursor(13,0);
   lcd.print(".");
   b=(current/100)%10;
   lcd.setCursor(12,0);
   lcd.print(b);
   b=(current/1000)%10;
   lcd.setCursor(11,0);
   if(current>999)lcd.print(b);
   else lcd.print(" ");
 

  lcd.setCursor(0,1);
  lcd.print("W:");
  b=power%10;
  lcd.setCursor(6,1);
  lcd.print(b);
  b=(power/10)%10;
  lcd.setCursor(5,1);
  lcd.print(b);
  lcd.setCursor(4,1);
  lcd.print(".");
  b=(power/100)%10;
  lcd.setCursor(3,1);
  lcd.print(b);
  b=(power/1000)%10;
  lcd.setCursor(2,1);
  if(power>999)lcd.print(b);
  else lcd.print(" ");
 
  
   lcd.setCursor(9,1);
   lcd.print("H:");
   b=ampHours%10;
   lcd.setCursor(15,1);
   lcd.print(b);
   b=(ampHours/10)%10;
   lcd.setCursor(14,1);
   lcd.print(b);
   lcd.setCursor(13,1);
   lcd.print(".");
   b=(ampHours/100)%10;
   lcd.setCursor(12,1);
   lcd.print(b);
   b=(ampHours/1000)%10;
   lcd.setCursor(11,1);
   if(ampHours>999)lcd.print(b);
   else lcd.print(" ");
 
 
  delay(500);
  
}  
