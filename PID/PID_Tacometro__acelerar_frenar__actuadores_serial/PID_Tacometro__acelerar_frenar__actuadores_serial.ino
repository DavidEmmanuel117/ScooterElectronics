#include <TFT_ILI9163C.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <Servo.h>

// Serial comm action definitions dictionary
#define _ACTION_LIBRE 0
#define _ACTION_FRENA 1
#define _ACTION_5KMH 2
#define _ACTION_10KMH 3
#define _ACTION_15KMH 4
#define _ACCION_PRECAUCION 5

// Receives input action from serial monitor
int serial_input_action = 0;

#define SAMPLE_TIME 50
unsigned long lastMillis = millis();

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

/*
Teensy3.x and Arduino's
You are using 4 wire SPI here, so:
 MOSI:  11//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 MISO:  12//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 SCK:   13//Teensy3.x/Arduino UNO (for MEGA/DUE refere to arduino site)
 the rest of pin below:
 */
#define __CS 10
#define __DC 9
#define __RST 12
TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC, __RST);
/*
Teensy 3.x can use: 2,6,9,10,15,20,21,22,23
Arduino's 8 bit: any
DUE: check arduino site
If you do not use reset, tie it to +3V3
*/

////////Variables para medir velocidad////////////
volatile float rpmcount = 0;
float rpm = 0;
float radial_vel = 0;
float linear_vel = 0;
unsigned long lastmillis = 0;
float linear_vel_km = 0;
////////////////////////////////////////////////

////////////Variables para controlar velocidad//////////
int pwm_pin = 6;
int pot_pin = A0;
int output;
int pwm_value;
///////////////////////////////////////////////////////

int pin_botonluz = 4;
int pin_servo = 3;
int pin_luzd = 7;
int pin_bocinas = 8;
int pin_luzt = 12;
boolean bandera_luz = false;
boolean bandera_emer = false;
String accion = "nada";
Servo servoMotor;

//////////Control PID//////////
double pv_speed = 0;
double set_speed = 0;
double e_speed = 0;     // error of speed = set_speed - pv_speed
double e_speed_pre = 0; // last error of speed
double e_speed_sum = 0; // sum error of speed
double pwm_pulse = 0;   // this value is 0~255
double kp = 0.0094186;
double ki = 0.05;
////////////////////////////////

void setup()
{
  pinMode(pwm_pin, OUTPUT);
  pinMode(5, INPUT);
  pinMode(pin_botonluz, INPUT);
  servoMotor.attach(pin_servo);
  servoMotor.write(0);
  pinMode(pin_luzd, OUTPUT);
  pinMode(pin_bocinas, OUTPUT);
  pinMode(pin_luzt, OUTPUT);
  tft.begin();
  //   attachInterrupt(0, rpm_bike, FALLING); //interrupt zero (0) is on pin two(2).
  Wire.begin(); /* join i2c bus as master */
  Serial.begin(9600);
}

void loop()
{
  linear_vel_km = readSpeed();

  // Reads an int from serial port
  serial_input_action = SerialEvent();
  // Map serial port input into an state action
  accion = getAction(serial_input_action);

  if (accion == "10KPH")
  {
    testText();
    set_speed = 80;
    pv_speed = (((linear_vel_km / 3.6) / 0.33) * 30) / M_PI;
    e_speed = set_speed - pv_speed;
    pwm_pulse = e_speed * kp + e_speed_sum * ki;
    e_speed_sum += e_speed; // sum of error
    if (e_speed_sum > 4000)
      e_speed_sum = 4000;
    if (e_speed_sum < -4000)
      e_speed_sum = -4000;
    if (pwm_pulse<255 & pwm_pulse> 0)
    {
      analogWrite(pwm_pin, pwm_pulse); // set motor speed
    }
    else
    {
      if (pwm_pulse > 255)
      {
        analogWrite(pwm_pin, 255);
      }
      else
      {
        analogWrite(pwm_pin, 0);
      }
    }
  }

  if (accion == "15KPH")
  {
    testText();
    set_speed = 120;
    pv_speed = (((linear_vel_km / 3.6) / 0.33) * 30) / M_PI;
    e_speed = set_speed - pv_speed;
    pwm_pulse = e_speed * kp + e_speed_sum * ki;
    e_speed_sum += e_speed; // sum of error
    if (e_speed_sum > 4000)
      e_speed_sum = 4000;
    if (e_speed_sum < -4000)
      e_speed_sum = -4000;
    if (pwm_pulse<255 & pwm_pulse> 0)
    {
      analogWrite(pwm_pin, pwm_pulse); // set motor speed
    }
    else
    {
      if (pwm_pulse > 255)
      {
        analogWrite(pwm_pin, 255);
      }
      else
      {
        analogWrite(pwm_pin, 0);
      }
    }
  }

  if (accion == "5KPH")
  {
    testText();
    set_speed = 50;
    pv_speed = (((linear_vel_km / 3.6) / 0.33) * 30) / M_PI;
    e_speed = set_speed - pv_speed;
    pwm_pulse = e_speed * kp + e_speed_sum * ki;
    e_speed_sum += e_speed; // sum of error
    if (e_speed_sum > 4000)
      e_speed_sum = 4000;
    if (e_speed_sum < -4000)
      e_speed_sum = -4000;
    if (pwm_pulse<255 & pwm_pulse> 0)
    {
      analogWrite(pwm_pin, pwm_pulse); // set motor speed
    }
    else
    {
      if (pwm_pulse > 255)
      {
        analogWrite(pwm_pin, 255);
      }
      else
      {
        analogWrite(pwm_pin, 0);
      }
    }
  }

  if (accion == "frena")
  {
    servoMotor.write(25);
    digitalWrite(pin_luzd, HIGH);
    digitalWrite(pin_bocinas, HIGH);
    digitalWrite(pin_luzt, HIGH);
    analogWrite(pwm_pin, 0);
    accion == "espera";
  }
  if (accion == "libre")
  {
    servoMotor.write(0);
    digitalWrite(pin_luzd, LOW);
    digitalWrite(pin_bocinas, LOW);
    digitalWrite(pin_luzt, LOW);
    accion = "nada";
  }

  if (accion == "nada")
  {
    testText();
    ///////////Medir velocidad y mostarla/////////////////////

    /*if (millis() - lastmillis >= 1000)
   {
    ///////Recibir velocidad del esclavo/////////////

  ///////////////////////////////////////////////////////////////
   rpm = rpmcount * 1;  //Convert frecuency to RPM, note: this works for one interruption per full rotation. For two interrups per full rotation use rpmcount * 30.
   radial_vel = rpm * M_PI / 30; //convert rpm to radial velocity in rad/s.
   linear_vel = radial_vel * 0.33; //convert radial velocity to linear velocity in m/s.
   linear_vel_km = 3.6 * linear_vel;
   Serial.print("RPM = "); //print the word "RPM".
   Serial.print(rpm); // print the rpm value.
   Serial.print("\t\t Linear Speed = "); //print the word "Linear Velocity".
   Serial.print(linear_vel_km); //print the linear velocity value.
   Serial.println(" k/h"); //print the word "m/s".

   Wire.beginTransmission(8);
   Wire.write(0);
   Wire.endTransmission();

   rpmcount = 0; // Restart the RPM counter
   lastmillis = millis(); // Update lasmillis
   testText();
   }*/

    ////////////////////////////////////////////////////

    /////////Acelerar y frenar////////////
    if (digitalRead(5) == HIGH)
    {
      analogWrite(pwm_pin, 0);
      digitalWrite(pin_luzt, HIGH);
    }
    if (digitalRead(5) == LOW)
    {
      // Reading from potentiometer
      output = analogRead(pot_pin);
      // Mapping the Values between 0 to 255 because we can give output
      // from 0 -255 using the analogwrite funtion
      pwm_value = map(output, 0, 1023, 0, 255);
      analogWrite(pwm_pin, pwm_value);
      digitalWrite(pin_luzt, LOW);
      delay(1);
    }
    ////////////Luz delantera/////////////
    if (digitalRead(pin_botonluz) == HIGH)
    {
      bandera_luz = !bandera_luz;
      delay(500);
      if (bandera_luz == true)
      {
        digitalWrite(pin_luzd, HIGH);
      }
      if (bandera_luz == false)
      {
        digitalWrite(pin_luzd, LOW);
      }
    }
    ////////////////////////////////////
  }

  // Print speed for SAC communication
  Serial.println(linear_vel_km);
}

float readSpeed()
{
  float speed;
  Wire.requestFrom(8, 1);
  while (Wire.available())
  {
    speed = Wire.read();
  }
  return speed;
}

unsigned long testText()
{
  tft.fillScreen();
  tft.setCursor(0, 0);
  tft.setTextColor(BLUE);
  tft.setTextSize(4);
  tft.println(linear_vel_km);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println("KM/H");
}

// Reads from serial port, return an int
int SerialEvent()
{
  while (Serial.available())
  {
    // readString() reads until "\r\n" and the parses to int
    int x = Serial.readString().toInt();

    // Map serial port input into an state action
    accion = getAction(x);
    return x;
  }
  return 0;
}

// Maps an interger into a state Action
String getAction(int x)
{
  if (x == _ACTION_LIBRE)
    return ("libre");
  else if (x == _ACTION_FRENA)
    return ("frena");
  else if (x == _ACTION_5KMH)
    return ("5KPH");
  else if (x == _ACTION_10KMH)
    return ("10KPH");
  else if (x == _ACTION_15KMH)
    return ("15KPH");
  else if (x == _ACCION_PRECAUCION)
    return ("15KPH");
  else
    return ("nada");
}