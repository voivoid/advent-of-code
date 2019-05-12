#include "AoC/2015/problem_19.h"

#include "AoC/problems_map.h"
#include "AoC/utils/anon_ret.h"
#include "AoC/utils/fusion.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges/generate.h"

#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/take_while.hpp"
#include "range/v3/view/transform.hpp"

#include "boost/fusion/container/vector.hpp"
#include "boost/safe_numerics/safe_integer.hpp"

#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace
{

using Molecule    = std::string;
using MoleculeMap = std::unordered_multimap<Molecule, Molecule>;

std::pair<Molecule, Molecule> parse_replacement( const std::string& line )
{
  namespace x3 = boost::spirit::x3;

  const auto molecule_parser = x3::lexeme[ +x3::alpha ];
  const auto parser          = molecule_parser > "=>" > molecule_parser;

  boost::fusion::vector<std::string, std::string> replacements;
  const bool is_parsed = AoC::x3_parse( line, parser, x3::space, replacements );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse replacement data" );
  }

  auto [ from, to ] = AoC::fusion_to_std_tuple( replacements );
  return { std::move( from ), std::move( to ) };
}

auto parse_input( std::istream& input )
{
  auto replacement_lines = ranges::getlines( input ) | ranges::view::take_while( []( const std::string& line ) { return line != ""; } ) |
                           ranges::view::transform( &parse_replacement );
  const MoleculeMap replacements_map = replacement_lines;

  std::string medicine_molecule;
  input >> medicine_molecule;

  AOC_RETURN_ANONYMOUS_STRUCT( replacements_map, medicine_molecule );
}

std::string make_replacement( Molecule molecule, const Molecule& from, const Molecule& to, const std::string::size_type pos )
{
  return molecule.replace( pos, from.size(), to );
}

using StrIdx = boost::safe_numerics::safe<int>;
auto find_next_replacement_pos( const Molecule& medicine, const Molecule& replacement, const StrIdx pos )
{
  const auto next_pos = medicine.find( replacement, pos + 1 );

  using O = std::optional<StrIdx>;
  return next_pos != std::string::npos ? O{ next_pos } : O{};
}

auto generate_replacements( const Molecule& from, const Molecule& to, const Molecule& medicine )
{
  return AoC::generate_while( StrIdx{ -1 },
                              std::bind( &find_next_replacement_pos, std::cref( medicine ), std::cref( from ), std::placeholders::_1 ) ) |
         ranges::view::transform( std::bind( &make_replacement, medicine, std::cref( from ), std::cref( to ), std::placeholders::_1 ) );
}

}  // namespace

namespace AoC_2015
{

namespace problem_19
{

size_t solve_1( std::istream& input )
{
  const auto parsed_input       = parse_input( input );
  const auto& medicine_molecule = parsed_input.medicine_molecule;

  auto replacements = parsed_input.replacements_map | ranges::view::transform( [&medicine_molecule]( const auto& kv ) {
                        const auto& [ from, to ] = kv;
                        return generate_replacements( from, to, medicine_molecule );
                      } ) |
                      ranges::view::join;

  const std::unordered_set<Molecule> all_distinct_molecules = replacements | ranges::to<std::unordered_set>;
  return all_distinct_molecules.size();
}

size_t solve_2( std::istream& )
{
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
}

REGISTER_IMPL_TEST( impl_tests );

#endif
