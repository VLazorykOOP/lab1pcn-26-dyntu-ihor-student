#include <iostream>
#include <winsock.h>

#pragma comment(lib, "wsock32.lib")
using namespace std;

int main() {
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(1, 1), &wsaData);
    if (result != 0) {
        cout << "WSAStartup failed: " << result << endl;
        return 1;
    }

    cout << "Loaded winsock version: "
        << (int)LOBYTE(wsaData.wVersion) << "."
        << (int)HIBYTE(wsaData.wVersion) << endl;

    cout << "Highest supported winsock version: "
        << (int)LOBYTE(wsaData.wHighVersion) << "."
        << (int)HIBYTE(wsaData.wHighVersion) << endl;
    
    cout << "Max socket count: " << wsaData.iMaxSockets << endl;
    cout << "Max packet size: " << wsaData.iMaxUdpDg<< endl;
    cout << "Description: " << wsaData.szDescription << endl;
    cout << "System status: " << wsaData.szSystemStatus << endl;

    WSACleanup();
    return 0;
}


