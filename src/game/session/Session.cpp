#include "stdafx.h"

#include "game/session/Session.h"
#include "game/session/SessionDetails.h"

/*
    Session constructor

    @param NetworkConnection ptr
    @return session instance
*/
Session::Session(NetworkConnection *networkConnection) : network_connection(network_connection), session_details(nullptr) {
    std::cout << " [SESSION] Client connected with ID: " << this->getNetworkConnection()->getConnectionId() << std::endl;
}


/*
    Send message composer to the session's socket

    @param MessageComposer ptr
    @return none
*/
void Session::send(MessageComposer &composer) {
    this->network_connection->send(composer);
}

/*
    Clear session of any responsibilities

    @return none
*/
void Session::clear() {

    
}

/*
    Session deconstructor, called when session disconnects, 
    is deleted from SessionManager class

    networkManager->connectionState has already been set to false 
    so there won't be any more packet receiving
*/
Session::~Session() {
    std::cout << " [SESSION] Client disconnected with ID: " << this->getNetworkConnection()->getConnectionId() << std::endl;

    if (session_details != nullptr) {
        delete session_details;
    }
}
