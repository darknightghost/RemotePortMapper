#pragma once

#include <common/utils/flags.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType>::Flags(
    typename ::std::underlying_type<FlagType>::type flags) :
    m_value(flags)
{}

/**
 * @brief       Constructor.
 */
template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType>::Flags() : m_value(0)
{}

/**
 * @brief       Constructor.
 */
template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType>::Flags(FlagType flag) :
    m_value(static_cast<typename ::std::underlying_type<FlagType>::type>(flag))
{}

/**
 * @brief       Copy constructor.
 */
template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType>::Flags(const Flags &flags) :
    m_value(flags.m_value)
{}

/**
 * @brief       Get value.
 */
template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline typename ::std::underlying_type<FlagType>::type
    Flags<FlagType>::value() const
{
    return m_value;
}

/**
 * @brief       Get value.
 */
template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType>::operator typename ::std::underlying_type<
    FlagType>::type() const
{
    return m_value;
}

/**
 * @brief       Get mask.
 */
template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline typename ::std::underlying_type<FlagType>::type
    Flags<FlagType>::mask() const
{
    return ~m_value;
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType> &Flags<FlagType>::operator=(const Flags &flags)
{
    m_value = flags.m_value;

    return *this;
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType> &
    Flags<FlagType>::operator=(const FlagType flag)
{
    m_value
        = static_cast<typename ::std::underlying_type<FlagType>::type>(flag);

    return *this;
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline bool Flags<FlagType>::operator==(const Flags &flags) const
{
    return m_value == flags.m_value;
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline bool Flags<FlagType>::operator==(FlagType flag) const
{
    return m_value
           == static_cast<typename ::std::underlying_type<FlagType>::type>(
               flag);
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline bool Flags<FlagType>::operator!=(const Flags &flags) const
{
    return m_value != flags.m_value;
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline bool Flags<FlagType>::operator!=(FlagType flag) const
{
    return m_value
           != static_cast<typename ::std::underlying_type<FlagType>::type>(
               flag);
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType>
    Flags<FlagType>::operator&(const Flags &flags) const
{
    return Flags(m_value & flags.m_value);
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType> Flags<FlagType>::operator&(FlagType flag) const
{
    return Flags(
        m_value
        & static_cast<typename ::std::underlying_type<FlagType>::type>(flag));
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType> &
    Flags<FlagType>::operator&=(const Flags &flags)
{
    m_value &= flags.m_value;
    return *this;
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType> &Flags<FlagType>::operator&=(FlagType flag)
{
    m_value
        &= static_cast<typename ::std::underlying_type<FlagType>::type>(flag);
    return *this;
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType>
    Flags<FlagType>::operator|(const Flags &flags) const
{
    return Flags(m_value | flags.m_value);
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType> Flags<FlagType>::operator|(FlagType flag) const
{
    return Flags(
        m_value
        | static_cast<typename ::std::underlying_type<FlagType>::type>(flag));
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType> &
    Flags<FlagType>::operator|=(const Flags &flags)
{
    m_value |= flags.m_value;
    return *this;
}

template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
constexpr inline Flags<FlagType> &Flags<FlagType>::operator|=(FlagType flag)
{
    m_value
        |= static_cast<typename ::std::underlying_type<FlagType>::type>(flag);
    return *this;
}

} // namespace remotePortMapper
