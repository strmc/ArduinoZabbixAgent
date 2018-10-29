/*
	Zabbix Agent Library for Arduino
	
	ZabbixAgent.h - Library for realising a simple Zabbix Agent with Arduino
	
	Copyright (C) 2018 strmc

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef Morse_h
#define Morse_h

#include "Arduino.h"
#include "Client.h"
#include "Server.h"

#define ZABBIX_PORT 10050
#define HEADER_LENGTH 5

class ZabbixAgent{
public:
	ZabbixAgent();
	String listen(Server serverSocket);
	bool answer(String message);
	
private:
	Server _serverSocket;
	Client _socket;
	byte _header[HEADER_LENGTH];
	
}

#endif
