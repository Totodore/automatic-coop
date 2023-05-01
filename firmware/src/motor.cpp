#include "motor.hpp"

void Motor::init()
{
	pinMode(pinMotorPWM, OUTPUT);
	pinMode(pinMotorDir, OUTPUT);

	// With PWM H bridge, the motor is always on
	digitalWrite(pinMotorPWM, HIGH);
}

void Motor::open(bool fromHuman)
{
	Serial.println("[DOOR] -> OPENING");
	digitalWrite(pinMotorDir, HIGH);
	isopen = true;
	humanInteraction = fromHuman;
}

void Motor::close(bool fromHuman)
{
	Serial.println("[DOOR] -> CLOSING");
	digitalWrite(pinMotorDir, LOW);
	isopen = false;
	humanInteraction = fromHuman;
}

void Motor::free()
{
	Serial.println("[DOOR] -> OPENING");
	humanInteraction = false;
}
bool Motor::isOpen() const
{
	return isopen;
}

bool Motor::hasHumanInteraction() const
{
	return humanInteraction;
}

void Motor::printInfo() const
{
	Serial.print("[DOOR] -> ");
	Serial.print(isopen ? "OPENED" : "CLOSED");
	Serial.print(" | HUMAN INTERACTION: ");
	Serial.println(humanInteraction ? "YES" : "NO");
}