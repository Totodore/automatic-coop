#include "motor.hpp"

void Motor::init()
{
	pinMode(pinMoteurPWM, OUTPUT);
	pinMode(pinMoteurDir, OUTPUT);
}

void Motor::open(bool fromHuman)
{
	Serial.println("Ouverture de la porte ...");
	digitalWrite(pinMoteurDir, LOW);
	digitalWrite(pinMoteurPWM, HIGH);
	isopen = true;
	humanInteraction = fromHuman;
}

void Motor::close(bool fromHuman)
{
	Serial.println("Fermeture de la porte ...");
	digitalWrite(pinMoteurDir, HIGH);
	digitalWrite(pinMoteurPWM, LOW);
	delay(1000);
	isopen = false;
	humanInteraction = fromHuman;
}

void Motor::free()
{
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