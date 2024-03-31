
#include <INA226.h>  // INA Library
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x23, 16, 2);  //(0x27, 16, 2);


const uint32_t SHUNT_MICRO_OHM = 100000;  ///< Сопротивление шунта в микроомах, например 100000 - это 0,1 Ом
const uint16_t MAXIMUM_AMPS    = 10;       ///< Максимальное  измеряемое значение тока, значения 1 А - ограничено до 1022 А
//INA_Class      INA;
INA226_Class INA226;                      ///< INA class instantiation
float  temperature = 0 ;
float  Temp_min = 25 ; // Минимальная температура при которой начнет работать ШИМ вентилятора.
float  Temp_max = 50 ; // Температура при которой скорость вентилятора будет максимальной
float  V, A , W , mAh ;
int   PWM = 0 ;

unsigned long  new_Millis ;


const byte tempPin = A0;//  в новой варсии плати A1
const byte  buzzerPin = 2;



void setup() {
  Serial.begin(9600);
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
  TCCR2B = TCCR2B & 0b11111000 | 0x01;     //Включаем частоту ШИМ'а  вентилятора на ногах 3 и 11: 31250 Гц. Это позволит избавиться от неприятного писка в работе вентилятора.

  pinMode( tempPin, INPUT );
  pinMode(buzzerPin, OUTPUT); 
  INA226.begin(MAXIMUM_AMPS, SHUNT_MICRO_OHM);
  INA226.setBusConversion(8244);             // Время конверсии в микросекундах (140,204,332,588,1100,2116,4156,8244)8244µs=8.244 ms
  INA226.setShuntConversion(8244);           // Время конверсии в микросекундах (140,204,332,588,1100,2116,4156,8244)8244µs=8.244 ms
  INA226.setAveraging(16);                  // Среднее количество чтений n раз (1,4,16,64,128,256,512,1024)
  INA226.setMode(INA_MODE_CONTINUOUS_BOTH);  // Шина / шунт измеряется постоянно
 // INA.alertOnBusOverVoltage(true, 5000);  // Уведомление про спусковой сигнал, если  более 5В

        


  
  new_Millis = millis();

}
void loop() {
        
        updatSensor();
        
        updateVoltage();
        
        updateCurrent();
        
        updatepower();
       
        updateEnergy();
  
        updatebuzzer();

  }

 void updatSensor()  {
 //----- Расчет всех динамических данных.------
 V = INA226.getBusMilliVolts()/ 1000.000 ;
 A = INA226.getBusMicroAmps()/ 10000.000 * 21.45;
 //W = INA226.getBusMicroWatts() / 10e5;
 W = V*A;
 if (V<0){V=0;}
   if (A<0){A=0;}
    if (W<0){W=0;}
  mAh += A * (millis() - new_Millis) / 3600000 * 1000; //расчет емкости  в мАч
  new_Millis = millis();
 
 }

void updatebuzzer() {

 if (A > 6.5 && A < 7) {
    blinkLED(4); // Моргаем светодиодом 4 раза
  } else if (A > 7) {
    blinkLED(5); // Моргаем светодиодом 5 раз
  } else {
    digitalWrite(buzzerPin, LOW); // Выключаем светодиод
  }

 
}

void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }

}

void updatshim()  {
 //int temperature = analogRead( tempPin );
//Рассчитываем  ШИМ вентилятора.
  if (temperature >= Temp_min && temperature <= Temp_max )  {
    PWM = ( temperature - Temp_min ) * 255 / ( Temp_max - Temp_min ); }
  else if (temperature < Temp_min)  { PWM = 0;}
  else if (temperature >= Temp_max)  {PWM = 255;}
    //analogWrite(3, PWM);   
  //PWM = map(temperature,Temp_min, Temp_max, 30, 255);
}

void updateVoltage() {
  lcd.setCursor(0, 0);
  lcd.print("V");
  lcd.setCursor(1, 0);
  lcd.print(":");
  lcd.print(V);
  lcd.print("   ");
}

 
void updatepower() {
  lcd.setCursor(0, 1);
  lcd.print("P");
  lcd.setCursor(1, 1);
  lcd.print(":");
  lcd.print(W);     
  lcd.print("   ");
  
}
 
void updateCurrent() {
  lcd.setCursor(9, 0);
  lcd.print("I");
  lcd.setCursor(10, 0);
  lcd.print(":");
  lcd.print(A );
  lcd.print("   ");
 
 }

void updateEnergy(){

  lcd.setCursor(9, 1);
  lcd.print("H");
  lcd.setCursor(10, 1);
  lcd.print(":");
  lcd.print(mAh);
  lcd.print("   ");
}
