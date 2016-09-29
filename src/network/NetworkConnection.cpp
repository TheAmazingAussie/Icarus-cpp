#include "stdafx.h"
#include "NetworkConnection.h"

/*
NetworkConnection constructor

@param connection id
@param tcp::socket connection socket

@return instance
*/
NetworkConnection::NetworkConnection(int connectionID, tcp::socket socket) : connectionID(connectionID), socket_(std::move(socket))
{
	printf("Client connected with ID: %i\n", this->connectionID);
}


NetworkConnection::~NetworkConnection()
{
}

/*
Receive data handle

@return none
*/
void NetworkConnection::recieve_data() {

	auto self(shared_from_this());

	socket_.async_read_some(boost::asio::buffer(this->data_read, this->max_length), [this, self]( boost::system::error_code ec, std::size_t length) {

		if ( !ec ) {
			printf("%s\n", data_read);
			this->recieve_data();
		} else {
			printf("disconnection \n");
		}

	});
}

/*
Write data handle

@return none
*/
void NetworkConnection::write_data() {

	auto self(shared_from_this());
	
	boost::asio::async_write(socket_,boost::asio::buffer(this->data_write, this->max_length), [this, self](boost::system::error_code ec, std::size_t length) {
		if ( !ec ) {
			printf("%s\n", data_read);
		}
	});
}

/*
Disconnect handle

@return none
*/
void NetworkConnection::disconnected() {


}
