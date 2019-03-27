#include "AoC/2017/problem_04.h"

#include "AoC/problems_map.h"

#include "range/v3/getlines.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/view/split.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/action/sort.hpp"

#include <string>
#include <set>

namespace
{

std::string sort_by_letters( std::string s )
{
    return std::move( s ) | ranges::action::sort;
}

template <std::string(*transform_word)( std::string )>
bool is_secure_passphrase( const std::string& line )
{
    std::set<std::string> words;
    for( const auto word_range : line | ranges::view::split( ' ' ) )
    {
        auto word = word_range | ranges::to<std::string>;

        if constexpr(transform_word != nullptr)
        {
            word = transform_word( word );
        }

        const auto [ _, inserted ] = words.insert( word );
        if( !inserted )
        {
            return false;
        }
    }

    return true;
}

template <std::string(*transform_word)( std::string ) = nullptr>
int calc_secure_passphrases( std::istream& input )
{
    return static_cast<int>( ranges::count_if( ranges::getlines( input ), &is_secure_passphrase<transform_word> ) );
}

}  // namespace

namespace AoC_2017
{

namespace problem_04
{

int solve_1( std::istream& input )
{
    return calc_secure_passphrases( input );
}

int solve_2( std::istream& input )
{
    return calc_secure_passphrases<&sort_by_letters>( input );
}

AOC_REGISTER_PROBLEM( 2017_04, solve_1, solve_2 );

}  // namespace problem_04

}  // namespace AoC_2017
