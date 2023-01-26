#pragma once

#include <cstdint>
#include <string>

#if defined(OS_LINUX)
    #include <netinet/in.h>
    #include <sys/socket.h>

#elif defined(OS_WINDOWS)
    #include <windsock2.h>

#endif

#include <common/error/error.h>
#include <common/types/result.h>

#include <common/utils/i_to_string.h>

namespace remotePortMapper {

/**
 * @brief       Socket address.
 */
class SocketAddress : virtual public IToString {
  public:
    /**
     * @brief       Type of the address.
     */
    enum class Type : uint32_t {
        Unknow = 0, ///< Unknow type.
        IPv4   = 1, ///< IPv4.
        IPv6   = 2  ///< IPv6
    };

    /**
     * @brief       Address data.
     */
    union AddressData {
        sockaddr     addr;  ///< Address.
        sockaddr_in  addr4; ///< IPv4 address.
        sockaddr_in6 addr6; ///< IPv6 address.
    };                      ///< Address.

  private:
    Type        m_type; ///< Address type.
    AddressData m_data; ///< Data of address.

  public:
    /**
     * @brief       Constructor.
     */
    inline SocketAddress();

    /**
     * @brief       Copy constructor.
     *
     * @param[in]   addr    Address to copy.
     */
    inline SocketAddress(const SocketAddress &addr);

    /**
     * @brief       Destructor.
     */
    ~SocketAddress() = default;

  public:
    /**
     * @brief       Fill the socket address.
     *
     * @param[in]   ip      IP address.
     * @param[in]   port    Port.
     *
     * @return      Fill result.
     */
    Result<void, Error> fill(const ::std::string &ip, uint16_t port);

    /**
     * @brief   Get address type.
     *
     * @return  Address type.
     */
    inline Type type() const;

    /**
     * @brief       Set address type.
     *
     * @param[in]   type    Address type.
     */
    inline void setType(Type type);

    /**
     * @brief   Get address size.
     *
     * @return  Size of address.
     */
    inline ::std::size_t size() const;

    /**
     * @brief   Get data of address.
     *
     * @return  Data of address.
     */
    inline AddressData &data();

    /**
     * @brief   Get data of address.
     *
     * @return  Data of address.
     */
    inline const AddressData &data() const;

  public:
    /**
     * @brief       Write object to stream.
     *
     * @param[in]   stream      Stream to write.
     * @param[in]   indent      Indent.
     * @param[in]   indentWidth Width of indent.
     *
     * @return      Stream.
     */
    virtual ::std::ostream &toStream(::std::ostream &stream,
                                     ::std::size_t   indent      = 0,
                                     ::std::size_t   indentWidth = 4) override;

  public:
    /**
     * @brief       operator=
     *
     * @param[in]   addr    Address to copy.
     *
     * @return      *this.
     */
    inline SocketAddress &operator=(const SocketAddress &addr);
};

} // namespace remotePortMapper

#include <common/socket/address.hpp>
