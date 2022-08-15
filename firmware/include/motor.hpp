#pragma once
#include "Arduino.h"

class Motor {
public:
	void init();
	void open(bool fromHuman = false);
	void close(bool fromHuman = false);
	void free();
	bool isOpen() const;
	bool hasHumanInteraction() const;
private:
	static constexpr int pinMoteurPWM = 3;
	static constexpr int pinMoteurDir = 2;
	bool isopen = true;
	bool humanInteraction = false;
};