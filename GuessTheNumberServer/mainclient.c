#include "main.h"

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed\n");
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Connection failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    const char *request = "Client request: Hello, server!";
    send(clientSocket, request, strlen(request), 0);

    char buffer[BUFFER_SIZE];
    int guess;

    while (1)
    {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytesReceived] = '\0';
        printf("%s", buffer);

        if (strstr(buffer, "Enter your guess") != NULL)
        {
            scanf("%d", &guess);
            send(clientSocket, (char *)&guess, sizeof(int), 0);
            while (getchar() != '\n')
                ;
        }
    }

    // Close the socket and cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
