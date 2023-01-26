#include <algorithm>
#include <regex>
#include <sstream>

#if defined(OS_LINUX)
    #include <arpa/inet.h>

#elif defined(OS_WINDOWS)
    #include <WS2tcpip.h>

#endif

#include <common/socket/address.h>

namespace remotePortMapper {

/**
 * @brief       Fill the socket address.
 */
Result<void, Error> SocketAddress::fill(const ::std::string &ip, uint16_t port)
{
    static ::std::regex ipv4Exp("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");

    ::memset(&m_data, 0, sizeof(m_data));
    if (::std::regex_match(ip, ipv4Exp)) {
        // IPv4
        if (::inet_pton(AF_INET, ip.c_str(), &(m_data.addr4.sin_addr))) {
            m_type                  = Type::IPv4;
            m_data.addr4.sin_family = AF_INET;
            m_data.addr4.sin_port   = htons(port);

            return Result<void, Error>::makeOk();
        }

    } else {
        // IPv6
        if (::inet_pton(AF_INET6, ip.c_str(), &(m_data.addr6.sin6_addr))) {
            m_type                   = Type::IPv6;
            m_data.addr6.sin6_family = AF_INET6;
            m_data.addr6.sin6_port   = htons(port);

            return Result<void, Error>::makeOk();
        }
    }

    // Error.
    m_type = Type::Unknow;
    ::std::ostringstream ss;

    ss << "Illegal IP address \"" << ip << "\".";

    return Result<void, Error>::makeError(
        Error {ErrorCode::InvalidValue, ss.str()});
}

/**
 * @brief       Write object to stream.
 */
::std::ostream &SocketAddress::toStream(::std::ostream &stream,
                                        ::std::size_t,
                                        ::std::size_t)
{
    // Check type.
    int      family;
    void    *src;
    uint16_t port;

    switch (m_type) {
        case Type::IPv4: {
            family = AF_INET;
            src    = &(m_data.addr4.sin_addr);
            port   = ntohs(m_data.addr4.sin_port);
        } break;

        case Type::IPv6: {
            family = AF_INET6;
            src    = &(m_data.addr6.sin6_addr);
            port   = ntohs(m_data.addr6.sin6_port);
        } break;

        default:
            stream << "{UNKNOW_SOCKET_ADDRESS}";
            return stream;
    }

    stream << "{\"";

    // Parse address.
    char buffer[::std::max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN) + 1];
    if (::inet_ntop(family, src, buffer, sizeof(buffer)) == nullptr) {
        stream << "ILLEGAL_ADDRESS";
    } else {
        stream << buffer;
    }
    stream << "\", " << port << "}";

    return stream;
}

} // namespace remotePortMapper
