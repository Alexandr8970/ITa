// http://msdn.microsoft.com/en-us/library/windows/desktop/ms737593%28v=vs.85%29.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms737591%28v=vs.85%29.aspx

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;



// ------------ GLOBAL VARIABLES -----------------------------------------------

char *array_clients_id[] = 
{
    "100",
    "101",
    "780"
};
size_t size_clients_array = 3;

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

SOCKET gListenSocket = INVALID_SOCKET;

const size_t S_INIT                 = 1;
const size_t S_WAIT_CONNECTION      = 2;
const size_t S_AUTHENTICATION       = 3;
const size_t S_REQUEST_PROCESSING   = 4;
const size_t S_CLOSE_CONNECTION     = 5;
const size_t S_EXIT                 = 6;

const std::string SA_GET_ID = "GetID";

size_t gServerState = S_INIT;
bool   gServerRun   = true;
//------------------------------------------------------------------------------

//------------- FUNCTIONS ------------------------------------------------------
bool initSocket();
bool startServer();
void closeClientSocket(SOCKET &aClient);
void stopServer();

std::string readMessage(SOCKET &aClient);
bool writeMessage(SOCKET &aClient, const std::string aMessage);


bool    server_init();
SOCKET  waitClient();
bool    client_authentication(SOCKET &aClient);
void    request_processing(SOCKET &aClient);
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool initSocket()
//------------------------------------------------------------------------------
{
    WSADATA wsaData;
    int     iResult;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) 
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool startServer()
//------------------------------------------------------------------------------
{
    struct addrinfo *result = NULL;
    struct addrinfo  hints;

    int iResult;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags    = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if(iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return false;
    }

    // Create a SOCKET for connecting to server
    gListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(gListenSocket == INVALID_SOCKET) 
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    // Setup the TCP listening socket
    iResult = bind(gListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if(iResult == SOCKET_ERROR) 
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(gListenSocket);
        WSACleanup();
        return false;
    }
    freeaddrinfo(result);

    iResult = listen(gListenSocket, SOMAXCONN);
    if(iResult == SOCKET_ERROR) 
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(gListenSocket);
        WSACleanup();
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
SOCKET waitClient()
//------------------------------------------------------------------------------
{
    SOCKET clientSocket = INVALID_SOCKET;

    clientSocket = accept(gListenSocket, NULL, NULL);
    if(clientSocket == INVALID_SOCKET) 
    {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(gListenSocket);
        WSACleanup();
        return clientSocket;
    }
    return clientSocket;
}

//------------------------------------------------------------------------------
void closeClientSocket(SOCKET &aClient)
//------------------------------------------------------------------------------
{
    int iResult;

    iResult = shutdown(aClient, SD_SEND);
    if (iResult == SOCKET_ERROR) 
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(aClient);
        WSACleanup();
        return;
    }
    closesocket(aClient);
}

//------------------------------------------------------------------------------
void stopServer()
//------------------------------------------------------------------------------
{
    WSACleanup();
}

//------------------------------------------------------------------------------
std::string readMessage(SOCKET &aClient)
//------------------------------------------------------------------------------
{
    char recvbuf[DEFAULT_BUFLEN];
    int  recvbuflen = DEFAULT_BUFLEN;

    int bytesRead = 0;
    std::string res;

    bytesRead = recv(aClient, recvbuf, recvbuflen, 0);

    if(bytesRead > 0)
        res = recvbuf;
    return res;
}

//------------------------------------------------------------------------------
bool writeMessage(SOCKET &aClient,const std::string aMessage)
//------------------------------------------------------------------------------
{
    int bytesWrite = 0;
    bytesWrite = send(aClient, aMessage.c_str(), aMessage.size(), 0);

    if(bytesWrite == SOCKET_ERROR)
        return false;
    return true;
}

//------------------------------------------------------------------------------
bool server_init()
//------------------------------------------------------------------------------
{
    if(!initSocket())
        return false;

    if(!startServer())
        return false;

    return true;
}

//------------------------------------------------------------------------------
bool client_authentication(SOCKET &aClient)
//------------------------------------------------------------------------------
{
    if(!writeMessage(aClient, SA_GET_ID))
        return false;

    std::string msg;

    msg = readMessage(aClient);
    if(msg.size() == 0)
        return false;

    for(size_t i = 0; i < size_clients_array; ++i)
        if(strcmp(array_clients_id[i],msg.c_str()) == 0)
            return true;

    return false;
}

//------------------------------------------------------------------------------
void request_processing(SOCKET &aClient)
//------------------------------------------------------------------------------
{

}

int main()
{
    SOCKET client;

    while(gServerRun)
    {
        switch(gServerState)
        {
            case S_INIT:
            {
                if(server_init())
                    gServerState = S_WAIT_CONNECTION;
                else
                    gServerState = S_EXIT;
                break;
            }

            case S_WAIT_CONNECTION:
            {
                client = waitClient();
                if(client == INVALID_SOCKET)
                {
                    stopServer();
                    gServerState = S_EXIT;
                }
                else
                    gServerState = S_AUTHENTICATION;
                break;
            }

            case S_AUTHENTICATION:
            {
                if(client_authentication(client))
                    gServerState = S_REQUEST_PROCESSING;
                else
                    gServerState = S_CLOSE_CONNECTION;
                break;
            }

            case S_REQUEST_PROCESSING:
            {
                request_processing(client);
                gServerState = S_CLOSE_CONNECTION;
                break;
            }

            case S_CLOSE_CONNECTION:
            {
                closeClientSocket(client);
                gServerState = S_WAIT_CONNECTION;
                break;
            }

            case S_EXIT:
            {
                stopServer();
                gServerRun = false;
                break;
            }
        }
    }

    return 0;
}

