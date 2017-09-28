#include "server.h"

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

Server::Server()
{
    serverSocket = 0;
}

int Server::connectTo(std::string host, int port)
{
    int newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket < 0)
    {
        std::cout << "failed to create a socket\n";
        exit(1);
    }

    struct hostent *server = gethostbyname(host.c_str());
    if (server == NULL)
    {
        std::cout << "ERROR, no such host" << std::endl;
        exit(1);
    }

    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serverAddr.sin_addr.s_addr,
          server->h_length);
    serverAddr.sin_port = htons(port);

    if (connect(newSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == ETIMEDOUT)
    {
        handle_error("connect");
    }

    const int flag = 1;
    if (setsockopt(newSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int)) < 0)
    {
        handle_error("setTCP_NODELAY");
    }

    sockets.push_back(newSocket);

    return sockets.size() - 1;
}

int Server::startListening(int myPort)
{
    if (serverSocket == 0)
    {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0)
            handle_error("socket");
        const int enable = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            handle_error("setsockopt");
        memset((char *)&myAddr, '\0', sizeof(myAddr));
        myAddr.sin_family = AF_INET;
        myAddr.sin_addr.s_addr = INADDR_ANY;
    }
    myAddr.sin_port = htons(myPort);

    if (bind(serverSocket, (struct sockaddr *)&myAddr, sizeof(myAddr)) < 0)
    {
        handle_error("bind");
    }

    listen(serverSocket, 5);

    return 0;
}

int Server::waitForConnection()
{
    struct sockaddr_in clientAddr;
    socklen_t clilen = sizeof(clientAddr);

    int newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clilen);
    if (newSocket < 0)
    {
        handle_error("accept");
    }
    const int flag = 1;
    if (setsockopt(newSocket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int)) < 0)
    {
        handle_error("setTCP_NODELAY");
    }

    sockets.push_back(newSocket);

    return sockets.size() - 1;
}

int Server::sendIntTo(int num, int i)
{
    int x = htonl(num);
    if (write(sockets[i], (const char *)&x, sizeof(x)) < 0)
        handle_error("sendIntTo");
    return 0;
}

int Server::receiveIntFrom(int *num, int i)
{
    int x;
    if (read(sockets[i], &x, sizeof(x)) < 0)
        handle_error("receiveIntFrom");
    *num = ntohl(x);
    return 0;
}

void Server::sendStrs(std::vector<std::string> strs, int to)
{
    sendIntTo(strs.size(), to);
    for (unsigned int i = 0; i < strs.size(); i++)
    {
        sendStr(strs[i], to);
    }
}

std::vector<std::string> Server::recvStrs(int from)
{
    int sz;
    receiveIntFrom(&sz, from);
    std::vector<std::string> res(sz);
    for (int i = 0; i < sz; i++)
    {
        res[i] = recvStr(from);
    }
    return res;
}

int Server::sendStr(std::string hex, int to)
{
    sendIntTo(hex.size(), to);
    if (write(sockets[to], hex.data(), hex.size() * sizeof(char)) < 0)
        handle_error("sendStr");
    return 0;
}

std::string Server::recvStr(int from)
{
    int sz;
    receiveIntFrom(&sz, from);
    std::string res(sz, 'a');
    if (read(sockets[from], &res.front(), sz * sizeof(char)) < 0)
        handle_error("recvStr");
    return res;
}

int Server::sendBytes(std::vector<char> bytes, int to)
{
    sendIntTo(bytes.size(), to);
    if (write(sockets[to], bytes.data(), bytes.size() * sizeof(char)) < 0)
        handle_error("sendStr");
    return 0;
}

std::vector<char> Server::recvBytes(int from)
{
    int sz;
    receiveIntFrom(&sz, from);
    std::vector<char> res(sz);
    if (read(sockets[from], res.data(), sz * sizeof(char)) < 0)
        handle_error("recvStr");
    return res;
}

int Server::sendIntVector(std::vector<int> input, int i)
{
    sendIntTo(input.size(), i);
    // const int32_t sz = input.size();
    // if(write(sockets[i], &sz, sizeof(sz)) < 0){
    //     std::cout << "Error writing size of vector" << std::endl;
    //     exit(1);
    // }
    if (write(sockets[i], &input[0], input.size() * sizeof(int)) < 0)
        handle_error("sendIntVector");
    return 0;
}

int Server::receiveIntVector(std::vector<int> &res, int i)
{
    int sz;
    receiveIntFrom(&sz, i);
    // int32_t sz;
    // if(read(sockets[i], &sz, sizeof(sz)) < 0){
    //     std::cout << "Error reading size of vector" << std::endl;
    //     exit(1);
    // }
    res.resize(sz);
    if (read(sockets[i], &res[0], sz * sizeof(int)) < 0)
        handle_error("receiveIntVector");
    return 0;
}

void Server::clean()
{
    for (unsigned int i = 0; i < sockets.size(); i++)
    {
        close(sockets[i]);
    }
    close(serverSocket);
}