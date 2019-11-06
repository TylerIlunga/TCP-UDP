#include "client.h"

int main(int argc, char **argv)
{
    char buffer[8192];
    std::string userInput;

    int socket_file_descriptor;
    int port_number;
    int message_length;

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

    while (handle_req_and_res(socket_file_descriptor, message_length, buffer, userInput, server, server_address, server_length))
    {
        // Handle requests and responses...
    }
    close(socket_file_descriptor);
    return 0;
}