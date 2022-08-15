#include "clock.hpp"

Clock::Clock()
{
	memcpy(sunset, base_sunset, 52);
	memcpy(sunrise, base_sunrise, 52);
}

void Clock::init()
{
	if (!rtc.begin())
		Serial.println("Impossible to find RTC!...");
	else
		Serial.println("RTC found!");

	if (rtc.lostPower())
		Serial.println("RTC lost power, rtc time is 00 !");
}

bool Clock::isDayTime()
{
	uint16_t weekOfYear = getWeekOfTheYear();

	DateTime rise = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), sunrise[weekOfYear - 1][0], sunrise[weekOfYear - 1][1], 0);
	DateTime set = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), sunset[weekOfYear - 1][0], sunset[weekOfYear - 1][1], 0);

	return rtc.now().unixtime() < set.unixtime() && rtc.now().unixtime() > rise.unixtime();
}

void Clock::printInfo()
{
	uint16_t weekOfYear = getWeekOfTheYear();

	DateTime rise = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), sunrise[weekOfYear - 1][0], sunrise[weekOfYear - 1][1], 0);
	DateTime set = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), sunset[weekOfYear - 1][0], sunset[weekOfYear - 1][1], 0);

	Serial.print("Coucher de soleil : ");
	Serial.print(set.hour());
	Serial.print(":");
	Serial.println(set.minute());
	Serial.println("----------------");

	Serial.print("Lever de soleil : ");
	Serial.print(rise.hour());
	Serial.print(":");
	Serial.println(rise.minute());
	Serial.println("----------------");

	Serial.print("Date : ");
	Serial.print(rtc.now().day(), DEC);
	Serial.print("/");
	Serial.print(rtc.now().month(), DEC);
	Serial.print("/");
	Serial.print(rtc.now().year(), DEC);
	Serial.print(" ");
	Serial.print(rtc.now().hour(), DEC); // 24-hr
	Serial.print(":");
	Serial.println(rtc.now().minute(), DEC);
	Serial.println("----------------");
}

uint16_t Clock::getWeekOfTheYear(uint32_t time)
{
	DateTime dt = DateTime(time);
	uint32_t spentDay = 0;
	for (uint8_t i = 0; i < dt.month() - 1; i++)
	{
		spentDay += maxMonth[i];
	}
	spentDay += dt.day();
	uint16_t spentWeek = (uint16_t)spentDay / 7;
	// si day == 0 on retourne 1 pour pas avoir de sefault array[-1]
	if (spentWeek < 1)
		return 1;
	else if (spentWeek > 52)
		return 52;
	else
		return spentWeek;
}
uint16_t Clock::getWeekOfTheYear()
{
	return getWeekOfTheYear(rtc.now().unixtime());
}

uint32_t Clock::getCurrentTime()
{
	return rtc.now().unixtime();
}
void Clock::setCurrentTime(uint32_t time)
{
	rtc.adjust(DateTime(time));
}
uint32_t Clock::getSunriseTime(uint32_t time)
{
	uint16_t weekOfYear = getWeekOfTheYear(time);
	DateTime dt(time);
	return DateTime(dt.year(), dt.month(), dt.day(), sunrise[weekOfYear - 1][0], sunrise[weekOfYear - 1][1], 0).unixtime();
}
uint32_t Clock::getSunsetTime(uint32_t time)
{
	uint16_t weekOfYear = getWeekOfTheYear(time);
	DateTime dt(time);
	return DateTime(dt.year(), dt.month(), dt.day(), sunset[weekOfYear - 1][0], sunset[weekOfYear - 1][1], 0).unixtime();
}

float Clock::getCurrentTemperature()
{
	return rtc.getTemperature();
}
