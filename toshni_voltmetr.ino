#include <Arduino.h>

const int analogInputPin = A0; // Аналоговый пин для измерения напряжения
float realVoltage = 42.00;     // Реальное напряжение, которое вы измерили (в вольтах)
float measuredVoltage = 41.71; // Измеренное напряжение, которое Arduino показало (в вольтах)
float correctionFactor;        // Коэффициент коррекции
float correctedVoltage;        // Скорректированное напряжение (в вольтах)
int batteryPercentage;         // Процент заряда батареи
long voltage = 0.0;
float VOLTtotal = 0.0;



void setup() {
  // Инициализация сериальной связи
  Serial.begin(115200);
}

void loop() {
  
  for(int i=0;i<200;i++) {
  int sensorValue  = analogRead(analogInputPin);
  voltage += sensorValue ;
  delay(1);
  } 
   voltage = voltage / 200 ;
   VOLTtotal = (voltage / 1023.0) * 42.00;
   correctionFactor = realVoltage / measuredVoltage;
   correctedVoltage = VOLTtotal * correctionFactor;
   batteryPercentage = map(correctedVoltage, 1.26, 42, 0, 100);

  // Выводим результаты
  Serial.print("Измеренное напряжение: ");
  Serial.print(VOLTtotal);
  Serial.print(" Вольта; Скорректированное напряжение: ");
  Serial.print(correctedVoltage);
  Serial.print(" Вольта; Процент заряда: ");
  Serial.print(batteryPercentage);
  Serial.println("%");

  delay(1000); // Задержка 1 секунда перед следующим измерением
}
