#include "chttp.h"

#include <WinSock2.h>
#include <stdio.h>
#include <windows.h>
#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "chttp_address.h"

const char* _win32_make_request(CHttpResponse* res, const char* url, const char* body)
{
    WSADATA wsa;
    int wsa_result = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (wsa_result != 0) {
        res->error = "Failed to initialize WinSock2";
        res->valid = FALSE;
        return "";
    }

    CHttpAddress addr = chttp_parse_address(strlen(url), url);

    struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int sock_status = getaddrinfo(addr.hostname.str, addr.port.str, &hints, &result);
    if (sock_status != 0) {
        res->error = "Failed to get address info";
        res->valid = FALSE;
        WSACleanup();
        return "";
    }

    SOCKET connect_socket = INVALID_SOCKET;
    ptr = result;
    connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol) ;   

    if (connect_socket == INVALID_SOCKET) {
        res->error = "Socket Error";
        res->valid = FALSE;
        freeaddrinfo(result);
        WSACleanup();
        return "";
    }

    sock_status = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (sock_status == SOCKET_ERROR) {
      closesocket(connect_socket);
      connect_socket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (connect_socket == INVALID_SOCKET) {
      res->valid = FALSE;
      res->error = "Unable to connect to address";
      WSACleanup();
      return "";
    }

    const char* sendbuf = "this is a test";
    char* recvbuf = malloc(sizeof(char) * 512);

    int i_result;

    i_result = send(connect_socket, sendbuf, (int) strlen(sendbuf), 0);
    if (i_result == SOCKET_ERROR) {
      res->valid = FALSE;
      res->error = "Send Failed";
      closesocket(connect_socket);
      WSACleanup();
      return "";
    }

    printf("bytes sent: %d\n", i_result);

    i_result = shutdown(connect_socket, SD_SEND);
    if (i_result == SOCKET_ERROR) {
      res->valid = FALSE;
      res->error = "Shutdown Failed";
      closesocket(connect_socket);
      WSACleanup();
      return "";
    }

    size_t bytes_received = 0;
    do {
      i_result = recv(connect_socket, recvbuf, 512, 0);
      if (i_result > 0) {
        printf("bytes received: %d\n", i_result);
        bytes_received += i_result;
      } else if (i_result == 0) {
        printf("connection closed\n");
      } else {
        printf("recv failed: %d\n", WSAGetLastError());
      }
    } while (i_result > 0);
 
    recvbuf[bytes_received] = 0;

    res->error = "";
    res->valid = TRUE;
    res->result = recvbuf;

    closesocket(connect_socket);
    WSACleanup();

    return recvbuf;
}

CHttpResponse chttp_get(const char* url)
{
    CHttpResponse res = {0};
    res.method = "GET";
    res.url = url;

    const char* response = _win32_make_request(&res, url, "");
    if (res.valid) res.result = response;

    return res;
}
