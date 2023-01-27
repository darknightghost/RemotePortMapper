#include <cstring>

#include <common/socket/socket_address.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
inline SocketAddress::SocketAddress() : m_type(Type::Unknow) {}

/**
 * @brief       Copy constructor.
 */
inline SocketAddress::SocketAddress(const SocketAddress &addr) :
    m_type(addr.m_type)
{
    ::memcpy(&m_data, &addr.m_data, sizeof(m_data));
}

/**
 * @brief   Get address type.
 */
inline SocketAddress::Type SocketAddress::type() const
{
    return m_type;
}

/**
 * @brief       Set address type.
 */
void inline SocketAddress::setType(Type type)
{
    m_type = type;
}

/**
 * @brief   Get address size.
 */
inline ::std::size_t SocketAddress::size() const
{
    switch (m_type) {
        case Type::IPv4:
            return sizeof(sockaddr_in);

        case Type::IPv6:
            return sizeof(sockaddr_in6);

        default:
            return 0;
    }
}

/**
 * @brief   Get data of address.
 */
inline SocketAddress::AddressData &SocketAddress::data()
{
    return m_data;
}

/**
 * @brief   Get data of address.
 */
inline const SocketAddress::AddressData &SocketAddress::data() const
{
    return m_data;
}

/**
 * @brief       operator=
 */
inline SocketAddress &SocketAddress::operator=(const SocketAddress &addr)
{
    m_type = addr.m_type;
    ::memcpy(&m_data, &addr.m_data, sizeof(m_data));

    return *this;
}

} // namespace remotePortMapper
