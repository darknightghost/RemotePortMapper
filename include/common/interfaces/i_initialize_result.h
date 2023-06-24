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
    inline IInitializeResult();

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
    const inline Result<void, Error> &takeInitializeResult() const;

    /**
     * @brief       Take result.
     *
     * @return      Initialize result.
     */
    inline Result<void, Error> &&takeInitializeResult();

  protected:
    /**
     * @brief       Set current object initialized.
     */
    void inline setInitializeResult(Result<void, Error> &&result);
};

} // namespace remotePortMapper

#include <common/interfaces/i_initialize_result.hpp>
