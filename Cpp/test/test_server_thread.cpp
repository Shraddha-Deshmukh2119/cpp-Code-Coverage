#include "Server.h"
#include "Thread.h"

#include <fstream>
#include <stdexcept>

#include <gtest/gtest.h>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace {

void noop_handler(Thread&) {}

void ensure_winsock()
{
    static bool ready = false;
    if (!ready)
    {
        WSADATA wsa{};
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            throw std::runtime_error("WSAStartup failed");
        ready = true;
    }
}

} // namespace

TEST(ServerCoverage, GetActivityReadsClientsFile)
{
    ensure_winsock();
    {
        std::ofstream f("clients.txt");
        f << "Client 1 connected\n";
    }
    Server server(noop_handler, 17801);
    EXPECT_NE(server.testGetActivity().find("connected"), std::string::npos);
}

TEST(ServerCoverage, GetActivityMissingFileReturnsEmpty)
{
    ensure_winsock();
    std::remove("clients.txt");
    Server server(noop_handler, 17802);
    EXPECT_TRUE(server.testGetActivity().empty());
}

TEST(ServerCoverage, CloseSocketDoesNotThrow)
{
    ensure_winsock();
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT_NE(s, INVALID_SOCKET);
    Server server(noop_handler, 17803);
    server.testCloseSocket(s);
    closesocket(s);
}

TEST(ServerCoverage, StartInitializesWinsock)
{
    ensure_winsock();
    Server server(noop_handler, 17804);
    EXPECT_TRUE(server.start());
}

TEST(ServerCoverage, DestructorClosesAfterStart)
{
    ensure_winsock();
    {
        Server server(noop_handler, 17807);
        ASSERT_TRUE(server.start());
    }
}

TEST(ServerCoverage, UpdateActivityAppendsClientsFile)
{
    ensure_winsock();
    const u_short port = 17956;
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT_NE(listen_sock, INVALID_SOCKET);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ASSERT_EQ(0, bind(listen_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)));
    ASSERT_EQ(0, listen(listen_sock, 1));

    SOCKET client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT_EQ(0, connect(client_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)));

    sockaddr_in peer{};
    int peer_len = sizeof(peer);
    SOCKET server_sock = accept(listen_sock, reinterpret_cast<sockaddr*>(&peer), &peer_len);
    ASSERT_NE(server_sock, INVALID_SOCKET);

    std::remove("clients.txt");
    Server server(noop_handler, 17805);
    server.testUpdateActivity(server_sock);
    EXPECT_TRUE(std::ifstream("clients.txt").good());

    closesocket(client_sock);
    closesocket(server_sock);
    closesocket(listen_sock);
}

TEST(ThreadCoverage, SendRecCodePathsExecute)
{
    ensure_winsock();
    Thread t;
    t.testSetClient(INVALID_SOCKET);
    (void)t.Send("ping");
    (void)t.Send(7);
    (void)t.Send(2.5);
    std::string reply;
    (void)t.Rec(reply);
}

TEST(ThreadCoverage, CreateCloseAndStateFlags)
{
    ensure_winsock();
    Thread t;
    t.create();
    EXPECT_TRUE(t.available());
    EXPECT_FALSE(t.isDestroyed());
    t.setFree();
    EXPECT_TRUE(t.isDestroyed());
    EXPECT_FALSE(t.available());
    t.CloseThread();
}
