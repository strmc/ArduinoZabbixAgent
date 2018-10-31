/*
	Zabbix Agent Library for Arduino
	
	ZabbixAgent.c - Library for realising a simple Zabbix Agent with Arduino
	
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

#include "ZabbixAgent.h"

ZabbixAgent::ZabbixAgent(WiFiServer& serverSocket): _serverSocket(serverSocket){
}

/*
	public method listen() waits for connection and parses the message from Zabbix.
	returns the itemkey requested by Zabbix or an empty String if something went wrong.
*/
String ZabbixAgent::listen(){
	//needed variables
	byte length[8];
	String msg = "";
	
	//wait for the Zabbix server to connect
	do{
		_socket = _serverSocket.available();
		yield();
	}
	while(!_socket.connected());
	
	//starts when the Zabbix server connected
	if(_socket.connected()){
		//wait for the socket to be ready for communication
		while(!_socket.available()){
			yield();
		}
		
		//the actual receive
		// 
		// usually Zabbix sends it's request starting with a 5 byte long header
		// which usually represents the characters Z B X D and a fifth byte with only a one in it
		// 
		// the second part of the request is an integer number encoded in 8 bytes in little-endian format
        // which contains the number of bytes used for the following data block
		// 
		// The data block is a sequence of chars which contain a request like 'agent.ping'
		//
		// Example:
		//
		//			HEADER							DATALENGTH									DATA
		//  ( 'Z' 'B' 'X' 'D' 0x01 | 0x0A 0x00 0x00 0x00 0x00 0x00 0x00 0x00 | 'a' 'g' 'e' 'n' 't' '.' 'p' 'i' 'n' 'g' ) 
		if(_socket.available()){
			//reading 5 bytes for the header
			for(int i = 0; i < sizeof(_header); i++){
				_header[i] = _socket.read();
			}
			
			//reading 8 bytes for the datalength
			for(int i = 0; i < sizeof(length); i++){
				length[i] = _socket.read();
			}
			
			//calculating the datalength from giiven bytearray
			uint8_t datalength = uint8_t(length[0]); //Only working with datalength <= 255 @TODO fix
			
			//reading as many bytes as given in datalength and converting it into a string
			byte data[datalength];
			for(int i = 0; i < datalength; i++){
				data[i] = _socket.read();
				msg += char(data[i]);
			}
		}
	}
	return msg;
}

/*
	public method answer(String) sends a message back to the server
	returns wether the sending was succesfull
*/
bool ZabbixAgent::answer(String message){
	
	//sending the same header received before
	for(int i = 0; i < sizeof(_header); i++){
		_socket.write(_header[i]);
	}
	
	//sending 8 bytes to communicate the length of following data
	_socket.write(byte(sizeof(message))); //Only working with datalength <= 255 @TODO fix
	for(int i = 0; i < 7; i++){
		_socket.write(byte(0x00));
	}
	
	//sending the actual data
	for(int i = 0; i < sizeof(message); i++){
		_socket.write(message.charAt(i));
	}
	_socket.flush();
	return true; //@TODO return wether writing was succesfull
}
