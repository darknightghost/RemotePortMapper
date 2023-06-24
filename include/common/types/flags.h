#pragma once

#include <type_traits>

#include <common/utils/nargs.h>

namespace remotePortMapper {

/**
 * @brief       Flags.
 *
 * @param[in]   FlagType        Type of the flag.
 */
template<typename FlagType>
    requires ::std::is_enum<FlagType>::value
class Flags {
  private:
    typename ::std::underlying_type<FlagType>::type m_value; ///< Value.

  private:
    /**
     * @brief       Constructor.
     *
     * @parag[in]   flags       Flags.
     */
    constexpr inline Flags(
        typename ::std::underlying_type<FlagType>::type flags);

  public:
    /**
     * @brief       Constructor.
     */
    constexpr inline Flags();

    /**
     * @brief       Constructor.
     *
     * @param[in]   flag    Flag.
     */
    constexpr inline Flags(FlagType flag);

    /**
     * @brief       Copy constructor.
     *
     * @parag[in]   flags       Flags.
     */
    constexpr inline Flags(const Flags &flags);

    /**
     * @brief       Destructor.
     */
    constexpr inline ~Flags() = default;

  public:
    /**
     * @brief       Get value.
     *
     * @return      Value.
     */
    constexpr inline typename ::std::underlying_type<FlagType>::type
        value() const;

    /**
     * @brief       Get value.
     *
     * @return      Value.
     */
    constexpr inline
        operator typename ::std::underlying_type<FlagType>::type() const;

  public:
    /**
     * @brief       Get mask.
     *
     * @return      Mask.
     */
    constexpr inline typename ::std::underlying_type<FlagType>::type
        mask() const;

  public:
    constexpr inline Flags &operator=(const Flags &flags);
    constexpr inline Flags &operator=(FlagType flag);

    constexpr inline bool operator==(const Flags &flags) const;
    constexpr inline bool operator==(FlagType flag) const;

    constexpr inline bool operator!=(const Flags &flags) const;
    constexpr inline bool operator!=(FlagType flag) const;

    constexpr inline Flags operator&(const Flags &flags) const;
    constexpr inline Flags operator&(FlagType flag) const;

    constexpr inline Flags &operator&=(const Flags &flags);
    constexpr inline Flags &operator&=(FlagType flag);

    constexpr inline Flags operator|(const Flags &flags) const;
    constexpr inline Flags operator|(FlagType flag) const;

    constexpr inline Flags &operator|=(const Flags &flags);
    constexpr inline Flags &operator|=(FlagType flag);
};

} // namespace remotePortMapper

#define __FLAGS_TYPE_IMPL2(type, flags_type)                               \
    using flags_type = ::remotePortMapper::Flags<type>;                    \
    constexpr inline ::remotePortMapper::Flags<type> operator&(type flag1, \
                                                               type flag2) \
    {                                                                      \
        ::remotePortMapper::Flags<type> ret(flag1);                        \
        ret &= flag2;                                                      \
        return ret;                                                        \
    }                                                                      \
    constexpr inline ::remotePortMapper::Flags<type> operator|(type flag1, \
                                                               type flag2) \
    {                                                                      \
        ::remotePortMapper::Flags<type> ret(flag1);                        \
        ret |= flag2;                                                      \
        return ret;                                                        \
    }

#define __FLAGS_TYPE_IMPL1(type) __FLAGS_TYPE_IMPL2(type, type##s)

#define __FLAGS_TYPE_IMPL_(n, ...) __FLAGS_TYPE_IMPL##n(__VA_ARGS__)

#define __FLAGS_TYPE_IMPL(n, ...) __FLAGS_TYPE_IMPL_(n, __VA_ARGS__)

#define FLAGS_TYPE(...) __FLAGS_TYPE_IMPL(VA_NARGS(__VA_ARGS__), ##__VA_ARGS__)

#include <common/types/flags.hpp>
