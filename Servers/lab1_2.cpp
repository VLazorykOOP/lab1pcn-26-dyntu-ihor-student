#include <iostream>
#include <winsock2.h>
#include <string>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

void printErrorText(int errorCode) {
    char* msg = NULL;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        0,
        (LPSTR)&msg,
        0,
        NULL
    );

    if (msg) {
        cout << msg;
        LocalFree(msg);
    }
}

int main() {
    WSADATA wsaData;
    int err;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "WSAStartup failed: " << result << endl;
        return 1;
    }

    cout << "Loaded winsock version: "
        << (int)LOBYTE(wsaData.wVersion) << "."
        << (int)HIBYTE(wsaData.wVersion) << endl;

    SOCKET s1 = socket(AF_INET, SOCK_DGRAM, 0);
    SOCKET s2 = socket(AF_INET, SOCK_DGRAM, 0);
    SOCKET s3 = socket(AF_INET, SOCK_STREAM, 0);
    SOCKET s4 = socket(AF_INET, SOCK_STREAM, 0);

    if (s1 == INVALID_SOCKET) {
        err = WSAGetLastError();
        cout << "Socket 1 error: " << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Socket 1 create success." << endl;
    }

    if (s2 == INVALID_SOCKET) {
        err = WSAGetLastError();
        cout << "Socket 2 error: " << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Socket 2 create success." << endl;
    }

    if (s3 == INVALID_SOCKET) {
        err = WSAGetLastError();
        cout << "Socket 3 error: " << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Socket 3 create success." << endl;
    }

    if (s4 == INVALID_SOCKET) {
        err = WSAGetLastError();
        cout << "Socket 4 error: " << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Socket 4 create success." << endl;
    }

    cout << endl;

    // Configure and bind socket 1.
    BOOL broadcast = TRUE;

    if (setsockopt(s1, SOL_SOCKET, SO_BROADCAST,
        (char*)&broadcast, sizeof(broadcast)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << "Socket 1 broadcast error: " << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Socket 1 broadcast enabled." << endl;
    }

    sockaddr_in addr1{};
    addr1.sin_family = AF_INET;
    addr1.sin_port = htons(5001);
    addr1.sin_addr.s_addr = INADDR_ANY;

    if (bind(s1, (sockaddr*)&addr1, sizeof(addr1)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << "Socket 1 bind error: " << err << endl;
        printErrorText(err);

    }
    else {
        cout << "Socket 1 bind success." << endl;
    }

    int XX = 27; 

    // Configure and bind socket 2.
    string ip2 = "10.1.2.1" + to_string(XX);

    sockaddr_in addr2{};
    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(5002);

    if (inet_pton(AF_INET, ip2.c_str(), &addr2.sin_addr) <= 0) {
        cout << "Socket 2 IP conversion error" << endl;
    }
    else {
        cout << "Socket 2 IP: " << ip2 << endl;
    }

    if (bind(s2, (sockaddr*)&addr2, sizeof(addr2)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << "Socket 2 bind error: " << err << endl;
        printErrorText(err);

    }
    else {
        cout << "Socket 2 bind success." << endl;
    }

    // Configure and bind socket 3.
    string ip3 = "10.1.2.1" + to_string(XX + 1);

    sockaddr_in addr3{};
    addr3.sin_family = AF_INET;
    addr3.sin_port = htons(5003);

    if (inet_pton(AF_INET, ip3.c_str(), &addr3.sin_addr) <= 0) {
        cout << "Socket 3 IP conversion error" << endl;
    }
    else {
        cout << "Socket 3 IP: " << ip3 << endl;
    }

    if (bind(s3, (sockaddr*)&addr3, sizeof(addr3)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << "Socket 3 bind error: " << err << endl;
        printErrorText(err);

    }
    else {
        cout << "Socket 3 bind success." << endl;
    }

    // COnfigure and bind socket 4.
    sockaddr_in addr4{};
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(5004);
    addr4.sin_addr.s_addr = INADDR_ANY;

    if (bind(s4, (sockaddr*)&addr4, sizeof(addr4)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << "Socket 4 bind error: " << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Socket 4 bind success." << endl;
    }

    cout << endl << "Bind errors showcase:" << endl;

    // Error 1.
    cout << endl << "WSAEINVAL" << endl;
    if (bind(s4, (sockaddr*)&addr4, sizeof(addr4)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Success" << endl;
    }
    
    // Error 2.
    cout << endl << "WSAEADDRINUSE" << endl;
    SOCKET s5 = socket(AF_INET, SOCK_STREAM, 0);
    if (s5 != INVALID_SOCKET) {
        if (bind(s5, (sockaddr*)&addr4, sizeof(addr4)) == SOCKET_ERROR) {
            err = WSAGetLastError();
            cout << err << endl;
            printErrorText(err);
        }
        else {
            cout << "Success" << endl;
        }
        closesocket(s5);
    }

    // Error 3.
    cout << endl << "WSAENOTSOCK" << endl;
    SOCKET fakeSocket = (SOCKET)12345;
    if (bind(fakeSocket, (sockaddr*)&addr1, sizeof(addr1)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Success" << endl;
    }

    // Error 4.
    cout << endl << "WSAEFAULT" << endl;
    if (bind(s2, nullptr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Success" << endl;
    }

    // Error 5.
    cout << endl << "WSAEAFNOSUPPORT" << endl;
    sockaddr wrongAddr{};
    wrongAddr.sa_family = AF_UNIX;
    if (bind(s3, &wrongAddr, sizeof(wrongAddr)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Success" << endl;
    }

    if (s1 != INVALID_SOCKET) closesocket(s1);
    if (s2 != INVALID_SOCKET) closesocket(s2);
    if (s3 != INVALID_SOCKET) closesocket(s3);
    if (s4 != INVALID_SOCKET) closesocket(s4);
    WSACleanup();

    // Error 6.
    cout << endl << "WSANOTINITIALISED" << endl;
    if (bind(s1, (sockaddr*)&addr1, sizeof(addr1)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        cout << err << endl;
        printErrorText(err);
    }
    else {
        cout << "Success" << endl;
    }
    
    return 0;
}
