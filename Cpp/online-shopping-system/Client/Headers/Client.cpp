#include "Client.h"

Client::~Client()
{
    WSACleanup();
}

bool Client::start()
{
    WORD version = MAKEWORD(2, 2);
    bool WSAok = !WSAStartup(version, &wsaData);

    if (WSAok)
        Connect();
        
    return WSAok;
}

void Client::Connect()
{
    closesocket(client);
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client != INVALID_SOCKET)
    {
        // entering client socket's info
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(m_port);
        addr.sin_addr.S_un.S_addr = inet_addr(m_ipAddress.c_str());
        ZeroMemory(addr.sin_zero, sizeof(addr.sin_zero));

        // connecting to server
        connect(client, (sockaddr *)&addr, sizeof(addr));
    }
}

int Client::Send(const std::string& str) const
{
    return send(client, str.c_str(), str.size() + 1, 0);
}

int Client::Send(const int& var) const
{
    std::ostringstream ss;
    ss << var;
    return Send(ss.str());
}

int Client::Send(const double& var) const
{
    std::ostringstream ss;
    ss << var;
    return Send(ss.str());
}

int Client::Rec(std::string &str)
{
    char buffer[4069];
    ZeroMemory(buffer, sizeof(buffer));
    
    int bytesRec = recv(client, buffer, sizeof(buffer), 0);
    str = std::string(buffer, 0, bytesRec);
    return bytesRec;
}