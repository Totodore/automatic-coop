#include "cclock.hpp"

CClock::CClock()
{
	memcpy(sunset, base_sunset, 52);
	memcpy(sunrise, base_sunrise, 52);
}

void CClock::init()
{
	// Wire.begin();
	if (rtc.begin())
		Serial.println("RTC started!");
	else
		Serial.println("RTC not found!");

	if (rtc.lostPower())
		Serial.println("RTC lost power, rtc time is 00 !");
}

bool CClock::isDayTime()
{
	uint16_t weekOfYear = getWeekOfTheYear();
	DateTime rise = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), sunrise[weekOfYear - 1][0], sunrise[weekOfYear - 1][1], 0);
	DateTime set = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), sunset[weekOfYear - 1][0], sunset[weekOfYear - 1][1], 0);
	return getTimestamp() < set.unixtime() && getTimestamp() > rise.unixtime();
}

void CClock::printInfo()
{
	uint16_t weekOfYear = getWeekOfTheYear();

	DateTime rise = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), sunrise[weekOfYear - 1][0], sunrise[weekOfYear - 1][1], 0);
	DateTime set = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), sunset[weekOfYear - 1][0], sunset[weekOfYear - 1][1], 0);

	Serial.print("[CLOCK] ->");
	Serial.print(" SUNSET: [");
	Serial.print(set.hour());
	Serial.print(":");
	Serial.print(set.minute());

	Serial.print("] | SUNRISE: [");
	Serial.print(rise.hour());
	Serial.print(":");
	Serial.print(rise.minute());

	Serial.print("] | NOW: [");
	Serial.print(rtc.now().day(), DEC);
	Serial.print("/");
	Serial.print(rtc.now().month(), DEC);
	Serial.print("/");
	Serial.print(rtc.now().year(), DEC);
	Serial.print(" ");
	Serial.print(rtc.now().hour(), DEC); // 24-hr
	Serial.print(":");
	Serial.print(rtc.now().minute(), DEC);
	Serial.println("]");
}

uint16_t CClock::getWeekOfTheYear(uint32_t time)
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
uint16_t CClock::getWeekOfTheYear()
{
	return getWeekOfTheYear(getTimestamp());
}

uint32_t CClock::getTimestamp()
{
	return rtc.now().unixtime();
}

uint32_t CClock::getCurrentTime()
{
	return getTimestamp();
}
void CClock::setCurrentTime(uint32_t time)
{
	rtc.adjust(DateTime(time));
}
uint32_t CClock::getSunriseTime(uint32_t time)
{
	uint16_t weekOfYear = getWeekOfTheYear(time);
	DateTime dt(time);
	return DateTime(dt.year(), dt.month(), dt.day(), sunrise[weekOfYear - 1][0], sunrise[weekOfYear - 1][1], 0).unixtime();
}
uint32_t CClock::getSunsetTime(uint32_t time)
{
	uint16_t weekOfYear = getWeekOfTheYear(time);
	DateTime dt(time);
	return DateTime(dt.year(), dt.month(), dt.day(), sunset[weekOfYear - 1][0], sunset[weekOfYear - 1][1], 0).unixtime();
}

float CClock::getCurrentTemperature()
{
	return rtc.getTemperature();
}
