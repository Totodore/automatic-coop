#include <Arduino.h>

#include "motor.hpp"
#include "clock.hpp"
#include "ble.hpp"

Clock clock;
Motor motor;
Ble ble(motor, clock);

unsigned long time = 0;
void setup(void)
{
	// Start the serial port
	Serial.begin(9600);
	Serial.println("Demarrage de la carte...");
	clock.init();
	motor.init();
	ble.init();
	Serial.println("Demarrage de la carte termine!");
	ble.configMode();
}

void loop(void)
{

	// Si l'heure actuelle est comprise entre le levé et le coucher de soleil
	// si il fait jour et que on a pas forcé l'ouverture ou la fermeture
	if (clock.isDayTime() && !motor.isOpen() && !motor.hasHumanInteraction())
		motor.open();
	else if (!clock.isDayTime() && motor.isOpen() && !motor.hasHumanInteraction())
		motor.close();

	if (time == 0 || millis() - time > 30000)
	{
		clock.printInfo();
		time = millis();
	}
	ble.loop();
}
