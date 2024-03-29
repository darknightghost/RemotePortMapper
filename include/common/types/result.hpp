#pragma once

#include <cstdlib>

#include <common/logger/logger.h>

#include <common/types/result.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline Result<OkType, ErrorType>::Result() : m_status(Status::Bad)
{}

/**
 * @brief       Copy constructor.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline Result<OkType, ErrorType>::Result(const Result &result) :
    m_status(Status::Bad)
{
    if (result.m_status != Status::Bad) {
        this->copy(result);
    }
}

/**
 * @brief       Move constructor.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline Result<OkType, ErrorType>::Result(Result &&result) :
    m_status(Status::Bad)
{
    if (result.m_status != Status::Bad) {
        this->move(::std::move(result));
    }
}

/**
 * @brief       Destructor.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline Result<OkType, ErrorType>::~Result()
{
    this->clear();
}

/**
 * @brief       Make an ok result.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
template<typename... Args>
Result<OkType, ErrorType> Result<OkType, ErrorType>::makeOk(Args &&...args)
{
    Result ret;
    ret.m_status = Status::Ok;
    ret.construct<OkType>(::std::forward<Args>(args)...);
    return ret;
}

/**
 * @brief       Make a error result.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
template<typename... Args>
Result<OkType, ErrorType> Result<OkType, ErrorType>::makeError(Args &&...args)
{
    Result ret;
    ret.m_status = Status::Error;
    ret.construct<ErrorType>(::std::forward<Args>(args)...);
    return ret;
}

/**
 * @brief       Check if the result is good.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline bool Result<OkType, ErrorType>::good() const
{
    return m_status != Status::Bad;
}

/**
 * @brief       Check if the result is ok.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline bool Result<OkType, ErrorType>::ok() const
{
    return m_status == Status::Ok;
}

/**
 * @brief       Get value.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(! ::std::is_reference<Type>::value)
                        && (! ::std::is_void<Type>::value)
inline typename Result<OkType, ErrorType>::template ValueReference<
    Type> Result<OkType, ErrorType>::value()
{
    // Check type.
    if (((! ::std::is_same<Type, OkType>::value) && m_status == Status::Ok)
        || ((! ::std::is_same<Type, ErrorType>::value)
            && m_status == Status::Error)) {
        panic("Data type mismatched!");
    } else if (m_status == Status::Bad) {
        panic("Trying to get the value from a bad result!");
    }

    return *reinterpret_cast<typename ::std::add_pointer<Type>::type>(
        reinterpret_cast<void *>(&m_data));
}

/**
 * @brief       Get value(reference).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_reference<Type>::value)
                        && (! ::std::is_void<Type>::value)
inline typename Result<OkType, ErrorType>::template ValueReference<
    Type> Result<OkType, ErrorType>::value()
{
    // Check type.
    if (((! ::std::is_same<Type, OkType>::value) && m_status == Status::Ok)
        || ((! ::std::is_same<Type, ErrorType>::value)
            && m_status == Status::Error)) {
        panic("Data type mismatched!");
    } else if (m_status == Status::Bad) {
        panic("Trying to get the value from a bad result!");
    }

    return static_cast<Type>(
        **reinterpret_cast<typename ::std::add_pointer<
             typename ::std::add_pointer<Type>::type>::type>(
            reinterpret_cast<void *>(&m_data)));
}

/**
 * @brief       Get value.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(! ::std::is_reference<Type>::value)
                        && (! ::std::is_void<Type>::value)
inline typename Result<OkType, ErrorType>::template ConstValueReference<
    Type> Result<OkType, ErrorType>::value() const
{
    // Check type.
    if (((! ::std::is_same<Type, OkType>::value) && m_status == Status::Ok)
        || ((! ::std::is_same<Type, ErrorType>::value)
            && m_status == Status::Error)) {
        panic("Data type mismatched!");
    } else if (m_status == Status::Bad) {
        panic("Trying to get the value from a bad result!");
    }

    return *reinterpret_cast<typename ::std::add_pointer<
        typename ::std::add_const<Type>::type>::type>(
        reinterpret_cast<const void *>(m_data));
}

/**
 * @brief       Get value(reference).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_reference<Type>::value)
                        && (! ::std::is_void<Type>::value)
inline typename Result<OkType, ErrorType>::template ConstValueReference<
    Type> Result<OkType, ErrorType>::value() const
{
    // Check type.
    if (((! ::std::is_same<Type, OkType>::value) && m_status == Status::Ok)
        || ((! ::std::is_same<Type, ErrorType>::value)
            && m_status == Status::Error)) {
        panic("Data type mismatched!");
    } else if (m_status == Status::Bad) {
        panic("Trying to get the value from a bad result!");
    }

    return **reinterpret_cast<
        typename ::std::add_pointer<typename ::std::add_pointer<
            typename ::std::add_const<Type>::type>::type>::type>(
        reinterpret_cast<const void *>(m_data));
}

/**
 * @brief       Clear the value.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline void Result<OkType, ErrorType>::clear()
{
    switch (m_status) {
        case Status::Bad: {
        } break;
        case Status::Ok: {
            this->destruct<OkType>();
        } break;
        case Status::Error: {
            this->destruct<ErrorType>();
        } break;
        default: {
            panic("Illegal result status!");
        }
    }
    m_status = Status::Bad;
}

/**
 * @brief       Operator bool.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline Result<OkType, ErrorType>::operator bool() const
{
    return this->ok();
}

/**
 * @brief       Operator=.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline Result<OkType, ErrorType> &Result<OkType, ErrorType>::operator=(
    const Result &result)
{
    this->copy(result);

    return *this;
}

/**
 * @brief       Operator=.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline Result<OkType, ErrorType> &Result<OkType, ErrorType>::operator=(
    Result &&result)
{
    this->move(::std::move(result));

    return *this;
}

/**
 * @brief       Copy value.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline void Result<OkType, ErrorType>::copy(const Result &result)
{
    if (result.m_status == Status::Bad) {
        this->clear();

    } else if (m_status == result.m_status) {
        switch (m_status) {
            case Status::Ok: {
                this->copyValue<OkType>(result);

            } break;
            case Status::Error: {
                this->copyValue<ErrorType>(result);

            } break;
            default:
                panic("Illegal result status!");
        }
    } else {
        this->clear();
        m_status = result.m_status;
        switch (m_status) {
            case Status::Ok: {
                this->copyConstructValue<OkType>(result);

            } break;
            case Status::Error: {
                this->copyConstructValue<ErrorType>(result);

            } break;
            default:
                panic("Illegal result status!");
        }
    }
}

/**
 * @brief       Copy value(void).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && ::std::is_void<Type>::value
inline void Result<OkType, ErrorType>::copyValue(const Result &)
{}

/**
 * @brief       Copy value(reference).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
                        && ::std::is_reference<Type>::value
inline void Result<OkType, ErrorType>::copyValue(const Result &result)
{
    using TypePtrPtr = typename ::std::add_pointer<typename ::std::add_pointer<
        typename ::std::remove_reference<Type>::type>::type>::type;

    using TypePtrConstPtr = typename ::std::add_const<
        typename ::std::add_pointer<typename ::std::add_pointer<
            typename ::std::remove_reference<Type>::type>::type>::type>::type;

    *reinterpret_cast<TypePtrPtr>(m_data)
        = *reinterpret_cast<TypePtrConstPtr>(&(result.m_data));
}

/**
 * @brief       Copy value(copy assignable).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
                        && (! ::std::is_reference<Type>::value)
                        && ::std::is_copy_assignable<Type>::value
inline void Result<OkType, ErrorType>::copyValue(const Result &result)
{
    using TypePtrPtr = typename ::std::add_pointer<
        typename ::std::add_pointer<Type>::type>::type;
    using ConstTypePtrPtr =
        typename ::std::add_pointer<typename ::std::add_const<
            typename ::std::add_pointer<Type>::type>::type>::type;
    *reinterpret_cast<TypePtrPtr>(m_data)
        = *reinterpret_cast<ConstTypePtrPtr>(result.m_data);
}

/**
 * @brief       Copy value(else).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
                        && (! ::std::is_reference<Type>::value)
                        && (! ::std::is_copy_assignable<Type>::value)
inline void Result<OkType, ErrorType>::copyValue(const Result &result)
{
    this->destruct<Type>();
    this->copyConstructValue<Type>(result);
}

/**
 * @brief       Copy construct value(void).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && ::std::is_void<Type>::value
inline void Result<OkType, ErrorType>::copyConstructValue(const Result &)
{
    this->construct<Type>();
}

/**
 * @brief       Copy construct value(else).
 *
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
inline void Result<OkType, ErrorType>::copyConstructValue(const Result &result)
{
    this->construct<Type>(result.value<Type>());
}

/**
 * @brief       Move value.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
inline void Result<OkType, ErrorType>::move(Result &&result)
{
    if (result.m_status == Status::Bad) {
        this->clear();

    } else if (m_status == result.m_status) {
        switch (m_status) {
            case Status::Ok: {
                this->moveValue<OkType>(::std::move(result));

            } break;
            case Status::Error: {
                this->moveValue<ErrorType>(::std::move(result));

            } break;
            default:
                panic("Illegal result status!");
        }
        result.clear();
    } else {
        this->clear();
        m_status = result.m_status;
        switch (m_status) {
            case Status::Ok: {
                this->moveConstructValue<OkType>(::std::move(result));

            } break;
            case Status::Error: {
                this->moveConstructValue<ErrorType>(::std::move(result));

            } break;
            default:
                panic("Illegal result status!");
        }
        result.clear();
    }
}

/**
 * @brief       Move value(void).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && ::std::is_void<Type>::value
inline void Result<OkType, ErrorType>::moveValue(Result &&)
{}

/**
 * @brief       Move value(reference).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
                        && ::std::is_reference<Type>::value
inline void Result<OkType, ErrorType>::moveValue(Result &&result)
{
    using TypePtrPtr = typename ::std::add_pointer<typename ::std::add_pointer<
        typename ::std::remove_reference<Type>::type>::type>::type;

    *reinterpret_cast<TypePtrPtr>(m_data)
        = *reinterpret_cast<TypePtrPtr>(result.m_data);
}

/**
 * @brief       Move value(move assignable).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
                        && (! ::std::is_reference<Type>::value)
                        && ::std::is_move_assignable<Type>::value
inline void Result<OkType, ErrorType>::moveValue(Result &&result)
{
    using TypePtr = typename ::std::add_pointer<Type>::type;
    *reinterpret_cast<TypePtr>(m_data)
        = ::std::move(*reinterpret_cast<TypePtr>(result.m_data));
}

/**
 * @brief       Move value(else).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
                        && (! ::std::is_reference<Type>::value)
                        && (! ::std::is_move_assignable<Type>::value)
                        && (! ::std::is_copy_assignable<Type>::value)
inline void Result<OkType, ErrorType>::moveValue(Result &&result)
{
    this->destruct<Type>();
    this->moveConstructValue<Type>(::std::move(result));
}

/**
 * @brief       Move construct value(void).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && ::std::is_void<Type>::value
inline void Result<OkType, ErrorType>::moveConstructValue(Result &&)
{
    this->construct<Type>();
}

/**
 * @brief       Move construct value(else).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
inline void Result<OkType, ErrorType>::moveConstructValue(Result &&result)
{
    this->construct<Type>(::std::forward<Type>(result.value<Type>()));
}

/**
 * @brief       Construct the value(void).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && ::std::is_void<Type>::value
inline void Result<OkType, ErrorType>::construct()
{}

/**
 * @brief       Construct the value(reference).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
                        && ::std::is_reference<Type>::value
inline void Result<OkType, ErrorType>::construct(Type ref)
{
    using TypePtrPtr = typename ::std::add_pointer<
        typename ::std::add_pointer<Type>::type>::type;
    *reinterpret_cast<TypePtrPtr>(m_data) = &ref;
}

/**
 * @brief       Construct the value(others).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type, typename... Args>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
                        && (! ::std::is_void<Type>::value)
                        && (! ::std::is_reference<Type>::value)
                        && ::std::is_constructible<Type, Args &&...>::value
inline void Result<OkType, ErrorType>::construct(Args &&...args)
{
    using TypePtr = typename ::std::add_pointer<Type>::type;
    new (reinterpret_cast<TypePtr>(m_data)) Type(::std::forward<Args>(args)...);
}

/**
 * @brief       Desctuct the value.
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(::std::is_same<Type, OkType>::value
                         || ::std::is_same<Type, ErrorType>::value)
inline void Result<OkType, ErrorType>::destruct()
{
    this->destructAt<Type>(
        reinterpret_cast<typename ::std::add_pointer<Type>::type>(m_data));
}

/**
 * @brief       Desctuct the value at the position(trival).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires ::std::is_void<Type>::value
                         || ::std::is_reference<Type>::value
                         || ((! ::std::is_array<Type>::value)
                             && ::std::is_trivial<Type>::value)
inline void Result<OkType, ErrorType>::destructAt(
    typename ::std::add_pointer<Type>::type)
{}

/**
 * @brief       Desctuct the value at the position(non-trival).
 */
template<typename OkType, typename ErrorType>
    requires(! ::std::is_array<OkType>::value)
            && (! ::std::is_array<ErrorType>::value)
            template<typename Type>
                requires(! ::std::is_void<Type>::value)
                        && (! ::std::is_reference<Type>::value)
                        && (! ::std::is_array<Type>::value)
                        && (! ::std::is_trivial<Type>::value)
inline void Result<OkType, ErrorType>::destructAt(
    typename ::std::add_pointer<Type>::type ptr)
{
    ptr->~Type();
}

} // namespace remotePortMapper
