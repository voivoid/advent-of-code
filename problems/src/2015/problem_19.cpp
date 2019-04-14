#include "AoC/2015/problem_19.h"

#include "AoC/problems_map.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/string.h"

#include "range/v3/action/insert.hpp"
#include "range/v3/action/sort.hpp"
#include "range/v3/action/unique.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/move.hpp"
#include "range/v3/view/take_while.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/algorithm/string/replace.hpp"
#include "boost/fusion/container/vector.hpp"
#include "boost/numeric/conversion/cast.hpp"


#include <istream>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace
{

using Molecule    = std::string;
using Formula     = std::string;
using MoleculeMap = std::unordered_multimap<Molecule, Molecule>;

std::pair<Molecule, Molecule> parse_replacement( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto molecule_parser = x3::lexeme[ +x3::alpha ];
  const auto parser          = molecule_parser > "=>" > molecule_parser;

  boost::fusion::vector<std::string, std::string> replacements;
  const bool is_parsed = AoC::x3_parse( line.cbegin(), line.cend(), parser, x3::space, replacements );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse replacement data" );
  }

  auto [ from, to ] = AoC::fusion_to_std_tuple( replacements );

  return { std::move( from ), std::move( to ) };
}

enum class ReplacementsMapMode
{
  Normal,
  Reversed
};

template <typename Replacements>
MoleculeMap make_replacements_map( Replacements replacements, const ReplacementsMapMode mode )
{
  MoleculeMap map;
  for ( auto [ key, value ] : replacements )
  {
    auto& k = mode == ReplacementsMapMode::Reversed ? value : key;
    auto& v = mode == ReplacementsMapMode::Reversed ? key : value;
    map.emplace( std::move( k ), std::move( v ) );
  };

  return map;
}

std::vector<Formula> do_replacements( const Formula& formula, const Molecule& from, const Molecule& to )
{
  std::vector<Formula> results;
  results.reserve( AoC::count_substrings( formula, from ) );

  std::string::size_type pos_to_search = 0;
  while ( true )
  {
    pos_to_search = formula.find( from, pos_to_search );
    if ( pos_to_search == std::string::npos )
    {
      break;
    }

    const auto begin = formula.cbegin() + boost::numeric_cast<std::string::difference_type>( pos_to_search );
    const auto end   = begin + boost::numeric_cast<std::string::difference_type>( from.length() );

    std::string replace_result( formula.length() + ( to.length() - from.length() ), '\0' );
    boost::algorithm::replace_range_copy( replace_result.begin(), formula, boost::make_iterator_range( begin, end ), to );
    results.push_back( std::move( replace_result ) );

    pos_to_search += from.length();
  }

  return results;
}

auto parse_input( std::istream& input, const ReplacementsMapMode mode )
{
  auto replacement_lines = ranges::getlines( input ) | ranges::view::take_while( []( const std::string& line ) { return line != ""; } ) |
                           ranges::view::transform( &parse_replacement );
  const auto replacements_map = make_replacements_map( replacement_lines, mode );
  std::string result_molecule;
  input >> result_molecule;

  return std::make_pair( std::move( replacements_map ), std::move( result_molecule ) );
}

size_t calc_all_replacements( const MoleculeMap& replacements_map, const Formula& formula )
{
  size_t counter = 0;
  for ( const auto& [ from, _ ] : replacements_map )
  {
    counter += AoC::count_substrings( formula, from );
  }

  return counter;
}

std::vector<Formula> get_all_replacements( const MoleculeMap& replacements_map, const Formula& formula )
{
  std::vector<Molecule> all_replacements;
  all_replacements.reserve( calc_all_replacements( replacements_map, formula ) );

  for ( const auto& [ from, to ] : replacements_map )
  {
    auto replacements = do_replacements( formula, from, to );
    ranges::insert( all_replacements, all_replacements.end(), replacements | ranges::view::move );
  }

  return std::move( all_replacements ) | ranges::action::sort | ranges::action::unique;
}

// void reduce( const MoleculeMap& replacements_map, const Formula& formula, std::set<Formula>& reduced )
//{
//  if ( formula == "e" )
//  {
//    throw std::runtime_error( "FOUND!" );
//  }

//  if ( reduced.find( formula ) != reduced.cend() )
//  {
//    return;
//  }

//  reduced.insert( formula );

//  for ( const auto& [ from, to ] : replacements_map )
//  {
//    auto replacements = do_replacements( formula, from, to );
//    for ( const auto& replacement : replacements )
//    {
//      reduce( replacements_map, replacement, reduced );
//    }
//  }
//}

}  // namespace

namespace AoC_2015
{

namespace problem_19
{

size_t solve_1( std::istream& input )
{
  const auto& [ replacements_map, formula ] = parse_input( input, ReplacementsMapMode::Normal );
  const auto all_replacements               = get_all_replacements( replacements_map, formula );

  return all_replacements.size();
}

size_t solve_2( std::istream& /*input*/ )
{
  //  const auto& [ replacements_map, result_molecule ] = parse_input( input, true );

  //  std::set<Formula> reduced_formulas;
  //  reduce( replacements_map, result_molecule, reduced_formulas );

  return 0;
}

AOC_REGISTER_PROBLEM( 2015_19, solve_1, solve_2 );

}  // namespace problem_19

}  // namespace AoC_2015


#ifndef NDEBUG

#  include "impl_tests.h"
#  include <cassert>

static void impl_tests()
{
  {
    const auto replacement = parse_replacement( "H => OH" );
    assert( replacement.first == "H" );
    assert( replacement.second == "OH" );
  }

  {
    const auto replacements = do_replacements( "HOH", "H", "HO" );
    assert( replacements[ 0 ] == "HOOH" );
    assert( replacements[ 1 ] == "HOHO" );
  }
}

REGISTER_IMPL_TEST( impl_tests );

#endif
