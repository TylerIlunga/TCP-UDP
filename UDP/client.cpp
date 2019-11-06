#include <iostream>     // C++ Input/Output library
#include <cstdlib>      // <stdlib.h> || C Standard General Utilities ;ibrary
#include <string>       // C++ Strings
#include <unistd.h>     // Standard symbolic constants and types (POSIX operating system API)
#include <sys/types.h>  // Sys data types
#include <sys/socket.h> // Main sockets header
#include <netinet/in.h> // Internet Protocol(IP) family
#include <netdb.h>      // Definitions for network database operations
#include "client.h"

bool handle_req_and_res(
    int &socket_file_descriptor, int &stream_length, char buffer[8192], std::string &userInput,
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
    stream_length = sendto(socket_file_descriptor, userInput.c_str(), userInput.size() + 1, 0, (sockaddr *)&server_address, server_length);
    if (stream_length < 0)
    {
        handle_error("Error writing to server\n");
        return 0;
    }
    // Read message from Server
    bzero(buffer, 8192);
    // recvfrom(): Blocks until a message is received since client_address will be need to send back a datagram.
    stream_length = recvfrom(socket_file_descriptor, buffer, 8192, 0, (sockaddr *)&server_address, &server_length);
    if (stream_length <= 0)
    {
        handle_error("Error reading from server\n");
        return 0;
    }
    std::cout << server + "> " << std::string(buffer, 0, stream_length) << std::endl;
    return 1;
}

int main(int argc, char **argv)
{
    char buffer[8192];
    std::string userInput;

    int socket_file_descriptor;
    int port_number;
    int stream_length;

    unsigned int server_length;
    struct sockaddr_in server_address;
    struct sockaddr_in from;
    struct hostent *server_host;

    if (argc < 3)
    {
        handle_error("Error: Need [{server hostname} {port}]]\n");
        return 1;
    };

    // Creates a TCP Socket for client process
    socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_file_descriptor < 0)
    {
        handle_error("Error creating socket\n");
        return 1;
    }

    // Set Server Host and Port
    server_host = gethostbyname(argv[1]);
    port_number = atoi(argv[2]);

    // bzero() places nbyte (null bytes) in the string s.
    bzero((char *)&server_address, sizeof(server_address));
    // Set Server Address
    server_address.sin_family = AF_INET;
    // bcopy: copies n bytes from src to dest
    bcopy((char *)server_host->h_addr, (char *)&server_address.sin_addr.s_addr, server_host->h_length);
    // htons: converts port_number(int) from host to TCP/IP network byte order (which is big-endian).
    server_address.sin_port = htons(port_number);
    server_length = sizeof(sockaddr_in);

    // DO NEED TO USE INVOKE connect() SYSCALL [UDP]
    std::string server = std::string(std::string(argv[1]) + ":" + std::string(argv[2]));
    std::cout << std::string("Attempting to contact " + server + "\n");

    while (handle_req_and_res(socket_file_descriptor, stream_length, buffer, userInput, server, server_address, server_length))
    {
        // Handle requests and responses...
    }
    close(socket_file_descriptor);
    return 0;
}