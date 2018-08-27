#pragma once

#include <utility>

namespace AoC
{

    template <typename T, typename F>
    auto iterate_n(T&& val, F&& func, size_t n)
    {
        while( n > 0 )
        {
            val = func( std::forward<T>( val ) );
            --n;
        }

        return val;
    }

}
