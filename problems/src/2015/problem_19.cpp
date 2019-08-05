#include "AoC/2015/problem_19.h"

#include "AoC/problems_map.h"
#include "AoC/utils/anon_ret.h"
#include "AoC/utils/parse.h"
#include "AoC/utils/ranges/generate.h"

#include "range/v3/range/conversion.hpp"
#include "range/v3/view/getlines.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/take_while.hpp"
#include "range/v3/view/transform.hpp"

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

  Molecule from;
  Molecule to;
  const bool is_parsed = AoC::x3_parse_m( line, parser, x3::space, from, to );
  if ( !is_parsed )
  {
    throw std::invalid_argument( "Failed to parse replacement data" );
  }

  return { std::move( from ), std::move( to ) };
}

auto parse_input( std::istream& input )
{
  auto replacement_lines = ranges::getlines( input ) | ranges::view::take_while( []( const std::string& line ) { return line != ""; } ) |
                           ranges::view::transform( &parse_replacement );
  const MoleculeMap replacements_map = replacement_lines | ranges::to<MoleculeMap>;

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

/*
namespace parser
{
    namespace x3 = boost::spirit::x3;

    auto y = x3::lit( 'Y' );
    auto rn = x3::lit( "Rn" );
    auto ar = x3::lit( "Ar" );

    auto mol1 = x3::upper - y;
    auto mol2 = ( x3::upper >> x3::lower ) - ( rn | ar );
    auto mol = ( mol2 | mol1 );

    x3::rule<struct _reduce> reduce;

    auto rule1 = reduce >> reduce;
    auto rule2 = reduce >> rn >> reduce >> ar;
    auto rule3 = reduce >> rn >> reduce >> y >> reduce >> ar;

    auto reduce_def = rule1 | mol;

    BOOST_SPIRIT_DEFINE( reduce )
}

bool reduce_molecule( const Molecule& molecule )
{
    const bool is_reduced = AoC::x3_parse( molecule, parser::reduce, boost::spirit::x3::space );
    return is_reduced;
}
*/

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
  // reduce_molecule( parse_input( input ).medicine_molecule );
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

  /*
{
  assert( reduce_molecule( "H" ) );
  assert( reduce_molecule( "Al" ) );
  assert( !reduce_molecule( "Y" ) );
  assert( !reduce_molecule( "Rn" ) );
  assert( !reduce_molecule( "Ar" ) );
  assert( reduce_molecule( "HO" ) );
  assert( reduce_molecule( "ThRnFAr" ) );
  assert( reduce_molecule( "SiRnFYFAr" ) );
} */
}

REGISTER_IMPL_TEST( impl_tests );

#endif
