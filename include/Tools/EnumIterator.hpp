#pragma once

template<typename T>
class EnumIterator
{
public:

    class Iterator
    {
    public:
        explicit Iterator(int value) :
            m_value(value)
        {}

        T operator*(void) const
        {
            return static_cast<T>(m_value);
        }

        void operator++(void)
        {
            ++m_value;
        }

        bool operator!=(const Iterator& rhs) const
        {
            return m_value != rhs.m_value;
        }

    private:
        int m_value;
    };


};

template< typename T >
typename EnumIterator<T>::Iterator begin( EnumIterator<T> )
{
    return typename EnumIterator<T>::Iterator( (int)T::First );
}

template< typename T >
typename EnumIterator<T>::Iterator end( EnumIterator<T> )
{
    return typename EnumIterator<T>::Iterator( ((int)T::Last) + 1 );
}