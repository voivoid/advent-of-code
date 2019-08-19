#include "AoC/2015/problem_16.h"

#include "AoC/problems_map.h"
#include "AoC/utils/assert.h"
#include "AoC/utils/parse.h"

#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/adapted/struct.hpp"
#include "boost/fusion/include/std_pair.hpp"
#include "boost/optional/optional.hpp"


#include <functional>
#include <istream>
#include <map>
#include <stdexcept>
#include <string>

namespace
{

using Quantity = unsigned int;

struct Aunt
{
  enum class Compound
  {
    children,
    cats,
    samoyeds,
    pomeranians,
    akitas,
    vizslas,
    goldfish,
    trees,
    cars,
    perfumes
  };

  unsigned int index;
  std::map<Compound, Quantity> compounds;
};

}  // namespace

BOOST_FUSION_ADAPT_STRUCT( Aunt, index, compounds )

namespace
{

Aunt parse_aunt( const std::string& input )
{
  namespace x3 = boost::spirit::x3;

  x3::symbols<Aunt::Compound> compounds;
  compounds.add( "children", Aunt::Compound::children )( "cats", Aunt::Compound::cats )( "samoyeds", Aunt::Compound::samoyeds )(
      "pomeranians", Aunt::Compound::pomeranians )( "akitas", Aunt::Compound::akitas )( "vizslas", Aunt::Compound::vizslas )(
      "goldfish", Aunt::Compound::goldfish )( "trees", Aunt::Compound::trees )( "cars", Aunt::Compound::cars )( "perfumes",
                                                                                                                Aunt::Compound::perfumes );

  const auto compound_parser = compounds > ':' > x3::uint_;
  const auto parser          = "Sue" > x3::uint_ > ':' > ( compound_parser % ',' );

  Aunt aunt;
  const bool is_parsed = AoC::x3_parse( input, parser, x3::space, aunt );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse aunt data" );
  }

  return aunt;
}

bool compound_comparator_1( const Aunt::Compound, const Quantity real_q, const Quantity candidate_q )
{
  return real_q == candidate_q;
}

bool compound_comparator_2( const Aunt::Compound compound, const Quantity real_q, const Quantity candidate_q )
{
  if ( compound == Aunt::Compound::cats || compound == Aunt::Compound::trees )
  {
    return real_q < candidate_q;
  }
  else if ( compound == Aunt::Compound::pomeranians || compound == Aunt::Compound::goldfish )
  {
    return real_q > candidate_q;
  }

  return real_q == candidate_q;
}

template <bool ( *cmp )( Aunt::Compound, Quantity, Quantity )>
bool cmp_aunt( const Aunt& real_aunt, const Aunt& candidate )
{
  for ( const auto& [ key, value ] : candidate.compounds )
  {
    if ( auto key_iter = real_aunt.compounds.find( key ); key_iter == real_aunt.compounds.cend() || !cmp( key, key_iter->second, value ) )
    {
      return false;
    }
  }
  return true;
}

Aunt real_aunt()
{
  return { 0,
           { { Aunt::Compound::children, 3 },
             { Aunt::Compound::cats, 7 },
             { Aunt::Compound::samoyeds, 2 },
             { Aunt::Compound::pomeranians, 3 },
             { Aunt::Compound::akitas, 0 },
             { Aunt::Compound::vizslas, 0 },
             { Aunt::Compound::goldfish, 5 },
             { Aunt::Compound::trees, 3 },
             { Aunt::Compound::cars, 2 },
             { Aunt::Compound::perfumes, 1 } } };
}

template <bool ( *Comparator )( const Aunt&, const Aunt& )>
size_t find_aunt( std::istream& input )
{
  auto aunts       = ranges::getlines( input ) | ranges::views::transform( &parse_aunt );
  auto result_aunt = ranges::find_if( aunts, std::bind( Comparator, real_aunt(), std::placeholders::_1 ) );

  if ( result_aunt == aunts.end() )
  {
    AOC_ASSERT_FALSE();
  }

  const Aunt aunt = *result_aunt;
  return aunt.index;
}

}  // namespace

namespace AoC_2015
{

namespace problem_16
{

size_t solve_1( std::istream& input )
{
  return find_aunt<&cmp_aunt<&compound_comparator_1>>( input );
}

size_t solve_2( std::istream& input )
{
  return find_aunt<&cmp_aunt<&compound_comparator_2>>( input );
}

AOC_REGISTER_PROBLEM( 2015_16, solve_1, solve_2 );

}  // namespace problem_16

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  auto a1 = parse_aunt( "Sue 1: goldfish: 6, trees: 9, akitas: 0" );
  assert( a1.index == 1 );
  assert( a1.compounds[ Aunt::Compound::goldfish ] == 6 );
  assert( a1.compounds[ Aunt::Compound::trees ] == 9 );
  assert( a1.compounds[ Aunt::Compound::akitas ] == 0 );
}

REGISTER_IMPL_TEST( impl_tests );

#endif
