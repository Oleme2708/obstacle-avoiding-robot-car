// library
#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <L298N.h>
// sonar sensor setup
#define SONAR_NUM 1
#define MAX_DISTANCE 200

// state setup
enum state
{
    Forward,
    Backward,
    TurnLeft,
    TurnRight
}

NewPing Sonar[SONAR_NUM] = {
    NewPing(4, 5, MAX_DISTANCE), // trigger pin, echo pin, max distance
}

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    // changing state
    switch (state)
    {
    case Forward:
        MoveForward();
        if (distance <= 20)
        {
            Stop();
            ChangeState(Backward);
        }
        break;
    case Backward:
        MoveBackward();
        if (millis() - stateStartTime >= 500)
        {
            Stop();
            ChangeState(Scan);
        }
        break;
    case scan:
        Scan();
        if (millis() - stateStartTime >= 500)
        {
            Stop();
            ChangeState(Scan);
        }
        break;
    }
}

// user-deifned function
void MoveForward()
{
}

void Backward()
{
}

void TurnLeft()
{
}

void TurnRight()
{
}

void Scan()
{
}

void Stop()
{
}

void ChangeState(State newstate)
{
    state = newstate;
    stateStartTime = millis();
}

void LookLeft()
{
}

void LookRight()
{
}
