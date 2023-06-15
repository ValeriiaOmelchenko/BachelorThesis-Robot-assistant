#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"


#define DHTPIN 2
#define IN1 3
#define IN2 4
#define IN3 5
#define IN4 6
#define DHTTYPE DHT11
#define GAS A0

long nowTime = 0;
int delayTime = 10;
long previousTime = 0;
int delayTimeGas = 2000;
long previousTimeGas = 0;
float humi = 0;
float temp = 0;
float gasValue = 0;
bool alarmState = 0;
char data;
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
Servo rightServo;
Servo leftServo;

byte leftHalf[] = {

  B00011,
  B00101,
  B01010,
  B10110,
  B10111,
  B01011,
  B00101,
  B00011
} ;
byte rightHalf[] = {

  B11000,
  B10100,
  B11010,
  B11101,
  B11101,
  B11010,
  B10100,
  B11000
} ;

byte smile[] = {

  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000
} ;

byte line[] = {
  B00000,
  B10000,
  B01000,
  B00100,
  B00010,
  B00001,
  B00000,
  B00000
} ;

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.createChar(0, leftHalf);
  lcd.createChar(1, rightHalf);
  lcd.createChar(2, smile);
  lcd.createChar(3, line);

  dht.begin();

  rightServo.attach(7);
  leftServo.attach(8);
  rightServo.write(0);
  leftServo.write(0);

  Serial.begin(9600);
  Serial3.begin(9600);



}

void move_forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void move_back() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}
void move_right() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void move_left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void move_stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void emotion_normal() {
  if (alarmState == 0) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.write(0);
    lcd.write(1);
    lcd.setCursor(10, 0);
    lcd.write(0);
    lcd.write(1);
    lcd.setCursor(8, 1);
    lcd.write(2);
  }
}
void emotion_blink() {
  if (alarmState == 0) {
    lcd.clear();
    emotion_normal();
    delay(500);
    lcd.setCursor(5, 0);
    lcd.print("/");
    lcd.write(3);
    lcd.setCursor(10, 0);
    lcd.print("/");
    lcd.write(3);
    lcd.setCursor(8, 1);
    lcd.write(2);
    delay(500);
    emotion_normal();
  }
}

void temp_reader() {
  temp = dht.readTemperature();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.print(temp);
  Serial3.print(0);
  Serial3.print(temp);
  delay(2000);
  
}

void humi_reader() {
  humi = dht.readHumidity();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity:");
  lcd.print(humi);
  Serial3.print(0);
  Serial3.print(humi);
  delay(2000);

}


void ears_move() {
  byte val = random(10, 90);
  rightServo.write(val);
  leftServo.write(val);
}

void gas_alarm() {
  if (nowTime - previousTimeGas > delayTimeGas) {
    previousTimeGas = millis();
    gasValue = analogRead(GAS);
    if (gasValue > 450) {
      alarmState = 1;
      lcd.clear();
      lcd.print("GAS ALARM!!!");
      Serial3.print('D');
    }
    else {
      lcd.clear();
      alarmState = 0;
    }
  }

}


void loop()
{

  if (Serial3.available()) {
    data = Serial3.read();
    if (data == 'F') {
    move_forward();
  } else if (data == 'B') {
    move_back();
  } else if (data == 'L') {
    move_left();
  } else if (data == 'R') {
    move_right();
  } else if (data == 'T') {
    temp_reader();
  } else if (data == 'H') {
    humi_reader();
  }else if (data == 'S') {
    move_stop();
  }
  }
  
  
  gas_alarm();
  Serial.print(data);
  Serial.print("\t");
  Serial.println(gasValue);
  ears_move();
  delay(400);
  emotion_normal();
  nowTime = millis();
  if (nowTime - previousTime > delayTime) {
    previousTime = millis();
    delayTime = random(1000, 3000);
    emotion_blink();
  }
}
