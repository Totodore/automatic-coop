#include <Arduino.h>

#include "motor.hpp"
#include "cclock.hpp"
#include "ble.hpp"

CClock cclock;
Motor motor;
Ble ble(motor, cclock);

unsigned long currTime = 0;
void setup(void)
{
	// Start the serial port
	Serial.begin(9600);
	Serial.println("Demarrage de la carte...");
	cclock.init();
	motor.init();
	ble.init();
	Serial.println("Demarrage de la carte termine!");
	ble.configMode();
}

void loop(void)
{

	// Si l'heure actuelle est comprise entre le levé et le coucher de soleil
	// si il fait jour et que on a pas forcé l'ouverture ou la fermeture
	if (cclock.isDayTime() && !motor.isOpen() && !motor.hasHumanInteraction())
		motor.open();
	else if (!cclock.isDayTime() && motor.isOpen() && !motor.hasHumanInteraction())
		motor.close();

	if (currTime == 0 || millis() - currTime > 30000)
	{
		cclock.printInfo();
		currTime = millis();
	}
	ble.loop();
}
