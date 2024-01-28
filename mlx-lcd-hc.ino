//load libraries
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//Define variables 

#define I2C_ADDR          0x27        //Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN      3
#define En_pin             2
#define Rw_pin             1
#define Rs_pin             0
#define D4_pin             4
#define D5_pin             5
#define D6_pin             6
#define D7_pin             7
#define BIP              2794
//Initialise the LCD
LiquidCrystal_I2C      lcd(I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

//Initialise the MLX
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//Variables for the HC-SR04
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
float duration, cm;
//double bias;
//LED Pins
int redPin = 8;
int greenPin = 10;


int compt=1;
float rap=0;

void setup()
 {
    Serial.begin(9600);  
    //Define inputs and outputs
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(redPin,OUTPUT);
    pinMode(greenPin,OUTPUT);
    
    //Define the LCD as 16 column by 2 rows 
    lcd.begin (16,2);
    
    //Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
   
    mlx.begin(); 
}

void red(int n){
  for(int i = 0; i<n; i++){
     digitalWrite(redPin,HIGH);
     delay(200);
     tone(13, BIP,200);
     digitalWrite(redPin,LOW);
     delay(200);
  }
}

void green(int n){
  for (int i = 0; i < n; i++)
  {
    /* code */
    digitalWrite(greenPin,HIGH);
    tone(13, BIP, 350);
    delay(350);
    digitalWrite(greenPin,LOW);
    delay(200);
  }
}

void showTemp(float temp){
  lcd.setCursor(0,0);
  lcd.print("Temperature");
  //goto first column (column 0) and second line (line 1)
  lcd.setCursor(0,1);
  lcd.print(temp);
  lcd.setCursor(6,1);
  lcd.print("*C");
}

void tryAgainMessage(){
    lcd.setCursor(0,0);
    lcd.print("Veuillez");
    lcd.setCursor(0,1);
    lcd.print("Reessayer");
}

void loop()
{
  
  float otemp, btemp, bias;
  //goto first column (column 0) and first line (Line 0)
  lcd.setCursor(0,0);
  lcd.print(" Veuillez Vous ");
  //goto first column (column 0) and second line (line 1)
  lcd.setCursor(0,1);
  lcd.print("   Rapprocher   ");

  
  digitalWrite(trigPin, LOW);
  //delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  otemp = mlx.readObjectTempC();
  // Convert the time into a distance
  cm = duration * 0.034 / 2;     // Divide by 29.1 or multiply by 0.0343
  Serial.print(otemp);Serial.print("  ");Serial.println(cm);
  if(cm>11&&cm<11){
    lcd.clear();
    bias = 8;
        
    btemp = otemp + bias;
    
    if(btemp<36){
        red(1);
        tryAgainMessage();
    }
    
    else if(btemp>38){
      showTemp(btemp);
      red(3);
    }
    else{
      showTemp(btemp);
      green(1);
    }
    Serial.print(btemp); Serial.println("*C");
    delay(1000);
    lcd.clear();
  }

}
 
