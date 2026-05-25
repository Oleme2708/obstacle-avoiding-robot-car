// library
#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <L298N.h>
// sonar sensor setup
#define MAX_DISTANCE 200
#define trig_pin 12
#define echo_pin 11
NewPing Sonar(trig_pin, echo_pin, MAX_DISTANCE); // trigger pin, echo pin, max distance
// motors setup
// motor A connections
int ENA = 9;
int IN1 = 2;
int IN2 = 3;
// motor B connections
int ENB = 10;
int IN3 = 4;
int IN4 = 5;
// Servo setup
Servo myservo;
int servopin = 6;
// state setup
enum State
{
    STATE_Forward,
    STATE_Backward,
    STATE_Scan,
    STATE_TurnLeft,
    STATE_TurnRight
};
State currentState = STATE_Forward;
unsigned long stateStartTime = 0;
// initial values
int distance = 0;
int distance_left = 0;
int distance_right = 0;
// Declare user_defined function
void MoveForward();
void MoveBackward();
void TurnLeft();
void TurnRight();
void Scan();
void Stop();
void ChangeState(State newState);
int LookLeft();
int LookRight();
int readPing();
//
void setup()
{
    Serial.begin(9600);
    // Motors pin setup
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    // setup max speed (change if 1 motor is faster than other)
    analogWrite(ENA, 255);
    analogWrite(ENB, 255);
    myservo.attach(servopin);
    myservo.write(115);
    // reading distance values 4 times
    int total = 0;
    for (int i = 0; i < 4; i++)
    {
        total += readPing();
        delay(100);
    }
    distance = total / 4;
    stateStartTime = millis();
}

void loop()
{
    // inital distance setup
    distance = readPing();
    Serial.println(distance);
    // changing state
    switch (currentState)
    {
    case STATE_Forward:
        MoveForward();
        if (distance <= 20)
        {
            Stop();
            ChangeState(STATE_Backward);
        }
        break;
    case STATE_Backward:
        MoveBackward();
        if (millis() - stateStartTime >= 500)
        {
            Stop();
            ChangeState(STATE_Scan);
        }
        break;
    case STATE_Scan:
        Scan();
        if (distance_right >= distance_left)
        {
            ChangeState(STATE_TurnRight);
        }
        else
        {
            ChangeState(STATE_TurnLeft);
        }
        break;
    case STATE_TurnRight:
        TurnRight();
        if (millis() - stateStartTime >= 500)
        {
            Stop();
            ChangeState(STATE_Forward);
        }
        break;
    case STATE_TurnLeft:
        TurnLeft();
        if (millis() - stateStartTime >= 500)
        {
            Stop();
            ChangeState(STATE_Forward);
        }
        break;
    }
}

// user-deifned function
void MoveForward()
{
    // motor A forward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    // motor B forward
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void MoveBackward()
{
    // motor A backward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    // motor B backward
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void TurnLeft()
{
    // motor A forward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    // motor B stop
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void TurnRight()
{
    // motor A stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    // motor B forward
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void Stop()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void ChangeState(State newState)
{
    currentState = newState;
    stateStartTime = millis();
}

void Scan()
{
    Stop();
    distance_left = LookLeft(); // scanining left distance
    delay(200);
    distance_right = LookRight(); // scaning right distance
    delay(200);
    myservo.write(115); // servo turn back to middle
    delay(200);
    // print distance on serial monitor for testing
    Serial.print("left: ");
    Serial.println(distance_left);
    Serial.print("right: ");
    Serial.println(distance_right);
}

int LookLeft()
{
    myservo.write(50);
    delay(500);
    return readPing();
}

int LookRight()
{
    myservo.write(170);
    delay(500);
    return readPing();
}

int readPing()
{
    delay(50);
    int cm = Sonar.ping_cm();
    if (cm == 0)
    {
        cm = MAX_DISTANCE;
    }
    return cm;
}