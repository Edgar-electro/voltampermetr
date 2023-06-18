//input voltage via 10 kom voltage pin connect potencometr  2 .. potencometr 1 pin gnd...potencometr 3 pin connect 10 kom  dis is highi voltage input 30 volt
//  board lgt8f328p



 #include <ACS712.h>
 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>


 LiquidCrystal_I2C lcd(0x23, 16, 2);


 
  const int fancurrdedect = 2;
  const int fandelayoff = 120;
  const unsigned long fanOffDelay  = fandelayoff * 1000;
  
  bool fanOn = false; 
  unsigned long fanOffTime = 0;


  const int  fanPin       = 4;
  const int  relayPin     = 2;
  const int  buttonPin    = 3;
  const int  voltage_pin  = A1 ;
  const int  current_pin  = A0 ;
  const int  adc_bit      = 12 ;
  
  
  ACS712 sensor(ACS712_20A, current_pin);

  int   voltRead    = 0;
  int   ampRead     = 0;
  long  current   = 0.0;         
  long  voltage   = 0.0;
  float watt      = 0.0;
  float VOLTtotal = 0.0;
  float AMPtotal  = 0.0;
  float ampHours  = 0.0;
  float voltcorect = 1.989;
  float  ampcorrect = 107.0;
  
  unsigned long previousMillis = 0;
  unsigned long interval = 3600; 
  
  
   bool relayActive = false;
   bool buttonState = false;
   bool prevButtonState = true;


 

void setup() {
  Serial.begin (115200);
  
  
  analogReference(INTERNAL4V096); 
  analogReadResolution (adc_bit);
  
  pinMode(voltage_pin, INPUT);
  pinMode(current_pin, INPUT);     
  
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW); 
  delay(200);
  
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
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
   VOLTtotal = voltage *(4.096 / 1023);
   VOLTtotal = VOLTtotal * voltcorect ;
}
  
  
  void amp_update(){
  for(int i=0;i<200;i++) {
  ampRead = sensor.getCurrentDC()*10;
  current += ampRead ;
  delay(1);
  } 
   
   current = current / 200 ;
   AMPtotal = current *(4.096 / 1023);
   AMPtotal = AMPtotal * ampcorrect ;
  
 }
   
    void watt_update(){
    for (int i = 0; i < 10; i++) {
    watt = AMPtotal * VOLTtotal ;
    delay(1);
  } 
}

void AH_update(){
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    float currentHours = AMPtotal / 3600.0;
    ampHours += currentHours;
    previousMillis = currentMillis;
  }
}

void updatebutton() {
buttonState = digitalRead(buttonPin);

  
  if (buttonState == LOW && prevButtonState == HIGH && relayActive) {
    digitalWrite(relayPin, LOW);
    relayActive = false;
  }
delay(10);
  
  prevButtonState = buttonState;
if (VOLTtotal > 5.0 && !relayActive) {
    digitalWrite(relayPin, HIGH);
    relayActive = true;
  }

  delay(100);


 }

void fancontrol() {
  
  if (VOLTtotal > fancurrdedect) {
    digitalWrite(fanPin, HIGH); 
    fanOn = true;
    fanOffTime = millis(); 
  } else {
    if (fanOn && (millis() - fanOffTime >= fanOffDelay)) {
      digitalWrite(fanPin, LOW); 
      fanOn = false;
    }
  }
  
}



void updateserial(){

   Serial.print("VOLTtotal   ");
   Serial.println(VOLTtotal);
   delay(1000);

   Serial.print("AMPtotal   ");
   Serial.println(AMPtotal);
   delay(1000);
  
   Serial.print("POWE       ");
   Serial.println(watt);
   delay(1000);

   Serial.print("AmpHours   ");
   Serial.println(ampHours);
   delay(1000);

   if (relayActive){
   Serial.println("Rely:ON");
   }else{ Serial.println("Rely:OFF");
   }
   
   delay(1000);

 if (fanOn){
   Serial.println("Fan:ON");
   }else{ Serial.println("Fan:OFF");
   }
   delay(0);
  
  
 

}



void updateVoltage() {
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(VOLTtotal);
  lcd.print("     ");
}

 
void updateCurrent() {
  lcd.setCursor(0, 1);
  lcd.print("I:");
  lcd.print(AMPtotal);
  lcd.print("     ");
  if (relayActive) {
  lcd.setCursor(0, 1); 
  lcd.print("Rely:ON "); 
  }
}
 
void updatepower() {
  lcd.setCursor(9, 0);
  lcd.print("P:");
  lcd.print(watt);
  lcd.print("     ");
}

void updateEnergy(){

  lcd.setCursor(9, 1);
  lcd.print("H:");
  lcd.print(ampHours);
  lcd.print("     ");
}






void loop() {
        delay(500);
        volt_update();
        delay(10);
        amp_update();
        delay(10);
        watt_update();
        delay(10);
        AH_update();
        delay(10);
        updateVoltage();
        
        updateCurrent();
        
        updatepower();
        
        updateEnergy();
        
        updatebutton();
        delay(10);
        updateserial();
        delay(10);
        fancontrol();
        
        delay(0);
   
  
}
