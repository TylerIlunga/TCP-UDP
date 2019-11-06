#include <iostream>     // C++ Input/Output library
#include <cstdlib>      // <stdlib.h> || C Standard General Utilities ;ibrary
#include <string>       // C++ Strings
#include <unistd.h>     // Standard symbolic constants and types (POSIX operating system API)
#include <sys/types.h>  // Sys data types
#include <sys/socket.h> // Main sockets header
#include <netinet/in.h> // Internet Protocol(IP) family
#include <netdb.h>      // Definitions for network database operations

void handle_error(std::string msg)
{
    std::cerr << msg;
}

bool handle_req_and_res(
    int &socket_file_descriptor, int &message_length, char buffer[8192], std::string &userInput,
    std::string server, sockaddr_in &server_address, unsigned int &server_length)
{
    // Write message to Server
    std::cout << "> ";
    getline(std::cin, userInput);
    if (userInput == "Exit" || userInput == "exit" || userInput.size() <= 0)
    {
        return 0;
    }
    // sendto(): used to send a datagram
    message_length = sendto(socket_file_descriptor, userInput.c_str(), userInput.size() + 1, 0, (sockaddr *)&server_address, server_length);
    if (message_length < 0)
    {
        handle_error("Error writing to server\n");
        return 0;
    }
    // Read message from Server
    bzero(buffer, 8192);
    // recvfrom(): Blocks until a message is received since client_address will be need to send back a datagram.
    message_length = recvfrom(socket_file_descriptor, buffer, 8192, 0, (sockaddr *)&server_address, &server_length);
    if (message_length <= 0)
    {
        handle_error("Error reading from server\n");
        return 0;
    }
    std::cout << server + "> " << std::string(buffer, 0, message_length) << std::endl;
    return 1;
}
