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
 * @brief       Clear the value.
 */
template<typename SuccessType, typename FailType>
inline void Result<SuccessType, FailType>::clear()
{
    switch (m_status) {
        case Status::Bad: {
        } break;
        case Status::Success: {
            this->destruct<SuccessType>();
        } break;
        case Status::Fail: {
            this->destruct<FailType>();
        } break;
        default: {
            ::abort();
        }
    }
    m_status = Status::Bad;
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

/**
 * @brief       Copy value.
 */
template<typename SuccessType, typename FailType>
inline void Result<SuccessType, FailType>::copy(const Result &result)
{
    if (result.m_status == Status::Bad) {
        this->clear();

    } else if (m_status == result.m_status) {
        switch (m_status) {
            case Status::Success: {
                this->copyValue<SuccessType>(result);

            } break;
            case Status::FailType: {
                this->copyValue<FailType>(result);

            } break;
            default:
                ::abort();
        }
    } else {
        this->clear();
        m_status = result.m_status;
        switch (m_status) {
            case Status::Success: {
                this->construct<SuccessType>(result.value<SuccessType>());

            } break;
            case Status::FailType: {
                this->construct<SuccessType>(result.value<FailType>());

            } break;
            default:
                ::abort();
        }
    }
}

/**
 * @brief       Copy value(void).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && ::std::is_void<Type>::value
inline void Result<SuccessType, FailType>::copyValue(const Result &)
{}

/**
 * @brief       Copy value(reference).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && ::std::is_reference<Type>::value
inline void Result<SuccessType, FailType>::copyValue(const Result &result)
{
    using TypePtrPtr = typename ::std::add_pointer<typename ::std::add_pointer<
        typename ::std::remove_reference<Type>::type>::type>::type;

    using TypePtrConstPtr = typename ::std::add_const<
        typename ::std::add_pointer<typename ::std::add_pointer<
            typename ::std::remove_reference<Type>::type>::type>::type>::type;

    *reinterpret_cast<TypePtrPtr>(m_data)
        = *reinterpret_cast<typename ::std::add_pointer<TypePtrConstPtr>::type>(
            &(result.m_data));
}

/**
 * @brief       Copy value(copy assignable).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && (! ::std::is_reference<Type>::value)
            && ::std::is_copy_assignable<Type>::value
inline void Result<SuccessType, FailType>::copyValue(const Result &result)
{
    using TypePtr      = typename ::std::add_pointer<Type>::type;
    using ConstTypePtr = typename ::std::add_pointer<
        typename ::std::add_const<Type>::type>::type;
    *reinterpret_cast<TypePtr>(m_data)
        = *reinterpret_cast<ConstTypePtr>(result.m_data);
}

/**
 * @brief       Copy value(else).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && (! ::std::is_reference<Type>::value)
            && (! ::std::is_copy_assignable<Type>::value)
inline void Result<SuccessType, FailType>::copyValue(const Result &result)
{
    this->destruct<Type>();
    this->construct<Type>(result.value<Type>());
}

/**
 * @brief       Move value.
 */
template<typename SuccessType, typename FailType>
inline void Result<SuccessType, FailType>::move(Result &&result)
{
    if (result.m_status == Status::Bad) {
        this->clear();

    } else if (m_status == result.m_status) {
        switch (m_status) {
            case Status::Success: {
                this->moveValue<SuccessType>(result);

            } break;
            case Status::FailType: {
                this->moveValue<FailType>(result);

            } break;
            default:
                ::abort();
        }
        result.clear();
    } else {
        this->clear();
        m_status = result.m_status;
        switch (m_status) {
            case Status::Success: {
                this->construct<SuccessType>(
                    ::std::move(result.value<SuccessType>()));

            } break;
            case Status::FailType: {
                this->construct<SuccessType>(
                    ::std::move(result.value<FailType>()));

            } break;
            default:
                ::abort();
        }
        result.clear();
    }
}

/**
 * @brief       Move value(void).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && ::std::is_void<Type>::value
inline void Result<SuccessType, FailType>::moveValue(Result &&)
{}

/**
 * @brief       Move value(reference).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && ::std::is_reference<Type>::value
inline void Result<SuccessType, FailType>::moveValue(Result &&result)
{
    using TypePtrPtr = typename ::std::add_pointer<typename ::std::add_pointer<
        typename ::std::remove_reference<Type>::type>::type>::type;

    *reinterpret_cast<TypePtrPtr>(m_data)
        = *reinterpret_cast<typename ::std::add_pointer<TypePtrPtr>::type>(
            &(result.m_data));
}

/**
 * @brief       Move value(move assignable).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && (! ::std::is_reference<Type>::value)
            && ::std::is_move_assignable<Type>::value
inline void Result<SuccessType, FailType>::moveValue(Result &&result)
{
    using TypePtr = typename ::std::add_pointer<Type>::type;
    *reinterpret_cast<TypePtr>(m_data)
        = ::std::move(*reinterpret_cast<TypePtr>(result.m_data));
}

/**
 * @brief       Move value(else).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && (! ::std::is_reference<Type>::value)
            && (! ::std::is_move_assignable<Type>::value)
            && (! ::std::is_copy_assignable<Type>::value)
inline void Result<SuccessType, FailType>::moveValue(Result &&result)
{
    this->destruct<Type>();
    this->construct<Type>(::std::move(result.value<Type>()));
}

/**
 * @brief       Construct the value(void).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && ::std::is_void<Type>::value
inline void Result<SuccessType, FailType>::construct()
{}

/**
 * @brief       Construct the value(reference).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && ::std::is_reference<Type>::value
inline void Result<SuccessType, FailType>::construct(Type ref)
{
    using TypePtr                     = typename ::std::add_pointer<Type>::type;
    reinterpret_cast<TypePtr>(m_data) = &ref;
}

/**
 * @brief       Construct the value(others).
 */
template<typename SuccessType, typename FailType>
template<typename Type, typename... Args>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && (! ::std::is_reference<Type>::value)
            && ::std::is_constructible<Type, Args &&...>::value
inline void Result<SuccessType, FailType>::construct(Args &&...args)
{
    using TypePtr = typename ::std::add_pointer<Type>::type;
    new (reinterpret_cast<TypePtr>(m_data)) Type(::std::forward<Args>(args)...);
}

/**
 * @brief       Desctuct the value.
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
inline void Result<SuccessType, FailType>::destruct()
{
    this->destructAt<Type>(m_data);
}

/**
 * @brief       Desctuct the value at the position(void/reference).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (::std::is_void<Type>::value || ::std::is_reference<Type>::value)
inline void Result<SuccessType, FailType>::destructAt(void *)
{}

/**
 * @brief       Desctuct the value at the position(array).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && (! ::std::is_reference<Type>::value)
            && ::std::is_array<Type>::value
inline void Result<SuccessType, FailType>::destructAt(void *ptr)
{
    using ArrayPtr = typename ::std::add_pointer<Type>::type;

    ArrayPtr arrayPtr = reinterpret_cast<ArrayPtr>(ptr);
    for (size_t i = 0;
         i < sizeof(Type) / sizeof(typename ::std::remove_extent<Type>::type);
         ++i) {
        this->destructAt<typename ::std::remove_extent<Type>::type>(
            &((*arrayPtr)[i]));
    }
}

/**
 * @brief       Desctuct the value at the position(trival).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && (! ::std::is_reference<Type>::value)
            && (! ::std::is_array<Type>::value)
            && ::std::is_trivial<Type>::value
inline void Result<SuccessType, FailType>::destructAt(void *)
{}

/**
 * @brief       Desctuct the value at the position(non-trival).
 */
template<typename SuccessType, typename FailType>
template<typename Type>
    requires(::std::is_same<Type, SuccessType>::value
             || ::std::is_same<Type, FailType>::value)
            && (! ::std::is_void<Type>::value)
            && (! ::std::is_reference<Type>::value)
            && (! ::std::is_array<Type>::value)
            && (! ::std::is_trivial<Type>::value)
inline void Result<SuccessType, FailType>::destructAt(void *ptr)
{
    using TypePtr = typename ::std::add_pointer<Type>::type;
    reinterpret_cast<TypePtr>(ptr)->~Type();
}

} // namespace remotePortMapper
