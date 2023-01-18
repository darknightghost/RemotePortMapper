#pragma once

#include <memory>

namespace remotePortMapper {

/**
 * @brief   Initialized status interface.
 */
class IInitialized {
  private:
    bool m_initialized; ///< Initialized status.

  protected:
    /**
     * @brief       Constructor.
     */
    IInitialized() : m_initialized(false) {}

  public:
    /**
     * @brief       Destructor.
     */
    virtual ~IInitialized() = default;

  public:
    /**
     * @brief       Check if current object initialized.
     *
     * @return      Initialized status.
     */
    bool initialized() const
    {
        return m_initialized;
    }

  protected:
    /**
     * @brief       Set current object initialized.
     */
    void setInitialized()
    {
        m_initialized = true;
    }
};

} // namespace remotePortMapper
