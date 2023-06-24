#pragma once

#include <sstream>

#include <common/interfaces/i_to_string.h>

namespace remotePortMapper {

/**
 * @brief       Convert object to string.
 */
inline ::std::string IToString::toString(::std::size_t indent,
                                         ::std::size_t indentWidth)
{
    ::std::ostringstream ss;
    this->toStream(ss, indent, indentWidth);

    return ss.str();
}

/**
 * @brief       Write indent.
 */
inline void IToString::writeIndent(::std::ostream &stream,
                                   ::std::size_t   indent,
                                   ::std::size_t   indentWidth)
{
    for (::std::size_t i = 0; i < indent * indentWidth; ++i) {
        stream << ' ';
    }
}

} // namespace remotePortMapper
