#define BLYNK_PRINT Serial

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char auth[] = "baf84adf07e647c6b5e4137d6df4e26a";
char ssid[] = "IOTIK";
char pass[] = "Terminator812";
IPAddress blynk_ip(139, 59, 206, 133);

#define PWMA 5
#define DIRA 4
#define PWMB 12
#define DIRB 13

#define SERVO1_PWM 2    // Вращение
#define SERVO2_PWM 14   // Подъем
#define SERVO3_PWM 16   // Хват
Servo servo_1;
Servo servo_2;
Servo servo_3;

float spd = 0;
float srv1 = 90;
float srv2 = 75;
float srv3 = 90;

void setup()
{
  // Инициализация последовательного порта
  Serial.begin(115200);

  // Инициализация выходов для управления моторами
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // Инициализация Blynk
  Blynk.begin(auth, ssid, pass, blynk_ip, 8442);

  // Инициализация сервомоторов
  servo_1.attach(SERVO1_PWM);
  servo_1.write(srv1);
  servo_2.attach(SERVO2_PWM);
  servo_2.write(srv2);
  servo_3.attach(SERVO3_PWM);
  servo_3.write(srv3);
}

void loop()
{
  Blynk.run();
}

// Мощность мотора "A" от -100% до +100% (от знака зависит направление вращения)
void motorA_setpower(float pwr, bool invert)
{
  // Проверка, инвертирован ли мотор
  if (invert)
  {
    pwr = -pwr;
  }
  // Проверка диапазонов
  if (pwr < -100)
  {
    pwr = -100;
  }
  if (pwr > 100)
  {
    pwr = 100;
  }
  // Установка направления
  if (pwr < 0)
  {
    digitalWrite(DIRA, LOW);
  }
  else
  {
    digitalWrite(DIRA, HIGH);
  }
  // Установка мощности
  int pwmvalue = fabs(pwr) * 10.23;
  analogWrite(PWMA, pwmvalue);
}

// Мощность мотора "B" от -100% до +100% (от знака зависит направление вращения)
void motorB_setpower(float pwr, bool invert)
{
  // Проверка, инвертирован ли мотор
  if (invert)
  {
    pwr = -pwr;
  }
  // Проверка диапазонов
  if (pwr < -100)
  {
    pwr = -100;
  }
  if (pwr > 100)
  {
    pwr = 100;
  }
  // Установка направления
  if (pwr < 0)
  {
    digitalWrite(DIRB, LOW);
  }
  else
  {
    digitalWrite(DIRB, HIGH);
  }
  // Установка мощности
  int pwmvalue = fabs(pwr) * 10.23;
  analogWrite(PWMB, pwmvalue);
}

BLYNK_WRITE(V12)
{
  int ctl =  param.asInt();
  if (ctl)
  {
    Serial.println("Backward");
    motorA_setpower(-spd, false);
    motorB_setpower(-spd, true);
  }
  else
  {
    Serial.println("Stop");
    motorA_setpower(0, false);
    motorB_setpower(0, true);
  }
}

BLYNK_WRITE(V18)
{
  int ctl =  param.asInt();
  if (ctl)
  {
    Serial.println("Forward");
    motorA_setpower(spd, false);
    motorB_setpower(spd, true);
  }
  else
  {
    Serial.println("Stop");
    motorA_setpower(0, false);
    motorB_setpower(0, true);
  }
}

BLYNK_WRITE(V14)
{
  int ctl =  param.asInt();
  if (ctl)
  {
    Serial.println("Left");
    motorA_setpower(spd, false);
    motorB_setpower(-spd, true);
  }
  else
  {
    Serial.println("Stop");
    motorA_setpower(0, false);
    motorB_setpower(0, true);
  }
}

BLYNK_WRITE(V16)
{
  int ctl =  param.asInt();
  if (ctl)
  {
    Serial.println("Right");
    motorA_setpower(-spd, false);
    motorB_setpower(spd, true);
  }
  else
  {
    Serial.println("Stop");
    motorA_setpower(0, false);
    motorB_setpower(0, true);
  }
}

BLYNK_WRITE(V13)
{
  int ctl =  param.asInt();
  if (ctl)
  {
    Serial.println("Right backward");
    motorA_setpower(-spd * 3 / 2, false);
    motorB_setpower(-spd, true);
  }
  else
  {
    Serial.println("Stop");
    motorA_setpower(0, false);
    motorB_setpower(0, true);
  }
}

BLYNK_WRITE(V11)
{
  int ctl =  param.asInt();
  if (ctl)
  {
    Serial.println("Left backward");
    motorA_setpower(-spd, false);
    motorB_setpower(-spd * 3 / 2, true);
  }
  else
  {
    Serial.println("Stop");
    motorA_setpower(0, false);
    motorB_setpower(0, true);
  }
}

BLYNK_WRITE(V17)
{
  int ctl =  param.asInt();
  if (ctl)
  {
    Serial.println("Right forward");
    motorA_setpower(spd * 3 / 2, false);
    motorB_setpower(spd, true);
  }
  else
  {
    Serial.println("Stop");
    motorA_setpower(0, false);
    motorB_setpower(0, true);
  }
}

BLYNK_WRITE(V19)
{
  int ctl =  param.asInt();
  if (ctl)
  {
    Serial.println("Left forward");
    motorA_setpower(spd, false);
    motorB_setpower(spd * 3 / 2, true);
  }
  else
  {
    Serial.println("Stop");
    motorA_setpower(0, false);
    motorB_setpower(0, true);
  }
}

BLYNK_WRITE(V100)
{
  spd =  param.asInt();
  Serial.print("Robot speed: ");
  Serial.println(spd);
}

BLYNK_WRITE(V102)
{
  srv1 =  param.asInt();
  if (srv1 <= 20)
    srv1 = 20;
  if (srv1 >= 160)
    srv1 = 160;
  Serial.print("Gear angle: ");
  Serial.println(srv1);
  servo_1.write(srv1);
}

BLYNK_WRITE(V101)
{
  srv2 =  param.asInt();
  if (srv2 <= 0)
    srv2 = 0;
  if (srv2 >= 90)
    srv2 = 90;
  Serial.print("Gear angle: ");
  Serial.println(srv2);
  servo_2.write(srv2);
}

BLYNK_WRITE(V103)
{
  srv3 =  param.asInt();
  if (srv3 <= 30)
    srv3 = 30;
  if (srv3 >= 110)
    srv3 = 110;
  Serial.print("Gear angle: ");
  Serial.println(srv3);
  servo_3.write(srv3);
}

