// Each light is 0.0545 volts 

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>

#include <LiquidCrystal.h>

// Variable definition 
int analogPin = 1; 
int returnedVal; 
double Realvoltage; 
double addedVals; 
double aveVolts; 
unsigned long timeRan = millis();
unsigned long lastCheck = 0; 
unsigned long iterations = 0; 
int mseconds = 500; 
unsigned long lastCheckDisplay = 0; 
int secondsDisplay = 1; 
float numLights; 
int roundedLights; 
double averageVoltage; 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8,8,8,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + 
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, 
  NEO_GRB + NEO_KHZ800); 

// Setup function that tells the arduino how often to collect data 
void setup() {
  Serial.begin(9600); 
  lcd.begin(16,2); 
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
}

// Loop function that runs over and over again 
void loop() { 
  checkVolts(); 
}

// Function that gets voltage based on the pin number and averages the voltages of the past 10 seconds 
void checkVolts(){
  if((timeRan-lastCheck)<mseconds){
    getVolts(analogPin); 
  } else {
    calculateVolts(); 
    resetVars(); 
  }
  displayValue(); 
}

// Function that resets all the counting variables to zero after the whole process is completed once 
void resetVars(){
  returnedVal = 0; 
  Realvoltage = 0; 
  addedVals = 0; 
  aveVolts = 0; 
  iterations = 0; 
}

// Function that gets the voltage from the analog pin and incriments important variables 
void getVolts(int pinNumber){
  returnedVal = analogRead(pinNumber); 
  addedVals = addedVals + returnedVal; 
  iterations = iterations + 1; 
  timeRan = millis(); 
}

// Function that calculates voltage after time requirements are met 
void calculateVolts(){
     lastCheck = timeRan;  
     aveVolts = addedVals/iterations; 
     averageVoltage = aveVolts; 
}

// Function that prints current voltage to LCD screen 
void displayValue(){
  lcd.setCursor(0,0); 
  if((timeRan-lastCheckDisplay)/1000==secondsDisplay){
    printToLCD(); 
    showLights(); 
    lastCheckDisplay = timeRan; 
  }
}

void printToLCD(){
    lcd.print("Voltage: "); 
    double printableVolts = (5*averageVoltage)/1024;
    lcd.print(printableVolts); 
    lcd.print(" V");
}

void showLights(){
  matrix.fillScreen(matrix.Color(0,0,0)); 
  numLights = (64*averageVoltage)/725; 
  roundedLights = (int) numLights; 
  int rows = 0; 
  for(int i=0; i<8; i++){
    if((roundedLights-8)>0){
      roundedLights = roundedLights-8; 
      rows = rows + 1; 
    }
  }
  int r = random(50,225); 
  int g = random(50,225); 
  int b = random(50,225); 
  for(int y=0; y<rows; y++){
    for(int x=0; x<8; x++){
      matrix.drawPixel(x,y,matrix.Color(r,g,b)); 
      Serial.print(x); 
      Serial.print(y); 
      Serial.print(", "); 
    }
  }
  Serial.println(""); 
  for(int x=0; x<roundedLights; x++){
    matrix.drawPixel(x,rows,matrix.Color(r,g,b)); 
    Serial.print(x); 
    Serial.print(rows); 
    Serial.print(", "); 
  }
  matrix.show(); 
}

