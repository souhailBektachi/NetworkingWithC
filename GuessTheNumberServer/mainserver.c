#include "main.h"

int sendResponse(SOCKET socketClient, char *response)
{
    int bytes_sent = send(socketClient, response, strlen(response), 0);
    printf("sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    if (bytes_sent < 0)
    {
        printf("Error: %d\n", WSAGetLastError());
        return 1;
    }
    return 0;
}
void handleClients(void *clientSocket)
{
    SOCKET socketClient = (SOCKET)clientSocket;
    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socketClient, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);
    printf("%.*s", bytes_received, request);
    printf("Sending response...\n");

    const char *response = "HTTP/1.1 200 OK\r\n"
                           "Connection: close\r\n"
                           "Content-Type: text/plain\r\n\r\n"
                           "Welcome to Guess the Number game!\n";
    int bytes_sent = send(socketClient, response, strlen(response), 0);
    printf("2-Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));
    int secretNumber;
    secretNumber = rand() % 100 + 1;

    int guess = -1;
    int attempts = 0;
    char *guessMessage = "Guess the number between 1 and 100.\n";
    sendResponse(socketClient, guessMessage);

    do
    {
        char buffer[sizeof(int)];
        static int tries = 0;
        memset(buffer, 0, sizeof(int));
        if (tries == 10)
        {
            sendResponse(socketClient, "\nToo many invalid inputs. Closing connection.\n");
            break;
        }
        guessMessage = "Enter your guess: ";
        if (sendResponse(socketClient, guessMessage))
        {
            break;
        }

        bytes_received = recv(socketClient, buffer, sizeof(int), 0);

        printf("Received %d bytes.\n", bytes_received);
        if (bytes_received < 0)
        {
            printf("Client disconnected.\n");
            break;
        }
        printf("Received %s\n", buffer);
        memcpy(&guess, buffer, sizeof(int));

        if (guess < 0 || guess > 100)
        {
            guessMessage = "Invalid input. Please enter a number.\n";
            sendResponse(socketClient, guessMessage);

            tries++;
            printf("tries: %d\n", tries);
            continue;
        }
        else
        {
            memcpy(&guess, buffer, sizeof(int));
            tries = 0;
        }

        attempts++;

        if (guess > secretNumber)
        {
            guessMessage = "Too high! Try again.\n";
            sendResponse(socketClient, guessMessage);
        }
        else if (guess < secretNumber)
        {
            guessMessage = "Too low! Try again.\n";
            sendResponse(socketClient, guessMessage);
        }
        else
        {
            printf("Correct! You got it in %d guesses.\n", attempts);
            char winmsg[50];
            sprintf(winmsg, "The secret number was %d.\n", secretNumber);

            sendResponse(socketClient, winmsg);
        }

    } while (guess != secretNumber);
}
int main()
{
    WSADATA wsaData;
    struct sockaddr_storage clientAddress;
    SOCKET socketClient;
    socklen_t clientLen;
    struct addrinfo hints;
    struct addrinfo *bindAddress;
    SOCKET socketListen;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        printf("wsastartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(0, "8080", &hints, &bindAddress);
    socketListen = socket(bindAddress->ai_family, bindAddress->ai_socktype, bindAddress->ai_protocol);
    if (socketListen == INVALID_SOCKET)
    {
        fprintf(stderr, "socket() failed. (%d)\n", WSAGetLastError());
        return 1;
    }
    if (bind(socketListen, bindAddress->ai_addr, bindAddress->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", WSAGetLastError());
        return 1;
    }
    freeaddrinfo(bindAddress);
    if (listen(socketListen, 10) < 0)
    {
        fprintf(stderr, "listen() failed. (%d)\n", WSAGetLastError());
        return 1;
    }
    printf("Listening on 8080...\n");

    while (1)
    {
        clientLen = sizeof(clientAddress);
        socketClient = accept(socketListen, (struct sockaddr *)&clientAddress, &clientLen);
        if (socketClient == INVALID_SOCKET)
        {
            fprintf(stderr, "accept() failed. (%d)\n", WSAGetLastError());
            return 1;
        }
        printf("Client is connected...\n");
        char address_buffer[100];

        getnameinfo((struct sockaddr *)&clientAddress, clientLen, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
        printf("%s\n", address_buffer);
        _beginthread(handleClients, 0, (void *)socketClient);
    }
    printf("Closing connection...\n");
    closesocket(socketClient);
    printf("Closing listening socket...\n");
    closesocket(socketListen);

    WSACleanup();

    return 0;
}
