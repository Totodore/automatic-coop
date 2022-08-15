#pragma once
#include <SoftwareSerial.h>
#include <Arduino.h>

#include "motor.hpp"
#include "clock.hpp"

class Ble
{

	// Packet Size: [Flag(1)] [Data(4)]
	enum Flags
	{
		ACK = 0x00,
		GET_DOOR_STATUS = 0x01,
		GET_CURRENT_TIME = 0x02,
		GET_SUNSET_TIME = 0x03,
		GET_SUNRISE_TIME = 0x04,
		GET_CURRENT_TEMPERATURE = 0x05,
		CLOSE_DOOR = 0x06,
		OPEN_DOOR = 0x07,
		FREE_DOOR = 0x08,
		SET_CURRENT_TIME = 0x09,
	};

public:
	Ble(Motor &motor, Clock &clock);
	void init();
	void loop();

	/**
	 * Block everything else and initialize communication in AT Mode to configure the module
	 */
	void configMode();

private:
	void readPacket();
	void sendPacket(Flags flag, uint32_t data);
	void sendPacket(Flags flag, unsigned int data = 0u);
	void sendPacket(Flags flag, void *data); 	// data is an array of 4 bytes
private:
	SoftwareSerial m_serial;
	Motor m_motor;
	Clock m_clock;
	constexpr static uint8_t AT_PIN = 9;
	constexpr static uint8_t TX_PIN = 10;
	constexpr static uint8_t RX_PIN = 11;
	constexpr static bool AT_MODE = false;
};