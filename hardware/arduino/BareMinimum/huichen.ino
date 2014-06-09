#include <LiquidCrystal.h>
int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
float pm25value = 0; 
float pm25coef = 0.00207916725464941;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  pinMode(8,INPUT);
  starttime = millis();
  lcd.begin(16,2);
}

void loop() {
    
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print(lowpulseoccupancy);
    Serial.print(",");
    Serial.print(ratio);
    Serial.print(",");
    Serial.println(concentration);
    pm25value = pm25coef * concentration * 10; // 10 to transform 0.01 cf to 0.1 ft
    lcd.setCursor(0,0);
    lcd.print("PM25 value:");
    lcd.println(pm25value);
   delay(5000);
    lcd.setCursor(0,1);
   if(pm25value<75)
      lcd.print("excellent");
  else if(pm25value<150)
      lcd.print("very good");
  else if(pm25value<300)
      lcd.print("good");
  else if(pm25value<1050)
      lcd.print("fair");
  else if(pm25value<3000)
      lcd.print("poor");
  else if(pm25value>3000)
      lcd.print("very poor");
       
        delay(15000);
     lcd.clear();
    lowpulseoccupancy = 0;
    starttime = millis();
  }
}

