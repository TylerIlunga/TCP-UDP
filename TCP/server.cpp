#include "server.h"

int main(int argc, char **argv)
{
    int socket_file_descriptor;
    int new_socket_file_descriptor;
    int port_number;
    int stream_length;

    socklen_t client_length;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    struct hostent *server;

    char buffer[4096];
    if (argc < 2)
    {
        handle_error("Error: Need {port}\n");
        return 1;
    };

    // Creates a TCP Socket for Server process
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor < 0)
    {
        handle_error("Error creating socket file descriptor: can not open\n");
        return 1;
    }
    // bzero() places nbyte (null bytes) in the string s.
    bzero((char *)&server_address, sizeof(server_address));
    // Set Server Port
    port_number = atoi(argv[1]);

    // Set Server IP Address
    server_address.sin_family = AF_INET;
    // htons: converts port_number(int) from host to TCP/IP network byte order (which is big-endian).
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the created socket
    int bind_status = bind(socket_file_descriptor, (sockaddr *)&server_address, sizeof(server_address));
    if (bind_status < 0)
    {
        handle_error("Error on binding socket\n");
        return 1;
    }

    // backlog = Max length to which the queue of pending connections, for our
    // socket_file_descriptor, may grow.
    int backlog = 10;
    int listen_status = listen(socket_file_descriptor, backlog);
    if (listen_status < 0)
    {
        handle_error("Error on listening for clients\n");
        return 1;
    }
    std::cout << "Listening on port " << port_number << std::endl;

    while (true)
    {
        // Handle connecting to Client process
        client_length = sizeof(client_address);
        new_socket_file_descriptor = accept(socket_file_descriptor, (struct sockaddr *)&client_address, &client_length);
        if (new_socket_file_descriptor < 0)
        {
            handle_error("Error establishing connection between client and server\n");
            return 1;
        }
        // Spawn child process to handle requests and responses.
        pid_t pid = fork();
        if (pid == 0)
        {
            close(socket_file_descriptor);
            while (handle_req_and_res(new_socket_file_descriptor, buffer, stream_length))
            {
                // Handle requests and responses...
            };
            exit(0);
        }
        else
        {
            while (is_child_process_running(pid))
            {
                // Parent process must wait for child process to avoid zombie processes...
            };
            close(new_socket_file_descriptor);
            exit(0);
        }
    }
};