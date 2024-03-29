#pragma once
#include "Arduino.h"

class Motor {
public:
	void init();
	void open(bool fromHuman = false);
	void close(bool fromHuman = false);
	void free();
	void printInfo() const;
	bool isOpen() const;
	bool hasHumanInteraction() const;
private:
	static constexpr int pinMotorPWM = 3;
	static constexpr int pinMotorDir = 2;
	bool isopen = true;
	bool humanInteraction = false;
};