#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
// Windows 平台
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;  // 统一类型定义
#else
// Unix/Linux 平台
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
    #define WSADATA int
    #define WSAStartup(x, y) (0)
    #define WSACleanup() (0)
#endif

int main() {
    // 初始化Winsock（Windows）
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup失败" << std::endl;
        return -1;
    }

    // 创建套接字
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //    这段代码调用了 `socket` 函数，用于创建一个新的套接字（socket），返回值赋给 `sockfd`。
    //    - `AF_INET`：表示使用 IPv4 地址族。
    //    - `SOCK_STREAM`：表示使用面向连接的流式套接字（即 TCP 协议）。
    //    - `0`：通常为 0，表示使用默认协议（TCP 对应 SOCK_STREAM）。
    //    整体作用：**创建一个用于 TCP 通信的 IPv4 套接字。**
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "创建套接字失败" << std::endl;
        WSACleanup();
        return -1;
    }

    // 设置服务器地址
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;// 使用 IPv4
    serverAddr.sin_port = htons(8080);
#ifdef _WIN32
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
#else
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif

    // 连接服务器
    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "连接失败" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return -1;
    }

    std::cout << "连接成功！" << std::endl;

    // 清理资源
    closesocket(sockfd);
    WSACleanup();
    return 0;
}