//input voltage via 10 kom.arduino analog pin  connect potencometr  2 .. potencometr 1 pin gnd...potencometr 3 pin connect 10 kom  dis is highi voltage input 30 volt
//  board arduino nano 
//arduino analog referanc pin connct to 3.3 volt arduino ..analog input voltage   maximum 3,28 volt
 // karektirovka anel ..testerov chaper real voltag ev grel realVoltage dimac...serialov naem VOLTtotal ev grem measuredVoltage Измеренное напряжение dimac
 #include <ACS712.h>
 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>


  LiquidCrystal_I2C lcd(0x23, 16, 2);  //(0x27, 16, 2);
  const int  buzzerPin = 2;
  const int  relayPin = 4;
  const int  buttonPin = 3;
  const int  voltage_pin  = A1 ;
  const int  current_pin =  A0 ;
  
  ACS712 sensor(ACS712_20A, current_pin);

  int  voltRead = 0;
  int  ampRead = 0;
  long current = 0.0;         
  long voltage = 0.0;
  float watt  = 0.0;
  float VOLTtotal = 0.0;
  float AMPtotal = 0.0;
  float realVoltage = 20.03;     // 31.84Реальное напряжение, которое вы измерили (в вольтах)  stex poxem maxsimal volty testerov chapvac jisht volti masina xosqy
  float measuredVoltage = 20.27; // 90  33.00 Измеренное напряжение, которое Arduino показало (в вольтах)
  float Maxviltage = 32.00;
  float correctionFactor;        // Коэффициент коррекции
  float correctedVoltage ;        // Скорректированное напряжение (в вольтах)
      
  float  ampcorrect = 67.00;
  float ampHours = 0.0;
unsigned long previousMillis = 0;
unsigned long interval = 3600; 
  
   
   bool relayActive = false;
   bool buttonState = false;
   bool prevButtonState = true;


 
void setup() {
  Serial.begin(115200);
  analogReference(EXTERNAL);  //  aref connect to 3.3 volt
  
  pinMode(voltage_pin, INPUT);
  pinMode(current_pin, INPUT);     
  pinMode(buzzerPin, OUTPUT); 
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  digitalWrite(relayPin, LOW);  
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("-calibration-");
  lcd.setCursor(2, 1);
  lcd.print("sensors...");
  int zero = sensor.calibrate();
  delay(1500);
  lcd.clear();
  
    
    
    
}

void volt_update() {
  
    for (int i = 0; i < 150; i++) {
      voltRead = analogRead(voltage_pin);
      voltage += voltRead;
      // Здесь можно выполнять другие операции, если необходимо
    }
 
   voltage = voltage / 150 ;
   VOLTtotal = (voltage / 1025.0) * Maxviltage;  //stex poxem maxsimal volty testerov chapvac jisht volti masina xosqy
   correctionFactor = realVoltage / measuredVoltage;
   correctedVoltage = VOLTtotal * correctionFactor;
   
  
  
}

  

  void amp_update() {
  

    for (int i = 0; i < 50; i++) {
      ampRead = sensor.getCurrentDC() * 10;
      current += ampRead;

    }
  
   current = current / 50 ;
   AMPtotal = current *(1.0 / 1025);
   AMPtotal = AMPtotal * ampcorrect ;
  
  
  
}
   
   

   
    void watt_update(){
   
   watt = AMPtotal * correctedVoltage ;
   
   
 }

void AH_update(){
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    float currentHours = AMPtotal / 3600.0;
    ampHours += currentHours * 1000.0;
    previousMillis = currentMillis;
  }
}

unsigned long lastButtonCheckTime = 0;  // Переменная для отслеживания времени последней проверки кнопки
unsigned long debounceDelay = 50;  // Время антидребезга (в миллисекундах)

void updatebutton() {
  unsigned long currentTime = millis();

  if (currentTime - lastButtonCheckTime >= debounceDelay) {
    buttonState = digitalRead(buttonPin);

    if (buttonState == LOW && prevButtonState == HIGH && relayActive) {
      digitalWrite(relayPin, LOW);
      relayActive = false;
    }

    prevButtonState = buttonState;

    if (AMPtotal > 6.8 && !relayActive) {
      digitalWrite(relayPin, HIGH);
      relayActive = true;
    }

    lastButtonCheckTime = currentTime;  // Обновляем время последней проверки кнопки
  }
 //Serial.println(relayActive);
}



void updatebuzzer() {

 if (AMPtotal > 6.5 && AMPtotal < 7) {
    blinkLED(4); // Моргаем светодиодом 4 раза
  } else if (AMPtotal > 7) {
    blinkLED(5); // Моргаем светодиодом 5 раз
  } else {
    digitalWrite(buzzerPin, LOW); // Выключаем светодиод
  }

 // delay(1000); // Делаем паузу перед следующей проверкой
}


void blinkLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}
void updateVoltage() {
  lcd.setCursor(0, 0);
  lcd.print("V:");

  // Извлекаем отдельные цифры из correctedVoltage
  int integerPart = int(correctedVoltage);           // Целая часть
  int decimalPart = int((correctedVoltage - integerPart) * 100);  // Десятичная часть, умноженная на 100

  // Выводим отдельные цифры на LCD
  lcd.setCursor(2, 0);  // Сдвигаем курсор, чтобы начать выводить после "V:"
  lcd.print(integerPart);
  lcd.print(".");
  if (decimalPart < 10) {
    lcd.print("0"); // Если десятичная часть меньше 10, добавляем ведущий ноль
  }
  lcd.print(decimalPart);
  lcd.print(" ");
}

 
void updatepower() {
 

lcd.setCursor(0, 1);
  lcd.print("P:");

  // Извлекаем отдельные цифры из correctedVoltage
  int integerPart = int(watt);           // Целая часть
  int decimalPart = int((watt - integerPart) * 100);  // Десятичная часть, умноженная на 100

  // Выводим отдельные цифры на LCD
  lcd.setCursor(2, 1);  // Сдвигаем курсор, чтобы начать выводить после "V:"
  lcd.print(integerPart);
  lcd.print(".");
  if (decimalPart < 10) {
    lcd.print("0"); // Если десятичная часть меньше 10, добавляем ведущий ноль
  }
  lcd.print(decimalPart);
  lcd.print(" ");
   if (relayActive) {
    lcd.setCursor(0, 1);
    lcd.print("Relay:ON");
  } 


}
 
void updateCurrent() {
  lcd.setCursor(9, 0);
  lcd.print("I:");

  // Извлекаем отдельные цифры из AMPtotal
  int integerPart = int(AMPtotal);           // Целая часть
  int decimalPart = int((AMPtotal - integerPart) * 100);  // Десятичная часть, умноженная на 100

  // Выводим отдельные цифры на LCD
  lcd.setCursor(11, 0);  // Сдвигаем курсор, чтобы начать выводить после "I:"
  lcd.print(integerPart);
  lcd.print(".");
  if (decimalPart < 10) {
    lcd.print("0"); // Если десятичная часть меньше 10, добавляем ведущий ноль
  }
  lcd.print(decimalPart);
  lcd.print(" ");

  
}


void updateEnergy(){


lcd.setCursor(9, 1);
  lcd.print("H:");

  // Извлекаем отдельные цифры из AMPtotal
  int integerPart = int(ampHours);           // Целая часть
  int decimalPart = int((ampHours - integerPart) * 100);  // Десятичная часть, умноженная на 100

  // Выводим отдельные цифры на LCD
  lcd.setCursor(11, 1);  // Сдвигаем курсор, чтобы начать выводить после "I:"
  lcd.print(integerPart);
  lcd.print(".");
  if (decimalPart < 10) {
    lcd.print("0"); // Если десятичная часть меньше 10, добавляем ведущий ноль
  }
  lcd.print(decimalPart);
  lcd.print(" ");



}




void loop() {        
        
        volt_update();
        updateVoltage();
        amp_update();
        updateCurrent();
        watt_update();
        updatepower();
        AH_update();
        updateEnergy();
        updatebutton();
        updatebuzzer();
 // Serial.print(" Измеренное напряжение: ");
   Serial.println(VOLTtotal);
 // Serial.print(" Скорректированное напряжение: ");
  Serial.println(correctedVoltage);
  //Serial.print(" analog read: ");
  //Serial.println(voltRead); 
 //Serial.print(" mijin read: ");
   //Serial.println(voltage); 
  
}
