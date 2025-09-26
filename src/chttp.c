#include "chttp.h"

#include <WinSock2.h>
#include <windows.h>
#include <winsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

_Bool win32_has_been_initialized = FALSE;

CHttpResponse chttp_get(const char* url)
{
    CHttpResponse res = {0};
    res.method = "GET";
    res.url = url;
    res.error = "Unimplemented";

    if (!win32_has_been_initialized) {
        WSADATA wsa;
        int wsa_result = WSAStartup(MAKEWORD(2, 2), &wsa);
        if (wsa_result != 0) {
            res.error = "Failed to initialize WinSock2";
            res.valid = FALSE;
            return res;
        }

        win32_has_been_initialized = TRUE;
    }

    CHttpAddress addr = chttp_parse_address(url);

    struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int sock_status = getaddrinfo(addr.hostname, addr.port, &hints, &result);
    if (sock_status != 0) {
        res.error = "Failed to get address info";
        res.valid = FALSE;
        WSACleanup();
        return res;
    }

    SOCKET connect_socket = INVALID_SOCKET;
    ptr = result;
    connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol) ;   

    if (connect_socket == INVALID_SOCKET) {
        res.error = "Socket Error";
        res.valid = FALSE;
        freeaddrinfo(result);
        WSACleanup();
        return res;
    }

    return res;
}