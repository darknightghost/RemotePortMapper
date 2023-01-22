#pragma once

#include <functional>

/**
 * @brief   Class to test the behavior of the class.
 */
class TestClass {
  private:
    ::std::function<void(TestClass *)> m_onConstruct; ///< Constructor callback.
    ::std::function<void(TestClass *)> m_onDestruct;  ///< Destructor callback.
    ::std::function<void(TestClass *)> m_onCopy;      ///< Copy callback.
    ::std::function<void(TestClass *)> m_onMove;      ///< Move callback.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   onConstruct Constructor callback.
     * @param[in]   onDestruct  Destructor callback.
     * @param[in]   onCopy      Copy callback.
     * @param[in]   onMove      Move callback.
     */
    TestClass(::std::function<void(TestClass *)> onConstruct,
              ::std::function<void(TestClass *)> onDestruct,
              ::std::function<void(TestClass *)> onCopy,
              ::std::function<void(TestClass *)> onMove) :
        m_onConstruct(onConstruct),
        m_onDestruct(onDestruct), m_onCopy(onCopy), m_onMove(onMove)
    {
        if (m_onConstruct) {
            m_onConstruct(this);
        }
    }

    /**
     * @brief       Copy constructor.
     *
     * @param[in]   o       Object to copy.
     */
    TestClass(const TestClass &o) :
        m_onConstruct(o.m_onConstruct), m_onDestruct(o.m_onDestruct),
        m_onCopy(o.m_onCopy), m_onMove(o.m_onMove)
    {
        if (m_onConstruct) {
            m_onConstruct(this);
        }
        if (m_onCopy) {
            m_onCopy(this);
        }
    }

    /**
     * @brief       Move constructor.
     *
     * @param[in]   o       Object to move.
     */
    TestClass(TestClass &&o) :
        m_onConstruct(o.m_onConstruct), m_onDestruct(o.m_onDestruct),
        m_onCopy(o.m_onCopy), m_onMove(o.m_onMove)
    {
        if (m_onConstruct) {
            m_onConstruct(this);
        }
        if (m_onMove) {
            m_onMove(this);
        }
    }

    /**
     * @brief       Operator=.
     *
     * @param[in]   o       Object to copy.
     *
     * @return      *this.
     */
    TestClass &operator=(const TestClass &o)
    {
        m_onConstruct = o.m_onConstruct;
        m_onDestruct  = o.m_onDestruct;
        m_onCopy      = o.m_onCopy;
        m_onMove      = o.m_onMove;

        if (m_onCopy) {
            m_onCopy(this);
        }

        return *this;
    }

    /**
     * @brief       Operator=.
     *
     * @param[in]   o       Object to move.
     *
     * @return      *this.
     */
    TestClass &operator=(TestClass &&o)
    {
        m_onConstruct = o.m_onConstruct;
        m_onDestruct  = o.m_onDestruct;
        m_onCopy      = o.m_onCopy;
        m_onMove      = o.m_onMove;

        if (m_onMove) {
            m_onMove(this);
        }

        return *this;
    }

    /**
     * @brief       Destructor.
     */
    virtual ~TestClass()
    {
        if (m_onDestruct) {
            m_onDestruct(this);
        }
    }
};
