#include <Servo.h>

#define ESC_PIN 9 // Arduino uno PWM pin
#define MIN_PULSE_WIDTH 1000
#define MAX_PULSE_WIDTH 2000

#define SERVO_PIN 10

Servo ESC;     // servo object to control the ESC
Servo Servo1; // servo object to control a servo

int speed = 0;
int angle = 90;
char string_seperator = ',';

void setup()
{
  // setup serial
  Serial.begin(9600);
  
  // Attach the ESC to the PWM pin
  ESC.attach(ESC_PIN, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH); // (pin, min pulse width, max pulse width in microseconds)
  // Setup the servo to a signal pin
  Servo1.attach(SERVO_PIN);

  // Set servo to neutral position
  Servo1.write(angle);
  // Arm the motor by send minimum PWM signal
  ESC.write(0);
  delay(5000);
}

void loop()
{
  // look for speed message - number from 0 - 100
  if(Serial.available() > 0)
  {
    String input = Serial.readString();

    // Isolate motor command and servo command
    int seperator_index = input.indexOf(string_seperator);
    String motor_command = input.substring(0, seperator_index);
    String servo_command = input.substring(seperator_index + 1, input.length());

    // convert serial inputs string to an integers
    int temp_speed = motor_command.toInt();
    int temp_angle = servo_command.toInt();
    
    if(temp_speed >= 0 && temp_speed <= 100)
    {
        speed = map(temp_speed, 0, 100, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
        Serial.print("Speed set to: ");
        Serial.println(temp_speed);
    }
    else
    {
        Serial.println("Invalid motor command!");
    }

    if(temp_angle >= 0 && temp_angle <= 180)
    {
        Servo1.write(temp_angle); // send angle signal to the servo
        Serial.print("Angle set to: ");
        Serial.println(temp_angle);
    }
    else
    {
        Serial.println("Invalid servo command!");
    }
  }
  
  ESC.write(speed);    // Send the signal to the ESC
}
