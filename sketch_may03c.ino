//input voltage via 10 kom.arduino read pin  connect potencometr  2 .. potencometr 1 pin gnd...potencometr 3 pin connect 10 kom  dis is highi voltage input 30 volt
//  board arduino nano 

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
  float realVoltage = 31.92;     // Реальное напряжение, которое вы измерили (в вольтах)  stex poxem maxsimal volty testerov chapvac jisht volti masina xosqy
  float measuredVoltage = 32.09; // Измеренное напряжение, которое Arduino показало (в вольтах)
  float Maxviltage = 32.00;
  float correctionFactor;        // Коэффициент коррекции
  float correctedVoltage;        // Скорректированное напряжение (в вольтах)
        
  float  ampcorrect = 107.0;
  float ampHours = 0.0;
unsigned long previousMillis = 0;
unsigned long interval = 3600; 
  
   
   bool relayActive = false;
   bool buttonState = false;
   bool prevButtonState = true;


 
void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT); 
  
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
  for(int i=0;i<200;i++) {
  voltRead = analogRead(voltage_pin);
  voltage += voltRead ;
  delay(1);
  } 
  
  voltage = voltage / 200 ;
   VOLTtotal = (voltage / 1023.0) * Maxviltage;  //stex poxem maxsimal volty testerov chapvac jisht volti masina xosqy
   correctionFactor = realVoltage / measuredVoltage;
   correctedVoltage = VOLTtotal * correctionFactor;
   
}
  void amp_update(){
  for(int i=0;i<200;i++) {
  ampRead = sensor.getCurrentDC()*10;
  current += ampRead ;
  delay(1);
  } 
   
   current = current / 200 ;
   AMPtotal = current *(1.0 / 1023);
   AMPtotal = AMPtotal * ampcorrect ;
  
 }
   
    void watt_update(){
    for (int i = 0; i < 10; i++) {
    watt = AMPtotal * correctedVoltage ;
    
  } 
}

void AH_update(){
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    float currentHours = AMPtotal / 3600.0;
    ampHours += currentHours * 1000.0;
    previousMillis = currentMillis;
  }
}

void updatebutton() {
buttonState = digitalRead(buttonPin);

  
  if (buttonState == LOW && prevButtonState == HIGH && relayActive) {
    digitalWrite(relayPin, LOW);
    relayActive = false;
  }
delay(1);
  
  prevButtonState = buttonState;
if (AMPtotal > 6.8 && !relayActive) {
    digitalWrite(relayPin, HIGH);
    relayActive = true;
  }

 
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
  lcd.print("V");
  lcd.setCursor(1, 0);
  lcd.print(":");
  lcd.print(correctedVoltage);
  lcd.print(" ");
}

 
void updatepower() {
  lcd.setCursor(0, 1);
  lcd.print("P");
  lcd.setCursor(1, 1);
  lcd.print(":");
  lcd.print(watt );     
  lcd.print(" ");
  
}
 
void updateCurrent() {
  lcd.setCursor(9, 0);
  lcd.print("I");
  lcd.setCursor(10, 0);
  lcd.print(":");
  lcd.print(AMPtotal );
  lcd.print(" ");
  if (relayActive) {
  lcd.setCursor(0, 1); 
  lcd.print("Rely:ON"); 
  }
 }

void updateEnergy(){

  lcd.setCursor(9, 1);
  lcd.print("H");
  lcd.setCursor(10, 1);
  lcd.print(":");
  lcd.print(ampHours );
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
    Serial.print(" Измеренное напряжение: ");
  Serial.print(VOLTtotal);
  Serial.print(" Скорректированное напряжение: ");
  Serial.print(correctedVoltage);
  Serial.print(" analog read: ");
  Serial.print(voltRead);
  Serial.println("%");
  
}
