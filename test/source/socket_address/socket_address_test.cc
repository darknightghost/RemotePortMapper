#include <cstdint>
#include <future>

#include <gtest/gtest.h>

#include <common/socket/socket_address.h>

TEST(SocketAddress, all)
{
    ::remotePortMapper::SocketAddress addr;
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::Unknow);
    ASSERT_EQ(addr.size(), 0);

    ASSERT_TRUE(addr.fill("127.0.0.1", 80));
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::IPv4);
    ASSERT_EQ(addr.size(), sizeof(sockaddr_in));
    ASSERT_EQ(addr.toString(), "{\"127.0.0.1\", 80}");

    ASSERT_TRUE(addr.fill("0.0.0.0", 8080));
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::IPv4);
    ASSERT_EQ(addr.size(), sizeof(sockaddr_in));
    ASSERT_EQ(addr.toString(), "{\"0.0.0.0\", 8080}");

    ASSERT_FALSE(addr.fill("1.0.0.0.0", 8080));
    ASSERT_EQ(addr.size(), 0);
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::Unknow);

    ASSERT_FALSE(addr.fill("127.0.0.256", 8080));
    ASSERT_EQ(addr.size(), 0);
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::Unknow);

    ASSERT_TRUE(addr.fill("::", 8080));
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::IPv6);
    ASSERT_EQ(addr.size(), sizeof(sockaddr_in6));
    ASSERT_EQ(addr.toString(), "{\"::\", 8080}");

    ASSERT_TRUE(addr.fill("00::", 8080));
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::IPv6);
    ASSERT_EQ(addr.size(), sizeof(sockaddr_in6));
    ASSERT_EQ(addr.toString(), "{\"::\", 8080}");

    ASSERT_TRUE(addr.fill("::ffff", 8080));
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::IPv6);
    ASSERT_EQ(addr.size(), sizeof(sockaddr_in6));
    ASSERT_EQ(addr.toString(), "{\"::ffff\", 8080}");

    ASSERT_FALSE(addr.fill("::::", 8080));
    ASSERT_EQ(addr.size(), 0);
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::Unknow);

    ASSERT_FALSE(addr.fill("::ffgg", 8080));
    ASSERT_EQ(addr.size(), 0);
    ASSERT_EQ(addr.type(), ::remotePortMapper::SocketAddress::Type::Unknow);
}
