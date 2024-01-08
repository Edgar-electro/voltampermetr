
#include <INA226.h>  // INA Library
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  //(0x27, 16, 2);


const uint32_t SHUNT_MICRO_OHM = 100000;  ///< Сопротивление шунта в микроомах, например 100000 - это 0,1 Ом
const uint16_t MAXIMUM_AMPS    = 10;       ///< Максимальное  измеряемое значение тока, значения 1 А - ограничено до 1022 А
//INA_Class      INA;
INA226_Class INA226;                      ///< INA class instantiation

float  Temp_min = 25 ; // Минимальная температура при которой начнет работать ШИМ вентилятора.
float  Temp_max = 50 ; // Температура при которой скорость вентилятора будет максимальной.
//-------Здесь хранятся все переменные
float  temperature = 0 ;
float  V,A, W , mAh , Wh ;
int   V_graf , A_graf , PWM_out ;
int   PWM = 0 ;
unsigned long  new_Millis ;

#define B 3950 // B-коэффициент
#define SERIAL_R 100000 // сопротивление последовательного резистора, 100 кОм
#define THERMISTOR_R 100000 // номинальное сопротивления термистора, 100 кОм
#define NOMINAL_T 25 // номинальная температура (при которой TR = 100 кОм)
const byte tempPin = A0;//  в новой варсии плати A1

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
  
  INA226.begin(MAXIMUM_AMPS, SHUNT_MICRO_OHM);
  INA226.setBusConversion(8244);             // Время конверсии в микросекундах (140,204,332,588,1100,2116,4156,8244)8244µs=8.244 ms
  INA226.setShuntConversion(8244);           // Время конверсии в микросекундах (140,204,332,588,1100,2116,4156,8244)8244µs=8.244 ms
  INA226.setAveraging(16);                  // Среднее количество чтений n раз (1,4,16,64,128,256,512,1024)
  INA226.setMode(INA_MODE_CONTINUOUS_BOTH);  // Шина / шунт измеряется постоянно
 // INA.alertOnBusOverVoltage(true, 5000);  // Уведомление про спусковой сигнал, если  более 5В

        


  
  new_Millis = millis();

}
void loop() {
  
        updateVoltage();
        
        updateCurrent();
        
        updatepower();
       
        updateEnergy();
  
  //----- Расчет всех динамических данных.------
 V = INA226.getBusMilliVolts()/ 1000.000 ;
 A = INA226.getBusMicroAmps()/ 10000.000 * 21.48;
 //W = INA226.getBusMicroWatts() / 10e5;
 W = V*A;
 if (V<0){V=0;}
   if (A<0){A=0;}
    if (W<0){W=0;}
  mAh += A * (millis() - new_Millis) / 3600000 * 1000; //расчет емкости  в мАч
  new_Millis = millis();
  
  // Определяем температуру на датчике.

  int t = analogRead( tempPin );
  float tr = 1023.0 / t - 1;
  tr = SERIAL_R / tr;
  float temperature;
  temperature = tr / THERMISTOR_R; // (R/Ro)
  temperature = log(temperature); // ln(R/Ro)
  temperature /= B; // 1/B * ln(R/Ro)
  temperature += 1.0 / (NOMINAL_T + 273.15); // + (1/To)
  temperature = 1.0 / temperature; // Invert
  temperature -= 273.15;
  //Рассчитываем  ШИМ вентилятора.
  if (temperature >= Temp_min && temperature <= Temp_max )  {
    PWM = ( temperature - Temp_min ) * 255 / ( Temp_max - Temp_min ); }
  else if (temperature < Temp_min)  { PWM = 0;}
  else if (temperature >= Temp_max)  {PWM = 255;}

  //PWM = map(temperature,Temp_min, Temp_max, 30, 255);


  
 

    
  
  
 
  //Serial.print("Temperature");
   // Serial.println(temperature);
   /* Serial.print   ("  A " );
    Serial.print   ( A  , 4 );
    Serial.print   ("  V " );
    Serial.print   (  V , 3);
    Serial.print   ("  W_ina " );
    Serial.print   ( W  , 4 );
    Serial.print   ("  W=V*A " );
    Serial.print   ( A*V  , 4 );
    Serial.print   ( "   % "  );
    Serial.println ((W/(V*A))*100 );
    */
    Serial.println   ( temperature  );
     Serial.println   (PWM  );
   // Serial.print   ( INA226.getBusMicroAmps() );
  analogWrite(3, PWM);
  delay (10);
}


void updateVoltage() {
  lcd.setCursor(0, 0);
  lcd.print("V");
  lcd.setCursor(1, 0);
  lcd.print(":");
  lcd.print(V);
  lcd.print(" ");
}

 
void updatepower() {
  lcd.setCursor(0, 1);
  lcd.print("P");
  lcd.setCursor(1, 1);
  lcd.print(":");
  lcd.print(W);     
  lcd.print(" ");
  
}
 
void updateCurrent() {
  lcd.setCursor(9, 0);
  lcd.print("I");
  lcd.setCursor(10, 0);
  lcd.print(":");
  lcd.print(A );
  lcd.print(" ");
 
 }

void updateEnergy(){

  lcd.setCursor(9, 1);
  lcd.print("H");
  lcd.setCursor(10, 1);
  lcd.print(":");
  lcd.print(mAh);
  lcd.print(" ");
}
