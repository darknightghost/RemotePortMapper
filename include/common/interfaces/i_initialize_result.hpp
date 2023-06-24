#pragma once

#include <common/interfaces/i_initialize_result.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
inline IInitializeResult::IInitializeResult() {}

/**
 * @brief       Check result.
 */
inline const Result<void, Error> &
    IInitializeResult::takeInitializeResult() const
{
    return m_initializeResult;
}

/**
 * @brief       Take result.
 */
inline Result<void, Error> &&IInitializeResult::takeInitializeResult()
{
    return ::std::move(m_initializeResult);
}

/**
 * @brief       Set current object initialized.
 */
inline void IInitializeResult::setInitializeResult(Result<void, Error> &&result)
{
    m_initializeResult = result;
}

} // namespace remotePortMapper
