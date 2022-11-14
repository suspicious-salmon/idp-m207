#include "motor.hpp"
#include "utility/Adafruit_MS_PWMServoDriver.h"

// AFMS needs to be stored here or bad things happen
Adafruit_MotorShield AFMS;
Adafruit_DCMotor *motor1;
Adafruit_DCMotor *motor2;

// desired values
int motor1_speed = 0;
int motor2_speed = 0;

bool is_moving() {
    return (motor1_speed && motor2_speed);
}

void setup_motors() {
    AFMS = Adafruit_MotorShield();
    motor1 = AFMS.getMotor(1);
    motor2 = AFMS.getMotor(2);
    AFMS.begin();
}

void force_motor_speed(Adafruit_DCMotor *motor, int speed) {
    uint8_t direction = FORWARD;
    int abs_speed = speed;
    if (speed < 0) {
        direction = BACKWARD;
        abs_speed = -speed;
    }
    motor->setSpeed(abs_speed);
    motor->run(direction);
}

void set_motor_speed(int n, int speed) {
    // Get the desired motor object
    Adafruit_DCMotor *motor;
    int prev_speed = -1;
    switch (n)
    {
    case 1:
        prev_speed = motor1_speed;
        motor1_speed = speed;
        motor = motor1;
        break;
    case 2:
        prev_speed = motor2_speed;
        motor2_speed = speed;
        motor = motor2;
        break;
    default:
        break;
    }

    if (motor == nullptr) {
        Serial.println("Error: null pointer motor");
    }
    // Only set new speed if changed to avoid overloading the circuits
    else if (prev_speed != speed) {
        force_motor_speed(motor, speed);
        Serial.print("Motor ");
        Serial.print(n);
        Serial.print(" set to speed: ");
        Serial.println(speed);
    }
}

void pause_motors () {
    motor1->setSpeed(0);
    motor1->run(FORWARD);
    motor2->setSpeed(0);
    motor2->run(FORWARD);
}

void resume_motors() {
    force_motor_speed(motor1, motor1_speed);
    force_motor_speed(motor2, motor2_speed);
}