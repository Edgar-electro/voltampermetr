
#define BLYNK_TEMPLATE_ID "TMPL5vNHuGZXB"
#define BLYNK_TEMPLATE_NAME "voltmetr"
#define BLYNK_AUTH_TOKEN "jBciBWi3GH2H7cFRB7okiYcP7AuZEaeC"



 
 #define BLYNK_PRINT Serial
 #include "MUX74HC4067.h"
 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>
 #include <WiFiManager.h> 
 #include <ESP8266WiFi.h> 
 #include <BlynkSimpleEsp8266.h>


 
 

 char ssid[] = "SFR_EDD0";
char pass[] = "vigen201402";

 LiquidCrystal_I2C lcd(0x27, 16, 2);  //(0x27, 16, 2);
  
 MUX74HC4067 mux(D3, D4, D5, D6, D7);

 char auth[] = BLYNK_AUTH_TOKEN;

 BlynkTimer timer;
  
  //int  voltRead0,  voltRead1 ,voltRead2 ,voltRead3;
  
  
  int  voltRead0 ;
  int  voltRead1 ;
  int  voltRead2 ;
  int  voltRead3 ;
  
  
  long voltage0 ;
  long voltage1 ;
  long voltage2 ;
  long voltage3 ;
  
  float VOLTtotal0 ;
  float VOLTtotal1 ;
  float VOLTtotal2 ;
  float VOLTtotal3 ;
  
  float realVoltage = 31.92;     // Реальное напряжение, которое вы измерили (в вольтах)  stex poxem maxsimal volty testerov chapvac jisht volti masina xosqy
  float measuredVoltage = 32.09; // Измеренное напряжение, которое Arduino показало (в вольтах)
  float Maxvoltage = 32.00;
  float correctionFactor;      // Коэффициент коррекции
  float correctedVoltage0;        // Скорректированное напряжение (в вольтах)
  float correctedVoltage1;
  float correctedVoltage2;
  float correctedVoltage3;
  float  ampcorrect = 107.0;


void setup() {

    Serial.begin(115200);  
    
    mux.signalPin(A0, INPUT, ANALOG); 
 
  delay(500);
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  delay(1000);
    
    WiFi.begin(ssid, pass);
    lcd.setCursor(0, 0);
    lcd.print("Connecting WIFI");
    delay(1500);
    lcd.clear();
    lcd.print("Connecting WIFI");
    delay(1500);
    lcd.clear();
    Serial.print("Connecting to WiFi");
    delay(1000);{
    WiFiManager wifiManager;
    wifiManager.autoConnect("BLYNK-IOT");
    Serial.println(WiFi.localIP());  
    }
   WiFi.setAutoReconnect(true);
   WiFi.persistent(true);
 
  Blynk.config(auth);
  delay(1000);
   
}


void loop(){

 
 update_value0();
  update_value1();
   update_value2();
    update_value3();
  Blynk.run();//Run the Blynk library
  
  
}
  
  
  
 void   update_value0() {
  for(int i=0;i<200;i++) {
  voltRead0= (mux.read(0));
  voltage0 += voltRead0 ;
    delay(1);
   }
   voltage0 = voltage0 / 200 ;
   VOLTtotal0 = (voltage0 / 1023.0) * Maxvoltage;  //stex poxem maxsimal volty testerov chapvac jisht volti masina xosqy
   correctionFactor = realVoltage / measuredVoltage;
   correctedVoltage0 = VOLTtotal0 * correctionFactor;
  Blynk.virtualWrite(V1, correctedVoltage0);
  lcd.setCursor(0, 0);
  lcd.print("1");
  lcd.setCursor(1, 0);
  lcd.print("V");
  lcd.print(correctedVoltage0);
  lcd.print(" ");
  Serial.print("Измеренное напряжение:   0 ");
  Serial.println(correctedVoltage0);
  Serial.print("Analog Value:            0 ");
  Serial.println(voltRead0);
  }
void  update_value1() {
   for(int i=0;i<200;i++) { 
  voltRead1= (mux.read(1));
  voltage1 += voltRead1 ;
    delay(1);
   }
   voltage1 = voltage1 / 200 ;
   VOLTtotal1 = (voltage1 / 1023.0) * Maxvoltage;  //stex poxem maxsimal volty testerov chapvac jisht volti masina xosqy
   correctionFactor = realVoltage / measuredVoltage;
   correctedVoltage1 = VOLTtotal1 * correctionFactor;
   Blynk.virtualWrite(V2, correctedVoltage1);
   lcd.setCursor(0, 1);
  lcd.print("2");
  lcd.setCursor(1, 1);
  lcd.print("V");
  lcd.print(correctedVoltage1);
  lcd.print(" ");
  Serial.print("Измеренное напряжение:   1 ");
  Serial.println(correctedVoltage1);
  Serial.print("Analog Value:            1 ");
  Serial.println(voltRead1);
 
    }
   
 void  update_value2() { 
   for(int i=0;i<200;i++) {  
  voltRead2= (mux.read(2));
  voltage2 += voltRead2 ;
   delay(1);
  }
   voltage2 = voltage2 / 200 ;
   VOLTtotal2 = (voltage2 / 1023.0) * Maxvoltage;  //stex poxem maxsimal volty testerov chapvac jisht volti masina xosqy
   correctionFactor = realVoltage / measuredVoltage;
   correctedVoltage2 = VOLTtotal2 * correctionFactor;
   Blynk.virtualWrite(V3, correctedVoltage2);
   lcd.setCursor(9, 0);
  lcd.print("3");
  lcd.setCursor(10, 0);
  lcd.print("V");
  lcd.print(correctedVoltage2);
  lcd.print(" ");
  Serial.print("Измеренное напряжение:    2 ");
  Serial.println(correctedVoltage2);
  Serial.print("Analog Value:             2 ");
  Serial.println(voltRead2);

 }

void   update_value3() {
  for(int i=0;i<200;i++) {
  voltRead3= (mux.read(3));
  voltage3 += voltRead3 ;
  delay(1);
  
 } 
   
   voltage3 = voltage3 / 200 - 828 ;
   VOLTtotal3 = voltage3 *(1.0 / 1023);
   correctedVoltage3 = VOLTtotal3  *correctionFactor ;
   Blynk.virtualWrite(V4, correctedVoltage3);
   lcd.setCursor(9, 1);
  lcd.print("A");
  lcd.setCursor(10, 1);
  lcd.print(":");
  lcd.print(correctedVoltage3);
  lcd.print(" ");
  Serial.print("Измеренное напряжение:    3 ");
  Serial.println(correctedVoltage3);
  Serial.print("Analog Value:             3 ");
  Serial.println(voltRead3);
}
