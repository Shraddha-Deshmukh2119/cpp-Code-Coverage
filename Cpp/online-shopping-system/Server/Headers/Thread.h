#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include <winsock2.h>
#include <string>
#include <windows.h>

class Server;
struct Data
{
    Server *server;
    SOCKET client;
    int index;
};
void worker(Data *);

class Thread
{
public:
    Thread() = default;
    virtual ~Thread() = default;
    Thread(Server* server, int index)
    {
        data.server = server;
        data.index = index;;
    }

    void create();
    bool available() const;
    void enter(SOCKET &Client);
    void setFree();
    bool isFree() const;
    bool isDestroyed() const;
    void CloseThread();
    HANDLE getHandle() const;
    SOCKET getClient() const;
    virtual void endServer();

    // extra function:
    virtual std::string getActivity();
    virtual void closeClient();
 
    virtual int Send(const std::string& str) const;
    virtual int Send(const int& var) const;
    virtual int Send(const double& var) const;
    virtual int Rec(std::string& receivingString);

#ifdef OSHOP_TEST_BUILD
    void testSetClient(SOCKET client) { data.client = client; }
#endif

private:
    HANDLE handle = nullptr;
    Data data;
    bool free = true;
    bool destroyed = false;
};