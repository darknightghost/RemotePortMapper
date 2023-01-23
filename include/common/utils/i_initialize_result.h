#pragma once

#include <memory>

#include <common/error/error.h>
#include <common/types/result.h>

namespace remotePortMapper {

/**
 * @brief   Initialize ressult interface.
 */
class IInitializeResult {
  private:
    Result<void, Error> m_initializeResult; ///< Initialize result.

  protected:
    /**
     * @brief       Constructor.
     */
    IInitializeResult() {}

  public:
    /**
     * @brief       Destructor.
     */
    virtual ~IInitializeResult() = default;

  public:
    /**
     * @brief       Check result.
     *
     * @return      Initialize result.
     */
    const Result<void, Error> &takeInitializeResult() const
    {
        return m_initializeResult;
    }

    /**
     * @brief       Take result.
     *
     * @return      Initialize result.
     */
    Result<void, Error> &&takeInitializeResult()
    {
        return ::std::move(m_initializeResult);
    }

  protected:
    /**
     * @brief       Set current object initialized.
     */
    void setInitializeResult(Result<void, Error> &&result)
    {
        m_initializeResult = result;
    }
};

} // namespace remotePortMapper
