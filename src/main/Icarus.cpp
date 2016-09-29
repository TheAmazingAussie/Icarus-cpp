#include "stdafx.h"
#include "Icarus.h"

/*
Initialise the static variables
*/
SessionManager* Icarus::sessionManager = new SessionManager();
NetworkServer* Icarus::networkServer;

/*
Call Boost Asio API to start server

@return none
*/
void Icarus::initialiseServer() {
	boost::asio::io_service io_service;
    networkServer = new NetworkServer(io_service, 30000);
	io_service.run();
}

/*
Method to boot server with nice print

@return none
*/
void Icarus::boot()
{
	printf("Booting server...");
	initialiseServer();
}