#include "ble.hpp"

Ble::Ble(Motor &motor, CClock &clock) : m_serial(TX_PIN, RX_PIN), m_motor(motor), m_clock(clock) {}
void Ble::init()
{
	m_serial.begin(9600);
	pinMode(AT_PIN, OUTPUT); // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
	digitalWrite(AT_PIN, AT_MODE ? HIGH : LOW);
}

void Ble::configMode()
{
	if (!AT_MODE)
		return;
	Serial.println("[BLE] Entering config mode...");
	while (true)
	{
		// Keep reading from HC-05 and send to Arduino Serial Monitor
		if (m_serial.available())
		{
			String command = m_serial.readString();
			Serial.println("[HC-05] -> " + command);
		}
		// Keep reading from Arduino Serial Monitor and send to HC-05
		if (Serial.available())
		{
			// uint32_t data = Serial.read();
			String command = Serial.readString();
			Serial.println("[HC-05] <- " + command);
			m_serial.println(command);
		}
	}
}

void Ble::loop()
{
	if (m_serial.available())
		readPacket();
}

void Ble::readPacket()
{
	uint8_t packet[5]; // packet
	m_serial.readBytes(&packet[0], 5);
	Serial.print("[BLE] <- ");
	uint32_t data;
	memcpy(&data, &packet[1], 4);
	switch (packet[0])
	{
	case Flags::GET_DOOR_STATUS:
		Serial.print("GET_DOOR_STATUS");
		sendPacket(Flags::GET_DOOR_STATUS, (uint32_t)m_motor.isOpen());
		break;
	case Flags::GET_DOOR_FREE:
		Serial.print("GET_DOOR_FREE");
		sendPacket(Flags::GET_DOOR_FREE, (uint32_t)!m_motor.hasHumanInteraction());
		break;
	case Flags::GET_CURRENT_TIME:
		Serial.print("GET_CURRENT_TIME");
		sendPacket(Flags::GET_CURRENT_TIME, m_clock.getCurrentTime());
		break;
	case Flags::GET_CURRENT_TEMPERATURE:
	{
		Serial.print("GET_CURRENT_TEMPERATURE");
		float temp = m_clock.getCurrentTemperature();
		sendPacket(Flags::GET_CURRENT_TEMPERATURE, (void *)&temp);
		break;
	}
	case Flags::GET_SUNSET_TIME:
		Serial.print("GET_SUNSET_TIME");
		sendPacket(Flags::GET_SUNSET_TIME, (uint32_t)m_clock.getSunsetTime(data));
		break;
	case Flags::GET_SUNRISE_TIME:
		Serial.print("GET_SUNRISE_TIME");
		sendPacket(Flags::GET_SUNRISE_TIME, (uint32_t)m_clock.getSunriseTime(data));
		break;
	case Flags::CLOSE_DOOR:
		Serial.print("CLOSE_DOOR");
		m_motor.close(true);
		sendPacket(Flags::CLOSE_DOOR);
		break;
	case Flags::OPEN_DOOR:
		Serial.print("OPEN_DOOR");
		m_motor.open(true);
		sendPacket(Flags::OPEN_DOOR);
		break;
	case Flags::FREE_DOOR:
		Serial.print("FREE_DOOR");
		m_motor.free();
		sendPacket(Flags::FREE_DOOR);
		break;
	case Flags::SET_CURRENT_TIME:
		Serial.print("SET_CURRENT_TIME");
		m_clock.setCurrentTime(data);
		sendPacket(Flags::SET_CURRENT_TIME);
		break;
	}
	if (data)
		Serial.println(" [" + String(data) + "]");
	else
		Serial.println();
}

void Ble::sendPacket(Flags flag, uint32_t data)
{
	uint8_t packet[5];
	packet[0] = flag;
	memcpy(packet + 1, &data, 4);
	m_serial.write(packet, 5);
}

void Ble::sendPacket(Flags flag, unsigned int data)
{
	sendPacket(flag, (uint32_t)data);
}

void Ble::sendPacket(Flags flag, void *data)
{
	uint8_t packet[5];
	packet[0] = flag;
	memcpy(packet + 1, data, 4);
	m_serial.write(packet, 5);
}