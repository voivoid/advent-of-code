#include "AoC/2018/problem_04.h"

#include "AoC/problems_map.h"
#include "AoC/utils/parse.h"

#include "range/v3/action/sort.hpp"
#include "range/v3/algorithm/max.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/group_by.hpp"
#include "range/v3/view/indices.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"


#include <functional>
#include <istream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{

using SleepMinute = size_t;

struct SleepInterval
{
  SleepMinute falls_asleep;
  SleepMinute wakes_up;
};

using GuardId = size_t;
struct GuardShift
{
  GuardId guard_id;
  std::vector<SleepInterval> sleep_intervals;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( SleepInterval, falls_asleep, wakes_up )
BOOST_FUSION_ADAPT_STRUCT( GuardShift, guard_id, sleep_intervals )

namespace
{

size_t calc_sleep_interval( const SleepInterval& sleep_interval )
{
  assert( sleep_interval.wakes_up >= sleep_interval.falls_asleep );
  return sleep_interval.wakes_up - sleep_interval.falls_asleep;
}

template <typename Range>
size_t sum_sleep_intervals( Range&& sleep_intervals )
{
  return ranges::accumulate( sleep_intervals | ranges::view::transform( &calc_sleep_interval ), size_t{ 0 } );
}

bool is_asleep_in_minute( const SleepMinute minute, const SleepInterval interval )
{
  assert( interval.falls_asleep <= interval.wakes_up );
  return minute >= interval.falls_asleep && minute < interval.wakes_up;
}

struct MostAsleepMinute
{
  SleepMinute minute;
  size_t num_of_intervals_asleep;
};

template <typename Range>
auto find_most_asleep_minute( Range&& sleep_intervals )
{
  const auto minutes_by_intervals =
      ranges::view::indices( SleepMinute{ 0 }, SleepMinute{ 60 } ) |
      ranges::view::transform( [&sleep_intervals]( const SleepMinute minute ) {
        auto intervals                     = sleep_intervals;
        const auto num_of_intervals_asleep = ranges::accumulate(
            intervals | ranges::view::transform( std::bind( &is_asleep_in_minute, minute, std::placeholders::_1 ) ), SleepMinute{ 0 } );
        return MostAsleepMinute{ minute, num_of_intervals_asleep };
      } );

  const auto minute_with_max_num_of_intervals = ranges::max(
      minutes_by_intervals, []( const auto m1, const auto m2 ) { return m1.num_of_intervals_asleep < m2.num_of_intervals_asleep; } );

  const auto minute                  = minute_with_max_num_of_intervals.minute;
  const auto num_of_intervals_asleep = minute_with_max_num_of_intervals.num_of_intervals_asleep;

  return MostAsleepMinute{ minute, num_of_intervals_asleep };
}

using GuardShifts = std::vector<GuardShift>;
GuardShifts parse_shifts( const std::string& input )
{
  namespace x3 = boost::spirit::x3;
  using AoC::x3_size_t_;

  const auto date_parser  = ( '[' > x3::omit[ x3_size_t_ ] > '-' > x3::omit[ x3_size_t_ ] > '-' > x3::omit[ x3::lexeme[ x3_size_t_ ] ] >
                             x3::omit[ x3_size_t_ ] > ':' > x3_size_t_ > ']' );
  const auto guard_parser = x3::omit[ date_parser ] > "Guard" > '#' > x3_size_t_ > "begins" > "shift";
  const auto falls_asleep_parser = ( date_parser >> "falls" ) > "asleep";
  const auto wakes_up_parser     = date_parser > "wakes" > "up";
  const auto sleep_parser = x3::rule<struct _sleep, SleepInterval>{} = falls_asleep_parser > wakes_up_parser;
  const auto shift_parser = x3::rule<struct _shift, GuardShift>{} = ( guard_parser > *( sleep_parser ) );
  const auto parser                                               = *shift_parser;

  GuardShifts shifts;
  const bool is_parsed = AoC::x3_parse( input, parser, x3::space, shifts );
  if ( !is_parsed )
  {
    throw std::runtime_error( "Failed to parse shifts input" );
  }

  return shifts;
}

GuardShifts parse_shifts_and_sort_by_guard_id( std::istream& input )
{
  const auto sorted_input_lines = ranges::getlines( input ) | ranges::to_vector | ranges::action::sort;
  return parse_shifts( sorted_input_lines | ranges::view::join ) | ranges::action::sort( std::less<GuardId>{}, &GuardShift::guard_id );
}

struct GuardStats
{
  GuardId guard_id;
  size_t total_sleep_time;
  MostAsleepMinute most_asleep_minute;
};

auto get_guards_stats( const GuardShifts& shifts )
{
  return shifts | ranges::view::group_by( []( const GuardShift& s1, const GuardShift& s2 ) { return s1.guard_id == s2.guard_id; } ) |
         ranges::view::transform( []( const auto guard_shifts ) {
           const auto guard_id           = guard_shifts.front().guard_id;
           auto all_sleep_intervals      = guard_shifts | ranges::view::transform( &GuardShift::sleep_intervals ) | ranges::view::join;
           const auto total_sleep_time   = sum_sleep_intervals( all_sleep_intervals );
           const auto most_asleep_minute = find_most_asleep_minute( all_sleep_intervals );

           return GuardStats{ guard_id, total_sleep_time, most_asleep_minute };
         } );
}

bool sort_by_total_sleep_time( const GuardStats& g1, const GuardStats& g2 )
{
  return g1.total_sleep_time < g2.total_sleep_time;
}

bool sort_by_most_asleep_minute( const GuardStats& g1, const GuardStats& g2 )
{
  return g1.most_asleep_minute.num_of_intervals_asleep < g2.most_asleep_minute.num_of_intervals_asleep;
}

template <bool ( *cmp )( const GuardStats&, const GuardStats& )>
size_t solve( std::istream& input )
{
  const GuardShifts shifts = parse_shifts_and_sort_by_guard_id( input );
  const auto guards        = get_guards_stats( shifts );

  const auto guard_with_max_sleep_time = ranges::max( guards, cmp );

  return guard_with_max_sleep_time.guard_id * guard_with_max_sleep_time.most_asleep_minute.minute;
}

}  // namespace

namespace AoC_2018
{

namespace problem_04
{

size_t solve_1( std::istream& input )
{
  return solve<&sort_by_total_sleep_time>( input );
}

size_t solve_2( std::istream& input )
{
  return solve<&sort_by_most_asleep_minute>( input );
}

AOC_REGISTER_PROBLEM( 2018_04, solve_1, solve_2 );

}  // namespace problem_04

}  // namespace AoC_2018

#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const std::string input = R"([1518-11-01 00:00] Guard #10 begins shift
                                 [1518-11-01 00:05] falls asleep
                                 [1518-11-01 00:25] wakes up
                                 [1518-11-01 00:30] falls asleep
                                 [1518-11-01 00:55] wakes up
                                 [1518-11-01 23:58] Guard #99 begins shift)";

    auto shifts = parse_shifts( input );
    assert( shifts.size() == 2 );
    const auto& shift = shifts[ 0 ];
    assert( shift.guard_id == 10 );
    assert( shift.sleep_intervals.size() == 2 );
    const auto& intervals = shift.sleep_intervals;
    assert( ( intervals[ 0 ].falls_asleep == 5 ) );
    assert( ( intervals[ 0 ].wakes_up == 25 ) );
    assert( ( intervals[ 1 ].falls_asleep == 30 ) );
    assert( ( intervals[ 1 ].wakes_up == 55 ) );
    assert( shifts[ 1 ].guard_id == 99 );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
