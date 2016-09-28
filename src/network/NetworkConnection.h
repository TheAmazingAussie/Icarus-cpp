#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <iostream>
#include <vector>
#include <sstream> 

using namespace std;

class NetworkConnection 
{

public:
    NetworkConnection(int connectionID, SOCKET socket);
    ~NetworkConnection();
    void write_data();
    void handle_data(char* data, int length);
    void sendPolicy();
    int getConnectionId() { return connectionID; }
    bool getConnectionState() { return connected; }
    void setConnectionState(bool state) { connected = state; }
    SOCKET getSocket() { return socket; };
    DWORD getThreadId() { return thread_id; }

private:
    int connectionID;
    bool connected;
    SOCKET socket;
    DWORD thread_id;
    void createThread();

};
