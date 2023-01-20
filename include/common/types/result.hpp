#include <cstdlib>

#include <common/types/result.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
template<typename SuccessType, typename FailType>
inline Result<SuccessType, FailType>::Result() : m_status(Status::Bad)
{}

/**
 * @brief       Copy constructor.
 */
template<typename SuccessType, typename FailType>
inline Result<SuccessType, FailType>::Result(const Result &result) :
    m_status(Status::Bad)
{
    if (result.m_status != Status::Bad) {
        this->copy(result);
    }
}

/**
 * @brief       Move constructor.
 */
template<typename SuccessType, typename FailType>
inline Result<SuccessType, FailType>::Result(Result &&result) :
    m_status(Status::Bad)
{
    if (result.m_status != Status::Bad) {
        this->move(result);
    }
}

/**
 * @brief       Destructor.
 */
template<typename SuccessType, typename FailType>
inline Result<SuccessType, FailType>::~Result()
{
    this->clear();
}

/**
 * @brief       Check if the result is good.
 */
template<typename SuccessType, typename FailType>
inline bool Result<SuccessType, FailType>::good() const
{
    return m_status != Status::Bad;
}

/**
 * @brief       Check if the result is success.
 */
template<typename SuccessType, typename FailType>
inline bool Result<SuccessType, FailType>::success() const
{
    return m_status == Status::Success;
}

/**
 * @brief       Get value.
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(! ::std::is_reference<Type>::value)
            && (! ::std::is_void<Type>::value)
inline typename Result<SuccessType, FailType>::template ValueReference<
    Type> Result<SuccessType, FailType>::value()
{
    // Check type.
    if (((! ::std::is_same<Type, SuccessType>::value)
         && m_status == Status::Success)
        || ((! ::std::is_same<Type, FailType>::value)
            && m_status == Status::Fail)) {
        ::abort();
    }

    return *reinterpret_cast<typename ::std::add_pointer<Type>::type>(
        reinterpret_cast<void *>(&m_data));
}

/**
 * @brief       Get value.
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_reference<Type>::value)
            && (! ::std::is_void<Type>::value)
inline typename Result<SuccessType, FailType>::template ValueReference<
    Type> Result<SuccessType, FailType>::value()
{
    // Check type.
    if (((! ::std::is_same<Type, SuccessType>::value)
         && m_status == Status::Success)
        || ((! ::std::is_same<Type, FailType>::value)
            && m_status == Status::Fail)) {
        ::abort();
    }

    return **reinterpret_cast<typename ::std::add_pointer<
        typename ::std::add_pointer<Type>::type>::type>(
        reinterpret_cast<void *>(&m_data));
}

/**
 * @brief       Get value.
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(! ::std::is_reference<Type>::value)
            && (! ::std::is_void<Type>::value)
inline typename Result<SuccessType, FailType>::template ConstValueReference<
    Type> Result<SuccessType, FailType>::value() const
{
    // Check type.
    if (((! ::std::is_same<Type, SuccessType>::value)
         && m_status == Status::Success)
        || ((! ::std::is_same<Type, FailType>::value)
            && m_status == Status::Fail)) {
        ::abort();
    }

    return *reinterpret_cast<typename ::std::add_pointer<
        typename ::std::add_const<Type>::type>::type>(
        reinterpret_cast<const void *>(&m_data));
}

/**
 * @brief       Get value.
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_reference<Type>::value)
            && (! ::std::is_void<Type>::value)
inline typename Result<SuccessType, FailType>::template ConstValueReference<
    Type> Result<SuccessType, FailType>::value() const
{
    // Check type.
    if (((! ::std::is_same<Type, SuccessType>::value)
         && m_status == Status::Success)
        || ((! ::std::is_same<Type, FailType>::value)
            && m_status == Status::Fail)) {
        ::abort();
    }

    return **reinterpret_cast<
        typename ::std::add_pointer<typename ::std::add_pointer<
            typename ::std::add_const<Type>::type>::type>::type>(
        reinterpret_cast<const void *>(&m_data));
}

/**
 * @brief       Operator bool.
 */
template<typename SuccessType, typename FailType>
inline Result<SuccessType, FailType>::operator bool() const
{
    return this->success();
}

/**
 * @brief       Operator=.
 */
template<typename SuccessType, typename FailType>
inline Result<SuccessType, FailType> &
    Result<SuccessType, FailType>::operator=(const Result &result)
{
    this->copy(result);

    return *this;
}

/**
 * @brief       Operator=.
 */
template<typename SuccessType, typename FailType>
inline Result<SuccessType, FailType> &
    Result<SuccessType, FailType>::operator=(Result &&result)
{
    this->move(result);

    return *this;
}

} // namespace remotePortMapper
