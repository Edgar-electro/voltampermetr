


 #include <ACS712.h>
 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>


  LiquidCrystal_I2C lcd(0x23, 16, 2);
  
  const int  relayPin = 2;
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
  float voltcorect = 6.470;
  float  ampcorrect = 107.0;
  float ampHours = 0.0;
unsigned long previousMillis = 0;
unsigned long interval = 3600; 
  
   
   bool relayActive = false;
   bool buttonState = false;
   bool prevButtonState = true;


 void volt_update() {
  for(int i=0;i<200;i++) {
  voltRead = analogRead(voltage_pin);
  voltage += voltRead ;
  delay(1);
  } 
  
   voltage = voltage / 200 ;
   VOLTtotal = voltage *(4.90 / 1023);
   VOLTtotal = VOLTtotal * voltcorect ;
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
if (AMPtotal > 5 && !relayActive) {
    digitalWrite(relayPin, HIGH);
    relayActive = true;
  }
  
  if (buttonState == LOW && prevButtonState == HIGH) {
    digitalWrite(relayPin, LOW);
    relayActive = false;
  }
  
  buttonState = digitalRead(buttonPin);
  prevButtonState = buttonState;


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



void setup() {
  
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(800);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-calibration-");
  lcd.setCursor(2, 1);
  lcd.print("sensors...");
  delay(1200);
  lcd.clear();
  analogReference(DEFAULT); 
  pinMode(voltage_pin, INPUT);
  pinMode(current_pin, INPUT);     
  int zero = sensor.calibrate();
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(relayPin, LOW);  
    
    
    
}



void loop() {
        
        volt_update();
        amp_update();
        watt_update();
        AH_update();
        updateVoltage();
        updateCurrent();
        updatepower();
        updateEnergy();
        updatebutton();
  
  delay(500);  
  
}
