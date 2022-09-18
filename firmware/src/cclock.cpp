#include "cclock.hpp"

void CClock::init()
{
	Wire.begin();
	Serial.println("[CLOCK] -> RTC started!");
}

bool CClock::isDayTime()
{
	uint32_t rise = getSunriseTime(getTimestamp());
	uint32_t set = getSunsetTime(getTimestamp());
	return getTimestamp() < set && getTimestamp() > rise;
}

void CClock::printInfo()
{
	uint16_t weekOfYear = getWeekOfTheYear();
	DateTime rise = DateTime(rtc.getYear(), rtc.getMonth(century), rtc.getDate(), sunrise[weekOfYear - 1][0], sunrise[weekOfYear - 1][1], 0);
	DateTime set = DateTime(rtc.getYear(), rtc.getMonth(century), rtc.getDate(), sunset[weekOfYear - 1][0], sunset[weekOfYear - 1][1], 0);
	Serial.println("[CLOCK] -> Rise: " + String(rise.unixtime() - 3600) + " - Set: " + String(set.unixtime() - 3600) + " - Current: " + String(getTimestamp()) + " - Week: " + String(weekOfYear));

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
	Serial.print(rtc.getDate(), DEC);
	Serial.print("/");
	Serial.print(rtc.getMonth(century), DEC);
	Serial.print("/");
	Serial.print(rtc.getYear(), DEC);
	Serial.print(" ");
	Serial.print(rtc.getHour(h12, pm), DEC); // 24-hr
	Serial.print(":");
	Serial.print(rtc.getMinute(), DEC);
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
	return RTClib::now().unixtime();
}

uint32_t CClock::getCurrentTime()
{
	return getTimestamp();
}
/**
 * For a weird reason the epoch for the RTC starts at 1/1/2000 and not 1/1/1970
 * We substract 30 years and add 1 day to get the correct epoch
 */
void CClock::setCurrentTime(uint32_t time)
{
	rtc.setEpoch(time, true);
	rtc.setYear(rtc.getYear() - 30);
	if (rtc.getDate() + 1u > maxMonth[rtc.getMonth(century) - 1])
	{
		rtc.setDate(1);
		rtc.setMonth(rtc.getMonth(century) + 1);
	}
	else
	{
		rtc.setDate(rtc.getDate() + 1);
	}
}
uint32_t CClock::getSunriseTime(uint32_t time)
{
	uint16_t weekOfYear = getWeekOfTheYear(time);
	DateTime dt(time);
	auto timestamp = DateTime(dt.year(), dt.month(), dt.day(), sunrise[weekOfYear - 1][0], sunrise[weekOfYear - 1][1], 0).unixtime();
	return timestamp - 3600 * 2 + rise_offset;
}
uint32_t CClock::getSunsetTime(uint32_t time)
{
	uint16_t weekOfYear = getWeekOfTheYear(time);
	DateTime dt(time);
	auto timestamp = DateTime(dt.year(), dt.month(), dt.day(), sunset[weekOfYear - 1][0], sunset[weekOfYear - 1][1], 0).unixtime();
	return timestamp - 3600 * 2 + set_offset;
}

float CClock::getCurrentTemperature()
{
	return rtc.getTemperature();
}