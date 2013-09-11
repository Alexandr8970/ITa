// http://msdn.microsoft.com/en-us/library/windows/desktop/ms737593%28v=vs.85%29.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms737591%28v=vs.85%29.aspx
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
using namespace std;

// ------------ GLOBAL VARIABLES -----------------------------------------------
#define DEFAULT_BUFLEN 512

//------------------------------------------------------------------------------

//------------- FUNCTIONS ------------------------------------------------------
bool initSocket();
bool connectToServer(std::string aServerAddress, size_t aPort, SOCKET &aServerSocket);
void closeSocket(SOCKET &aServerSocket);
bool shutDownSocket(SOCKET &aServerSocket);

std::string readMessage(SOCKET &aClient);
bool writeMessage(SOCKET &aClient, std::string aMessage);
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
bool connectToServer(std::string aServerAddress, size_t aPort, SOCKET &aServerSocket)
//------------------------------------------------------------------------------
{
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    int iResult;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char port[10] = {0};
    itoa(aPort, &port[0], 10);

    iResult = getaddrinfo(aServerAddress.c_str(), port, &hints, &result);
    if ( iResult != 0 ) 
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return false;
    }

    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) 
    {
        aServerSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (aServerSocket == INVALID_SOCKET) 
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        iResult = connect(aServerSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) 
        {
            closesocket(aServerSocket);
            aServerSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if(aServerSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool shutDownSocket(SOCKET &aServerSocket)
//------------------------------------------------------------------------------
{
    if (shutdown(aServerSocket, SD_SEND) == SOCKET_ERROR) 
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(aServerSocket);
        WSACleanup();
        return false;
    }

    return true;
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
bool writeMessage(SOCKET &aClient, std::string aMessage)
//------------------------------------------------------------------------------
{
    int bytesWrite = 0;
    bytesWrite = send(aClient, aMessage.c_str(), aMessage.size(), 0);

    if(bytesWrite == SOCKET_ERROR)
        return false;
    return true;
}


int main()
{
    SOCKET ConnectSocket = INVALID_SOCKET;
    char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    if(!initSocket())
        return 1;

    if(!connectToServer("localhost", 27015, ConnectSocket))
        return 1;

    std::string msg;
    msg = readMessage(ConnectSocket);
    writeMessage(ConnectSocket, "780");

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}

