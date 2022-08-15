#pragma once
#include <SoftwareSerial.h>
#include <Arduino.h>

#include "motor.hpp"
#include "cclock.hpp"

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
	Ble(Motor &motor, CClock &clock);
	void init();
	void loop();

	/**
	 * Block everything else and initialize communication in AT Mode to configure the module
	 */
	void configMode();

private:
	void readPacket();
	void sendPacket(Flags flag, uint32_t data);
	void sendPacket(Flags flag, unsigned int data);
	void sendPacket(Flags flag) { sendPacket(flag, 0u); }
	void sendPacket(Flags flag, void *data); 	// data is an array of 4 bytes
private:
	SoftwareSerial m_serial;
	Motor m_motor;
	CClock m_clock;
	constexpr static uint8_t AT_PIN = 7;
	constexpr static uint8_t TX_PIN = 6;
	constexpr static uint8_t RX_PIN = 5;
	constexpr static bool AT_MODE = false;
};