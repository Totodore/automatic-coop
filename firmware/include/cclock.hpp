#pragma once

#include <stdint.h>

#include <DS3231.h>
#include <Wire.h>

#include <string.h>
#include "data/hours.cpp"

class CClock
{
public:
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
	bool century = false;
	bool pm = false;
	bool h12 = false;
	constexpr static int16_t rise_offset = 0;
	constexpr static int16_t set_offset = -30 * 60;	// 30 minutes
};