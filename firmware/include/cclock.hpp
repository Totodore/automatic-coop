#pragma once

#include <stdint.h>

#include <DS3231.h>
#include <Wire.h>

#include <string.h>
#include "data/hours.cpp"

class CClock
{
public:
	CClock();
	void init();
	bool isDayTime();
	void printInfo();

	uint32_t getCurrentTime();
	uint32_t getSunsetTime(uint32_t time);
	uint32_t getSunriseTime(uint32_t time);
	float getCurrentTemperature();

	void setCurrentTime(uint32_t time);

private:
	uint16_t getWeekOfTheYear();
	uint16_t getWeekOfTheYear(uint32_t time);
	uint32_t getTimestamp();
private:
	DS3231 rtc;
	uint8_t sunset[52][2];
	uint8_t sunrise[52][2];
	bool century = false;
	bool pm = false;
	bool h12 = false;
};