#include <iostream>     // C++ Input/Output library
#include <cstdlib>      // <stdlib.h> || C Standard General Utilities ;ibrary
#include <string>       // C++ Strings
#include <unistd.h>     // Standard symbolic constants and types (POSIX operating system API)
#include <sys/types.h>  // Sys data types
#include <sys/socket.h> // Main sockets header
#include <netinet/in.h> // Internet Protocol(IP) family
#include <signal.h>     /* Signals: Asynchronous notifications sent to a process, or to a specific thread,
                        within the same process in order to notify it of an event that occurred. */

void handle_error(std::string msg)
{
    std::cerr << msg;
}

bool handle_req_and_res(int &new_socket_file_descriptor, char buffer[4096], int &stream_length)
{
    // Read data from client
    bzero(buffer, 4096);
    stream_length = recv(new_socket_file_descriptor, buffer, 4096, 0);
    if (stream_length <= 0)
    {
        handle_error("Error reading from server\n");
        return 0;
    }
    std::string msg = "Data Received";
    std::cout << std::string("\nData Received: " + std::string(buffer) + "\n");

    // Respond/write to client
    stream_length = write(new_socket_file_descriptor, msg.c_str(), msg.size() + 1);
    if (stream_length < 0)
    {
        handle_error("Error responding to client socket\n");
        return 0;
    }
    return 1;
}

bool is_child_process_running(pid_t c_pid)
{
    while (waitpid(-1, 0, WNOHANG) > 0)
    {
        // Waiting for process to exit...
    }

    if (0 == kill(c_pid, 0))
    {
        return 1;
    }
    return 0;
}