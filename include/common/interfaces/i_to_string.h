#pragma once

#include <ostream>
#include <string>

namespace remotePortMapper {

/**
 * @brief       Print interface.
 */
class IToString {
  protected:
    /**
     * @brief       Constructor.
     */
    IToString() = default;

  public:
    /**
     * @brief       Destructor.
     */
    virtual ~IToString() = default;

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
                                     ::std::size_t   indentWidth = 4)
        = 0;

    /**
     * @brief       Convert object to string.
     *
     * @param[in]   indent      Indent.
     * @param[in]   indentWidth Width of indent.
     */
    inline ::std::string toString(::std::size_t indent      = 0,
                                  ::std::size_t indentWidth = 4);

  protected:
    /**
     * @brief       Write indent.
     *
     * @param[in]   stream      Stream to write.
     * @param[in]   indent      Indent.
     * @param[in]   indentWidth Width of indent.
     */
    inline void writeIndent(::std::ostream &stream,
                            ::std::size_t   indent,
                            ::std::size_t   indentWidth);
};

} // namespace remotePortMapper

#include <common/interfaces/i_to_string.hpp>
