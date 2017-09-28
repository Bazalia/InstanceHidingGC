#ifndef server_h
#define server_h

#include <cstddef>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

class Server
{
    struct sockaddr_in myAddr;
    int serverSocket;

  protected:
    std::vector<int> sockets;

  public:
    Server();
    int connectTo(std::string, int);
    int startListening(int);
    int waitForConnection();
    int sendIntTo(int, int);
    int receiveIntFrom(int *, int);
    int sendIntVector(std::vector<int>, int);
    int receiveIntVector(std::vector<int> &, int);

    void sendStrs(std::vector<std::string> strs, int to);
    std::vector<std::string> recvStrs(int from);
    int sendStr(std::string hex, int to);
    std::string recvStr(int from);
    int sendBytes(std::vector<char> bytes, int to);
    std::vector<char> recvBytes(int from);
    void clean();
};

#endif